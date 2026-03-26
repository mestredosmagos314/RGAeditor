#pragma once

#include <cstdint>
#include <memory>

namespace rgaeditor {

struct CrdtId {
  uint64_t clock;
  uint32_t client_id;

  auto operator<(const CrdtId& other) const -> bool {
    if (clock != other.clock) {
      return clock < other.clock;
    }
    return client_id < other.client_id;
  }

  auto operator==(const CrdtId& other) const -> bool {
    return clock == other.clock && client_id == other.client_id;
  }
};

struct Node {
  char value;
  CrdtId id;
  CrdtId origin_left;

  bool is_deleted = false;

  std::shared_ptr<Node> next = nullptr;

  Node(char val, CrdtId my_id, CrdtId parent)  // NOLINT(bugprone-easily-swappable-parameters)
      : value(val), id(my_id), origin_left(parent) {}
};

}  // namespace rgaeditor