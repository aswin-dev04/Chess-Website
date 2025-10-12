/**
 * @file magic.hpp
 * @brief Defines the structures and functions for magic bitboard move
 * generation. This file contains the pre-computed magic numbers, attack tables,
 * and functions for generating sliding piece attacks (rooks and bishops).
 */
#ifndef MAGIC_HPP
#define MAGIC_HPP

#include "types.hpp"
#include <cstdint>

using u64 = std::uint64_t;

namespace Magic {
// Pre-computed magic numbers for rooks and bishops
extern const u64 rookMagics[64];
extern const u64 bishopMagics[64];

// Masks for relevant occupancy bits for each square
extern u64 rookMasks[64];
extern u64 bishopMasks[64];

// Lookup tables for rook and bishop attacks
extern u64 rookAttacks[64][4096];
extern u64 bishopAttacks[64][512];

// Number of relevant occupancy bits for each square
extern const int rookBits[64];
extern const int bishopBits[64];

// Initializes the magic bitboard tables
void initMagics();

// Generates rook attacks using magic bitboards
inline u64 getRookAttacks(Square sq, u64 occupied) {
  occupied &= rookMasks[sq];
  occupied *= rookMagics[sq];
  occupied >>= (64 - rookBits[sq]);
  return rookAttacks[sq][(std::size_t)occupied];
}

// Generates bishop attacks using magic bitboards
inline u64 getBishopAttacks(Square sq, u64 occupied) {
  occupied &= bishopMasks[sq];
  occupied *= bishopMagics[sq];
  occupied >>= (64 - bishopBits[sq]);
  return bishopAttacks[sq][(std::size_t)occupied];
}
} // namespace Magic

#endif // MAGIC_HPP
