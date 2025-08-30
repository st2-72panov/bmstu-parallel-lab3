#include <iostream>

#include <boost/asio.hpp>

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

            std::cout << "Reading message...\n";
            std::string input;
            boost::asio::read_until(socket, dynamic_buffer(input), "\n");
            std::cout << "Message is read\n";

            int n;
            {
                std::istringstream iss{input};
                std::string first_word;
                iss >> first_word >> n;
            }
            
            std::cout << "Writing back...\n";
            std::string output = "Ready in " + std::to_string(n) + " sec\n";
            boost::asio::write(socket, buffer(output));
            std::cout << "Message is sent\n";
            
            boost::asio::steady_timer timer{io_context, std::chrono::seconds(n)};
            timer.async_wait(
                [&socket](auto) {
                    std::cout << "Writing back...\n";
                    std::string output = "Done!\n";
                    boost::asio::write(socket, buffer(output));
                    std::cout << "Message is sent\n";
                }
            );
            io_context.run();
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
