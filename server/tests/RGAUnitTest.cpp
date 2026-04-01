#include <gtest/gtest.h>

#include <stdexcept>

#include "RGA.hpp"

using namespace rgaeditor;

// 1. HAPPY PATH: Normal insertion
TEST(RgaUnitTest, HappyPathSequentialInsertions) {
  constexpr uint32_t client_one = 1;
  constexpr uint64_t start_clock = 0;
  constexpr uint32_t start_client = 0;

  RGA doc(client_one);
  CrdtId start_id = {start_clock, start_client};

  auto node_a = doc.insertLocal('A', start_id);
  doc.insertLocal('B', node_a->id);

  EXPECT_EQ(doc.toString(), "AB");
}

// 2. EDGE CASE: Concurrent operations
TEST(RgaUnitTest, EdgeCaseConcurrentInsertions) {
  constexpr uint32_t client_alice = 1;
  constexpr uint32_t client_bob = 2;
  constexpr uint64_t start_clock = 0;
  constexpr uint32_t start_client = 0;

  RGA alice(client_alice);
  RGA bob(client_bob);
  CrdtId start_id = {start_clock, start_client};

  auto node_h = alice.insertLocal('H', start_id);
  bob.insertRemote(node_h->value, node_h->id, node_h->origin_left);

  auto node_a = alice.insertLocal('A', node_h->id);
  auto node_b = bob.insertLocal('B', node_h->id);

  alice.insertRemote(node_b->value, node_b->id, node_b->origin_left);
  bob.insertRemote(node_a->value, node_a->id, node_a->origin_left);

  EXPECT_EQ(alice.toString(), "HBA");
  EXPECT_EQ(bob.toString(), "HBA");
}

// 3. SAD PATH: Broken dependency
TEST(RgaUnitTest, ErrorPathMissingOriginLeftThrows) {
  constexpr uint32_t client_one = 1;
  RGA doc(client_one);

  constexpr char new_char = 'X';
  constexpr uint64_t new_clock = 2;
  constexpr uint32_t new_client = 2;
  CrdtId new_id = {new_clock, new_client};

  constexpr uint64_t fake_clock = 999;
  constexpr uint32_t fake_client_id = 999;
  CrdtId fake_origin = {fake_clock, fake_client_id};

  EXPECT_THROW(doc.insertRemote(new_char, new_id, fake_origin), std::runtime_error);
}