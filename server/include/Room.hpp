#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>

#include "RGA.hpp"

namespace rgaeditor {

class Session;

class Room {
private:
  RGA rga_;
  std::unordered_set<std::shared_ptr<Session>> sessions_;
  uint32_t next_client_id_counter_{1};

public:
  Room();

  auto join(const std::shared_ptr<Session>& session) -> void;
  auto leave(const std::shared_ptr<Session>& session) -> void;
  auto processMessage(const std::string& message, const std::shared_ptr<Session>& sender) -> void;

private:
  auto broadcast(const std::string& message, const std::shared_ptr<Session>& sender) -> void;

  auto sendWelcomePackage(const std::shared_ptr<Session>& session, uint32_t assigned_id) -> void;
};

}  // namespace rgaeditor