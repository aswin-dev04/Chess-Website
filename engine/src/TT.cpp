#include "../include/TT.hpp"

TTEntry::TTEntry()
    : zobristKey(0), depth(-1), score(0), flag(TT_EXACT), bestMove(Move()) {}

TranspositionTable::TranspositionTable(size_t sizeMB) {
  size = (sizeMB * 1024 * 1024) / sizeof(TTEntry);
  table.resize(size);
}

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

bool TranspositionTable::probe(u64 hash, int depth, int alpha, int beta,
                               int &score, Move &bestMove) {
  size_t index = hash % size;
  TTEntry &entry = table[index];

  if (entry.zobristKey != hash) {
    return false;
  }

  bestMove = entry.bestMove;

  if (entry.depth < depth) {
    return false;
  }

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

void TranspositionTable::clear() {
  std::fill(table.begin(), table.end(), TTEntry());
}

Move TranspositionTable::getBestMove(u64 hash) {
  size_t index = hash % size;
  TTEntry &entry = table[index];

  if (entry.zobristKey == hash) {
    return entry.bestMove;
  }
  return Move();
}
