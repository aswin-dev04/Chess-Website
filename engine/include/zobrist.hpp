/**
 * @file zobrist.hpp
 * @brief Defines the Zobrist class for generating Zobrist keys.
 * This file contains the data structures and class definition for Zobrist
 * hashing, which is used to create a unique hash for each board position.
 */
#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP
#include "types.hpp"
#include <stdint.h>

using u64 = uint64_t;

// Generates and provides Zobrist keys for hashing board states
class Zobrist {

private:
  u64 pieceKeys[64][12]; // Keys for each piece on each square
  u64 castleKeys[16];    // Keys for each possible castling rights state
  u64 enPassantKeys[64]; // Keys for each possible en passant square
  u64 blackToMoveKey;    // Key to indicate if it is black's turn to move

public:
  Zobrist();
  ~Zobrist() = default;
  inline u64 getPieceKey(int square, PieceType piece) const {
    return pieceKeys[square][piece - 1];
  };
  inline u64 getCastleKey(int castlingRights) const {
    return castleKeys[castlingRights];
  };
  inline u64 getEnPassantKey(int square) const {
    return enPassantKeys[square];
  };
  inline u64 getBlackToMoveKey() const { return blackToMoveKey; };
};

#endif
