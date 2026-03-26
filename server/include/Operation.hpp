#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "RGAStructs.hpp"

namespace rgaeditor {

enum class OpType { Insert, Delete };

struct Operation {
  OpType type;
  char value;
  CrdtId id;
  CrdtId origin_left;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CrdtId, clock, client_id)

inline auto to_json(nlohmann::json& json_data, const Operation& operation)  // NOLINT(readability-identifier-naming)
    -> void {
  json_data = nlohmann::json{{"type", operation.type == OpType::Insert ? "insert" : "delete"},
                             {"value", std::string(1, operation.value)},
                             {"id", operation.id},
                             {"origin_left", operation.origin_left}};
}

inline auto from_json(const nlohmann::json& json_data, Operation& operation)  // NOLINT(readability-identifier-naming)
    -> void {
  std::string type_str = json_data.at("type").get<std::string>();
  operation.type = (type_str == "insert") ? OpType::Insert : OpType::Delete;

  std::string val_str = json_data.at("value").get<std::string>();
  operation.value = val_str.empty() ? '\0' : val_str[0];

  json_data.at("id").get_to(operation.id);
  json_data.at("origin_left").get_to(operation.origin_left);
}

}  // namespace rgaeditor