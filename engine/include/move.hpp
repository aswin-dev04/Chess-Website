/**
 * @file move.hpp
 * @brief Defines the Move class, which represents a single move in a chess
 * game. This file contains the data structure for a move, including its type,
 * origin, destination, and any special flags.
 */
#ifndef MOVE_HPP
#define MOVE_HPP
#include "types.hpp"
#include <iostream>

// Represents a single move in a chess game
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
  // Default constructor for a null move
  Move()
      : fromSquare(Square::SQ_NONE), toSquare(Square::SQ_NONE),
        pieceType(EMPTY), capturedPiece(EMPTY), promotionPiece(EMPTY),
        isCapture(false), isEnPassant(false), isKingSideCastle(false),
        isQueenSideCastle(false), isPromotion(false) {}
  // Constructor for a simple move
  Move(Square from, Square to, PieceType piece, PieceType captured = EMPTY)
      : fromSquare(from), toSquare(to), pieceType(piece),
        capturedPiece(captured), promotionPiece(EMPTY),
        isCapture(captured != EMPTY), isEnPassant(false),
        isKingSideCastle(false), isQueenSideCastle(false), isPromotion(false) {}

  // Constructor for special moves (castling, en passant, promotion)
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
  bool operator==(const Move &other) const;

  // --- Getters for move details ---
  inline Square getFromSquare() const { return fromSquare; }
  inline Square getToSquare() const { return toSquare; }
  inline PieceType getPieceType() const { return pieceType; }
  inline PieceType getCapturedPiece() const { return capturedPiece; }
  inline PieceType getPromotionPiece() const { return promotionPiece; }

  // --- Getters for move flags ---
  inline bool getIsCapture() const { return isCapture; }
  inline bool getIsEnPassant() const { return isEnPassant; }
  inline bool getIsKingSideCastle() const { return isKingSideCastle; }
  inline bool getIsQueenSideCastle() const { return isQueenSideCastle; }
  inline bool getIsPromotion() const { return isPromotion; }

  // --- Convenience checkers ---
  // Checks if the move is a castling move
  inline bool isCastling() const {
    return isKingSideCastle || isQueenSideCastle;
  }
  // Checks if the move is a special move (en passant, castling, or promotion)
  inline bool isSpecialMove() const {
    return isEnPassant || isCastling() || isPromotion;
  }
  friend std::ostream &operator<<(std::ostream &os, const Move &move);
};
#endif
