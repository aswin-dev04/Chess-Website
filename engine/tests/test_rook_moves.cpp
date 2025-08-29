#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: White rook on empty board (center)
bool test_white_rook_center_empty() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000010000000ULL); // E4
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

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, true);

  // Rook on E4 should have 13 moves (3+2+4+4 in each direction)
  ASSERT_EQ(13, moves.size());

  return true;
}

// Test 2: Black rook blocked by own pieces
bool test_black_rook_blocked_own() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0020000000000000ULL); // F7 - blocks rook
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0010000000000000ULL); // E7
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8 - blocks rook
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, false);

  ASSERT_EQ(10, moves.size());

  return true;
}

// Test 3: White rook with capture opportunities
bool test_white_rook_captures() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000008000000ULL); // D4
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0000000080000000ULL);   // H4 - capturable
  board.setBlackKnights(0x0000080000000000ULL); // D6 - capturable
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, true);

  ASSERT_EQ(12, moves.size());

  int captureCount = Utils::getCaptureCount(board, WHITE_ROOK, true);
  ASSERT_EQ(2, captureCount);

  return true;
}

// Test 4: Rook on edge file (A-file)
bool test_rook_edge_file() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000000000001ULL); // A1
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0ULL); // E1
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, true);

  ASSERT_EQ(14, moves.size());

  return true;
}

// Test 5: Multiple rooks
bool test_multiple_rooks() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000000000081ULL); // A1 and H1
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

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, true);

  ASSERT_EQ(19, moves.size());

  return true;
}

// Test 6: Rook completely surrounded (minimal moves)
bool test_rook_surrounded() {
  Board board;
  board.setWhiteKnights(0x0000000028000000ULL); // D4 and F4 - blocking rook
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0x0000000010000000ULL); // E4
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000010ULL);  // E1
  board.setWhitePawns(0x0000001000100000ULL); // E5 and E3 - blocking rook
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateRookMoves(board, true);

  // Rook completely blocked by own pieces and enemy pieces
  ASSERT_EQ(0, moves.size());

  return true;
}

int main() {
  std::cout << "Running Rook Move Generation Tests..." << std::endl;
  RUN_TEST(test_white_rook_center_empty);
  RUN_TEST(test_black_rook_blocked_own);
  RUN_TEST(test_white_rook_captures);
  RUN_TEST(test_rook_edge_file);
  RUN_TEST(test_multiple_rooks);
  RUN_TEST(test_rook_surrounded);
  std::cout << "Rook tests completed!" << std::endl;
  return 0;
}
