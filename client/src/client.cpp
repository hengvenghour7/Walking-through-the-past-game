// Prevent Windows API conflicts
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Avoid collisions
#define NOGDI
#define NOUSER

// Include Raylib first
#include "raylib.h"
#include "raymath.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

#include "game.h"

using boost::asio::ip::udp;

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_context io_context;

        // --------------------------------------------------
        // Create UDP socket
        // --------------------------------------------------
        udp::resolver resolver(io_context);

        udp::endpoint receiver_endpoint =
            *resolver.resolve(
                udp::v4(),
                "127.0.0.1",
                "5000"
            ).begin();

        udp::socket socket(io_context);
        socket.open(udp::v4());

        // IMPORTANT:
        // Do not block the game loop waiting for network data.
        socket.non_blocking(true);

        // --------------------------------------------------
        // Connect to server
        // --------------------------------------------------
        std::string connect_message = "Connect";

        std::cout << "Sending Connect\n";

        socket.send_to(
            boost::asio::buffer(connect_message),
            receiver_endpoint
        );

        std::cout << "Waiting for server...\n";

        // --------------------------------------------------
        // Wait for initial Join message
        //
        // We cannot immediately continue because we need
        // our player ID from the server.
        // --------------------------------------------------
        std::array<char, 1024> recv_buf;

        udp::endpoint sender_endpoint;

        size_t len = 0;

        while (len == 0)
        {
            boost::system::error_code err;

            len = socket.receive_from(
                boost::asio::buffer(recv_buf),
                sender_endpoint,
                0,
                err
            );

            if (err == boost::asio::error::would_block ||
                err == boost::asio::error::try_again)
            {
                // Nothing received yet.
                // Give the OS a tiny amount of time before trying again.
                continue;
            }

            if (err)
            {
                throw boost::system::system_error(err);
            }
        }

        // --------------------------------------------------
        // Parse initial Join message
        // --------------------------------------------------
        std::string initial_message(
            recv_buf.data(),
            len
        );

        std::cout
            << "Receiving initial data: "
            << initial_message
            << "\n";

        std::istringstream iss(initial_message);

        std::string message_type;
        std::string token;

        std::getline(
            iss,
            message_type,
            ','
        );

        if (message_type != "Join")
        {
            std::cerr
                << "Expected Join message but received: "
                << message_type
                << "\n";

            return 1;
        }

        int playerId = -1;
        int xLocation = 0;

        // First player ID
        if (std::getline(iss, token, ','))
        {
            playerId = std::stoi(token);
        }

        // First player X
        if (std::getline(iss, token, ','))
        {
            xLocation = std::stoi(token);
        }

        std::cout
            << "My player ID: "
            << playerId
            << "\n";

        std::cout
            << "My starting X: "
            << xLocation
            << "\n";

        // --------------------------------------------------
        // Create game
        // --------------------------------------------------
        Game game(playerId);

        game.addPlayer(
            TempPlayer{
                playerId,
                Rectangle{
                    float(xLocation),
                    200.0f,
                    40.0f,
                    40.0f
                }
            }
        );

        // --------------------------------------------------
        // Create window
        // --------------------------------------------------
        InitWindow(
            800,
            800,
            "Walking through the past"
        );

        SetTargetFPS(60);

        // --------------------------------------------------
        // Main game loop
        // --------------------------------------------------
        while (!WindowShouldClose())
        {
            // ==================================================
            // 1. RECEIVE NETWORK DATA
            //
            // Non-blocking:
            // If there is no packet, we simply continue.
            // ==================================================

            for (;;)
            {
                boost::system::error_code err;

                size_t received_len = socket.receive_from(
                    boost::asio::buffer(recv_buf),
                    sender_endpoint,
                    0,
                    err
                );

                // No packet available.
                if (err == boost::asio::error::would_block ||
                    err == boost::asio::error::try_again)
                {
                    break;
                }

                if (err)
                {
                    std::cerr
                        << "receive_from error: "
                        << err.message()
                        << "\n";

                    break;
                }

                if (received_len == 0)
                {
                    break;
                }

                // ==================================================
                // Parse received message
                // ==================================================

                std::string received_message(
                    recv_buf.data(),
                    received_len
                );

                std::cout
                    << "RECEIVED: "
                    << received_message
                    << "\n";

                std::istringstream message_stream(
                    received_message
                );

                std::string received_type;
                std::string token1;

                std::getline(
                    message_stream,
                    received_type,
                    ','
                );

                // ==================================================
                // JOIN
                // ==================================================

                if (received_type == "Join")
                {
                    while (
                        std::getline(
                            message_stream,
                            token1,
                            ','
                        )
                    )
                    {
                        int temp_id = std::stoi(token1);

                        if (!std::getline(
                                message_stream,
                                token1,
                                ','))
                        {
                            break;
                        }

                        int locationX =
                            std::stoi(token1);

                        if (!std::getline(
                                message_stream,
                                token1,
                                ','))
                        {
                            break;
                        }

                        int locationY =
                            std::stoi(token1);

                        std::cout
                            << "Join player "
                            << temp_id
                            << " X="
                            << locationX
                            << " Y="
                            << locationY
                            << "\n";

                        // Do not add ourselves twice.
                        if (temp_id != playerId)
                        {
                            game.addPlayer(
                                TempPlayer{
                                    temp_id,
                                    Rectangle{
                                        float(locationX),
                                        float(locationY),
                                        40.0f,
                                        40.0f
                                    }
                                }
                            );
                        }
                    }
                }

                // ==================================================
                // MOVE
                // ==================================================

                else if (received_type == "Move")
                {
                    while (
                        std::getline(
                            message_stream,
                            token1,
                            ','
                        )
                    )
                    {
                        if (token1.empty())
                        {
                            break;
                        }

                        int temp_id =
                            std::stoi(token1);

                        if (!std::getline(
                                message_stream,
                                token1,
                                ','))
                        {
                            break;
                        }

                        int locationX =
                            std::stoi(token1);

                        if (!std::getline(
                                message_stream,
                                token1,
                                ','))
                        {
                            break;
                        }

                        int locationY =
                            std::stoi(token1);

                        std::cout
                            << "Player "
                            << temp_id
                            << " X="
                            << locationX
                            << " Y="
                            << locationY
                            << "\n";

                        // Update the player received from server.
                        game.updatePlayerbyId(
                            temp_id,
                            Rectangle{
                                float(locationX),
                                float(locationY),
                                40.0f,
                                40.0f
                            }
                        );
                    }
                }
            }

            // ==================================================
            // 2. UPDATE LOCAL GAME
            // ==================================================

            float deltaTime = GetFrameTime();

            game.tick(deltaTime);

            // ==================================================
            // 3. SEND OUR CURRENT POSITION
            // ==================================================

            Rectangle myPlayer =
                game.getPlayerLocationById(playerId);

            std::string update_message =
                "update," +
                std::to_string(playerId) +
                "," +
                std::to_string(myPlayer.x) +
                "," +
                std::to_string(myPlayer.y);

            boost::system::error_code send_error;

            socket.send_to(
                boost::asio::buffer(update_message),
                receiver_endpoint,
                0,
                send_error
            );

            if (send_error)
            {
                std::cerr
                    << "send_to error: "
                    << send_error.message()
                    << "\n";
            }
        }

        // --------------------------------------------------
        // Disconnect
        // --------------------------------------------------

        boost::system::error_code disconnect_error;

        socket.send_to(
            boost::asio::buffer("Disconnect"),
            receiver_endpoint,
            0,
            disconnect_error
        );

        if (disconnect_error)
        {
            std::cerr
                << "Disconnect error: "
                << disconnect_error.message()
                << "\n";
        }

        CloseWindow();
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Exception: "
            << e.what()
            << "\n";
    }

    return 0;
}