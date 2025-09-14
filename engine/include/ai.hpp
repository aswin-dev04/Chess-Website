#ifndef AI_HPP
#define AI_HPP
#include "board.hpp"
#include "movegen.hpp"
#include <vector>

class ChessAI {
public:
  ChessAI();
  ~ChessAI() = default;

  Move getBestMove(Board &board);
  Move getBestMove(Board &board, int depth); // helper method with depth used in
                                             // the actual getBestMove() method

private:
  int minimax(Board &board, int depth, bool maximizingPlayer);
};

#endif
