#include "../include/utils.hpp"
#include "../include/movegen.hpp"

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

int Utils::getKingCaptureCount(Board &board, bool isWhite) {

  int captureCount = 0;

  u64 kingLoc = isWhite ? board.getWhiteKing() : board.getBlackKing();
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 kingMoveBB = validMoveBB::kingMoves(kingLoc, ownPieces);

  u64 capturedPieces = enemyPieces & kingMoveBB;

  captureCount = Utils::popcount(capturedPieces);

  return captureCount;
}
