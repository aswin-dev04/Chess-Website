#ifndef AI_HPP
#define AI_HPP
#include "board.hpp"
#include "movegen.hpp"
#include <limits.h>
#include <vector>

class ChessAI {
public:
  ChessAI();
  ~ChessAI() = default;

  Move getBestMove(Board &board);
  Move getBestMove(Board &board, int depth); // helper method with depth used in
                                             // the actual getBestMove() method

  int minimax(Board &board, int depth, bool maximizingPlayer);
  inline bool isCheckmate(Board &board, bool maximizingPlayer) {
    return maximizingPlayer ? minimax(board, 1, maximizingPlayer) == INT_MIN
                            : minimax(board, 1, maximizingPlayer) == INT_MAX;
  }
};

#endif
