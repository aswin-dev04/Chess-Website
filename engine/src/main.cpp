#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include <iostream>
#include <vector>

void printBoard(const Board &board) {
  std::cout << "\n  a b c d e f g h\n";
  std::cout << "  ---------------\n";

  for (int rank = 7; rank >= 0; rank--) {
    std::cout << (rank + 1) << "|";

    for (int file = 0; file < 8; file++) {
      Square square = static_cast<Square>(rank * 8 + file);
      u64 squareBit = Utils::squareToBitboard(square);

      char piece = '.';

      // Check white pieces
      if (board.getWhitePawns() & squareBit)
        piece = 'P';
      else if (board.getWhiteKnights() & squareBit)
        piece = 'N';
      else if (board.getWhiteBishops() & squareBit)
        piece = 'B';
      else if (board.getWhiteRooks() & squareBit)
        piece = 'R';
      else if (board.getWhiteQueens() & squareBit)
        piece = 'Q';
      else if (board.getWhiteKing() & squareBit)
        piece = 'K';

      // Check black pieces (lowercase)
      else if (board.getBlackPawns() & squareBit)
        piece = 'p';
      else if (board.getBlackKnights() & squareBit)
        piece = 'n';
      else if (board.getBlackBishops() & squareBit)
        piece = 'b';
      else if (board.getBlackRooks() & squareBit)
        piece = 'r';
      else if (board.getBlackQueens() & squareBit)
        piece = 'q';
      else if (board.getBlackKing() & squareBit)
        piece = 'k';

      std::cout << piece << " ";
    }
    std::cout << "|" << (rank + 1) << "\n";
  }

  std::cout << "  ---------------\n";
  std::cout << "  a b c d e f g h\n\n";
}

void printMoves(const std::vector<Move> &moves) {
  std::cout << "Available moves (" << moves.size() << "):\n";
  for (size_t i = 0; i < moves.size(); i++) {

    // Convert squares to chess notation (you might need to implement this)
    std::cout << moves[i];

    if (moves[i].getIsCapture()) {
      std::cout << " (capture)";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

int main() {
  Board board;

  std::cout << "=== Chess Engine Testing ===\n";
  std::cout << "Initial board:\n";
  printBoard(board);

  // Test king moves
  std::cout << "Testing white knight moves:\n";
  std::vector<Move> knightMoves =
      MoveGeneration::generateKnightMoves(board, true);
  printMoves(knightMoves);

  // Make a move
  if (!knightMoves.empty()) {

    for (int i = 0; i < knightMoves.size(); i++) {
      std::cout << "Making " << i + 1 << ". available move...\n";
      board.makeMove(knightMoves[i]);
      printBoard(board);

      // Test undo
      std::cout << "Undoing move...\n";
      board.undoMove();
      printBoard(board);
    }
  }

  return 0;
}
