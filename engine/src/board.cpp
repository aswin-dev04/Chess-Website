#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"

Board::Board() {
  setWhitePawns();
  setBlackPawns();
  setWhiteKnights();
  setBlackKnights();
  setWhiteBishops();
  setBlackBishops();
  setWhiteRooks();
  setBlackRooks();
  setWhiteQueens();
  setBlackQueens();
  setWhiteKing();
  setBlackKing();
  setALLPiecesAggregate();
  prevState = nullptr;
}

Board::Board(u64 wPawns, u64 bPawns, u64 wKnights, u64 bKnights, u64 wBishops,
             u64 bBishops, u64 wRooks, u64 bRooks, u64 wQueens, u64 bQueens,
             u64 wKing, u64 bKing) {
  setWhitePawns(wPawns);
  setBlackPawns(bPawns);
  setWhiteKnights(wKnights);
  setBlackKnights(bKnights);
  setWhiteBishops(wBishops);
  setBlackBishops(bBishops);
  setWhiteRooks(wRooks);
  setBlackRooks(bRooks);
  setWhiteQueens(wQueens);
  setBlackQueens(bQueens);
  setWhiteKing(wKing);
  setBlackKing(bKing);
  setALLPiecesAggregate();
  prevState = nullptr;
}

Board &Board::operator=(const Board &other) {
  if (this == &other)
    return *this;
  whitePawns = other.whitePawns;
  whiteKnights = other.whiteKnights;
  whiteBishops = other.whiteBishops;
  whiteRooks = other.whiteRooks;
  whiteQueens = other.whiteQueens;
  whiteKing = other.whiteKing;
  blackPawns = other.blackPawns;
  blackKnights = other.blackKnights;
  blackBishops = other.blackBishops;
  blackRooks = other.blackRooks;
  blackQueens = other.blackQueens;
  blackKing = other.blackKing;
  allWhitePieces = other.allWhitePieces;
  allBlackPieces = other.allBlackPieces;
  allPieces = other.allPieces;
  return *this;
}

bool Board::isKingChecked(bool isWhite) {

  u64 enemyAttacks = validMoveBB::allEnemyAttacks(*this, !isWhite);
  u64 king = isWhite ? getWhiteKing() : getBlackKing();
  return (enemyAttacks & king) != 0;
}

void Board::makeMove(const Move &move) {

  if (!prevState)
    prevState = std::make_unique<Board>();
  *prevState = *this;

  Square fromSquare = move.getFromSquare();
  Square toSquare = move.getToSquare();
  PieceType piece = move.getPieceType();
  PieceType capturedPiece = move.getCapturedPiece();
  bool isCapture = move.getIsCapture();
  bool isPromotion = move.getIsPromotion();
  bool isEnPassant = move.getIsEnPassant();
  bool isCastling = move.isCastling();

  u64 currPieceLoc = 0ULL;
  u64 capturedPieceLoc = 0ULL;

  // Handle special moves
  /*if (isEnPassant) {
  }

  if (isCastling) {
  }

  if (isPromotion) {
  }*/

  switch (piece) {
  case EMPTY:
    break;
  case WHITE_PAWN:
    currPieceLoc = getWhitePawns();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhitePawns(currPieceLoc);
    break;
  case BLACK_PAWN:
    currPieceLoc = getBlackPawns();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackPawns(currPieceLoc);
    break;
  case WHITE_KNIGHT:
    currPieceLoc = getWhiteKnights();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteKnights(currPieceLoc);
    break;
  case BLACK_KNIGHT:
    currPieceLoc = getBlackKnights();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackKnights(currPieceLoc);
    break;
  case WHITE_BISHOP:
    currPieceLoc = getWhiteBishops();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteBishops(currPieceLoc);
    break;
  case BLACK_BISHOP:
    currPieceLoc = getBlackBishops();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackBishops(currPieceLoc);
    break;
  case WHITE_ROOK:
    currPieceLoc = getWhiteRooks();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteRooks(currPieceLoc);
    break;
  case BLACK_ROOK:
    currPieceLoc = getBlackRooks();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackRooks(currPieceLoc);
    break;
  case WHITE_QUEEN:
    currPieceLoc = getWhiteQueens();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteQueens(currPieceLoc);
    break;
  case BLACK_QUEEN:
    currPieceLoc = getBlackQueens();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackQueens(currPieceLoc);
    break;
  case WHITE_KING:
    currPieceLoc = getWhiteKing();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteKing(currPieceLoc);
    break;
  case BLACK_KING:
    currPieceLoc = getBlackKing();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setBlackKing(currPieceLoc);
    break;
  }

  if (isCapture) {
    switch (capturedPiece) {
    case EMPTY:
      break;
    case WHITE_PAWN:
      capturedPieceLoc = getWhitePawns();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setWhitePawns(capturedPieceLoc);
      break;
    case BLACK_PAWN:
      capturedPieceLoc = getBlackPawns();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setBlackPawns(capturedPieceLoc);
      break;
    case WHITE_KNIGHT:
      capturedPieceLoc = getWhiteKnights();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setWhiteKnights(capturedPieceLoc);
      break;
    case BLACK_KNIGHT:
      capturedPieceLoc = getBlackKnights();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setBlackKnights(capturedPieceLoc);
      break;
    case WHITE_BISHOP:
      capturedPieceLoc = getWhiteBishops();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setWhiteBishops(capturedPieceLoc);
      break;
    case BLACK_BISHOP:
      capturedPieceLoc = getBlackBishops();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setBlackBishops(capturedPieceLoc);
      break;
    case WHITE_ROOK:
      capturedPieceLoc = getWhiteRooks();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setWhiteRooks(capturedPieceLoc);
      break;
    case BLACK_ROOK:
      capturedPieceLoc = getBlackRooks();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setBlackRooks(capturedPieceLoc);
      break;
    case WHITE_QUEEN:
      capturedPieceLoc = getWhiteQueens();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setWhiteQueens(capturedPieceLoc);
      break;
    case BLACK_QUEEN:
      capturedPieceLoc = getBlackQueens();
      capturedPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      setBlackQueens(capturedPieceLoc);
      break;
    default:
      break;
    }
  }

  setALLPiecesAggregate();
}
