/**
 * @file moveorder.cpp
 * @brief Implements the move ordering functions for the chess engine.
 * This file contains the implementation of the move ordering logic, which is
 * crucial for the efficiency of the alpha-beta search.
 */
#include "../include/moveorder.hpp"
#include "../include/evaluation.hpp"
#include <algorithm>
#include <utility>

// Orders all legal moves based on a scoring heuristic
std::vector<Move> MoveOrder::getOrderedMoves(Board &board) {
  std::vector<Move> allMoves =
      MoveGeneration::generateAllMoves(board, board.getWhiteToMove());

  std::vector<std::pair<Move, int>> scoredMoves;
  scoredMoves.reserve(allMoves.size());

  for (const Move &move : allMoves) {
    int score = getMoveScore(board, move);
    scoredMoves.push_back({move, score});
  }

  // Sort moves in descending order of their scores
  std::sort(scoredMoves.begin(), scoredMoves.end(),
            [](const std::pair<Move, int> &a, const std::pair<Move, int> &b) {
              return a.second > b.second;
            });

  std::vector<Move> orderedMoves;
  orderedMoves.reserve(scoredMoves.size());
  for (auto &entry : scoredMoves) {
    orderedMoves.push_back(entry.first);
  }

  return orderedMoves;
}

// Orders a list of captures based on the Most Valuable Victim - Least Valuable
// Attacker heuristic
void MoveOrder::orderCaptures(std::vector<Move> &captures) {
  std::sort(captures.begin(), captures.end(),
            [&](const Move &a, const Move &b) {
              return getCaptureScore(a) > getCaptureScore(b);
            });
}

// Assigns a score to a move for ordering purposes
int MoveOrder::getMoveScore(Board &board, Move move) {
  int score = 0;

  // Captures are given the highest priority
  if (move.getIsCapture()) {
    int victimValue = Evaluation::materialValue[move.getCapturedPiece() - 1];
    int attackerValue = Evaluation::materialValue[move.getPieceType() - 1];

    score = 100000 + (victimValue * 10 - attackerValue); // MVV-LVA

  }

  // Promotions are the next highest priority
  else if (move.getIsPromotion()) {
    score = 90000 + Evaluation::materialValue[move.getPromotionPiece() - 1];
  }

  // Castling is given a high priority
  else if (move.isCastling()) {
    score = 50000;
  }

  else {
    score = 0;
  }

  // Penalize moves to squares attacked by pawns
  if ((Utils::squareToBitboard(move.getToSquare()) &
       ((move.getPieceType() >= 1 && move.getPieceType() <= 6)
            ? validMoveBB::blackPawnAttacks(board.getBlackPawns())
            : validMoveBB::whitePawnAttacks(board.getWhitePawns()))) != 0)
    score -= 1000;

  return score;
}

// Calculates the score for a capture move using MVV-LVA
int MoveOrder::getCaptureScore(const Move &move) {
  int victimValue = Evaluation::materialValue[move.getCapturedPiece() - 1];
  int attackerValue = Evaluation::materialValue[move.getPieceType() - 1];
  return victimValue * 10 - attackerValue;
}
