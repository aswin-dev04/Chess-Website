#include "../include/evaluation.hpp"

int Evaluation::getKingSafetyScore(Board &board) {
  int score = 0;
  bool blackCastled = false;
  bool whiteCastled = false;

  for (const Move &move : board.getMoveHistory()) {
    if (move.getPieceType() == PieceType::WHITE_KING &&
        (move.getIsKingSideCastle() || move.getIsQueenSideCastle())) {
      score += 50;
      whiteCastled = true;
    }
    if (move.getPieceType() == PieceType::BLACK_KING &&
        (move.getIsKingSideCastle() || move.getIsQueenSideCastle())) {
      score -= 50;
      blackCastled = true;
    }
    if (blackCastled && whiteCastled)
      break;
  }

  return score;
}

int Evaluation::getPieceSquareScore(Board &board) {

  int totalBonus = 0;

  for (int square = 0; square < 64; square++) {
    PieceType piece = Utils::getPieceTypeAt(board, static_cast<Square>(square));

    if (piece == EMPTY)
      continue; // Skip empty squares

    // Get piece type index (0=pawn, 1=knight, 2=bishop, etc.)
    int pieceIndex;
    bool isWhite;

    if (piece >= WHITE_PAWN && piece <= WHITE_KING) {
      isWhite = true;
      pieceIndex = piece - WHITE_PAWN; // Maps to 0-5
    } else {
      isWhite = false;
      pieceIndex = piece - BLACK_PAWN; // Maps to 0-5
    }

    // Get the bonus from the piece-square table
    int bonus;
    if (isWhite) {
      bonus = pieceSquareTables[pieceIndex][square] + materialValue[pieceIndex];
    } else {
      int flippedSquare = square ^ 56; // Flip vertically
      bonus = -(pieceSquareTables[pieceIndex][flippedSquare] +
                materialValue[pieceIndex]);
    }
    totalBonus += bonus;
  }

  return totalBonus;
}
int Evaluation::evaluate(Board &board) {
  int score = getPieceSquareScore(board) + getKingSafetyScore(board);
  if (!board.getWhiteToMove())
    score *= -1;

  return score;
}
