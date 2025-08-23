#include "../include/types.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

bool test_utils_functions() {

  // Test square to bitboard conversion (corner cases)
  ASSERT_EQ(0x0000000000000001ULL, Utils::squareToBitboard(A1));
  ASSERT_EQ(0x0000000000000080ULL, Utils::squareToBitboard(H1));
  ASSERT_EQ(0x0100000000000000ULL, Utils::squareToBitboard(A8));
  ASSERT_EQ(0x8000000000000000ULL, Utils::squareToBitboard(H8));
  ASSERT_EQ(0x0000000000001000ULL,
            Utils::squareToBitboard(E2)); // Middle square

  // Test bitboard to square conversion
  ASSERT_EQ(A1, Utils::bitboardToSquare(0x0000000000000001ULL));
  ASSERT_EQ(H1, Utils::bitboardToSquare(0x0000000000000080ULL));
  ASSERT_EQ(A8, Utils::bitboardToSquare(0x0100000000000000ULL));
  ASSERT_EQ(H8, Utils::bitboardToSquare(0x8000000000000000ULL));
  ASSERT_EQ(E2, Utils::bitboardToSquare(0x0000000000001000ULL));

  // Test LSB operations
  ASSERT_EQ(0, Utils::getLSBIndex(0x0000000000000001ULL));  // A1
  ASSERT_EQ(7, Utils::getLSBIndex(0x0000000000000080ULL));  // H1
  ASSERT_EQ(63, Utils::getLSBIndex(0x8000000000000000ULL)); // H8

  // Test clearLSB
  ASSERT_EQ(0x0000000000000000ULL, Utils::clearLSB(0x0000000000000001ULL));
  ASSERT_EQ(0x0000000000000002ULL,
            Utils::clearLSB(0x0000000000000003ULL)); // Clear bit 0, leave bit 1
  ASSERT_EQ(0x0000000000000008ULL,
            Utils::clearLSB(0x000000000000000CULL)); // Clear bit 2, leave bit 3

  // Test bit manipulation
  ASSERT_EQ(0x0000000000000000ULL, Utils::clearBitAt(0x0000000000000001ULL, 0));
  ASSERT_EQ(0x0000000000000001ULL, Utils::clearBitAt(0x0000000000000081ULL, 7));

  ASSERT_EQ(0x0000000000000001ULL, Utils::setBitAt(0x0000000000000000ULL, 0));
  ASSERT_EQ(0x0000000000000081ULL, Utils::setBitAt(0x0000000000000001ULL, 7));

  // Test popcount (count set bits)
  ASSERT_EQ(0, Utils::popcount(0x0000000000000000ULL));
  ASSERT_EQ(1, Utils::popcount(0x0000000000000001ULL));
  ASSERT_EQ(2, Utils::popcount(0x0000000000000003ULL));  // bits 0,1
  ASSERT_EQ(8, Utils::popcount(0x00FF000000000000ULL));  // All rank 7 bits
  ASSERT_EQ(64, Utils::popcount(0xFFFFFFFFFFFFFFFFULL)); // All bits

  // Test isOneBit
  ASSERT_TRUE(Utils::isOneBit(0x0000000000000001ULL));
  ASSERT_TRUE(Utils::isOneBit(0x8000000000000000ULL));
  ASSERT_TRUE(!Utils::isOneBit(0x0000000000000003ULL)); // Two bits
  ASSERT_TRUE(!Utils::isOneBit(0x0000000000000000ULL)); // No bits

  // Test rank/file extraction
  ASSERT_EQ(0, Utils::getRank(A1)); // Rank 1 (0-indexed)
  ASSERT_EQ(0, Utils::getRank(H1));
  ASSERT_EQ(7, Utils::getRank(A8)); // Rank 8
  ASSERT_EQ(7, Utils::getRank(H8));
  ASSERT_EQ(3, Utils::getRank(E4)); // Middle ranks

  ASSERT_EQ(0, Utils::getFile(A1)); // A file (0-indexed)
  ASSERT_EQ(7, Utils::getFile(H1)); // H file
  ASSERT_EQ(0, Utils::getFile(A8));
  ASSERT_EQ(7, Utils::getFile(H8));
  ASSERT_EQ(4, Utils::getFile(E4)); // E file

  // Test makeSquare
  ASSERT_EQ(A1, Utils::makeSquare(0, 0)); // rank 0, file 0
  ASSERT_EQ(H1, Utils::makeSquare(0, 7)); // rank 0, file 7
  ASSERT_EQ(A8, Utils::makeSquare(7, 0)); // rank 7, file 0
  ASSERT_EQ(H8, Utils::makeSquare(7, 7)); // rank 7, file 7
  ASSERT_EQ(E4, Utils::makeSquare(3, 4)); // rank 3, file 4

  return true;
}

bool test_PieceAt_util() {
  Board board;

  // Test white pieces on rank 1
  ASSERT_EQ(WHITE_ROOK, Utils::getPieceTypeAt(board, A1));
  ASSERT_EQ(WHITE_KNIGHT, Utils::getPieceTypeAt(board, B1));
  ASSERT_EQ(WHITE_BISHOP, Utils::getPieceTypeAt(board, C1));
  ASSERT_EQ(WHITE_QUEEN, Utils::getPieceTypeAt(board, D1));
  ASSERT_EQ(WHITE_KING, Utils::getPieceTypeAt(board, E1));
  ASSERT_EQ(WHITE_BISHOP, Utils::getPieceTypeAt(board, F1));
  ASSERT_EQ(WHITE_KNIGHT, Utils::getPieceTypeAt(board, G1));
  ASSERT_EQ(WHITE_ROOK, Utils::getPieceTypeAt(board, H1));

  // Test white pawns on rank 2
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, A2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, B2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, C2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, D2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, E2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, F2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, G2));
  ASSERT_EQ(WHITE_PAWN, Utils::getPieceTypeAt(board, H2));

  // Test empty squares in middle ranks
  ASSERT_EQ(EMPTY, Utils::getPieceTypeAt(board, A3));
  ASSERT_EQ(EMPTY, Utils::getPieceTypeAt(board, E4));
  ASSERT_EQ(EMPTY, Utils::getPieceTypeAt(board, D5));
  ASSERT_EQ(EMPTY, Utils::getPieceTypeAt(board, H6));

  // Test black pawns on rank 7
  ASSERT_EQ(BLACK_PAWN, Utils::getPieceTypeAt(board, A7));
  ASSERT_EQ(BLACK_PAWN, Utils::getPieceTypeAt(board, E7));
  ASSERT_EQ(BLACK_PAWN, Utils::getPieceTypeAt(board, H7));

  // Test black pieces on rank 8
  ASSERT_EQ(BLACK_ROOK, Utils::getPieceTypeAt(board, A8));
  ASSERT_EQ(BLACK_KNIGHT, Utils::getPieceTypeAt(board, B8));
  ASSERT_EQ(BLACK_BISHOP, Utils::getPieceTypeAt(board, C8));
  ASSERT_EQ(BLACK_QUEEN, Utils::getPieceTypeAt(board, D8));
  ASSERT_EQ(BLACK_KING, Utils::getPieceTypeAt(board, E8));
  ASSERT_EQ(BLACK_BISHOP, Utils::getPieceTypeAt(board, F8));
  ASSERT_EQ(BLACK_KNIGHT, Utils::getPieceTypeAt(board, G8));
  ASSERT_EQ(BLACK_ROOK, Utils::getPieceTypeAt(board, H8));

  return true;
}

int main() {
  Board board;
  std::cout << "Running Chess Engine Tests..." << std::endl;

  RUN_TEST(test_utils_functions);
  RUN_TEST(test_PieceAt_util);

  std::cout << "Tests completed!" << std::endl;
  return 0;
}
