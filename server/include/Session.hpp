#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <queue>
#include <string>

namespace rgaeditor {

class Room;

class Session : public std::enable_shared_from_this<Session> {
private:
  boost::beast::websocket::stream<boost::beast::tcp_stream> websocket_;
  std::shared_ptr<Room> room_;
  boost::beast::flat_buffer buffer_;
  std::queue<std::string> write_queue_;

public:
  explicit Session(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<Room> room);
  ~Session();

  Session(const Session&) = delete;
  auto operator=(const Session&) -> Session& = delete;

  Session(Session&&) = delete;
  auto operator=(Session&&) -> Session& = delete;

  auto run() -> void;
  auto send(const std::string& message) -> void;

private:
  auto onAccept(boost::beast::error_code error_code) -> void;
  auto doRead() -> void;
  auto onRead(boost::beast::error_code error_code, std::size_t bytes_transferred) -> void;
  auto doWrite() -> void;
  auto onWrite(boost::beast::error_code error_code, std::size_t bytes_transferred) -> void;
};

}  // namespace rgaeditor