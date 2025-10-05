#include "../include/zobrist.hpp"
#include <random>

Zobrist::Zobrist() {
  std::mt19937_64 rng(12345);

  for (int sq = 0; sq < 64; sq++) {
    for (int piece = 0; piece < 12; piece++) {
      pieceKeys[sq][piece] = rng();
    }
    enPassantKeys[sq] = rng();
  }

  for (int i = 0; i < 16; i++) {
    castleKeys[i] = rng();
  }

  blackToMoveKey = rng();
}
