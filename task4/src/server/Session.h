#pragma once
#include <memory>

#include <boost/asio.hpp>

#include "Logger.h"

class Session : public std::enable_shared_from_this<Session>  {
public:
    static std::shared_ptr<Session> create(
        boost::asio::io_context& io_context,
        boost::asio::ip::tcp::socket&& socket,
        std::shared_ptr<Logger> logger); 
    void start();

private:
    const std::chrono::seconds OPERATION_TIME_LIMIT{30};
    boost::asio::io_context& io_context_;
    boost::asio::streambuf stream_buffer;
    std::istream is{&stream_buffer};
    std::ostream os{&stream_buffer};
    boost::asio::ip::tcp::socket socket_;
    boost::asio::steady_timer timeout_timer_{io_context_};
    const std::shared_ptr<Logger> logger_;

    Session(
        boost::asio::io_context& io_context,
        boost::asio::ip::tcp::socket&& socket,
        std::shared_ptr<Logger> logger);
    void step_read_number();
    void step_handle_request();
    void step_write_answer();
    double do_calculation(int n);
};
