// Prevent Windows API conflicts
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Avoid collisions
#define NOGDI           // This prevents GDI functions like Rectangle()
#define NOUSER          // This prevents user functions like CloseWindow, ShowCursor

// Include Raylib first
#include "raylib.h"
#include "raymath.h"

#include <array>
#include <iostream>
#include <boost/asio.hpp>

#include "game.h"

using boost::asio::ip::udp;

Game game;
int main(int argc, char *argv[])
{
    try
    {
        // if (argc != 2)
        // {
        //     std::cerr << "Usage: client <host>" << std::endl;
        //     return 1;
        // }
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint reciever_endpoint =
        *resolver.resolve(udp::v4(), "127.0.0.1", "5000").begin();
        udp::socket socket(io_context);
        socket.open(udp::v4());

        std::string send_buf = "Connect";
        socket.send_to(boost::asio::buffer(send_buf), reciever_endpoint);
        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout << "receiving initial data " << std::string(recv_buf.data(), len)  << "\n";
        std::istringstream iss(std::string(recv_buf.data(), len));
        std::string token1;
        int playerId;
        while(std::getline(iss, token1, ','))
        {
            playerId = std::stoi(token1);
            std::cout<< token1 << "\n";
        }
        std::cout.write(recv_buf.data(), len);
        InitWindow(800, 800, "Walking through the past");
        SetTargetFPS(60);
        Game game(playerId);
        while(!WindowShouldClose())
        {
            float deltaTime = GetFrameTime();
            game.tick(deltaTime);
            socket.send_to(boost::asio::buffer(playerId + "," + std::to_string(game.getPlayerLocation().x)), reciever_endpoint);
        }
        socket.send_to(boost::asio::buffer("Disconnect"), reciever_endpoint);
        CloseWindow();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    
    return 0;
}