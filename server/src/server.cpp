#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main()
{
    try
    {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 5000));
        for (;;)
        {
            std::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            std::string message = "Message from the server haha";
            boost::system::error_code err;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, err);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}