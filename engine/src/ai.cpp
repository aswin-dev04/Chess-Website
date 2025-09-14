#include "../include/ai.hpp"
#include "../include/evaluation.hpp"
#include <limits.h>

ChessAI::ChessAI() {}

int ChessAI::minimax(Board &board, int depth, bool maximizingPlayer) {
  if (depth == 0) {
    return Evaluation::evaluate(board);
  }

  std::vector<Move> moves =
      MoveGeneration::generateAllMoves(board, board.getWhiteToMove());

  if (moves.empty()) {
    bool currentPlayerTurn = board.getWhiteToMove();
    bool currentPlayerInCheck = board.isKingChecked(currentPlayerTurn);

    if (currentPlayerInCheck && maximizingPlayer)
      return INT_MIN;
    else if (currentPlayerInCheck && !maximizingPlayer)
      return INT_MAX;
    else
      return 0;
  }
  if (maximizingPlayer) {
    int maxEval = INT_MIN;
    for (Move &move : moves) {
      board.makeMove(move);
      int eval = minimax(board, depth - 1, false);
      board.undoMove();
      maxEval = std::max(maxEval, eval);
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (Move &move : moves) {
      board.makeMove(move);
      int eval = minimax(board, depth - 1, true);
      board.undoMove();
      minEval = std::min(minEval, eval);
    }
    return minEval;
  }
}

Move ChessAI::getBestMove(Board &board) { return getBestMove(board, 4); }

Move ChessAI::getBestMove(Board &board, int depth) {
  std::vector<Move> moves =
      MoveGeneration::generateAllMoves(board, board.getWhiteToMove());
  if (moves.empty())
    return Move(); // No legal moves

  Move bestMove = moves[0];
  int bestScore = INT_MIN;

  for (Move &move : moves) {
    board.makeMove(move);
    int score = minimax(board, depth - 1, false);
    board.undoMove();

    if (score > bestScore) {
      bestScore = score;
      bestMove = move;
    }
  }

  return bestMove;
}
