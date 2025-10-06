#ifndef AI_HPP
#define AI_HPP
#include "TT.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include <limits.h>
#include <vector>

class ChessAI {
private:
  TranspositionTable tt;

public:
  ChessAI() : tt(64) {}
  ~ChessAI() = default;

  Move getBestMove(Board &board);
  Move getBestMove(Board &board, int depth); // helper method with depth used in
                                             // the actual getBestMove() method

  int minimax(Board &board, int depth, long long int alpha, long long int beta,
              bool maximizingPlayer);
  inline bool isCheckmate(Board &board, bool maximizingPlayer) {
    return maximizingPlayer
               ? minimax(board, 1, INT_MIN, INT_MAX, maximizingPlayer) ==
                     INT_MIN
               : minimax(board, 1, INT_MIN, INT_MAX, maximizingPlayer) ==
                     INT_MAX;
  }
  int quiescence(Board &board, int alpha, int beta, bool maximizingPlayer,
                 int qDepth = 0);
  std::vector<Move> generateTacticalMoves(Board &board);
};

#endif
