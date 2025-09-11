#include "../include/perft.hpp"

u64 Perft::perft(Board &board, int depth) {
  return perftRecursive(board, depth, board.getWhiteToMove());
}

u64 Perft::perftRecursive(Board &board, int depth, bool isWhite) {
  if (depth == 0) {
    return 1;
  }

  std::vector<Move> moves = MoveGeneration::generateAllMoves(board, isWhite);

  if (depth == 1) {
    return moves.size();
  }

  u64 nodes = 0;
  for (const Move &move : moves) {
    board.makeMove(move);
    nodes += perftRecursive(board, depth - 1, !isWhite);
    board.undoMove();
  }

  return nodes;
}

u64 Perft::perftDivide(Board &board, int depth) {
  if (depth <= 0)
    return 0;

  bool isWhite = board.getWhiteToMove();
  std::vector<Move> moves = MoveGeneration::generateAllMoves(board, isWhite);

  u64 totalNodes = 0;

  std::cout << "\nPerft Divide (depth " << depth << "):\n";
  std::cout << "Move\t\tNodes\n";
  std::cout << "--------------------\n";

  for (const Move &move : moves) {
    board.makeMove(move);

    u64 nodes;
    if (depth == 1) {
      nodes = 1;
    } else {
      nodes = perftRecursive(board, depth - 1, !isWhite);
    }

    totalNodes += nodes;

    std::cout << move << "\t\t" << nodes << "\n";

    board.undoMove();
  }

  std::cout << "--------------------\n";
  std::cout << "Total: " << totalNodes << " nodes\n";

  return totalNodes;
}

void Perft::runPerftTest(const std::string &fen, int maxDepth) {
  Board board(fen);

  std::cout << "Running Perft test on position: " << fen << "\n";
  std::cout << "Depth\tNodes\t\tTime(ms)\n";
  std::cout << "--------------------------------\n";

  for (int depth = 1; depth <= maxDepth; depth++) {
    auto start = std::chrono::high_resolution_clock::now();
    u64 nodes = perft(board, depth);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << depth << "\t" << nodes << "\t\t" << duration.count() << "\n";
  }
}
