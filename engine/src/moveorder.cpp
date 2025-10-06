#include "../include/moveorder.hpp"
#include "../include/evaluation.hpp"
#include <algorithm>
#include <utility>

std::vector<Move> MoveOrder::getOrderedMoves(Board &board) {
  std::vector<Move> allMoves =
      MoveGeneration::generateAllMoves(board, board.getWhiteToMove());

  std::vector<std::pair<Move, int>> scoredMoves;
  scoredMoves.reserve(allMoves.size());

  for (const Move &move : allMoves) {
    int score = getMoveScore(board, move);
    scoredMoves.push_back({move, score});
  }

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

std::vector<Move> MoveOrder::getOrderedMoves(Board &board,
                                             std::vector<Move> &moves) {
  std::vector<std::pair<Move, int>> scoredMoves;
  scoredMoves.reserve(moves.size());

  for (const Move &move : moves) {
    int score = getMoveScore(board, move);
    scoredMoves.push_back({move, score});
  }

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

int MoveOrder::getMoveScore(Board &board, Move move) {
  int score = 0;

  if (move.getIsCapture()) {
    int victimValue = Evaluation::materialValue[move.getCapturedPiece() - 1];
    int attackerValue = Evaluation::materialValue[move.getPieceType() - 1];

    score = 100000 + (victimValue * 10 - attackerValue);

  }

  else if (move.getIsPromotion()) {
    score = 90000 + Evaluation::materialValue[move.getPromotionPiece() - 1];
  }

  // Castling
  else if (move.isCastling()) {
    score = 50000;
  }

  else {
    score = 0;
  }

  if ((Utils::squareToBitboard(move.getToSquare()) &
       ((move.getPieceType() >= 1 && move.getPieceType() <= 6)
            ? validMoveBB::blackPawnAttacks(board.getBlackPawns())
            : validMoveBB::whitePawnAttacks(board.getWhitePawns()))) != 0)
    score -= 1000;

  return score;
}
