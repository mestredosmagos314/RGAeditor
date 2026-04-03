#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <memory>

#include "Listener.hpp"
#include "Room.hpp"

auto main() -> int {
  try {
    const auto address = boost::asio::ip::make_address("0.0.0.0");
    constexpr uint16_t port = 8080;

    boost::asio::io_context io_context{1};

    const auto room = std::make_shared<rgaeditor::Room>();

    std::make_shared<rgaeditor::Listener>(io_context, boost::asio::ip::tcp::endpoint{address, port}, room)->run();

    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](const boost::system::error_code& /*error*/, int /*signal_number*/) { io_context.stop(); });

    std::cout << "--- RGA Editor Backend C++ ---\n";
    std::cout << "Websocker server running at port " << port << "...\n";

    io_context.run();

    return 0;

  } catch (const std::exception& exception) {
    std::cerr << "Fatal error: " << exception.what() << '\n';
    return 1;
  }
}