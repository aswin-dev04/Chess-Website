#include "../include/ai.hpp"
#include "../include/evaluation.hpp"
#include "../include/moveorder.hpp"
#include <algorithm>
#include <chrono>
#include <limits.h>

int nodesSearched = 0;
int ttHits = 0;
int qNodes = 0;

int ChessAI::minimax(Board &board, int depth, long long int alpha,
                     long long int beta, bool maximizingPlayer) {

  nodesSearched++;
  u64 hash = board.getZobristHash();
  int originalAlpha = alpha;

  int ttScore;
  Move ttMove;
  if (tt.probe(hash, depth, alpha, beta, ttScore, ttMove)) {
    ttHits++;
    return ttScore;
  }

  if (depth == 0) {
    return quiescence(board, alpha, beta, maximizingPlayer, 0);
  }

  std::vector<Move> moves = MoveOrder::getOrderedMoves(board);

  if (ttMove.getFromSquare() != ttMove.getToSquare()) {
    auto it = std::find(moves.begin(), moves.end(), ttMove);
    if (it != moves.end()) {
      moves.erase(it);
      moves.insert(moves.begin(), ttMove);
    }
  }

  if (moves.empty()) {
    bool currentPlayerTurn = board.getWhiteToMove();
    bool currentPlayerInCheck = board.isKingChecked(currentPlayerTurn);
    if (currentPlayerInCheck && maximizingPlayer)
      return INT_MIN + depth;
    else if (currentPlayerInCheck && !maximizingPlayer)
      return INT_MAX - depth;
    else
      return 0;
  }

  Move bestMove = moves[0];

  if (maximizingPlayer) {
    int maxEval = INT_MIN;

    for (Move &move : moves) {
      board.makeMove(move);
      int eval = minimax(board, depth - 1, alpha, beta, false);
      board.undoMove();

      if (eval > maxEval) {
        maxEval = eval;
        bestMove = move;
      }

      alpha = std::max((long long int)alpha, (long long int)eval);
      if (beta <= alpha) {
        break;
      }
    }

    TTFlag flag;
    if (maxEval <= originalAlpha) {
      flag = TT_ALPHA;
    } else if (maxEval >= beta) {
      flag = TT_BETA;
    } else {
      flag = TT_EXACT;
    }

    tt.store(hash, depth, maxEval, flag, bestMove);
    return maxEval;

  } else {
    int minEval = INT_MAX;

    for (Move &move : moves) {
      board.makeMove(move);
      int eval = minimax(board, depth - 1, alpha, beta, true);
      board.undoMove();

      if (eval < minEval) {
        minEval = eval;
        bestMove = move;
      }

      beta = std::min((long long int)beta, (long long int)eval);
      if (beta <= alpha) {
        break;
      }
    }

    TTFlag flag;
    if (minEval >= beta) {
      flag = TT_ALPHA;
    } else if (minEval <= alpha) {
      flag = TT_BETA;
    } else {
      flag = TT_EXACT;
    }

    tt.store(hash, depth, minEval, flag, bestMove);
    return minEval;
  }
}

Move ChessAI::getBestMove(Board &board, int maxDepth) {
  nodesSearched = 0;
  ttHits = 0;
  qNodes = 0;
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<Move> moves = MoveOrder::getOrderedMoves(board);
  if (moves.empty())
    return Move();

  u64 hash = board.getZobristHash();
  Move ttMove = tt.getBestMove(hash);

  if (ttMove.getFromSquare() != ttMove.getToSquare()) {
    auto it = std::find(moves.begin(), moves.end(), ttMove);
    if (it != moves.end()) {
      moves.erase(it);
      moves.insert(moves.begin(), ttMove);
    }
  }

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

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Nodes: " << nodesSearched << std::endl;
  std::cout << "TT Hits: " << ttHits << " (" << (100.0 * ttHits / nodesSearched)
            << "%)" << std::endl;
  std::cout << "Q-Nodes: " << qNodes << " (" << (100.0 * qNodes / nodesSearched)
            << "%)" << std::endl;
  std::cout << "Time: " << duration.count() << "ms" << std::endl;

  return bestMove;
}

Move ChessAI::getBestMove(Board &board) { return getBestMove(board, 6); }

int ChessAI::quiescence(Board &board, int alpha, int beta,
                        bool maximizingPlayer, int qDepth) {

  qNodes++;

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
  tacticalMoves = MoveOrder::getOrderedMoves(board, tacticalMoves);

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
