#include <iostream>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "../config.h"

using namespace boost::asio;

int main() {
    try {
        io_context io_context{};
        ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v6(), SERVER_PORT));
        while (true) {
            std::cout << "Waiting for connection...\n";
            ip::tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Connection established\n";
            
            std::string message;
            
            std::cout << "Reading message...\n"; 
            boost::asio::read_until(socket, dynamic_buffer(message), "\n");
            std::cout << "Message is read: \"" << message << "\"\n";
            boost::algorithm::to_upper(message);
            
            std::cout << "Writing back...\n";
            boost::asio::write(socket, buffer(message));
            std::cout << "Message is sent: \"" << message << "\"\n";
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
