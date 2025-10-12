/**
 * @file evaluation.cpp
 * @brief Implements the evaluation functions for the chess engine.
 * This file contains the implementation of the board evaluation logic,
 * including material balance, piece-square tables, and king safety.
 */
#include "../include/evaluation.hpp"

// Evaluates the safety of the kings, rewarding castling
int Evaluation::getKingSafetyScore(Board &board) {
  int score = 0;

  if (board.getHasWhiteCastled())
    score += 50;
  if (board.getHasBlackCastled())
    score -= 50;

  return score;
}

// Calculates the score based on piece positions using piece-square tables
int Evaluation::getPieceSquareScore(Board &board) {

  int totalBonus = 0;

  for (int square = 0; square < 64; square++) {
    PieceType piece = Utils::getPieceTypeAt(board, static_cast<Square>(square));

    if (piece == EMPTY)
      continue; // Skip empty squares

    int pieceIndex;
    bool isWhite;

    if (piece >= WHITE_PAWN && piece <= WHITE_KING) {
      isWhite = true;
      pieceIndex = piece - WHITE_PAWN; // Maps to 0-5 for white pieces
    } else {
      isWhite = false;
      pieceIndex = piece - BLACK_PAWN; // Maps to 0-5 for black pieces
    }

    // Get the bonus from the corresponding piece-square table
    int bonus;
    if (isWhite) {
      bonus = pieceSquareTables[pieceIndex][square] + materialValue[pieceIndex];
    } else {
      int flippedSquare = square ^ 56; // Flip the square for black pieces
      bonus = -(pieceSquareTables[pieceIndex][flippedSquare] +
                materialValue[pieceIndex]);
    }
    totalBonus += bonus;
  }

  return totalBonus;
}

// Main evaluation function, combines all evaluation components
int Evaluation::evaluate(Board &board) {
  int score = getPieceSquareScore(board) + getKingSafetyScore(board);
  // Return a negative score if it's black's turn to move
  if (!board.getWhiteToMove())
    score *= -1;

  return score;
}
