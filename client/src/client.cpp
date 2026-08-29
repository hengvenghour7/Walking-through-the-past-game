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
        float tickGap{0.03};
        float serverUpdateTick{tickGap};

        socket.non_blocking(true);

        std::istringstream iss(std::string(recv_buf.data(), len));
        std::string messageType;
        std::string token1;
        std::getline(iss, messageType, ',');
        int playerId;
        int XLocation{};
        if (std::getline(iss, token1, ','))
        {
            playerId = std::stoi(token1);
            std::getline(iss, token1, ',');
            XLocation = std::stoi(token1);
        }
        Game game(playerId);
        
        game.addPlayer(TempPlayer{playerId, Rectangle{float(XLocation), 200, 40, 40}});
        
        std::cout.write(recv_buf.data(), len);
        InitWindow(800, 800, "Walking through the past");
        SetTargetFPS(60);
        while(!WindowShouldClose())
        {
            boost::system::error_code err;
            size_t len2 = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 0, err);
            std::istringstream iss(std::string(recv_buf.data(), len2));
            std::string messageType;
            std::string token1;
            std::getline(iss, messageType, ',');
            if (messageType == "Join") {
                if (std::getline(iss, token1, ','))
                {
                    int temp_Id2 = std::stoi(token1);
                    std::getline(iss, token1, ',');
                    XLocation = std::stoi(token1);
                    game.addPlayer(TempPlayer{temp_Id2, Rectangle{float(XLocation), 200, 40, 40}, {0,0}});
                }
            }
            if (serverUpdateTick >= tickGap)
            {
                if (messageType == "Move")
                {
                    while(std::getline(iss, token1, ','))
                    {
                        int temp_id = std::stoi(token1);
                        std::getline(iss, token1, ',');
                        int locationX = std::stoi(token1);
                        std::getline(iss, token1, ',');
                        int locationY = std::stoi(token1);
                        game.updatePlayerbyId(temp_id, Rectangle{float(locationX), float(locationY), 40, 40});
                        game.setIsUpdateFromtheServer(true);
                    }
                    std::cout<< "server updateddd" << std::endl;
                }
                serverUpdateTick = 0;
            }
            float deltaTime = GetFrameTime();
            game.tick(deltaTime);
            serverUpdateTick+=deltaTime;
            socket.send_to(boost::asio::buffer("update" + std::string(",", 1) + std::to_string(playerId) + "," + std::to_string(game.getPlayerLocationById(playerId).x) + "," + std::to_string(game.getPlayerLocationById(playerId).y)), reciever_endpoint);
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