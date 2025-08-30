#include "Server.h"

#include <iostream>

#include <boost/algorithm/string.hpp>

#include "../../../config.h"
#include "Session.h"

using namespace boost::asio;

////////////////////////////////////////////////////////////////////////////////
// public

Server::Server(const int max_session_count)
: max_session_count_{max_session_count},
  io_context_{max_session_count},
  strand_{io_context_.get_executor()},
  acceptor_{io_context_, ip::tcp::endpoint(ip::tcp::v4(), SERVER_PORT)} {
    logger_ = std::make_shared<Logger>(strand_);
    
    for (int i = max_session_count; i; --i)
        boost::asio::post(
            thread_pool_, [this]() { io_context_.run(); });
}

Server::~Server() {
    thread_pool_.join();
}

void Server::start() {
    do_accept();
}

void Server::stop() {
    flag_work_ = false;
    acceptor_.cancel();
}

////////////////////////////////////////////////////////////////////////////////
// private

void Server::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, ip::tcp::socket socket) {
            on_accept(ec, std::move(socket));
        });
}

void Server::on_accept(boost::system::error_code ec, ip::tcp::socket socket) {
    if (!flag_work_) {
        socket.shutdown(ip::tcp::socket::shutdown_both);
        socket.close();
        return;
    }
    if (ec)
        logger_->log_error(ec.what());
    else {
        auto new_session = Session::create(std::ref(io_context_), std::move(socket), logger_);
        new_session->start();
    }
    do_accept();
}
