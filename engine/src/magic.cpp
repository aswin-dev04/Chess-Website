#include "../include/magic.hpp"
#include "../include/utils.hpp"

namespace Magic {

// Pre-computed magic numbers that create perfect hash functions
const u64 rookMagics[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,
    0x100081001000420ULL,  0x200020010080420ULL,  0x3001c0002010008ULL,
    0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,
    0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
    0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL,
    0x801000060821100ULL,  0x80044006422000ULL,   0x100808020004000ULL,
    0x12108a0010204200ULL, 0x140848010000802ULL,  0x481828014002800ULL,
    0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,
    0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,
    0x80088400100102ULL,   0x80004600042881ULL,   0x4040008040800020ULL,
    0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,
    0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,
    0x200046502000484ULL,  0x480400080088020ULL,  0x1000422010034000ULL,
    0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL,
    0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL,
    0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,
    0x2935610830022400ULL, 0x44440041009200ULL,   0x280001040802101ULL,
    0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,
    0x1004081002402ULL};

const u64 bishopMagics[64] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,
    0x108060845042010ULL,  0x581104180800210ULL,  0x2112080446200010ULL,
    0x1080820820060210ULL, 0x3c0808410220200ULL,  0x4050404440404ULL,
    0x21001420088ULL,      0x24d0080801082102ULL, 0x1020a0a020400ULL,
    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,   0x404022024108200ULL,
    0x810018200204102ULL,  0x4002801a02003ULL,    0x85040820080400ULL,
    0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL, 0x152048408022401ULL,
    0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL,   0x1c004001012080ULL,   0x8004200962a00220ULL,
    0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL,
    0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL,
    0x42008c0340209202ULL, 0x209188240001000ULL,  0x400408a884001800ULL,
    0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,  0x4000020e01040044ULL,
    0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,
    0x5a84841004010310ULL, 0x4010801011c04ULL,    0xa010109502200ULL,
    0x4a02012000a03002ULL, 0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL,
    0x4010011029020020ULL};

// Number of relevant bits (determines table size)
const int rookBits[64] = {12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10,
                          10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10,
                          10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10,
                          11, 11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10,
                          10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

const int bishopBits[64] = {6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5,
                            5, 5, 7, 7, 7, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5,
                            5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5,
                            5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

u64 rookMasks[64];
u64 bishopMasks[64];
u64 rookAttacks[64][4096];
u64 bishopAttacks[64][512];

// Generate occupancy mask for rook (exclude board edges)
u64 maskRookOccupancy(Square sq) {
  u64 mask = 0ULL;
  int rank = sq / 8;
  int file = sq % 8;

  // North - stop before rank 7
  for (int r = rank + 1; r <= 6; r++)
    mask |= (1ULL << (r * 8 + file));

  // South - stop after rank 1
  for (int r = rank - 1; r >= 1; r--)
    mask |= (1ULL << (r * 8 + file));

  // East - stop before file H
  for (int f = file + 1; f <= 6; f++)
    mask |= (1ULL << (rank * 8 + f));

  // West - stop after file A
  for (int f = file - 1; f >= 1; f--)
    mask |= (1ULL << (rank * 8 + f));

  return mask;
}

// Generate occupancy mask for bishop (exclude board edges)
u64 maskBishopOccupancy(Square sq) {
  u64 mask = 0ULL;
  int rank = sq / 8;
  int file = sq % 8;

  // Northeast
  for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++)
    mask |= (1ULL << (r * 8 + f));

  // Northwest
  for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--)
    mask |= (1ULL << (r * 8 + f));

  // Southeast
  for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++)
    mask |= (1ULL << (r * 8 + f));

  // Southwest
  for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--)
    mask |= (1ULL << (r * 8 + f));

  return mask;
}

// Generate rook attacks for specific blocker configuration
u64 rookAttacksOnTheFly(Square sq, u64 blockers) {
  u64 attacks = 0ULL;
  int rank = sq / 8;
  int file = sq % 8;

  // North
  for (int r = rank + 1; r <= 7; r++) {
    attacks |= (1ULL << (r * 8 + file));
    if (blockers & (1ULL << (r * 8 + file)))
      break;
  }

  // South
  for (int r = rank - 1; r >= 0; r--) {
    attacks |= (1ULL << (r * 8 + file));
    if (blockers & (1ULL << (r * 8 + file)))
      break;
  }

  // East
  for (int f = file + 1; f <= 7; f++) {
    attacks |= (1ULL << (rank * 8 + f));
    if (blockers & (1ULL << (rank * 8 + f)))
      break;
  }

  // West
  for (int f = file - 1; f >= 0; f--) {
    attacks |= (1ULL << (rank * 8 + f));
    if (blockers & (1ULL << (rank * 8 + f)))
      break;
  }

  return attacks;
}

// Generate bishop attacks for specific blocker configuration
u64 bishopAttacksOnTheFly(Square sq, u64 blockers) {
  u64 attacks = 0ULL;
  int rank = sq / 8;
  int file = sq % 8;

  // Northeast
  for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
    attacks |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  // Northwest
  for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
    attacks |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  // Southeast
  for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
    attacks |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  // Southwest
  for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
    attacks |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  return attacks;
}

// Set occupancy bits based on index (generates all 2^n blocker patterns)
u64 setOccupancy(int index, int bitsInMask, u64 attackMask) {
  u64 occupancy = 0ULL;

  for (int count = 0; count < bitsInMask; count++) {
    // Get LS1B
    int square = __builtin_ctzll(attackMask);

    // Pop LS1B
    attackMask &= attackMask - 1;

    // If bit is set in index, set it in occupancy
    if (index & (1ULL << count)) {
      occupancy |= (1ULL << square);
    }
  }

  return occupancy;
}

void initMagics() {
  // Initialize rook attacks
  for (int sq = 0; sq < 64; sq++) {
    rookMasks[sq] = maskRookOccupancy((Square)sq);

    int relevantBits = rookBits[sq];
    int occupancyIndices = (1 << relevantBits);

    for (int index = 0; index < occupancyIndices; index++) {
      u64 occupancy = setOccupancy(index, relevantBits, rookMasks[sq]);

      // Apply magic formula: (occupancy * magic) >> (64 - bits)
      int magicIndex = (occupancy * rookMagics[sq]) >> (64 - relevantBits);

      rookAttacks[sq][magicIndex] = rookAttacksOnTheFly((Square)sq, occupancy);
    }
  }

  // Initialize bishop attacks
  for (int sq = 0; sq < 64; sq++) {
    bishopMasks[sq] = maskBishopOccupancy((Square)sq);

    int relevantBits = bishopBits[sq];
    int occupancyIndices = (1 << relevantBits);

    for (int index = 0; index < occupancyIndices; index++) {
      u64 occupancy = setOccupancy(index, relevantBits, bishopMasks[sq]);

      int magicIndex = (occupancy * bishopMagics[sq]) >> (64 - relevantBits);

      bishopAttacks[sq][magicIndex] =
          bishopAttacksOnTheFly((Square)sq, occupancy);
    }
  }
}

} // namespace Magic
