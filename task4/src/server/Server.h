#pragma once
#include <memory>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "Logger.h"

class Server {
public:
    explicit Server(const int max_session_count);
    ~Server();
    void start();
    void stop();

private:
    bool flag_work_{true};
    const int max_session_count_;
    boost::asio::io_context io_context_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::thread_pool thread_pool_;
    std::shared_ptr<Logger> logger_{nullptr};

    void do_accept();
    void on_accept(boost::system::error_code ec, boost::asio::ip::tcp::socket socket);
};
