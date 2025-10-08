#ifndef MOVEORDER_HPP
#define MOVEORDER_HPP

#include "movegen.hpp"

class MoveOrder {
public:
  MoveOrder() {}
  static std::vector<Move> getOrderedMoves(Board &board);
  static std::vector<Move> getOrderedMoves(Board &board,
                                           std::vector<Move> &moves);
  static int getMoveScore(Board &board, Move move);
  static int getCaptureScore(const Move &move);
  static void orderCaptures(std::vector<Move> &captures);
};

#endif
