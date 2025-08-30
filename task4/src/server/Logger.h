#pragma once
#include <string>

#include <boost/asio.hpp>

class Logger {
public:
    Logger(boost::asio::strand<boost::asio::io_context::executor_type> strand);
    void log(std::string message);
    void log_error(std::string error);
private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
};
