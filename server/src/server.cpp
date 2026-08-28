#include <iostream>
#include <boost/asio.hpp>
#include <vector>

#include "playersHandler/playersHandler.h"

using boost::asio::ip::udp;

int main()
{
    int playerID{0};
    struct playerInfo {
        int id;
        int x;
        int y;
    };
    PlayersHandler playersHandler;
    std::vector<playerInfo> allPlayerInfo{};

    try
    {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 5000));
        for (;;)
        {
            std::array<char, 1024> recv_buf;
            udp::endpoint remote_endpoint;
            boost::system::error_code err;
            std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            if (len > 0)
            {
                std::string rec_message(recv_buf.data(), len);
                if (rec_message == "Connect")
                {
                    std::cout<< "new player join the game" << std::endl;
                    playersHandler.addNewPlayer(&playerID);
                    std::string tempMsg = "Join" + std::string(",", 1) + playersHandler.getDataAsString();
                    std::cout << rec_message << std::endl;
                    socket.send_to(boost::asio::buffer(tempMsg), remote_endpoint, 0, err);
                }
                else{
                    std::istringstream iss(rec_message);
                    std::string token1;
                    std::string messageType;
                    std::getline(iss, messageType, ',');
                    if (messageType == "update")
                    {
                        while (std::getline(iss, token1, ','))
                        {
                            int temp_id = std::stoi(token1);
                            std::getline(iss, token1, ',');
                            int temp_x = std::stoi(token1);
                            std::cout<< "x location" << temp_x << std::flush;
                            std::getline(iss, token1, ',');
                            int temp_y = std::stoi(token1);
                            playersHandler.updatePlayerById(temp_id, SRectangle{temp_x, temp_y, 40, 40});
                        }
                    }
                    std::string tempMsg = "Move" + std::string(",", 1) + playersHandler.getDataAsString();
                    socket.send_to(boost::asio::buffer(tempMsg), remote_endpoint, 0, err);
                }
                // std::cout << rec_message << std::endl;
            }
            
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}