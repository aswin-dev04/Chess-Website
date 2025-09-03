#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: White bishop on empty board (center)
bool test_white_bishop_center_empty() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0x0000000010000000ULL); // E4
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, true);

  ASSERT_EQ(13, moves.size());

  return true;
}

// Test 2: Black bishop blocked by own pieces
bool test_black_bishop_blocked_own() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0000200000000000ULL);   // F6 - blocks SE diagonal
  board.setBlackKnights(0x0000080000000000ULL); // D6 - blocks NW diagonal
  board.setBlackBishops(0x0010000000000000ULL); // E7
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8 - blocks North
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, false);

  ASSERT_EQ(2, moves.size());

  return true;
}

// Test 3: White bishop with capture opportunities
bool test_white_bishop_captures() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0x0000000008000000ULL); // D4
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0000200000000000ULL); // F6 - capturable on NE diagonal
  board.setBlackKnights(
      0x0000000000002000ULL); // F2 - capturable on SE diagonal
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, true);

  ASSERT_EQ(10, moves.size());

  int captureCount = Utils::getCaptureCount(board, WHITE_BISHOP, true);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 4: Bishop in corner (limited moves)
bool test_bishop_corner() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0x0000000000000001ULL); // A1
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, true);

  // Bishop on A1 can only move NE: B2,C3,D4,E5,F6,G7,H8 = 7 moves
  ASSERT_EQ(7, moves.size());

  return true;
}

// Test 5: Multiple bishops
bool test_multiple_bishops() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0x0000000000000024ULL); // C1 and F1
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, true);

  ASSERT_EQ(14, moves.size());

  return true;
}

// Test 6: Bishop completely blocked
bool test_bishop_completely_blocked() {
  Board board;
  board.setWhiteKnights(0x0000002800000000ULL); // D5 and F5
  board.setWhiteBishops(0x0000000010000000ULL); // E4
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0000000000280000ULL); // D3 and F3
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateBishopMoves(board, true);

  // Bishop on E4 blocked by own pieces on all 4 diagonals
  ASSERT_EQ(0, moves.size());

  return true;
}

bool test_bishop_pinned_horizontally() {
  Board board;
  board.setWhiteKing(0x0000000010000000ULL);    // E4
  board.setWhiteBishops(0x0000000020000000ULL); // F4 - pinned bishop
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000000080000000ULL); // H4 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateBishopLegalMoves(board, true);
  ASSERT_EQ(0, moves.size());
  return true;
}

bool test_bishop_pinned_vertically() {
  Board board;
  board.setWhiteKing(0x0000000000001000ULL);    // E2
  board.setWhiteBishops(0x0000000010000000ULL); // E4 - pinned bishop
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000100000000000ULL); // E7 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateBishopLegalMoves(board, true);
  // Vertically pinned bishop should have 0 legal moves
  ASSERT_EQ(0, moves.size());
  return true;
}

bool test_bishop_pinned_diagonally_by_bishop() {
  Board board;

  board.setWhiteKing(0x0000000000000001ULL);    // A1
  board.setWhiteBishops(0x0000000000040000ULL); // C3 - pinned bishop
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0x0000200000000000ULL); // F6 - pinning bishop
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateBishopLegalMoves(board, true);

  ASSERT_EQ(4, moves.size());

  return true;
}

int main() {
  std::cout << "Running Bishop Move Generation Tests..." << std::endl;
  RUN_TEST(test_white_bishop_center_empty);
  RUN_TEST(test_black_bishop_blocked_own);
  RUN_TEST(test_white_bishop_captures);
  RUN_TEST(test_bishop_corner);
  RUN_TEST(test_multiple_bishops);
  RUN_TEST(test_bishop_completely_blocked);

  RUN_TEST(test_bishop_pinned_horizontally);
  RUN_TEST(test_bishop_pinned_vertically);
  RUN_TEST(test_bishop_pinned_diagonally_by_bishop);
  std::cout << "Bishop tests completed!" << std::endl;
  return 0;
}
