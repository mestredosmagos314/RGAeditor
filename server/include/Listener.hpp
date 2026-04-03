#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

namespace rgaeditor {

class Room;

class Listener : public std::enable_shared_from_this<Listener> {
private:
  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::shared_ptr<Room> room_;

public:
  explicit Listener(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& endpoint,
                    std::shared_ptr<Room> room);

  ~Listener() = default;

  Listener(const Listener&) = delete;
  auto operator=(const Listener&) -> Listener& = delete;
  Listener(Listener&&) = delete;
  auto operator=(Listener&&) -> Listener& = delete;

  auto run() -> void;

private:
  auto doAccept() -> void;
  auto onAccept(boost::system::error_code error_code, boost::asio::ip::tcp::socket socket) -> void;
};

}  // namespace rgaeditor