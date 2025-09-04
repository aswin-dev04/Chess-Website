#ifndef MOVE_HPP
#define MOVE_HPP
#include "types.hpp"
#include <iostream>

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
  // Default constructor - initialize all members
  Move()
      : fromSquare(Square::SQ_NONE), toSquare(Square::SQ_NONE),
        pieceType(EMPTY), capturedPiece(EMPTY), promotionPiece(EMPTY),
        isCapture(false), isEnPassant(false), isKingSideCastle(false),
        isQueenSideCastle(false), isPromotion(false) {} // Basic constructor
  Move(Square from, Square to, PieceType piece, PieceType captured = EMPTY)
      : fromSquare(from), toSquare(to), pieceType(piece),
        capturedPiece(captured), promotionPiece(EMPTY),
        isCapture(captured != EMPTY), isEnPassant(false),
        isKingSideCastle(false), isQueenSideCastle(false), isPromotion(false) {}

  // Constructor with special move flags
  Move(Square from, Square to, PieceType piece, PieceType captured,
       bool enPassant, bool ksCastle, bool qsCastle, bool promotion,
       PieceType promoPiece = EMPTY)
      : fromSquare(from), toSquare(to), pieceType(piece),
        capturedPiece(captured), promotionPiece(promoPiece),
        isCapture(captured != EMPTY || enPassant), isEnPassant(enPassant),
        isKingSideCastle(ksCastle), isQueenSideCastle(qsCastle),
        isPromotion(promotion) {}

  ~Move() = default;

  Move &operator=(const Move &other);

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
  friend std::ostream &operator<<(std::ostream &os, const Move &move);
};
#endif
