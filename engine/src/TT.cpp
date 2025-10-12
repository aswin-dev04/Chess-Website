/**
 * @file TT.cpp
 * @brief Implements the TranspositionTable class.
 * This file contains the implementation of the transposition table, which is
 * used to cache previously evaluated board positions to speed up the search.
 */
#include "../include/TT.hpp"

TTEntry::TTEntry()
    : zobristKey(0), depth(-1), score(0), flag(TT_EXACT), bestMove(Move()) {}

// Initializes the transposition table with a given size in megabytes
TranspositionTable::TranspositionTable(size_t sizeMB) {
  size = (sizeMB * 1024 * 1024) / sizeof(TTEntry);
  table.resize(size);
}

// Stores a new entry in the transposition table
void TranspositionTable::store(u64 hash, int depth, int score, TTFlag flag,
                               Move bestMove) {
  size_t index = hash % size;
  TTEntry &entry = table[index];

  entry.zobristKey = hash;
  entry.depth = depth;
  entry.score = score;
  entry.flag = flag;
  entry.bestMove = bestMove;
}

// Probes the transposition table for an existing entry
bool TranspositionTable::probe(u64 hash, int depth, int alpha, int beta,
                               int &score, Move &bestMove) {
  size_t index = hash % size;
  TTEntry &entry = table[index];

  // Check if the entry belongs to the current position
  if (entry.zobristKey != hash) {
    return false;
  }

  bestMove = entry.bestMove;

  // Check if the stored depth is sufficient
  if (entry.depth < depth) {
    return false;
  }

  // Use the stored score if it is exact or provides a bound
  if (entry.flag == TT_EXACT) {
    score = entry.score;
    return true;
  }
  if (entry.flag == TT_ALPHA && entry.score <= alpha) {
    score = alpha;
    return true;
  }
  if (entry.flag == TT_BETA && entry.score >= beta) {
    score = beta;
    return true;
  }

  return false;
}

// Clears all entries in the transposition table
void TranspositionTable::clear() {
  std::fill(table.begin(), table.end(), TTEntry());
}

// Retrieves the best move for a given hash, if available
Move TranspositionTable::getBestMove(u64 hash) {
  size_t index = hash % size;
  TTEntry &entry = table[index];

  if (entry.zobristKey == hash) {
    return entry.bestMove;
  }
  return Move();
}
