#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "board.hpp"
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

u64 whitePawnAttacks(u64 pawnLoc);
u64 blackPawnAttacks(u64 pawnLoc);
u64 allEnemyAttacks(Board &board, bool isWhite);

u64 kingLegalMoves(Board &board, bool isWhite);
}; // namespace validMoveBB

class MoveGeneration {
public:
  // These methods only generate Pseudolegal moves

  // non-sliding pieces
  static std::vector<Move> generateKingMoves(Board &board, bool isWhite);
  static std::vector<Move> generateKnightMoves(Board &board, bool isWhite);
  static std::vector<Move> generatePawnMoves(Board &board, bool isWhite);

  // sliding pieces
  static std::vector<Move> generateRookMoves(Board &board, bool isWhite);
  static std::vector<Move> generateBishopMoves(Board &board, bool isWhite);
  static std::vector<Move> generateQueenMoves(Board &board, bool isWhite);

  // Legal Moves

  static std::vector<Move> generateAllMoves(Board &board, bool isWhite);
  static std::vector<Move>
  generateLegalMovesWhileInCheck(Board &board, bool isWhite,
                                 std::vector<Move> allMoves);

  // non-sliding pieces
  static std::vector<Move> generateKingLegalMoves(Board &board, bool isWhite);
  static std::vector<Move> generateKnightLegalMoves(Board &board, bool isWhite,
                                                    bool inCheck,
                                                    u64 pinnedPieces);
  static std::vector<Move> generatePawnLegalMoves(Board &board, bool isWhite,
                                                  bool inCheck,
                                                  u64 pinnedPieces);

  // sliding pieces
  static std::vector<Move> generateRookLegalMoves(Board &board, bool isWhite,
                                                  bool inCheck,
                                                  u64 pinnedPieces);
  static std::vector<Move> generateBishopLegalMoves(Board &board, bool isWhite,
                                                    bool inCheck,
                                                    u64 pinnedPieces);
  static std::vector<Move> generateQueenLegalMoves(Board &board, bool isWhite,
                                                   bool inCheck,
                                                   u64 pinnedPieces);

  // special moves
  static std::vector<Move> generatePawnPromotionMoves(Board &board, u64 pawnLoc,
                                                      bool isWhite);
  static std::vector<Move> generateCastlingMoves(Board &board, bool isWhite);
  static std::vector<Move> generateEnPassantMoves(Board &board, bool isWhite);
};
#endif
