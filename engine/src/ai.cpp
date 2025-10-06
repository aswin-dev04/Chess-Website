#include "../include/ai.hpp"
#include "../include/evaluation.hpp"
#include "../include/moveorder.hpp"
#include <algorithm>
#include <limits.h>

ChessAI::ChessAI() {}

int ChessAI::minimax(Board &board, int depth, long long int alpha,
                     long long int beta, bool maximizingPlayer) {
  if (depth == 0) {
    return quiescence(board, alpha, beta, maximizingPlayer, 0);
  }

  std::vector<Move> moves;

  if (depth >= 3)
    moves = MoveOrder::getOrderedMoves(board);
  else
    moves = MoveGeneration::generateAllMoves(board, board.getWhiteToMove());

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
      int eval = minimax(board, depth - 1, alpha, beta, false);
      board.undoMove();
      maxEval = std::max(maxEval, eval);
      alpha = std::max((long long int)alpha, (long long int)eval);
      if (beta <= alpha)
        break;
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (Move &move : moves) {
      board.makeMove(move);
      int eval = minimax(board, depth - 1, alpha, beta, true);
      board.undoMove();
      minEval = std::min(minEval, eval);
      beta = std::min((long long int)beta, (long long int)eval);
      if (beta <= alpha)
        break;
    }
    return minEval;
  }
}

Move ChessAI::getBestMove(Board &board, int maxDepth) {
  std::vector<Move> moves = MoveOrder::getOrderedMoves(board);
  if (moves.empty())
    return Move();

  Move bestMove = moves[0];

  for (int currentDepth = 1; currentDepth <= maxDepth; currentDepth++) {
    int bestScore = INT_MIN;
    Move currentBestMove = moves[0];

    for (Move &move : moves) {
      board.makeMove(move);
      int score = minimax(board, currentDepth - 1, INT_MIN, INT_MAX, false);
      board.undoMove();

      if (score > bestScore) {
        bestScore = score;
        currentBestMove = move;
      }
    }

    bestMove = currentBestMove;

    auto it = std::find(moves.begin(), moves.end(), bestMove);
    if (it != moves.end()) {
      moves.erase(it);
      moves.insert(moves.begin(), bestMove);
    }
  }

  return bestMove;
}

Move ChessAI::getBestMove(Board &board) { return getBestMove(board, 4); }

int ChessAI::quiescence(Board &board, int alpha, int beta,
                        bool maximizingPlayer, int qDepth = 0) {

  const int MAX_Q_DEPTH = 4;

  int standPat = Evaluation::evaluate(board);

  if (qDepth >= MAX_Q_DEPTH) {
    return standPat;
  }

  if (maximizingPlayer) {
    if (standPat >= beta) {
      return beta;
    }
    if (standPat > alpha) {
      alpha = standPat;
    }
  } else {
    if (standPat <= alpha) {
      return alpha;
    }
    if (standPat < beta) {
      beta = standPat;
    }
  }

  std::vector<Move> tacticalMoves = generateTacticalMoves(board);

  if (tacticalMoves.empty()) {
    return standPat;
  }
  std::vector<Move> orderedTacticalMoves =
      MoveOrder::getOrderedMoves(board, tacticalMoves);

  const int DELTA = 900;
  if (maximizingPlayer && standPat + DELTA < alpha) {
    return standPat;
  }
  if (!maximizingPlayer && standPat - DELTA > beta) {
    return standPat;
  }

  if (maximizingPlayer) {
    int maxEval = standPat;

    for (Move &move : tacticalMoves) {
      board.makeMove(move);
      int eval = quiescence(board, alpha, beta, false, qDepth + 1);
      board.undoMove();

      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);

      if (beta <= alpha) {
        break;
      }
    }

    return maxEval;
  } else {
    int minEval = standPat;

    for (Move &move : tacticalMoves) {
      board.makeMove(move);
      int eval = quiescence(board, alpha, beta, true, qDepth + 1);
      board.undoMove();

      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);

      if (beta <= alpha) {
        break;
      }
    }

    return minEval;
  }
}

std::vector<Move> ChessAI::generateTacticalMoves(Board &board) {
  bool isWhite = board.getWhiteToMove();

  std::vector<Move> tactical = MoveGeneration::generateCaptures(board, isWhite);

  u64 pawns = isWhite ? board.getWhitePawns() : board.getBlackPawns();
  u64 promoPawns =
      isWhite ? pawns & Tables::maskRank[6] : pawns & Tables::maskRank[1];

  if (promoPawns != 0) {
    std::vector<Move> promos =
        MoveGeneration::generatePawnPromotionMoves(board, promoPawns, isWhite);
    tactical.insert(tactical.end(), promos.begin(), promos.end());
  }

  return tactical;
}
