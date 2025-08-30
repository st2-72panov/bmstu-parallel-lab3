#pragma once
#include <boost/asio.hpp>

class Client {
public:
    explicit Client();
    void start_session_with(boost::asio::ip::tcp::endpoint endpoint);

private:
    boost::asio::streambuf stream_buffer;
    std::istream is{&stream_buffer};
    std::ostream os{&stream_buffer};
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;

    inline void log(std::string message);
    void connect_to(boost::asio::ip::tcp::endpoint endpoint);
    size_t do_write();
    size_t do_read_line();
};
