#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <chrono>

#include "playersHandler/playersHandler.h"

using boost::asio::ip::udp;

using steady_clock = std::chrono::steady_clock;

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
    auto nextTick = steady_clock::now();
    const auto tickRate = std::chrono::milliseconds(50);
    std::vector<udp::endpoint> allClientEndpoints{};

    try
    {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 5000));
        // socket.non_blocking(true);
        for (;;)
        {
            auto now = steady_clock::now();
                std::array<char, 1024> recv_buf;
                udp::endpoint remote_endpoint;
                boost::system::error_code err;
                std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
                if (len > 0)
                {
                    std::string rec_message(recv_buf.data(), len);
                    if (rec_message == "Connect")
                    {
                        playersHandler.addNewPlayer(&playerID);
                        std::string tempMsg = "Join" + std::string(",", 1) + playersHandler.getDataAsString();
                        socket.send_to(boost::asio::buffer(tempMsg), remote_endpoint, 0, err);
                        allClientEndpoints.push_back(remote_endpoint);
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
                                std::getline(iss, token1, ',');
                                int temp_y = std::stoi(token1);
                                playersHandler.updatePlayerById(temp_id, SRectangle{temp_x, temp_y, 40, 40});
                            }
                        }
                        
                    }
                }
                if (now >= nextTick)
                {
                    std::string tempMsg = "Move" + std::string(",", 1) + playersHandler.getDataAsString();
                        for (auto& endpoint: allClientEndpoints)
                        {
                            socket.send_to(boost::asio::buffer(tempMsg), endpoint, 0, err);
                        }
                    nextTick += tickRate;
                }
            
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}