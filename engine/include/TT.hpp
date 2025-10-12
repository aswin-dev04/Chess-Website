/**
 * @file TT.hpp
 * @brief Defines the TranspositionTable class for the chess engine.
 * This file contains the data structures and class definition for the
 * transposition table, which is used to cache previously evaluated board
 * positions.
 */
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "move.hpp"
#include "types.hpp"
#include <cstdint>
#include <vector>

using u64 = std::uint64_t;

// Flags to indicate the type of score stored in a transposition table entry
enum TTFlag { TT_EXACT = 0, TT_ALPHA = 1, TT_BETA = 2 };

// Represents a single entry in the transposition table
struct TTEntry {
  u64 zobristKey; // The Zobrist hash of the board position
  int depth;      // The depth of the search that produced this entry
  int score;      // The evaluation score of the position
  TTFlag flag;    // The type of score (exact, alpha, or beta)
  Move bestMove;  // The best move found for this position

  TTEntry();
};

// A hash table that stores previously evaluated board positions
class TranspositionTable {
private:
  std::vector<TTEntry> table;
  size_t size;

public:
  TranspositionTable(size_t sizeMB = 64);
  // Stores a new entry in the transposition table
  void store(u64 hash, int depth, int score, TTFlag flag, Move bestMove);
  // Probes the transposition table for an existing entry
  bool probe(u64 hash, int depth, int alpha, int beta, int &score,
             Move &bestMove);
  // Clears the transposition table
  void clear();
  // Retrieves the best move for a given hash, if available
  Move getBestMove(u64 hash);
};

#endif
