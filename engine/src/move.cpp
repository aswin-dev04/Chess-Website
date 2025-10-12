/**
 * @file move.cpp
 * @brief Implements the Move class.
 * This file contains the implementation of the Move class, including methods
 * for comparing and printing moves.
 */
#include "../include/move.hpp"
#include <string>

// Overloads the << operator to print a move in a human-readable format
std::ostream &operator<<(std::ostream &os, const Move &move) {
  // Helper lambda to convert a square to a string in algebraic notation
  auto squareToString = [](Square sq) -> std::string {
    int file = sq % 8;
    int rank = sq / 8;
    char fileChar = 'a' + file;
    char rankChar = '1' + rank;
    return std::string(1, fileChar) + std::string(1, rankChar);
  };

  // Helper lambda to convert a piece type to a string
  auto pieceToString = [](PieceType piece) -> std::string {
    switch (piece) {
    case WHITE_PAWN:
      return "WP";
    case BLACK_PAWN:
      return "BP";
    case WHITE_KNIGHT:
      return "WN";
    case BLACK_KNIGHT:
      return "BN";
    case WHITE_BISHOP:
      return "WB";
    case BLACK_BISHOP:
      return "BB";
    case WHITE_ROOK:
      return "WR";
    case BLACK_ROOK:
      return "BR";
    case WHITE_QUEEN:
      return "WQ";
    case BLACK_QUEEN:
      return "BQ";
    case WHITE_KING:
      return "WK";
    case BLACK_KING:
      return "BK";
    case EMPTY:
      return "--";
    default:
      return "??";
    }
  };

  os << squareToString(move.getFromSquare()) << "->"
     << squareToString(move.getToSquare()) << " ["
     << pieceToString(move.getPieceType()) << "]";

  if (move.getIsEnPassant()) {
    os << " (en passant)";
  }

  if (move.getIsCapture()) {
    os << " captures [" << pieceToString(move.getCapturedPiece()) << "]";
  }

  if (move.getIsPromotion()) {
    os << " promotes to [" << pieceToString(move.getPromotionPiece()) << "]";
  }

  return os;
}

// Overloads the == operator to compare two moves for equality
bool Move::operator==(const Move &other) const {
  return fromSquare == other.fromSquare && toSquare == other.toSquare &&
         pieceType == other.pieceType && capturedPiece == other.capturedPiece &&
         promotionPiece == other.promotionPiece &&
         isCapture == other.isCapture && isEnPassant == other.isEnPassant &&
         isKingSideCastle == other.isKingSideCastle &&
         isQueenSideCastle == other.isQueenSideCastle &&
         isPromotion == other.isPromotion;
}

// Overloads the = operator to assign one move to another
Move &Move::operator=(const Move &other) {
  if (this != &other) {
    fromSquare = other.fromSquare;
    toSquare = other.toSquare;
    pieceType = other.pieceType;
    capturedPiece = other.capturedPiece;
    promotionPiece = other.promotionPiece;
    isCapture = other.isCapture;
    isEnPassant = other.isEnPassant;
    isKingSideCastle = other.isKingSideCastle;
    isQueenSideCastle = other.isQueenSideCastle;
    isPromotion = other.isPromotion;
  }
  return *this;
}
