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

  u64 wpawns = 0ULL;
  u64 wknights = 0x80000ULL;      // D3
  u64 wbishops = 0x8000000000ULL; // H5
  u64 wrooks = 0ULL;
  u64 wqueens = 0ULL;
  u64 wking = 0x10000000ULL; // E4

  u64 bpawns = 0ULL;
  u64 bknights = 0ULL;
  u64 bbishops = 0ULL;
  u64 brooks = 0ULL;
  u64 bqueens = 0x1000000000000000ULL; // E8
  u64 bking = 0ULL;

  Board board = Board(wpawns, bpawns, wknights, bknights, wbishops, bbishops,
                      wrooks, brooks, wqueens, bqueens, wking, bking);

  std::cout << "=== Chess Engine Testing ===\n";
  std::cout << "Initial board:\n";
  printBoard(board);

  std::cout << "Is the White King checked? "
            << (board.isKingChecked(true) ? "Yes" : "No") << "\n";

  std::vector<Move> legalMoves = MoveGeneration::generateAllMoves(board, true);

  printMoves(legalMoves);

  board.makeMove(legalMoves[0]);
  printBoard(board);

  std::cout << "Is the White King checked? "
            << (board.isKingChecked(true) ? "Yes" : "No") << "\n";

  return 0;
}
