/**
 * @file utils.hpp
 * @brief Defines utility functions for the chess engine.
 * This file contains various helper functions for bitboard manipulation,
 * square conversions, and other common tasks.
 */
#ifndef UTILS_HPP
#define UTILS_HPP
#include "board.hpp"
#include "tables.hpp"
#include "types.hpp"
#include <cstdint>

using u64 = uint64_t;

namespace Utils {
// Converts a bitboard with a single set bit to a square index
inline Square bitboardToSquare(u64 bitboard) {
  return static_cast<Square>(__builtin_ctzll(bitboard));
}

// Converts a square index to a bitboard with a single set bit
inline u64 squareToBitboard(int sq) { return Tables::Piece[sq]; }

// --- Bit manipulation helpers ---
inline int getLSBIndex(u64 bitboard) { return __builtin_ctzll(bitboard); }

inline u64 clearLSB(u64 bitboard) { return bitboard & (bitboard - 1); }

inline u64 clearBitAt(u64 bitboard, int square) {
  return bitboard & ~Tables::Piece[square];
}

inline u64 setBitAt(u64 bitboard, int square) {
  return bitboard | Tables::Piece[square];
}

// Returns the least significant bit and clears it from the bitboard
inline Square popLSB(u64 &bitboard) {
  Square sq = static_cast<Square>(__builtin_ctzll(bitboard));
  bitboard = clearLSB(bitboard);
  return sq;
}

// Counts the number of set bits in a bitboard
inline int popcount(u64 bitboard) { return __builtin_popcountll(bitboard); }

// Checks if a bitboard has exactly one bit set
inline bool isOneBit(u64 bitboard) {
  return bitboard && !(bitboard & (bitboard - 1));
}

// --- Square manipulation helpers ---
inline int getRank(Square sq) { return sq / 8; }

inline int getFile(Square sq) { return sq % 8; }

inline Square makeSquare(int rank, int file) {
  return static_cast<Square>(rank * 8 + file);
}

// --- Piece and board helpers ---
// Returns the type of piece at a given square
PieceType getPieceTypeAt(Board &board, Square square);
// Counts the number of captures a piece type can make
int getCaptureCount(Board &board, PieceType pieceType, bool isWhite);

// Converts a string in algebraic notation to a square index
Square stringToSquare(const std::string &square);
// Converts a character to a piece type
PieceType charToPiece(char c);

// Counts the number of pieces of a given type on the board
int getPieceCount(Board &board, PieceType pieceType);

} // namespace Utils
#endif
