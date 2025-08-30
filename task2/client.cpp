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
        
        std::string output = "1 88 5 13\n";
        
        std::cout << "Sending message...\n";
        boost::asio::write(socket, buffer(output));
        std::cout << "Message is sent: \"" << output << "\"\n";

        std::cout << "Waiting for reply...\n";
        std::string input;
        boost::asio::read_until(socket, dynamic_buffer(input), '\n');
        std::cout << "Message is received: \"" << input << "\"\n";

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
