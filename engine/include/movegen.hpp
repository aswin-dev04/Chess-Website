#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "board.hpp"
#include "move.hpp"
#include "tables.hpp"
#include <vector>

namespace validMoveBB {
u64 kingMoves(u64 kingLoc, u64 ownPieces);
u64 rookMoves(u64 rookLoc, u64 ownPieces, u64 enemyPieces);
u64 bishopMoves(u64 bishopLoc, u64 ownPieces, u64 enemyPieces);
u64 queenMoves(u64 queenLoc, u64 ownPieces, u64 enemyPieces);
u64 knightMoves(u64 knightLoc, u64 ownPieces);
u64 whitePawnMoves(u64 pawnLoc, u64 ownPieces, u64 allPieces, u64 enemyPieces);
u64 blackPawnMoves(u64 pawnLoc, u64 ownPieces, u64 allPieces, u64 enemyPieces);
}; // namespace validMoveBB

class MoveGeneration {
public:
  // non-sliding pieces
  static std::vector<Move> generateKingMoves(Board &board, bool isWhite);
  static std::vector<Move> generateKnightMoves(Board &board, bool isWhite);
  static std::vector<Move> generatePawnMoves(Board &board, bool isWhite);

  // sliding pieces
  static std::vector<Move> generateRookMoves(Board &board, bool isWhite);
  static std::vector<Move> generateBishopMoves(Board &board, bool isWhite);
  static std::vector<Move> generateQueenMoves(Board &board, bool isWhite);
};
#endif
