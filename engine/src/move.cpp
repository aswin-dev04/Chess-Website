#include "../include/move.hpp"
#include <string>

std::ostream &operator<<(std::ostream &os, const Move &move) {
  // Convert squares to chess notation (a1, b2, etc.)
  auto squareToString = [](Square sq) -> std::string {
    int file = sq % 8;
    int rank = sq / 8;
    char fileChar = 'a' + file;
    char rankChar = '1' + rank;
    return std::string(1, fileChar) + std::string(1, rankChar);
  };

  // Convert piece type to string
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

  // Use getter methods instead of direct member access
  os << squareToString(move.getFromSquare()) << "->"
     << squareToString(move.getToSquare()) << " ["
     << pieceToString(move.getPieceType()) << "]";

  if (move.getIsCapture()) {
    os << " captures [" << pieceToString(move.getCapturedPiece()) << "]";
  }

  return os;
}
