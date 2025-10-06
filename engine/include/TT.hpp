#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "move.hpp"
#include "types.hpp"
#include <cstdint>
#include <vector>

using u64 = std::uint64_t;

enum TTFlag { TT_EXACT = 0, TT_ALPHA = 1, TT_BETA = 2 };

struct TTEntry {
  u64 zobristKey;
  int depth;
  int score;
  TTFlag flag;
  Move bestMove;

  TTEntry();
};

class TranspositionTable {
private:
  std::vector<TTEntry> table;
  size_t size;

public:
  TranspositionTable(size_t sizeMB = 64);
  void store(u64 hash, int depth, int score, TTFlag flag, Move bestMove);
  bool probe(u64 hash, int depth, int alpha, int beta, int &score,
             Move &bestMove);
  void clear();
  Move getBestMove(u64 hash);
};

#endif
