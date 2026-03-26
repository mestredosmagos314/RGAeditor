#include <iostream>

#include "RGA.hpp"
#include "RGAStructs.hpp"

using namespace rgaeditor;

auto main() -> int {
  std::cout << "--- Testing RGA CRDT (with insertions and deletions) ---\n\n";

  RGA client1(1);
  RGA client2(2);
  CrdtId head_id = {0, 0};

  std::cout << "[1] Client 1 types 'CASA'\n";
  auto num1 = client1.insertLocal('C', head_id);
  auto num2 = client1.insertLocal('A', num1->id);
  auto num3 = client1.insertLocal('S', num2->id);
  auto num4 = client1.insertLocal('A', num3->id);

  std::cout << "Text C1: " << client1.toString() << "\n\n";

  std::cout << "[2] Synchronizing edits to Client 2...\n";
  client2.insertRemote('C', num1->id, head_id);
  client2.insertRemote('A', num2->id, num1->id);
  client2.insertRemote('S', num3->id, num2->id);
  client2.insertRemote('A', num4->id, num3->id);

  std::cout << "Text C2: " << client2.toString() << "\n\n";

  std::cout << "[3] --- SIMULATING NETWORK DROP ---\n";
  std::cout << "Client 1 deletes 'S' (from CASA)\n";
  client1.deleteLocal(num3->id);

  std::cout << "Client 2, concurrently, inserts an 'H' right after 'S'\n";
  auto n5_c2 = client2.insertLocal('H', num3->id);

  std::cout << "Text C1 during disconnection: " << client1.toString() << " (C1 deleted)\n";
  std::cout << "Text C2 during disconnection: " << client2.toString() << " (C2 inserted)\n\n";

  std::cout << "[4] --- NETWORK RESTORED ---\n";
  std::cout << "Integrating edits...\n";

  client1.insertRemote('H', n5_c2->id, num3->id);

  client2.deleteRemote(num3->id);

  std::string text_c1 = client1.toString();
  std::string text_c2 = client2.toString();

  std::cout << "Final Text C1: " << text_c1 << "\n";
  std::cout << "Final Text C2: " << text_c2 << "\n\n";

  if (text_c1 == text_c2) {
    std::cout << "SUCCESS! Text converged to: " << text_c1 << "\n";
    std::cout << "Notice that the 'S' disappeared, but served as an anchor (Tombstone) for the 'H'.\n";
  } else {
    std::cout << "FAILURE! States diverge.\n";
  }

  return 0;
}