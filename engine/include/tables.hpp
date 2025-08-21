#ifndef TABLES_HPP
#define TABLES_HPP

#include <cstdint>

using u64 = uint64_t;

namespace Tables {

extern const u64 maskRank[8];
extern const u64 maskFile[8];
extern const u64 clearRank[8];
extern const u64 clearFile[8];
extern const u64 Piece[64];
} // namespace Tables
#endif
