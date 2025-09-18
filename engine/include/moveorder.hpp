#ifndef MOVEORDER_HPP
#define MOVEORDER_HPP

#include "movegen.hpp"

class MoveOrder {
public:
  MoveOrder() {}
  static std::vector<Move> getOrderedMoves(Board &board);
  static int getMoveScore(Board &board, Move move);
};

#endif
