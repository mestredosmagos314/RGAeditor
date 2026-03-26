#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>

#include "Operation.hpp"
#include "RGA.hpp"

using json = nlohmann::json;

auto main() -> int {
  try {
    std::cout << "--- RGA Editor & JSON Test ---\n\n";

    // 1. Setup two clients
    rgaeditor::RGA alice(1);
    rgaeditor::RGA bob(2);

    // 2. Alice types 'A' at the initial position
    rgaeditor::CrdtId head_id = {0, 0};
    auto node_a = alice.insertLocal('A', head_id);

    // 3. Alice creates the operation payload
    rgaeditor::Operation op_alice_insert = {rgaeditor::OpType::Insert, node_a->value, node_a->id, node_a->origin_left};

    // 4. Convert C++ object to String (to send over the network)
    json json_payload = op_alice_insert;
    std::string json_string = json_payload.dump(4);

    std::cout << "[NETWORK] Alice sent the following JSON:\n";
    std::cout << json_string << "\n\n";

    // (The JSON travels through the internet to Bob's computer)

    json json_received = json::parse(json_string);
    auto op_received = json_received.get<rgaeditor::Operation>();

    if (op_received.type == rgaeditor::OpType::Insert) {
      bob.insertRemote(op_received.value, op_received.id, op_received.origin_left);
    }

    std::cout << "Alice's text: " << alice.toString() << "\n";
    std::cout << "Bob's text:   " << bob.toString() << "\n";

    if (alice.toString() == bob.toString()) {
      std::cout << "\nSUCCESS! The CRDT and JSON serialization are working in harmony.\n";
    }

    return 0;

  } catch (const std::exception& e) {
    std::cerr << "Fatal server error: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Unknown fatal error.\n";
    return 1;
  }
}