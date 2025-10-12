# ♟️ C++ Chess Engine

A high-performance **chess engine written in C++**, featuring **bitboards**, **alpha-beta pruning**, **transposition tables**, **quiescence search**, and **magic bitboards** for blazing-fast move generation.  
It also includes an **SFML-based GUI** for playing and visualizing games against the engine.

---

## Features

### Core Engine
- **Bitboard Representation:** Efficient move generation for all pieces using 64-bit bitboards.  
- **Magic Bitboards:** Optimized sliding move generation (rook, bishop, queen) using precomputed magic numbers.  
- **Alpha-Beta Pruning:** Reduces the search space drastically compared to plain minimax.  
- **Quiescence Search:** Extends captures to avoid horizon effect.  
- **Delta Pruning:** Skips hopeless captures during quiescence for faster evaluation.  
- **Transposition Table:** Caches previously searched positions to eliminate redundant computations.  
  - Stores: Zobrist hash, depth, score, flag (`TT_EXACT`, `TT_ALPHA`, `TT_BETA`), and best move.  
  - Correct handling of bounds for fail-high/fail-low cutoffs.  
- **Move Ordering:** (In progress) Moves are sorted using basic heuristics to improve alpha-beta efficiency.  
- **Perft Testing:** Validates correctness of move generation and search.  

---

### Graphical Interface
- **Built with SFML (C++17)**  
- Displays the chessboard, pieces, and highlights legal moves.  
- Allows human vs. engine interaction.  
- Syncs directly with the engine’s backend for move validation and state updates.  

---

## Building

### Requirements
- **C++17 or later**
- **CMake ≥ 3.16**
- **SFML 3.x**
- A modern compiler (e.g., `g++`, `clang++`)

### Build Instructions
```bash
git clone https://github.com/<your-username>/Chess-Website.git
cd Chess-Website
mkdir build && cd build
cmake ..
make
./chess_gui
