#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: Knight in center
bool test_knight_moves_basic() {
  Board board;
  // Place knight in center of board (E4)
  board.setWhiteKnights(0x0000000010000000ULL); // E4
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Knight on E4 should have 8 possible moves (all L-shaped moves)
  ASSERT_EQ(8, moves.size());

  // Should have 0 captures since no enemy pieces
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 2: Knight in corner
bool test_knight_corner_moves() {
  Board board;
  // Clear all pieces and place white knight in corner A1
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0x0000000000000001ULL); // A1
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1 - away from knight
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8 - far away
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Knight in corner A1 should only have 2 moves: B3, C2
  ASSERT_EQ(2, moves.size());

  // Should have 0 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 3: Knight blocked by own pieces
bool test_knight_blocked_by_own_pieces() {
  Board board;
  // Place knight in center, block some squares with own pieces
  board.setWhiteKnights(0x0000000010000000ULL); // E4
  board.setWhitePawns(0x0000000000000800ULL);   // D2 - blocks one knight move
  board.setWhiteBishops(
      0x0000200000000000ULL);                 // F6 - blocks another knight move
  board.setWhiteRooks(0x0000000000040000ULL); // C3 - blocks third knight move
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Should have 5 moves instead of 8 (3 blocked by own pieces)
  ASSERT_EQ(5, moves.size());

  // Should have 0 captures since no enemy pieces
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 4: Knight can capture enemy pieces
bool test_knight_capture_moves() {
  Board board;
  // Place knight in center with enemy pieces on some L-shaped squares
  board.setWhiteKnights(0x0000000010000000ULL); // E4
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);    // E1
  board.setBlackPawns(0x0000000000000800ULL);   // D2 - capturable
  board.setBlackKnights(0x0000200000000000ULL); // F6 - capturable
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000000000400000ULL); // G3 - capturable
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Should have all 8 moves (5 to empty squares + 3 captures)
  ASSERT_EQ(8, moves.size());

  // Should have 3 captures using your getCaptureCount function
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(3, captureCount);

  return true;
}

// Test 5: Knight on edge of board
bool test_knight_edge_moves() {
  Board board;
  // Place knight on edge (A4)
  board.setWhiteKnights(0x0000000001000000ULL); // A4
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Knight on A4 should have 4 moves (B2, C3, C5, B6)
  ASSERT_EQ(4, moves.size());

  // Should have 0 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 6: Black knight moves
bool test_black_knight_moves() {
  Board board;
  // Test black knight in center
  board.setWhiteKing(0x0000000000000010ULL); // E1 - away from black knight
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0x0000001000000000ULL); // E5
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves =
      MoveGeneration::generateKnightMoves(board, false); // false for black

  // Black knight in center should have 8 moves
  ASSERT_EQ(8, moves.size());

  // Should have 0 captures
  int captureCount = Utils::getCaptureCount(board, BLACK_KNIGHT, false);
  ASSERT_EQ(0, captureCount);

  return true;
}

// Test 7: Multiple knights
bool test_multiple_knights() {
  Board board;
  // Place two white knights (B1 and G1)
  board.setWhiteKnights(0x0000000000000042ULL); // B1 + G1
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // B1 knight has 3 moves, G1 knight has 3 moves = 6 total
  ASSERT_EQ(6, moves.size());

  // Should have 0 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  // Verify both knights generate moves
  bool foundB1Move = false;
  bool foundG1Move = false;
  for (const Move &move : moves) {
    if (move.getFromSquare() == 1)
      foundB1Move = true; // B1 = square 1
    if (move.getFromSquare() == 6)
      foundG1Move = true; // G1 = square 6
  }
  ASSERT_TRUE(foundB1Move);
  ASSERT_TRUE(foundG1Move);
  return true;
}

// Test 8: Multiple knights with captures
bool test_multiple_knights_with_captures() {
  Board board;
  // Place two white knights (B1 and G1) with enemy pieces to capture
  board.setWhiteKnights(0x0000000000000042ULL); // B1 + G1
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0ULL);                   // E1
  board.setBlackPawns(0x0000000000020000ULL); // B3 - capturable by B1 knight
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0x0000000000080000ULL); // D3 - capturable by B1 knight
  board.setBlackRooks(0x0000000000200000ULL);   // F3 - capturable by G1 knight
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  ASSERT_EQ(6, moves.size());

  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(1, captureCount);

  return true;
}

// Test 9: Mixed scenario - own pieces, enemy pieces, captures
bool test_knight_mixed_scenario() {
  Board board;
  // Knight on D5 with mixed surrounding pieces
  board.setWhiteKnights(0x0000000800000000ULL); // D5
  board.setWhitePawns(0x0000000000040000ULL);   // C3 - blocks knight move
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setBlackPawns(0x0000000000020000ULL); // B3
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0x0000200000000000ULL); // F6 - capturable
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Should have 7 moves (8 total - 1 blocked by own pawn at C3)
  ASSERT_EQ(7, moves.size());

  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(1, captureCount);

  return true;
}

// Test 10: Knight near corner (H8 corner)
bool test_knight_near_corner() {
  Board board;
  // Clear all pieces first
  board.setWhitePawns(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // H8

  // Place knight at H8 (corner)
  board.setWhiteKnights(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKnightMoves(board, true);

  // Knight at H8 should have only 2 moves (F7, G6)
  ASSERT_EQ(2, moves.size());

  // Should have 0 captures
  int captureCount = Utils::getCaptureCount(board, WHITE_KNIGHT, true);
  ASSERT_EQ(0, captureCount);

  return true;
}

int main() {
  std::cout << "Running Knight Move Generation Tests..." << std::endl;

  RUN_TEST(test_knight_moves_basic);
  RUN_TEST(test_knight_corner_moves);
  RUN_TEST(test_knight_blocked_by_own_pieces);
  RUN_TEST(test_knight_capture_moves);
  RUN_TEST(test_knight_edge_moves);
  RUN_TEST(test_black_knight_moves);
  RUN_TEST(test_multiple_knights);
  RUN_TEST(test_multiple_knights_with_captures);
  RUN_TEST(test_knight_mixed_scenario);
  RUN_TEST(test_knight_near_corner);

  std::cout << "Knight tests completed!" << std::endl;
  return 0;
}
