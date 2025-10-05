#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP
#include "types.hpp"
#include <stdint.h>

using u64 = uint64_t;

class Zobrist {

private:
  u64 pieceKeys[64][12];
  u64 castleKeys[16];
  u64 enPassantKeys[64];
  u64 blackToMoveKey;

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
