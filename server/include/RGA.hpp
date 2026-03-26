#pragma once

#include <map>
#include <memory>
#include <string>

#include "RGAStructs.hpp"

namespace rgaeditor {

class RGA {
private:
  uint32_t client_id_;
  uint64_t clock_;

  std::shared_ptr<Node> head_;
  std::map<CrdtId, std::shared_ptr<Node>> lookup_;

public:
  // Construtores não possuem tipo de retorno (nem auto)
  explicit RGA(uint32_t client_id);

  auto insertLocal(char value, CrdtId origin_left) -> std::shared_ptr<Node>;
  auto insertRemote(char value, CrdtId new_id, CrdtId origin_left) -> void;

  auto deleteLocal(CrdtId target_id) -> void;
  auto deleteRemote(CrdtId target_id) -> void;

  [[nodiscard]] auto toString() const -> std::string;

private:
  auto integrate(const std::shared_ptr<Node>& new_node) -> void;
};

}  // namespace rgaeditor