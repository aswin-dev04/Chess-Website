#include "../include/board.hpp"
#include "test.hpp"

bool test_board_initialization() {
  Board board;

  // Test white pieces
  ASSERT_EQ(0x000000000000FF00ULL, board.getWhitePawns());   // Rank 2
  ASSERT_EQ(0x0000000000000042ULL, board.getWhiteKnights()); // B1, G1
  ASSERT_EQ(0x0000000000000024ULL, board.getWhiteBishops()); // C1, F1
  ASSERT_EQ(0x0000000000000081ULL, board.getWhiteRooks());   // A1, H1
  ASSERT_EQ(0x0000000000000010ULL, board.getWhiteQueens());  // D1
  ASSERT_EQ(0x0000000000000008ULL, board.getWhiteKing());    // E1

  // Test black pieces
  ASSERT_EQ(0x00FF000000000000ULL, board.getBlackPawns());   // Rank 7
  ASSERT_EQ(0x4200000000000000ULL, board.getBlackKnights()); // B8, G8
  ASSERT_EQ(0x2400000000000000ULL, board.getBlackBishops()); // C8, F8
  ASSERT_EQ(0x8100000000000000ULL, board.getBlackRooks());   // A8, H8
  ASSERT_EQ(0x1000000000000000ULL, board.getBlackQueens());  // D8
  ASSERT_EQ(0x0800000000000000ULL, board.getBlackKing());    // E8

  return true;
}

int main() {
  std::cout << "Running Chess Engine Tests..." << std::endl;

  RUN_TEST(test_board_initialization);

  std::cout << "Tests completed!" << std::endl;
  return 0;
}
