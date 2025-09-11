#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include <cctype>
#include <sstream>
#include <string>

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
  stateHistory = {};
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
  stateHistory = {};
  moveHistory = {};
}

Board::Board(const std::string &fen) {
  loadFromFen(fen);
  stateHistory = {};
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
  canWhiteCastleKS = other.canWhiteCastleKS;
  canWhiteCastleQS = other.canWhiteCastleQS;
  canBlackCastleKS = other.canBlackCastleKS;
  canBlackCastleQS = other.canBlackCastleQS;
  enPassantSquare = other.enPassantSquare;
  whiteToMove = other.whiteToMove;
  return *this;
}

bool Board::isKingChecked(bool isWhite) {

  u64 enemyAttacks = validMoveBB::allEnemyAttacks(*this, !isWhite);
  u64 king = isWhite ? getWhiteKing() : getBlackKing();
  return (enemyAttacks & king) != 0;
}

void Board::makeMove(const Move &move) {

  undoInfo undo;
  undo.move = move;
  undo.capturedPiece = move.getCapturedPiece();
  undo.enPassantSquare = enPassantSquare;
  undo.canWhiteCastleKS = canWhiteCastleKS;
  undo.canWhiteCastleQS = canWhiteCastleQS;
  undo.canBlackCastleKS = canBlackCastleKS;
  undo.canBlackCastleQS = canBlackCastleQS;
  stateHistory.push_back(undo);

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

  if (piece == WHITE_KING) {
    canWhiteCastleKS = false;
    canWhiteCastleQS = false;
  } else if (piece == BLACK_KING) {
    canBlackCastleKS = false;
    canBlackCastleQS = false;
  }

  // Update castling rights if rooks move or are captured
  if (fromSquare == H1 || toSquare == H1)
    canWhiteCastleKS = false;
  if (fromSquare == A1 || toSquare == A1)
    canWhiteCastleQS = false;
  if (fromSquare == H8 || toSquare == H8)
    canBlackCastleKS = false;
  if (fromSquare == A8 || toSquare == A8)
    canBlackCastleQS = false;

  // Update en passant target
  if ((piece == WHITE_PAWN || piece == BLACK_PAWN) &&
      abs(toSquare - fromSquare) == 16) {
    // Double pawn move
    enPassantSquare = static_cast<Square>((fromSquare + toSquare) / 2);
  } else {
    enPassantSquare = SQ_NONE;
  }

  // Switch turns
  whiteToMove = !whiteToMove;
}

void Board::undoMove() {
  undoInfo undo = stateHistory.back();
  stateHistory.pop_back();

  Move move = undo.move;
  Square fromSquare = move.getFromSquare();
  Square toSquare = move.getToSquare();
  PieceType piece = move.getPieceType();
  PieceType capturedPiece = undo.capturedPiece;
  bool isCapture = move.getIsCapture();
  bool isPromotion = move.getIsPromotion();
  bool isEnPassant = move.getIsEnPassant();
  bool isKingSideCastling = move.getIsKingSideCastle();
  bool isQueenSideCastling = move.getIsQueenSideCastle();

  whiteToMove = !whiteToMove;

  enPassantSquare = undo.enPassantSquare;
  canWhiteCastleKS = undo.canWhiteCastleKS;
  canWhiteCastleQS = undo.canWhiteCastleQS;
  canBlackCastleKS = undo.canBlackCastleKS;
  canBlackCastleQS = undo.canBlackCastleQS;

  moveHistory.pop_back();

  u64 currPieceLoc = 0ULL;
  u64 capturedPieceLoc = 0ULL;

  if (isEnPassant) {
    if (piece == WHITE_PAWN) {
      u64 ownPawns = getWhitePawns();
      ownPawns &= ~(Utils::squareToBitboard(toSquare));
      ownPawns |= Utils::squareToBitboard(fromSquare);
      setWhitePawns(ownPawns);

      Square capturedPawnSquare = static_cast<Square>(toSquare - 8);
      u64 enemyPawns = getBlackPawns();
      enemyPawns |= Utils::squareToBitboard(capturedPawnSquare);
      setBlackPawns(enemyPawns);

      setALLPiecesAggregate();
      return;
    } else {
      u64 ownPawns = getBlackPawns();
      ownPawns &= ~(Utils::squareToBitboard(toSquare));
      ownPawns |= Utils::squareToBitboard(fromSquare);
      setBlackPawns(ownPawns);

      Square capturedPawnSquare = static_cast<Square>(toSquare + 8);
      u64 enemyPawns = getWhitePawns();
      enemyPawns |= Utils::squareToBitboard(capturedPawnSquare);
      setWhitePawns(enemyPawns);

      setALLPiecesAggregate();
      return;
    }
  }

  switch (piece) {
  case EMPTY:
    break;
  case WHITE_PAWN:
    currPieceLoc = getWhitePawns();
    if (isPromotion) {
      PieceType promotedPiece = move.getPromotionPiece();

      switch (promotedPiece) {
      case WHITE_QUEEN: {
        u64 promoPieces = getWhiteQueens();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setWhiteQueens(promoPieces);
        break;
      }
      case WHITE_ROOK: {
        u64 promoPieces = getWhiteRooks();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setWhiteRooks(promoPieces);
        break;
      }
      case WHITE_BISHOP: {
        u64 promoPieces = getWhiteBishops();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setWhiteBishops(promoPieces);
        break;
      }
      case WHITE_KNIGHT: {
        u64 promoPieces = getWhiteKnights();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setWhiteKnights(promoPieces);
        break;
      }
      default:
        break;
      }
      currPieceLoc |= Utils::squareToBitboard(fromSquare);
    } else {
      currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      currPieceLoc |= Utils::squareToBitboard(fromSquare);
    }
    setWhitePawns(currPieceLoc);
    break;

  case BLACK_PAWN:
    currPieceLoc = getBlackPawns();
    if (isPromotion) {
      PieceType promotedPiece = move.getPromotionPiece();

      switch (promotedPiece) {
      case BLACK_QUEEN: {
        u64 promoPieces = getBlackQueens();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setBlackQueens(promoPieces);
        break;
      }
      case BLACK_ROOK: {
        u64 promoPieces = getBlackRooks();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setBlackRooks(promoPieces);
        break;
      }
      case BLACK_BISHOP: {
        u64 promoPieces = getBlackBishops();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setBlackBishops(promoPieces);
        break;
      }
      case BLACK_KNIGHT: {
        u64 promoPieces = getBlackKnights();
        promoPieces &= ~(Utils::squareToBitboard(toSquare));
        setBlackKnights(promoPieces);
        break;
      }
      default:
        break;
      }
      currPieceLoc |= Utils::squareToBitboard(fromSquare);
    } else {
      currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
      currPieceLoc |= Utils::squareToBitboard(fromSquare);
    }
    setBlackPawns(currPieceLoc);
    break;

  case WHITE_KNIGHT:
    currPieceLoc = getWhiteKnights();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setWhiteKnights(currPieceLoc);
    break;

  case BLACK_KNIGHT:
    currPieceLoc = getBlackKnights();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setBlackKnights(currPieceLoc);
    break;

  case WHITE_BISHOP:
    currPieceLoc = getWhiteBishops();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setWhiteBishops(currPieceLoc);
    break;

  case BLACK_BISHOP:
    currPieceLoc = getBlackBishops();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setBlackBishops(currPieceLoc);
    break;

  case WHITE_ROOK:
    currPieceLoc = getWhiteRooks();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setWhiteRooks(currPieceLoc);
    break;

  case BLACK_ROOK:
    currPieceLoc = getBlackRooks();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setBlackRooks(currPieceLoc);
    break;

  case WHITE_QUEEN:
    currPieceLoc = getWhiteQueens();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setWhiteQueens(currPieceLoc);
    break;

  case BLACK_QUEEN:
    currPieceLoc = getBlackQueens();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setBlackQueens(currPieceLoc);
    break;

  case WHITE_KING:
    currPieceLoc = getWhiteKing();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setWhiteKing(currPieceLoc);

    if (isKingSideCastling) {
      u64 rookloc = getWhiteRooks();
      rookloc &= ~(Utils::squareToBitboard(F1));
      rookloc |= Utils::squareToBitboard(H1);
      setWhiteRooks(rookloc);
    }
    if (isQueenSideCastling) {
      u64 rookloc = getWhiteRooks();
      rookloc &= ~(Utils::squareToBitboard(D1));
      rookloc |= Utils::squareToBitboard(A1);
      setWhiteRooks(rookloc);
    }
    break;

  case BLACK_KING:
    currPieceLoc = getBlackKing();
    currPieceLoc &= ~(Utils::squareToBitboard(toSquare));
    currPieceLoc |= Utils::squareToBitboard(fromSquare);
    setBlackKing(currPieceLoc);

    // Reverse castling rook moves
    if (isKingSideCastling) {
      u64 rookloc = getBlackRooks();
      rookloc &= ~(Utils::squareToBitboard(F8));
      rookloc |= Utils::squareToBitboard(H8);
      setBlackRooks(rookloc);
    }
    if (isQueenSideCastling) {
      u64 rookloc = getBlackRooks();
      rookloc &= ~(Utils::squareToBitboard(D8));
      rookloc |= Utils::squareToBitboard(A8);
      setBlackRooks(rookloc);
    }
    break;
  }

  if (isCapture && capturedPiece != EMPTY) {
    switch (capturedPiece) {
    case WHITE_PAWN:
      capturedPieceLoc = getWhitePawns();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setWhitePawns(capturedPieceLoc);
      break;
    case BLACK_PAWN:
      capturedPieceLoc = getBlackPawns();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setBlackPawns(capturedPieceLoc);
      break;
    case WHITE_KNIGHT:
      capturedPieceLoc = getWhiteKnights();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setWhiteKnights(capturedPieceLoc);
      break;
    case BLACK_KNIGHT:
      capturedPieceLoc = getBlackKnights();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setBlackKnights(capturedPieceLoc);
      break;
    case WHITE_BISHOP:
      capturedPieceLoc = getWhiteBishops();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setWhiteBishops(capturedPieceLoc);
      break;
    case BLACK_BISHOP:
      capturedPieceLoc = getBlackBishops();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setBlackBishops(capturedPieceLoc);
      break;
    case WHITE_ROOK:
      capturedPieceLoc = getWhiteRooks();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setWhiteRooks(capturedPieceLoc);
      break;
    case BLACK_ROOK:
      capturedPieceLoc = getBlackRooks();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setBlackRooks(capturedPieceLoc);
      break;
    case WHITE_QUEEN:
      capturedPieceLoc = getWhiteQueens();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setWhiteQueens(capturedPieceLoc);
      break;
    case BLACK_QUEEN:
      capturedPieceLoc = getBlackQueens();
      capturedPieceLoc |= Utils::squareToBitboard(toSquare);
      setBlackQueens(capturedPieceLoc);
      break;
    default:
      break;
    }
  }

  setALLPiecesAggregate();
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

bool Board::canCastleKingSide(bool isWhite) {
  // Check FEN castling rights first
  bool hasCastleRights = isWhite ? canWhiteCastleKS : canBlackCastleKS;
  if (!hasCastleRights)
    return false;

  // Check if king and rook are in correct positions
  u64 kingLoc = isWhite ? getWhiteKing() : getBlackKing();
  u64 rookLoc = isWhite ? getWhiteRooks() : getBlackRooks();

  bool kingPresent = isWhite ? (kingLoc & Utils::squareToBitboard(E1)) != 0
                             : (kingLoc & Utils::squareToBitboard(E8)) != 0;
  bool rookPresent = isWhite ? (rookLoc & Utils::squareToBitboard(H1)) != 0
                             : (rookLoc & Utils::squareToBitboard(H8)) != 0;

  if (!kingPresent || !rookPresent)
    return false;

  // Check if king is in check
  if (isKingChecked(isWhite))
    return false;

  // Check if squares between king and rook are clear and not attacked
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

bool Board::canCastleQueenSide(bool isWhite) {
  // Check FEN castling rights first
  bool hasCastleRights = isWhite ? canWhiteCastleQS : canBlackCastleQS;
  if (!hasCastleRights)
    return false;

  // Check if king and rook are in correct positions
  u64 kingLoc = isWhite ? getWhiteKing() : getBlackKing();
  u64 rookLoc = isWhite ? getWhiteRooks() : getBlackRooks();

  bool kingPresent = isWhite ? (kingLoc & Utils::squareToBitboard(E1)) != 0
                             : (kingLoc & Utils::squareToBitboard(E8)) != 0;
  bool rookPresent = isWhite ? (rookLoc & Utils::squareToBitboard(A1)) != 0
                             : (rookLoc & Utils::squareToBitboard(A8)) != 0;

  if (!kingPresent || !rookPresent)
    return false;

  // Check if king is in check
  if (isKingChecked(isWhite))
    return false;

  // Check if squares between king and rook are clear and not attacked
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

void Board::loadFromFen(const std::string &fen) {

  // Clear all bitboards first
  whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens =
      whiteKing = 0;
  blackPawns = blackKnights = blackBishops = blackRooks = blackQueens =
      blackKing = 0;

  std::istringstream ss(fen);
  std::string token;

  // 1. Parse piece placement (first part of FEN)
  std::getline(ss, token, ' ');
  int rank = 7; // Start from rank 8 (index 7)
  int file = 0; // Start from file a (index 0)

  for (char c : token) {
    if (c == '/') {
      // Move to next rank
      rank--;
      file = 0;
    } else if (std::isdigit(c)) {
      // Skip empty squares
      file += (c - '0');
    } else {
      // Place piece
      Square sq = static_cast<Square>(rank * 8 + file);
      PieceType piece = Utils::charToPiece(c);
      u64 bitboard = Utils::squareToBitboard(sq);

      switch (piece) {
      case WHITE_PAWN:
        whitePawns |= bitboard;
        break;
      case BLACK_PAWN:
        blackPawns |= bitboard;
        break;
      case WHITE_KNIGHT:
        whiteKnights |= bitboard;
        break;
      case BLACK_KNIGHT:
        blackKnights |= bitboard;
        break;
      case WHITE_BISHOP:
        whiteBishops |= bitboard;
        break;
      case BLACK_BISHOP:
        blackBishops |= bitboard;
        break;
      case WHITE_ROOK:
        whiteRooks |= bitboard;
        break;
      case BLACK_ROOK:
        blackRooks |= bitboard;
        break;
      case WHITE_QUEEN:
        whiteQueens |= bitboard;
        break;
      case BLACK_QUEEN:
        blackQueens |= bitboard;
        break;
      case WHITE_KING:
        whiteKing |= bitboard;
        break;
      case BLACK_KING:
        blackKing |= bitboard;
        break;
      default:
        break;
      }
      file++;
    }
  }

  // 2. Parse active color (whose turn it is)
  std::getline(ss, token, ' ');
  whiteToMove = (token == "w");

  // 3. Parse castling rights
  std::getline(ss, token, ' ');
  canWhiteCastleKS = (token.find('K') != std::string::npos);
  canWhiteCastleQS = (token.find('Q') != std::string::npos);
  canBlackCastleKS = (token.find('k') != std::string::npos);
  canBlackCastleQS = (token.find('q') != std::string::npos);

  // 4. Parse en passant target square
  std::getline(ss, token, ' ');
  if (token == "-") {
    enPassantSquare = SQ_NONE;
  } else {
    enPassantSquare = Utils::stringToSquare(token);
  }

  // Update aggregate bitboards
  setALLPiecesAggregate();
}
