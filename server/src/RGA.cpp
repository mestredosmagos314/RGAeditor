#include "RGA.hpp"

#include <algorithm>
#include <stdexcept>

namespace rgaeditor {

RGA::RGA(uint32_t client_id) : client_id_(client_id), clock_(0) {
  CrdtId head_id = {0, 0};
  head_ = std::make_shared<Node>('\0', head_id, head_id);
  lookup_[head_id] = head_;
}

auto RGA::insertLocal(char value, CrdtId origin_left) -> std::shared_ptr<Node> {
  clock_++;
  CrdtId new_id = {clock_, client_id_};
  auto new_node = std::make_shared<Node>(value, new_id, origin_left);

  integrate(new_node);

  return new_node;
}

auto RGA::insertRemote(char value, CrdtId new_id, CrdtId origin_left) -> void {
  clock_ = std::max(clock_, new_id.clock);
  auto new_node = std::make_shared<Node>(value, new_id, origin_left);

  integrate(new_node);
}

auto RGA::deleteLocal(CrdtId target_id) -> void {
  auto iterator = lookup_.find(target_id);
  if (iterator != lookup_.end()) {
    iterator->second->is_deleted = true;
  }
}

auto RGA::deleteRemote(CrdtId target_id) -> void {
  auto iterator = lookup_.find(target_id);
  if (iterator != lookup_.end()) {
    iterator->second->is_deleted = true;
  }
}

auto RGA::toString() const -> std::string {
  std::string text;
  auto curr = head_->next;

  while (curr != nullptr) {
    if (!curr->is_deleted) {
      text += curr->value;
    }
    curr = curr->next;
  }

  return text;
}

auto RGA::integrate(const std::shared_ptr<Node>& new_node) -> void {
  auto iterator = lookup_.find(new_node->origin_left);
  if (iterator == lookup_.end()) {
    throw std::runtime_error("Causality error: origin_left was not found.");
  }

  auto prev = iterator->second;
  auto curr = prev->next;

  while (curr != nullptr && new_node->id < curr->id) {
    prev = curr;
    curr = curr->next;
  }

  new_node->next = curr;
  prev->next = new_node;

  lookup_[new_node->id] = new_node;
}

}  // namespace rgaeditor