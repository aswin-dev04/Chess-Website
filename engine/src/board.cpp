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
  moveHistory = {};
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
  moveHistory = {};
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
  bool isKingSideCastling = move.getIsKingSideCastle();
  bool isQueenSideCastling = move.getIsQueenSideCastle();

  u64 currPieceLoc = 0ULL;
  u64 capturedPieceLoc = 0ULL;

  if (isEnPassant) {
    if (piece == WHITE_PAWN) {
      Square capturedPawnSquare = static_cast<Square>(toSquare - 8);
      u64 enemyPawns = getBlackPawns();
      enemyPawns &= ~(Utils::squareToBitboard(capturedPawnSquare));
      setBlackPawns(enemyPawns);

      u64 ownPawns = getWhitePawns();
      ownPawns &= ~(Utils::squareToBitboard(fromSquare));
      ownPawns |= Utils::squareToBitboard(toSquare);
      setWhitePawns(ownPawns);

      moveHistory.push_back(move);
      setALLPiecesAggregate();
      return;
    } else {
      Square capturedPawnSquare = static_cast<Square>(toSquare + 8);
      u64 enemyPawns = getWhitePawns();
      enemyPawns &= ~(Utils::squareToBitboard(capturedPawnSquare));
      setWhitePawns(enemyPawns);

      u64 ownPawns = getBlackPawns();
      ownPawns &= ~(Utils::squareToBitboard(fromSquare));
      ownPawns |= Utils::squareToBitboard(toSquare);
      setBlackPawns(ownPawns);

      moveHistory.push_back(move);
      setALLPiecesAggregate();
      return;
    }
  }

  switch (piece) {
  case EMPTY:
    break;
  case WHITE_PAWN:
    currPieceLoc = getWhitePawns();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));
    if (isPromotion) {
      PieceType promotedPiece = move.getPromotionPiece();

      switch (promotedPiece) {
      case WHITE_QUEEN: {
        u64 promoPieces = getWhiteQueens();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setWhiteQueens(promoPieces);
        break;
      }

      case WHITE_ROOK: {
        u64 promoPieces = getWhiteRooks();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setWhiteRooks(promoPieces);
        break;
      }

      case WHITE_BISHOP: {
        u64 promoPieces = getWhiteBishops();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setWhiteBishops(promoPieces);
        break;
      }

      case WHITE_KNIGHT: {
        u64 promoPieces = getWhiteKnights();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setWhiteKnights(promoPieces);
        break;
      }
      default:
        break;
      }
    } else
      currPieceLoc |= Utils::squareToBitboard(toSquare);

    setWhitePawns(currPieceLoc);
    break;
  case BLACK_PAWN:
    currPieceLoc = getBlackPawns();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));

    if (isPromotion) {
      PieceType promotedPiece = move.getPromotionPiece();

      switch (promotedPiece) {
      case BLACK_QUEEN: {
        u64 promoPieces = getBlackQueens();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setBlackQueens(promoPieces);
        break;
      }

      case BLACK_ROOK: {
        u64 promoPieces = getBlackRooks();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setBlackRooks(promoPieces);
        break;
      }

      case BLACK_BISHOP: {
        u64 promoPieces = getBlackBishops();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setBlackBishops(promoPieces);
        break;
      }

      case BLACK_KNIGHT: {
        u64 promoPieces = getBlackKnights();
        promoPieces |= Utils::squareToBitboard(toSquare);
        setBlackKnights(promoPieces);
        break;
      }
      default:
        break;
      }
    } else
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

    if (isKingSideCastling) {
      u64 rookloc = getWhiteRooks();
      rookloc &= ~(Utils::squareToBitboard(H1));
      rookloc |= Utils::squareToBitboard(F1);
      setWhiteRooks(rookloc);
    }
    if (isQueenSideCastling) {
      u64 rookloc = getWhiteRooks();
      rookloc &= ~(Utils::squareToBitboard(A1));
      rookloc |= Utils::squareToBitboard(D1);
      setWhiteRooks(rookloc);
    }
    currPieceLoc |= Utils::squareToBitboard(toSquare);
    setWhiteKing(currPieceLoc);
    break;
  case BLACK_KING:
    currPieceLoc = getBlackKing();
    currPieceLoc &= ~(Utils::squareToBitboard(fromSquare));

    if (isKingSideCastling) {
      u64 rookloc = getBlackRooks();
      rookloc &= ~(Utils::squareToBitboard(H8));
      rookloc |= Utils::squareToBitboard(F8);
      setBlackRooks(rookloc);
    }
    if (isQueenSideCastling) {
      u64 rookloc = getBlackRooks();
      rookloc &= ~(Utils::squareToBitboard(A8));
      rookloc |= Utils::squareToBitboard(D8);
      setBlackRooks(rookloc);
    }
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

  moveHistory.push_back(move);
  setALLPiecesAggregate();
}

void Board::undoMove() {
  if (prevState)
    *this = *prevState;

  if (moveHistory.size() > 0)
    moveHistory.pop_back();
}

int Board::getAttackersCount(bool isWhite) {

  u64 kingLoc = isWhite ? getWhiteKing() : getBlackKing();

  u64 enemyPawnAttacks = isWhite
                             ? validMoveBB::blackPawnAttacks(getBlackPawns())
                             : validMoveBB::whitePawnAttacks(getWhitePawns());

  u64 enemyKnightAttacks =
      isWhite
          ? validMoveBB::knightMoves(getBlackKnights(), getAllBlackPieces())
          : validMoveBB::knightMoves(getWhiteKnights(), getAllWhitePieces());

  u64 enemyBishopAttacks =
      isWhite ? validMoveBB::bishopMoves(getBlackBishops(), getAllBlackPieces(),
                                         getAllWhitePieces())
              : validMoveBB::bishopMoves(getWhiteBishops(), getAllWhitePieces(),
                                         getAllBlackPieces());

  u64 enemyRookAttacks =
      isWhite ? validMoveBB::rookMoves(getBlackRooks(), getAllBlackPieces(),
                                       getAllWhitePieces())
              : validMoveBB::rookMoves(getWhiteRooks(), getAllWhitePieces(),
                                       getAllBlackPieces());

  u64 enemyQueenAttacks =
      isWhite ? validMoveBB::queenMoves(getBlackQueens(), getAllBlackPieces(),
                                        getAllWhitePieces())
              : validMoveBB::queenMoves(getWhiteQueens(), getAllWhitePieces(),
                                        getAllBlackPieces());

  int count = 0;

  if (kingLoc & enemyPawnAttacks)
    count++;
  if (kingLoc & enemyKnightAttacks)
    count++;
  if (kingLoc & enemyBishopAttacks)
    count++;
  if (kingLoc & enemyRookAttacks)
    count++;
  if (kingLoc & enemyQueenAttacks)
    count++;

  return count;
}

bool Board::hasKingSideRookMoved(bool isWhite) {

  PieceType rookType = isWhite ? WHITE_ROOK : BLACK_ROOK;

  if (isWhite) {
    for (const Move &move : moveHistory) {
      if ((move.getPieceType() == rookType) && (move.getFromSquare() == H1)) {
        return true;
      }
    }
  } else {
    for (const Move &move : moveHistory) {
      if ((move.getPieceType() == rookType) && (move.getFromSquare() == H8)) {
        return true;
      }
    }
  }

  return false;
}

bool Board::hasQueenSideRookMoved(bool isWhite) {

  PieceType rookType = isWhite ? WHITE_ROOK : BLACK_ROOK;

  if (isWhite) {
    for (const Move &move : moveHistory) {
      if ((move.getPieceType() == rookType) && (move.getFromSquare() == A1)) {
        return true;
      }
    }
  } else {
    for (const Move &move : moveHistory) {
      if ((move.getPieceType() == rookType) && (move.getFromSquare() == A8)) {
        return true;
      }
    }
  }

  return false;
}

bool Board::hasKingMoved(bool isWhite) {

  if (isWhite) {
    for (const Move &move : moveHistory) {
      if (move.getPieceType() == WHITE_KING) {
        return true;
      }
    }
    return false;
  } else {
    for (const Move &move : moveHistory) {
      if (move.getPieceType() == BLACK_KING) {
        return true;
      }
    }
    return false;
  }
}

bool Board::canCastleKingSide(bool isWhite) {

  u64 rookLoc = isWhite ? getWhiteRooks() : getBlackRooks();
  bool rookPresent = isWhite ? (rookLoc & Utils::squareToBitboard(H1)) != 0
                             : (rookLoc & Utils::squareToBitboard(H8)) != 0;

  if (!hasKingMoved(isWhite) && !hasKingSideRookMoved(isWhite) && rookPresent) {

    // checking if squares between king and rook are attacked or blocked
    if (isKingChecked(isWhite))
      return false;

    u64 enemyAttacks = validMoveBB::allEnemyAttacks(*this, !isWhite);
    u64 blockers = getAllWhitePieces() | getAllBlackPieces();

    if (isWhite) {
      u64 F1_square = Utils::squareToBitboard(F1);
      u64 G1_square = Utils::squareToBitboard(G1);

      if ((enemyAttacks & F1_square) != 0 || (enemyAttacks & G1_square) != 0 ||
          (blockers & F1_square) != 0 || (blockers & G1_square) != 0)
        return false;

      return true;
    } else {
      u64 F8_square = Utils::squareToBitboard(F8);
      u64 G8_square = Utils::squareToBitboard(G8);

      if ((enemyAttacks & F8_square) != 0 || (enemyAttacks & G8_square) != 0 ||
          (blockers & F8_square) != 0 || (blockers & G8_square) != 0)
        return false;

      return true;
    }
  }

  return false;
}

bool Board::canCastleQueenSide(bool isWhite) {

  u64 rookLoc = isWhite ? getWhiteRooks() : getBlackRooks();
  bool rookPresent = isWhite ? (rookLoc & Utils::squareToBitboard(A1)) != 0
                             : (rookLoc & Utils::squareToBitboard(A8)) != 0;

  if (!hasKingMoved(isWhite) && !hasQueenSideRookMoved(isWhite) &&
      rookPresent) {

    // checking if squares between king and rook are attacked or blocked
    if (isKingChecked(isWhite))
      return false;

    u64 enemyAttacks = validMoveBB::allEnemyAttacks(*this, !isWhite);
    u64 blockers = getAllWhitePieces() | getAllBlackPieces();

    if (isWhite) {
      u64 D1_square = Utils::squareToBitboard(D1);
      u64 C1_square = Utils::squareToBitboard(C1);
      u64 B1_square = Utils::squareToBitboard(B1);

      if ((enemyAttacks & D1_square) != 0 || (enemyAttacks & C1_square) != 0 ||
          (blockers & D1_square) != 0 || (blockers & C1_square) != 0 ||
          (blockers & B1_square) != 0)
        return false;

      return true;
    } else {
      u64 D8_square = Utils::squareToBitboard(D8);
      u64 C8_square = Utils::squareToBitboard(C8);
      u64 B8_square = Utils::squareToBitboard(B8);

      if ((enemyAttacks & D8_square) != 0 || (enemyAttacks & C8_square) != 0 ||
          (blockers & D8_square) != 0 || (blockers & C8_square) != 0 ||
          (blockers & B8_square) != 0)
        return false;

      return true;
    }
  }

  return false;
}
