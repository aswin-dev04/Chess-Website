/**
 * @file zobrist.cpp
 * @brief Implements the Zobrist class for Zobrist hashing.
 * This file contains the implementation of the Zobrist key generation, which is
 * used to create a unique hash for each board position.
 */
#include "../include/zobrist.hpp"
#include <random>

// Initializes the Zobrist keys with random 64-bit numbers
Zobrist::Zobrist() {
  std::mt19937_64 rng(12345); // Mersenne Twister random number generator

  // Initialize piece keys for each piece on each square
  for (int sq = 0; sq < 64; sq++) {
    for (int piece = 0; piece < 12; piece++) {
      pieceKeys[sq][piece] = rng();
    }
    enPassantKeys[sq] = rng();
  }

  // Initialize castling keys for each possible castling rights state
  for (int i = 0; i < 16; i++) {
    castleKeys[i] = rng();
  }

  // Initialize the key for black to move
  blackToMoveKey = rng();
}
