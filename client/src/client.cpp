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

using boost::asio::ip::udp;

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint reciever_endpoint =
        *resolver.resolve(udp::v4(), argv[1], "5000").begin();
        udp::socket socket(io_context);
        socket.open(udp::v4());

        std::array<char, 1> send_buf = {{0}};
        socket.send_to(boost::asio::buffer(send_buf), reciever_endpoint);
        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::cout.write(recv_buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    InitWindow(800, 800, "Walking through the past");
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(ORANGE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}