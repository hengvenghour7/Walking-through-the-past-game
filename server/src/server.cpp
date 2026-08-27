#include <iostream>
#include <boost/asio.hpp>
#include <vector>

using boost::asio::ip::udp;

int main()
{
    int playerID{0};
    struct playerInfo {
        int id;
        int x;
        int y;
    };
    int x{300};
    std::vector<playerInfo> allPlayerInfo{};
    try
    {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 5000));
        for (;;)
        {
            std::array<char, 1024> recv_buf;
            udp::endpoint remote_endpoint;
            std::string message = "Message from the server haha";
            boost::system::error_code err;
            std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            if (len > 0)
            {
                std::string rec_message(recv_buf.data(), len);
                if (rec_message == "Connect")
                {
                    std::cout<< "new player join the game" << std::endl;
                    playerID++;
                    std::string tempMsg = std::to_string(playerID) + "," + "3";
                    socket.send_to(boost::asio::buffer(tempMsg), remote_endpoint, 0, err);
                }
                // std::cout << rec_message << std::endl;
            }
            
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, err);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}