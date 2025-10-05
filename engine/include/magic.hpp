#ifndef MAGIC_HPP
#define MAGIC_HPP

#include "types.hpp"
#include <cstdint>

using u64 = std::uint64_t;

namespace Magic {
// Pre-computed magic numbers (from Stockfish)
extern const u64 rookMagics[64];
extern const u64 bishopMagics[64];

// Relevant occupancy masks (exclude edge squares)
extern u64 rookMasks[64];
extern u64 bishopMasks[64];

// Attack lookup tables
extern u64 rookAttacks[64][4096];
extern u64 bishopAttacks[64][512];

// Number of relevant occupancy bits per square
extern const int rookBits[64];
extern const int bishopBits[64];

// Initialize tables (call once at startup)
void initMagics();

// Fast attack lookups
inline u64 getRookAttacks(Square sq, u64 occupied) {
  occupied &= rookMasks[sq];
  occupied *= rookMagics[sq];
  occupied >>= (64 - rookBits[sq]);
  // Cast to size_t to ensure proper array indexing
  return rookAttacks[sq][(std::size_t)occupied];
}

inline u64 getBishopAttacks(Square sq, u64 occupied) {
  occupied &= bishopMasks[sq];
  occupied *= bishopMagics[sq];
  occupied >>= (64 - bishopBits[sq]);
  // Cast to size_t to ensure proper array indexing
  return bishopAttacks[sq][(std::size_t)occupied];
}
} // namespace Magic

#endif // MAGIC_HPP
