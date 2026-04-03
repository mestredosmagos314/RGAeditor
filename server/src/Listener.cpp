#include "Listener.hpp"

#include <boost/asio/strand.hpp>
#include <boost/beast/core/bind_handler.hpp>
#include <iostream>

#include "Room.hpp"
#include "Session.hpp"

namespace net = boost::asio;

namespace rgaeditor {

Listener::Listener(net::io_context& io_context, const net::ip::tcp::endpoint& endpoint, std::shared_ptr<Room> room)
    : io_context_(io_context), acceptor_(net::make_strand(io_context)), room_(std::move(room)) {
  boost::system::error_code error_code;

  acceptor_.open(endpoint.protocol(), error_code);
  if (error_code) {
    std::cerr << "[LISTENER] Error at open: " << error_code.message() << '\n';
    return;
  }

  acceptor_.set_option(net::socket_base::reuse_address(true), error_code);
  if (error_code) {
    std::cerr << "[LISTENER] Error at set_option: " << error_code.message() << '\n';
    return;
  }

  acceptor_.bind(endpoint, error_code);
  if (error_code) {
    std::cerr << "[LISTENER] Error at bind: " << error_code.message() << '\n';
    return;
  }

  acceptor_.listen(net::socket_base::max_listen_connections, error_code);
  if (error_code) {
    std::cerr << "[LISTENER] Error at listen: " << error_code.message() << '\n';
    return;
  }
}

auto Listener::run() -> void {
  if (!acceptor_.is_open()) {
    return;
  }
  doAccept();
}

auto Listener::doAccept() -> void {
  acceptor_.async_accept(net::make_strand(io_context_),
                         boost::beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
}

auto Listener::onAccept(boost::system::error_code error_code, net::ip::tcp::socket socket) -> void {
  if (error_code) {
    std::cerr << "[LISTENER] Error at accept: " << error_code.message() << '\n';
  } else {
    std::make_shared<Session>(std::move(socket), room_)->run();
  }

  doAccept();
}

}  // namespace rgaeditor