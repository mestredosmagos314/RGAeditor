# Converge

> **Low latency real-time collaborative text editor backend in C++23 using CRDTs (RGA) and WebSockets.**

**Project Status:** *In early development (Architecture & Core Logic implementation).*

## About
**RGAeditor** is a distributed system designed to handle real-time text collaboration with eventual consistency. Unlike traditional locking mechanisms, RGAeditor uses **Conflict-free Replicated Data Types (CRDTs)** to allow multiple users to edit a document simultaneously without conflicts, ensuring mathematical convergence of the document state across all clients.

The goal is to demonstrate high-performance engineering in C++, handling concurrency, memory management, and low-level networking.

## Tech Stack

### Backend (Core Engineering)
*   **Language:** C++23 (Modern C++ features, Smart Pointers, Move Semantics)
*   **Algorithm:** Custom implementation of **RGA (Replicated Growable Array)** for conflict resolution.
*   **Optimization:** Indexable Skip List (planned) for $O(\log N)$ mapping between visual indices and CRDT nodes.
*   **Networking:**
    *   **Boost.Beast:** High-performance asynchronous WebSocket & HTTP server.
    *   **Boost.Asio:** Non-blocking I/O model.

### Frontend (Client)
*   **Framework:** React + Vite (Fast HMR and bundling).
*   **Editor Engine:** **TipTap** (based on ProseMirror) for headless rich-text state management.
*   **Communication:**
    *   Optimistic UI updates for zero-latency typing feel.
    *   Custom JSON protocol over WebSockets for operation synchronization.

### Build & Tooling
*   **Build System:** CMake.
*   **Package Manager:** Vcpkg.
*   **Quality Control:** Clang-Tidy (Static Analysis), Google Test (Unit Testing).


## Getting Started

*Instructions for building and running the server will be added soon.*

*Created by Caio Monteiro*