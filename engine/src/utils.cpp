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

int Utils::getCaptureCount(Board &board, PieceType pieceType, bool isWhite) {

  int captureCount = 0;

  switch (pieceType) {
  case WHITE_PAWN:
  case BLACK_PAWN: {
    u64 pawnLoc = isWhite ? board.getWhitePawns() : board.getBlackPawns();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();

    u64 allCaptures = 0;
    u64 pawns = pawnLoc; // Copy to iterate through

    while (pawns) {
      Square pawnSquare = Utils::popLSB(pawns);
      u64 singlePawn = Utils::squareToBitboard(pawnSquare);
      u64 pawnMoveBB =
          isWhite
              ? validMoveBB::whitePawnMoves(singlePawn, ownPieces,
                                            board.getAllPieces(), enemyPieces)
              : validMoveBB::blackPawnMoves(singlePawn, ownPieces,
                                            board.getAllPieces(), enemyPieces);
      allCaptures |= (enemyPieces & pawnMoveBB);
    }

    captureCount = Utils::popcount(allCaptures);
    break;
  }
  case WHITE_KNIGHT:
  case BLACK_KNIGHT: {
    u64 knightLoc = isWhite ? board.getWhiteKnights() : board.getBlackKnights();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();

    u64 allCaptures = 0;
    u64 knights = knightLoc; // Copy to iterate through

    while (knights) {
      Square knightSquare = Utils::popLSB(knights);
      u64 singleKnight = Utils::squareToBitboard(knightSquare);
      u64 knightMoveBB = validMoveBB::knightMoves(singleKnight, ownPieces);
      allCaptures |= (enemyPieces & knightMoveBB);
    }

    captureCount = Utils::popcount(allCaptures);
    break;
  }
  case WHITE_BISHOP:
  case BLACK_BISHOP: {
    u64 bishopLoc = isWhite ? board.getWhiteBishops() : board.getBlackBishops();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();

    u64 allCaptures = 0;
    u64 bishops = bishopLoc; // Copy to iterate through

    while (bishops) {
      Square bishopSquare = Utils::popLSB(bishops);
      u64 singleBishop = Utils::squareToBitboard(bishopSquare);
      u64 bishopMoveBB =
          validMoveBB::bishopMoves(singleBishop, ownPieces, enemyPieces);
      allCaptures |= (enemyPieces & bishopMoveBB);
    }

    captureCount = Utils::popcount(allCaptures);
    break;
  }
  case WHITE_ROOK:
  case BLACK_ROOK: {
    u64 rookLoc = isWhite ? board.getWhiteRooks() : board.getBlackRooks();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();

    u64 allCaptures = 0;
    u64 rooks = rookLoc; // Copy to iterate through

    while (rooks) {
      Square rookSquare = Utils::popLSB(rooks);
      u64 singleRook = Utils::squareToBitboard(rookSquare);
      u64 rookMoveBB =
          validMoveBB::rookMoves(singleRook, ownPieces, enemyPieces);
      allCaptures |= (enemyPieces & rookMoveBB);
    }

    captureCount = Utils::popcount(allCaptures);
    break;
  }
  case WHITE_QUEEN:
  case BLACK_QUEEN: {
    u64 queenLoc = isWhite ? board.getWhiteQueens() : board.getBlackQueens();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();

    u64 allCaptures = 0;
    u64 queens = queenLoc; // Copy to iterate through

    while (queens) {
      Square queenSquare = Utils::popLSB(queens);
      u64 singleQueen = Utils::squareToBitboard(queenSquare);
      u64 queenMoveBB =
          validMoveBB::queenMoves(singleQueen, ownPieces, enemyPieces);
      allCaptures |= (enemyPieces & queenMoveBB);
    }

    captureCount = Utils::popcount(allCaptures);
    break;
  }
  case WHITE_KING:
  case BLACK_KING: {
    u64 kingLoc = isWhite ? board.getWhiteKing() : board.getBlackKing();
    u64 ownPieces =
        isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
    u64 enemyPieces =
        isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
    u64 kingMoveBB = validMoveBB::kingMoves(kingLoc, ownPieces);

    u64 capturedPieces = enemyPieces & kingMoveBB;

    captureCount = Utils::popcount(capturedPieces);
    break;
  }
  default:
    break;
  }

  return captureCount;
}

Square Utils::stringToSquare(const std::string &square) {
  if (square.length() != 2)
    return SQ_NONE;

  int file = square[0] - 'a';
  int rank = square[1] - '1';

  if (file < 0 || file > 7 || rank < 0 || rank > 7) {
    return SQ_NONE;
  }

  return static_cast<Square>(rank * 8 + file);
}

PieceType Utils::charToPiece(char c) {
  switch (c) {
  case 'P':
    return WHITE_PAWN;
  case 'N':
    return WHITE_KNIGHT;
  case 'B':
    return WHITE_BISHOP;
  case 'R':
    return WHITE_ROOK;
  case 'Q':
    return WHITE_QUEEN;
  case 'K':
    return WHITE_KING;
  case 'p':
    return BLACK_PAWN;
  case 'n':
    return BLACK_KNIGHT;
  case 'b':
    return BLACK_BISHOP;
  case 'r':
    return BLACK_ROOK;
  case 'q':
    return BLACK_QUEEN;
  case 'k':
    return BLACK_KING;
  default:
    return EMPTY;
  }
}
