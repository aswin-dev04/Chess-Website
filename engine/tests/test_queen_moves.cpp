#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: White queen on empty board (center)
bool test_white_queen_center_empty() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0x0000000010000000ULL); // E4
  board.setWhiteKing(0ULL);
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, true);
  ASSERT_EQ(27, moves.size());

  return true;
}

// Test 2: Black queen blocked by own pieces
bool test_black_queen_blocked_own() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0ULL);
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0000001000000000ULL);   // E5 - blocks south
  board.setBlackKnights(0x0000000010000000ULL); // E4
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000000800000000ULL); // D5 - blocks west
  board.setBlackQueens(0x0010000000000000ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8 - blocks north
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, false);

  ASSERT_EQ(17, moves.size()); // only a few diagonals/side squares remain

  return true;
}

// Test 3: White queen with capture opportunities
bool test_white_queen_captures() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0x0000000010000000ULL); // E4
  board.setWhiteKing(0ULL);
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0x0000001000000000ULL);   // E5 - capturable north
  board.setBlackKnights(0x0000000000002000ULL); // F2
  board.setBlackBishops(0x0000200000000000ULL); // F6
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL); // E8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, true);

  ASSERT_EQ(24, moves.size());

  int captureCount = Utils::getCaptureCount(board, WHITE_QUEEN, true);
  ASSERT_EQ(1, captureCount);

  return true;
}

// Test 4: Queen in corner (limited moves)
bool test_queen_corner() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0x0000000000000001ULL); // A1
  board.setWhiteKing(0ULL);
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, true);
  ASSERT_EQ(21, moves.size());

  return true;
}

// Test 5: Multiple queens
bool test_multiple_queens() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0x0000000000000024ULL);
  board.setWhiteKing(0x0000000000000010ULL);
  board.setWhitePawns(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x1000000000000000ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, true);

  ASSERT_EQ(33, moves.size());

  return true;
}

// Test 6: Queen completely blocked
bool test_queen_completely_blocked() {
  Board board;
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0x0000000010000000ULL); // E4
  board.setWhiteKing(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0ULL);
  board.setWhitePawns(0x3828380000ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateQueenMoves(board, true);

  ASSERT_EQ(0, moves.size()); // blocked in all directions

  return true;
}

bool test_queen_pinned_horizontally() {
  Board board;
  board.setWhiteKing(0x0000000010000000ULL);   // E4
  board.setWhiteQueens(0x0000000020000000ULL); // F4 - pinned queen
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0000000080000000ULL); // H4 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateQueenLegalMoves(board, true);
  ASSERT_EQ(2, moves.size());
  return true;
}

bool test_queen_pinned_vertically() {
  Board board;
  board.setWhiteKing(0x0000000000001000ULL);   // E2
  board.setWhiteQueens(0x0000000010000000ULL); // E4 - pinned queen
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0x0010000000000000ULL); // E7 - pinning rook
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateQueenLegalMoves(board, true);
  ASSERT_EQ(4, moves.size());
  return true;
}

bool test_queen_pinned_diagonally() {
  Board board;
  board.setWhiteKing(0x0000000000000001ULL);   // A1
  board.setWhiteQueens(0x0000000000040000ULL); // C3 - pinned queen
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0x0000200000000000ULL); // F6 - pinning bishop
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();
  std::vector<Move> moves =
      MoveGeneration::generateQueenLegalMoves(board, true);
  ASSERT_EQ(4, moves.size());
  return true;
}

int main() {
  std::cout << "Running Queen Move Generation Tests..." << std::endl;
  RUN_TEST(test_white_queen_center_empty);
  RUN_TEST(test_black_queen_blocked_own);
  RUN_TEST(test_white_queen_captures);
  RUN_TEST(test_queen_corner);
  RUN_TEST(test_multiple_queens);
  RUN_TEST(test_queen_completely_blocked);

  RUN_TEST(test_queen_pinned_horizontally);
  RUN_TEST(test_queen_pinned_vertically);
  RUN_TEST(test_queen_pinned_diagonally);

  std::cout << "Queen tests completed!" << std::endl;
  return 0;
}
