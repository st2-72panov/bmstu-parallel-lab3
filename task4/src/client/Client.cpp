#include "Client.h"

#include <iostream>

#include <boost/asio.hpp>

using namespace boost::asio;

////////////////////////////////////////////////////////////////////////////////
// public

Client::Client(): socket_{io_context_} {}

void Client::start_session_with(boost::asio::ip::tcp::endpoint endpoint) {
    try {
        connect_to(endpoint);
    
        int n = 42;  // example
        os << n << "\n";
        do_write();
        
        do_read_line();
        int response;
        is >> response;
        log("Result: " + std::to_string(response));
    }  catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
// private

inline void Client::log(std::string message) {
    std::cout << message << '\n';
}

void Client::connect_to(boost::asio::ip::tcp::endpoint endpoint) {
    log("Connecting...");
    socket_.connect(endpoint);
    log("Connection established");
}

size_t Client::do_write() {
    log("Sending request...");
    auto size = boost::asio::write(socket_, stream_buffer);
    log("Request sent");
    return size;
}

size_t Client::do_read_line() {
    log("Waiting for response...");
    auto size = boost::asio::read_until(socket_, stream_buffer, '\n');
    log("Response received");
    return size;
}
