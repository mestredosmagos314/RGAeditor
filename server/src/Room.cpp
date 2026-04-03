#include "Room.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

#include "Operation.hpp"
#include "Session.hpp"

using json = nlohmann::json;

namespace rgaeditor {

Room::Room() : rga_(0) {}

auto Room::join(const std::shared_ptr<Session>& session) -> void {
  sessions_.insert(session);
  std::cout << "[ROOM] New client connected. Total: " << sessions_.size() << '\n';
}

auto Room::leave(const std::shared_ptr<Session>& session) -> void {
  sessions_.erase(session);
  std::cout << "[ROOM] Client disconnected. Total: " << sessions_.size() << '\n';
}

auto Room::processMessage(const std::string& message, const std::shared_ptr<Session>& sender) -> void {
  try {
    const json json_received = json::parse(message);
    const auto operation = json_received.get<Operation>();

    if (operation.type == OpType::Insert) {
      rga_.insertRemote(operation.value, operation.id, operation.origin_left);
    } else {
      rga_.deleteRemote(operation.id);
    }

    std::cout << "[ROOM] Text: " << rga_.toString() << '\n';

    broadcast(message, sender);

  } catch (const std::exception& exception) {
    std::cerr << "[ROOM] Error processing message: " << exception.what() << '\n';
  }
}

auto Room::broadcast(const std::string& message, const std::shared_ptr<Session>& sender) -> void {
  for (const auto& session : sessions_) {
    if (session != sender) {
      session->send(message);
    }
  }
}

}  // namespace rgaeditor