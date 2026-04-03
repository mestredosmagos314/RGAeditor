#include "Session.hpp"

#include <iostream>

#include "Room.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;

namespace rgaeditor {

Session::Session(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<Room> room)
    : websocket_(std::move(socket)), room_(std::move(room)) {}

Session::~Session() {
  std::cout << "[SESSION] Conection finished.\n";
}

auto Session::run() -> void {
  websocket_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

  websocket_.set_option(websocket::stream_base::decorator(
      [](websocket::response_type& response) { response.set(beast::http::field::server, "RGAeditor"); }));

  websocket_.async_accept(beast::bind_front_handler(&Session::onAccept, shared_from_this()));
}

auto Session::onAccept(beast::error_code error_code) -> void {
  if (error_code) {
    std::cerr << "[SESSION] Error at accept: " << error_code.message() << '\n';
    return;
  }

  room_->join(shared_from_this());
  doRead();
}

auto Session::doRead() -> void {
  websocket_.async_read(buffer_, beast::bind_front_handler(&Session::onRead, shared_from_this()));
}

auto Session::onRead(beast::error_code error_code, std::size_t bytes_transferred) -> void {
  boost::ignore_unused(bytes_transferred);

  if (error_code == websocket::error::closed) {
    room_->leave(shared_from_this());
    return;
  }

  if (error_code) {
    std::cerr << "[SESSION] Erro no read: " << error_code.message() << '\n';
    room_->leave(shared_from_this());
    return;
  }

  const std::string message = beast::buffers_to_string(buffer_.data());
  buffer_.consume(buffer_.size());

  room_->processMessage(message, shared_from_this());
  doRead();
}

auto Session::send(const std::string& message) -> void {
  net::post(websocket_.get_executor(), beast::bind_front_handler([self = shared_from_this(), message]() {
              const bool is_writing = !self->write_queue_.empty();
              self->write_queue_.push(message);

              if (!is_writing) {
                self->doWrite();
              }
            }));
}

auto Session::doWrite() -> void {
  websocket_.text(true);
  websocket_.async_write(net::buffer(write_queue_.front()),
                         beast::bind_front_handler(&Session::onWrite, shared_from_this()));
}

auto Session::onWrite(beast::error_code error_code, std::size_t bytes_transferred) -> void {
  boost::ignore_unused(bytes_transferred);

  if (error_code) {
    std::cerr << "[SESSION] Error at write: " << error_code.message() << '\n';
    return;
  }

  write_queue_.pop();

  if (!write_queue_.empty()) {
    doWrite();
  }
}

}  // namespace rgaeditor