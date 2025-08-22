#include "../include/utils.hpp"

PieceType Utils::getPieceTypeAt(Board &board, Square square) {
  u64 squareBB = squareToBitboard(square);

  if (board.getWhitePawns() & squareBB)
    return WHITE_PAWN;
  if (board.getBlackPawns() & squareBB)
    return BLACK_PAWN;
  if (board.getWhiteKnights() & squareBB)
    return WHITE_KNIGHT;
  if (board.getBlackKnights() & squareBB)
    return BLACK_KNIGHT;
  if (board.getWhiteBishops() & squareBB)
    return WHITE_BISHOP;
  if (board.getBlackBishops() & squareBB)
    return BLACK_BISHOP;
  if (board.getWhiteRooks() & squareBB)
    return WHITE_ROOK;
  if (board.getBlackRooks() & squareBB)
    return BLACK_ROOK;
  if (board.getWhiteQueens() & squareBB)
    return WHITE_QUEEN;
  if (board.getBlackQueens() & squareBB)
    return BLACK_QUEEN;
  if (board.getWhiteKing() & squareBB)
    return WHITE_KING;
  if (board.getBlackKing() & squareBB)
    return BLACK_KING;

  return EMPTY;
}
