#ifndef MOVE_HPP
#define MOVE_HPP
#include "types.hpp"

class Move {
private:
  Square fromSquare;
  Square toSquare;
  PieceType pieceType;
  PieceType capturedPiece;
  PieceType promotionPiece;
  bool isCapture;
  bool isEnPassant;
  bool isKingSideCastle;
  bool isQueenSideCastle;
  bool isPromotion;

public:
  // Basic constructor
  Move(Square from, Square to, PieceType piece, PieceType captured = EMPTY);

  // Constructor with special move flags
  Move(Square from, Square to, PieceType piece, PieceType captured,
       bool enPassant, bool ksCastle, bool qsCastle, bool promotion,
       PieceType promoPiece = EMPTY);

  ~Move() = default;

  // Basic getters
  inline Square getFromSquare() const { return fromSquare; }
  inline Square getToSquare() const { return toSquare; }
  inline PieceType getPieceType() const { return pieceType; }
  inline PieceType getCapturedPiece() const { return capturedPiece; }
  inline PieceType getPromotionPiece() const { return promotionPiece; }

  // Flag getters
  inline bool getIsCapture() const { return isCapture; }
  inline bool getIsEnPassant() const { return isEnPassant; }
  inline bool getIsKingSideCastle() const { return isKingSideCastle; }
  inline bool getIsQueenSideCastle() const { return isQueenSideCastle; }
  inline bool getIsPromotion() const { return isPromotion; }

  // Convenience checkers
  inline bool isCastling() const {
    return isKingSideCastle || isQueenSideCastle;
  }
  inline bool isSpecialMove() const {
    return isEnPassant || isCastling() || isPromotion;
  }
};
#endif
