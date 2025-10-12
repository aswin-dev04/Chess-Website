/**
 * @file ai.hpp
 * @brief Defines the ChessAI class, which is responsible for the engine's
 * decision-making. This file contains the main AI logic, including the minimax
 * algorithm with alpha-beta pruning and other AI-related functionalities.
 */
#ifndef AI_HPP
#define AI_HPP
#include "TT.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include <limits.h>
#include <vector>

class ChessAI {
private:
  TranspositionTable tt; // Transposition table to cache board evaluations

public:
  ChessAI() : tt(64) {}
  ~ChessAI() = default;

  // Finds the best move using an iterative deepening search
  Move getBestMove(Board &board);
  Move getBestMove(Board &board, int depth);

  // Core search function implementing minimax with alpha-beta pruning
  int minimax(Board &board, int depth, long long int alpha, long long int beta,
              bool maximizingPlayer);

  // Checks if the current player is in a checkmate position
  inline bool isCheckmate(Board &board, bool maximizingPlayer) {
    return maximizingPlayer
               ? minimax(board, 1, INT_MIN, INT_MAX, maximizingPlayer) ==
                     INT_MIN
               : minimax(board, 1, INT_MIN, INT_MAX, maximizingPlayer) ==
                     INT_MAX;
  }
  // Quiescence search to evaluate tactical positions more accurately
  int quiescence(Board &board, int alpha, int beta, bool maximizingPlayer,
                 int qDepth = 0);
  // Generates only tactical moves like captures and promotions
  std::vector<Move> generateTacticalMoves(Board &board);
};

#endif
