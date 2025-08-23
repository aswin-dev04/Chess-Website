#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "test.hpp"

bool test_king_moves_basic() {
  Board board;
  // Place king in center of board (E4)
  board.setWhiteKing(0x0000000010000000ULL); // E4
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // King on E4 should have 8 possible moves (if no pieces blocking)
  ASSERT_EQ(8, moves.size());

  return true;
}

int main() {
  std::cout << "Running Chess Engine Tests..." << std::endl;

  RUN_TEST(test_king_moves_basic);

  std::cout << "Tests completed!" << std::endl;
  return 0;
}
