#include "Logger.h"

#include <iostream>

Logger::Logger(
    boost::asio::strand<boost::asio::io_context::executor_type> strand)
: strand_{strand} {}

// thread safe way to log messages
void Logger::log(std::string message) { 
    boost::asio::post(strand_, 
        [message = std::move(message)]() { std::cout << message << '\n'; });
}

// thread safe way to log error messages
void Logger::log_error(std::string error) { 
    boost::asio::post(strand_, 
        [error = std::move(error)]() { std::cerr << error << '\n'; });
}
