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

            std::cout << "Calculating max number...n";
            int result;
            boost::asio::post(
                io_context,
                [&result, &input]() {
                    result = 0;
                    std::istringstream numbers{input};
                    int number;
                    while (numbers >> number)
                        if (number > result)
                            result = number;
                });
            io_context.run();
            std::cout << "Max number is calculated\n";

            std::cout << "Writing back...\n";
            std::string output = std::to_string(result) + "\n";
            boost::asio::write(socket, buffer(output));
            std::cout << "Message is sent\n";
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
