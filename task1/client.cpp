#include <iostream>

#include <boost/asio.hpp>

#include "../config.h"

using namespace boost::asio;

int main() {
    try {
        io_context io_context{};
        ip::tcp::resolver resolver{io_context};

        auto address = boost::asio::ip::make_address(SERVER_IP);
        boost::asio::ip::tcp::endpoint endpoint(address, SERVER_PORT);
        
        std::cout << "Connecting...\n";
        ip::tcp::socket socket(io_context);
        socket.connect(endpoint);
        std::cout << "Connected to: \"" << endpoint.address() << "\"\n";
        
        std::string message = "Hello Moto\n";
        
        std::cout << "Sending message...\n";
        boost::asio::write(socket, buffer(message));
        std::cout << "Message is sent: \"" << message << "\"\n";

        std::cout << "Waiting for reply...\n";
        boost::asio::read(socket, buffer(message));
        std::cout << "Message is received: \"" << message << "\"\n";

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
