#include <gtest/gtest.h>

#include <limits>
#include <nlohmann/json.hpp>

#include "Operation.hpp"

using namespace rgaeditor;
using json = nlohmann::json;

// Part A: SERIALIZATION (C++ -> JSON)

// 1. HAPPY PATH: Normal insertion
TEST(JsonSerializeTest, HappyPathInsertOperation) {
  Operation op{OpType::Insert, 'A', {1, 100}, {0, 0}};

  json json_data = op;

  EXPECT_EQ(json_data["type"], "insert");
  EXPECT_EQ(json_data["value"], "A");
  EXPECT_EQ(json_data["id"]["clock"], 1);
  EXPECT_EQ(json_data["id"]["client_id"], 100);
}

// 2. EDGE CASE: Huge numbers
TEST(JsonSerializeTest, EdgeCaseMaxIntegerLimits) {
  constexpr uint64_t max_clock = std::numeric_limits<uint64_t>::max();
  constexpr uint32_t max_client = std::numeric_limits<uint32_t>::max();

  Operation op{OpType::Delete, '\0', {max_clock, max_client}, {0, 0}};

  json json_data = op;

  EXPECT_EQ(json_data["type"], "delete");
  EXPECT_EQ(json_data["id"]["clock"], max_clock);
  EXPECT_EQ(json_data["id"]["client_id"], max_client);
}

// 3. SAD PATH: Null Characters
TEST(JsonSerializeTest, EdgeCaseNullCharacter) {
  Operation op{OpType::Insert, '\0', {1, 1}, {0, 0}};

  json json_data = op;

  std::string value_str = json_data["value"];
  EXPECT_EQ(value_str.length(), 1);
  EXPECT_EQ(value_str[0], '\0');
}

// Part B: DESERIALIZATION (JSON -> C++)

// 4. HAPPY PATH: Read perfect JSON
TEST(JsonDeserializeTest, HappyPathValidJson) {
  json valid_json = {{"type", "insert"},
                     {"value", "Z"},
                     {"id", {{"clock", 5}, {"client_id", 42}}},
                     {"origin_left", {{"clock", 4}, {"client_id", 42}}}};

  auto op = valid_json.get<Operation>();

  EXPECT_EQ(op.type, OpType::Insert);
  EXPECT_EQ(op.value, 'Z');
  EXPECT_EQ(op.id.clock, 5);
  EXPECT_EQ(op.origin_left.clock, 4);
}

// 5. EDGE CASE: Read empty string
TEST(JsonDeserializeTest, EdgeCaseEmptyValueString) {
  json json_with_empty_val = {{"type", "delete"},
                              {"value", ""},
                              {"id", {{"clock", 1}, {"client_id", 1}}},
                              {"origin_left", {{"clock", 0}, {"client_id", 0}}}};

  auto op = json_with_empty_val.get<Operation>();

  EXPECT_EQ(op.type, OpType::Delete);
  EXPECT_EQ(op.value, '\0');
}

// 6. SAD PATH: Read Malformed JSON
TEST(JsonDeserializeTest, ErrorPathMissingKeysThrows) {
  json malformed_json = {{"type", "insert"}, {"id", {{"clock", 1}, {"client_id", 1}}}};

  EXPECT_THROW(malformed_json.get<Operation>(), nlohmann::json::out_of_range);
}