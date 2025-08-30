#include "Session.h"

#include <iostream>
#include <math.h>

using namespace boost::asio;

////////////////////////////////////////////////////////////////////////////////
// public

std::shared_ptr<Session> Session::create(
    boost::asio::io_context& io_context,
    boost::asio::ip::tcp::socket&& socket,
    std::shared_ptr<Logger> logger) 
{
    return std::shared_ptr<Session>{new Session(io_context, std::move(socket), logger)};
}

void Session::start() {
    logger_->log("starting session...");
    step_read_number();
}

////////////////////////////////////////////////////////////////////////////////
// private

Session::Session(
    boost::asio::io_context& io_context,
    boost::asio::ip::tcp::socket&& socket,
    std::shared_ptr<Logger> logger)
: io_context_{io_context}, socket_{std::move(socket)}, logger_{logger} {}

void Session::step_read_number() {
    logger_->log("reading number...");

    bool flag_done = false;
    auto self{shared_from_this()};
    boost::asio::async_read_until(
        socket_, stream_buffer, '\n',
        [this, self, &flag_done](boost::system::error_code ec, std::size_t length) {
            if (ec) {
                logger_->log_error(ec.what());
                return;
            }
            flag_done = true;
            logger_->log("reading done");
            step_handle_request();
        }
    );
    timeout_timer_.async_wait([this, self, &flag_done](boost::system::error_code ec) {
        if (ec || flag_done)
            return;
        socket_.cancel();
        socket_.shutdown(ip::tcp::socket::shutdown_both);
        socket_.close();
        logger_->log("reading failed: timeout reached");
    });
}

void Session::step_handle_request() {
    auto self{shared_from_this()};
    boost::asio::post(
        io_context_,
        [this, self]() {
            logger_->log("executing request...");
            int n;
            is >> n;

            std::string result = std::to_string(do_calculation(n));
            os << result << '\n';
            logger_->log("request executed");
            
            step_write_answer();
        }
    );
}

void Session::step_write_answer() {
    logger_->log("writing answer...");

    auto self{shared_from_this()};
    boost::asio::async_write(
        socket_, stream_buffer,
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (ec) {
                logger_->log_error(ec.what());
                return;
            }
            logger_->log("writing done\nsession is over");
        }
    );
}

double Session::do_calculation(int n) {
    logger_->log("doing calculations...");
    double result(n);
    for (int i = 100000000; i; --i)
        result += std::sin(std::log(i)) * std::cos(std::exp(n / i));;
    logger_->log("calculations are done");
    return result;
}
