#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: White pawns starting position - double moves
bool test_white_pawns_starting_position() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x000000000000FF00ULL); // All pawns on rank 2
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // 8 pawns * 2 moves each (one forward, two forward) = 16 moves
  ASSERT_EQ(16, moves.size());

  // No captures possible
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 2: Black pawns starting position - double moves
bool test_black_pawns_starting_position() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x00FF000000000000ULL); // All pawns on rank 7
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, false);

  // 8 pawns * 2 moves each = 16 moves
  ASSERT_EQ(16, moves.size());

  // No captures possible
  int captureCount = Utils::getCaptureCount(board, BLACK_PAWN, false);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 3: Single white pawn blocked by own piece
bool test_white_pawn_blocked_by_own_piece() {
  Board board;
  board.setWhiteKnights(0x0000000000002000ULL); // F3 - blocks pawn
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0000000000000020ULL); // F2 pawn
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // Pawn blocked, no moves possible
  ASSERT_EQ(0, moves.size());

  return true;
}

// Test 4: White pawn with diagonal capture opportunities
bool test_white_pawn_captures() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);    // E1
  board.setWhitePawns(0x0000000000001000ULL);   // E2 pawn
  board.setBlackPawns(0x0000000000200000ULL);   // F3 - capturable
  board.setBlackKnights(0x0000000000080000ULL); // D3 - capturable
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // 2 forward moves + 2 captures = 4 moves total
  ASSERT_EQ(4, moves.size());

  // Should have 2 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 5: Black pawn with diagonal capture opportunities
bool test_black_pawn_captures() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0000080000000000ULL); // D6 - capturable
  board.setWhiteRooks(0x0000200000000000ULL); // F6 - capturable
  board.setBlackPawns(0x0010000000000000ULL); // E7 pawn
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, false);
  // 2 forward moves + 2 captures = 4 moves total
  ASSERT_EQ(4, moves.size());

  // Should have 2 captures
  int captureCount = Utils::getCaptureCount(board, BLACK_PAWN, false);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 6: Pawn blocked by enemy piece (can't move forward but can capture)
bool test_pawn_blocked_by_enemy() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);    // E1
  board.setWhitePawns(0x0000000000001000ULL);   // E2 pawn
  board.setBlackPawns(0x0000000000100000ULL);   // E3 - blocks forward movement
  board.setBlackKnights(0x0000000000080000ULL); // D3 - capturable
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // Only 1 capture move possible (D3)
  ASSERT_EQ(1, moves.size());

  // Should have 1 capture
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(1, captureCount);

  return true;
}

// Test 7: Pawn on edge files (A and H files)
bool test_pawn_edge_files() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);    // E1
  board.setWhitePawns(0x0000000000008100ULL);   // A2 and H2 pawns
  board.setBlackPawns(0x0000000000020000ULL);   // B3 - capturable by A2 pawn
  board.setBlackKnights(0x0000000000400000ULL); // G3 - capturable by H2 pawn
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // A2: 2 forward + 1 capture (B3) = 3 moves
  // H2: 2 forward + 1 capture (G3) = 3 moves
  // Total = 6 moves
  ASSERT_EQ(6, moves.size());

  // Should have 2 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 8: Multiple pawns with mixed scenarios
bool test_multiple_pawns_mixed() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);    // E1
  board.setWhitePawns(0x0000000000005400ULL);   // C2, E2, G2 pawns
  board.setBlackPawns(0x0000000000200000ULL);   // F3 - capturable by E2 and G2
  board.setBlackKnights(0x0000000000080000ULL); // D3 - capturable by C2 and E2
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // C2: 2 forward + 1 capture (D3) = 3 moves
  // E2: 2 forward + 2 captures (D3, F3) = 4 moves
  // G2: 2 forward + 1 capture (F3) = 3 moves
  // Total = 10 moves
  ASSERT_EQ(10, moves.size());

  // Total captures available = 4 (but only 2 enemy pieces)
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 9: Pawns advanced to middle of board (no double moves)
bool test_advanced_pawns() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(
      0x0000001800000000ULL); // D5, E5 pawns (not on starting rank)
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnMoves(board, true);

  // Each pawn can only move 1 square forward (no double moves from middle)
  // D5->D6, E5->E6 = 2 moves total
  ASSERT_EQ(2, moves.size());

  // No captures
  int captureCount = Utils::getCaptureCount(board, WHITE_PAWN, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

bool test_pawn_pinned_horizontally() {
  Board board;
  board.setWhiteKing(0x0000000010000000ULL);  // E4
  board.setWhitePawns(0x0000000020000000ULL); // F4
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000000080000000ULL); // H4 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnLegalMoves(board, true);

  ASSERT_EQ(0, moves.size());
  return true;
}

bool test_pawn_pinned_vertically() {
  Board board;
  board.setWhiteKing(0x0000000000001000ULL);  // E2
  board.setWhitePawns(0x0000000010000000ULL); // E4
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000100000000000ULL); // E7 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnLegalMoves(board, true);

  ASSERT_EQ(1, moves.size());
  return true;
}

bool test_pawn_pinned_diagonally() {
  Board board;
  board.setWhiteKing(0x0000000000000400ULL);  // C2
  board.setWhitePawns(0x0000000010000000ULL); // E4 - pinned pawn
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0x0000400000000000ULL); // G6 - pinning bishop
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnLegalMoves(board, true);

  ASSERT_EQ(0, moves.size());
  return true;
}

bool test_white_pawn_promotion_queen() {
  Board board;
  // White pawn on G7, ready to promote
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0040000000000000ULL); // G7 - promotion pawn
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnLegalMoves(board, true);

  // Should find 4 promotion moves (Queen, Rook, Bishop, Knight)
  ASSERT_EQ(4, moves.size());

  // Find the queen promotion move
  Move queenPromotion;
  bool foundQueenPromo = false;
  for (const Move &move : moves) {
    if (move.getIsPromotion() && move.getPromotionPiece() == WHITE_QUEEN) {
      queenPromotion = move;
      foundQueenPromo = true;
      break;
    }
  }
  ASSERT_TRUE(foundQueenPromo);

  // Make the queen promotion move
  board.makeMove(queenPromotion);

  // Verify pawn is gone from G7
  ASSERT_EQ(0ULL, board.getWhitePawns());

  // Verify queen is on G8
  ASSERT_EQ(0x4000000000000000ULL, board.getWhiteQueens()); // G8

  return true;
}

bool test_black_pawn_promotion_capture() {
  Board board;
  // Black pawn on F2, White rook on G1 (capture promotion target)
  board.setWhiteKing(0x0000000000000100ULL); // E1
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000000000040ULL); // G1 - capture target
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0x0000000000002000ULL); // F2 - promotion pawn
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves =
      MoveGeneration::generatePawnLegalMoves(board, false);

  // Should find promotion moves (including capture promotions)
  ASSERT_EQ(8, moves.size());

  // Find a capture promotion move (promote to rook while capturing)
  Move capturePromotion;
  bool foundCapturePromo = false;
  for (const Move &move : moves) {
    if (move.getIsPromotion() && move.getIsCapture() &&
        move.getPromotionPiece() == BLACK_ROOK) {
      capturePromotion = move;
      foundCapturePromo = true;
      break;
    }
  }
  ASSERT_TRUE(foundCapturePromo);

  // Make the capture promotion move
  board.makeMove(capturePromotion);

  // Verify pawn is gone
  ASSERT_EQ(0ULL, board.getBlackPawns());

  // Verify white rook is captured (gone)
  ASSERT_EQ(0ULL, board.getWhiteRooks());

  // Verify black rook is on G1
  ASSERT_EQ(0x0000000000000040ULL, board.getBlackRooks()); // G1

  return true;
}

bool test_white_pawn_underpromotion_knight() {
  Board board;
  // White pawn on A7, ready to promote
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0001000000000000ULL); // A7 - promotion pawn
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generatePawnLegalMoves(board, true);

  // Find the knight promotion move
  Move knightPromotion;
  bool foundKnightPromo = false;
  for (const Move &move : moves) {
    if (move.getIsPromotion() && move.getPromotionPiece() == WHITE_KNIGHT) {
      knightPromotion = move;
      foundKnightPromo = true;
      break;
    }
  }
  ASSERT_TRUE(foundKnightPromo);

  // Make the knight promotion move
  board.makeMove(knightPromotion);

  // Verify pawn is gone from A7
  ASSERT_EQ(0ULL, board.getWhitePawns());

  // Verify knight is on A8
  ASSERT_EQ(0x0100000000000000ULL, board.getWhiteKnights()); // A8

  return true;
}

int main() {
  std::cout << "Running Pawn Move Generation Tests..." << std::endl;

  RUN_TEST(test_white_pawns_starting_position);
  RUN_TEST(test_black_pawns_starting_position);
  RUN_TEST(test_white_pawn_blocked_by_own_piece);
  RUN_TEST(test_white_pawn_captures);
  RUN_TEST(test_black_pawn_captures);
  RUN_TEST(test_pawn_blocked_by_enemy);
  RUN_TEST(test_pawn_edge_files);
  RUN_TEST(test_multiple_pawns_mixed);
  RUN_TEST(test_advanced_pawns);

  RUN_TEST(test_pawn_pinned_horizontally);
  RUN_TEST(test_pawn_pinned_vertically);
  RUN_TEST(test_pawn_pinned_diagonally);

  RUN_TEST(test_white_pawn_promotion_queen);
  RUN_TEST(test_black_pawn_promotion_capture);
  RUN_TEST(test_white_pawn_underpromotion_knight);

  std::cout << "Pawn tests completed!" << std::endl;
  return 0;
}
