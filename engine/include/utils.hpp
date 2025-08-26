#ifndef UTILS_HPP
#define UTILS_HPP
#include "board.hpp"
#include "tables.hpp"
#include "types.hpp"
#include <cstdint>

using u64 = uint64_t;

namespace Utils {
// Bitboard to Square conversion
inline Square bitboardToSquare(u64 bitboard) {
  return static_cast<Square>(__builtin_ctzll(bitboard));
}

// Square to Bitboard conversion
inline u64 squareToBitboard(Square sq) { return Tables::Piece[sq]; }

// Bit manipulation helpers
inline int getLSBIndex(u64 bitboard) { return __builtin_ctzll(bitboard); }

inline u64 clearLSB(u64 bitboard) { return bitboard & (bitboard - 1); }

inline u64 clearBitAt(u64 bitboard, int square) {
  return bitboard & ~Tables::Piece[square];
}

inline u64 setBitAt(u64 bitboard, int square) {
  return bitboard | Tables::Piece[square];
}

// Pop LSB and return its square
inline Square popLSB(u64 &bitboard) {
  Square sq = static_cast<Square>(__builtin_ctzll(bitboard));
  bitboard = clearLSB(bitboard);
  return sq;
}

// Count number of set bits
inline int popcount(u64 bitboard) { return __builtin_popcountll(bitboard); }

// Check if bitboard has exactly one bit set
inline bool isOneBit(u64 bitboard) {
  return bitboard && !(bitboard & (bitboard - 1));
}

// Get rank/file from square
inline int getRank(Square sq) { return sq / 8; }

inline int getFile(Square sq) { return sq % 8; }

// Create square from rank/file
inline Square makeSquare(int rank, int file) {
  return static_cast<Square>(rank * 8 + file);
}

// Get piece type at square
PieceType getPieceTypeAt(Board &board, Square square);

int getCaptureCount(Board &board, PieceType pieceType, bool isWhite);
} // namespace Utils
#endif
