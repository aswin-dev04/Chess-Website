#include "../include/perft.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Known perft results for validation
struct PerftTestCase {
  std::string fen;
  std::string description;
  std::vector<u64> expectedResults; // Results for depths 1, 2, 3, etc.
};

bool runPerftValidation() {
  std::vector<PerftTestCase> testCases = {
      {
          "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
          "Starting position",
          {20, 400, 8902, 197281, 4865609, 119060324} // Depths 1-5
      },
      // {
      //     "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
      //     "en passant validation",
      //     {48, 2039, 97862, 4085603} // Depths 1-4
      // },
      // {
      //     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK1NR w - - 0 1",
      //     "castling check 1",
      //     {14, 191, 2812, 43238, 674624} // Depths 1-5
      // },
      // {
      //     "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      //     "castling check 2",
      //     {6, 264, 9467, 422333} // Depths 1-4
      // },
      // {
      //     "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
      //     "Complex middle game",
      //     {44, 1486, 62379, 2103487} // Depths 1-4
      // }
  };

  bool allPassed = true;

  for (const auto &testCase : testCases) {
    std::cout << "\nTesting: " << testCase.description << std::endl;
    std::cout << "FEN: " << testCase.fen << std::endl;

    Board board(testCase.fen);

    for (size_t i = 0; i < testCase.expectedResults.size(); i++) {
      int depth = i + 1;
      u64 result = Perft::perft(board, depth);
      u64 expected = testCase.expectedResults[i];

      std::cout << "Depth " << depth << ": " << result;

      if (result == expected) {
        std::cout << " ✓" << std::endl;
      } else {
        std::cout << " ✗ (expected " << expected << ")" << std::endl;
        allPassed = false;
      }
    }
  }

  return allPassed;
}

void runBasicPerftTests() {
  std::cout << "=== Basic Perft Tests ===" << std::endl;

  // Test starting position
  std::cout << "\n1. Starting Position Test:" << std::endl;
  Perft::runPerftTest(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4);

  // Test divide function
  std::cout << "\n2. Perft Divide Test (depth 4):" << std::endl;
  Board startBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  Perft::perftDivide(startBoard, 4);
}
//
// void runCustomPositionTests() {
//     std::cout << "\n=== Custom Position Tests ===" << std::endl;
//
//     // Test en passant
//     std::cout << "\n1. En Passant Position:" << std::endl;
//     Perft::runPerftTest("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w
//     KQkq d6 0 3", 3);
//
//     // Test castling
//     std::cout << "\n2. Castling Position:" << std::endl;
//     Perft::runPerftTest("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1", 4);
//
//     // Test promotion
//     std::cout << "\n3. Promotion Position:" << std::endl;
//     Perft::runPerftTest("8/P1P5/8/8/8/8/p1p5/8 w - - 0 1", 4);
// }

//
// void perftWithOutput(Board& board, int depth, std::ofstream& file,
// std::string moveSequence = "", bool filterFirst = false) {
//     if (depth == 0) {
//         file << moveSequence << std::endl;
//         return;
//     }
//
//     std::vector<Move> moves = MoveGeneration::generateAllMoves(board,
//     board.getWhiteToMove()); for (const Move& move : moves) {
//         std::stringstream ss;
//         ss << move;
//         std::string moveStr = ss.str();
//
//         // Only continue if this is a move we want to analyze
//         if (filterFirst && depth == 4) {
//             // Only analyze sequences starting with e2->e3, e2->e4, or d2->d4
//             if (moveStr.find("e2->e3") == std::string::npos &&
//                 moveStr.find("e2->e4") == std::string::npos &&
//                 moveStr.find("d2->d4") == std::string::npos) {
//                 continue;
//             }
//         }
//
//         board.makeMove(move);
//         std::string newSequence = moveSequence + (moveSequence.empty() ? "" :
//         " -> ") + moveStr; perftWithOutput(board, depth - 1, file,
//         newSequence, filterFirst); board.undoMove();
//     }
// }

int main() {
  std::cout << "Starting Perft Tests..." << std::endl;

  // Run basic tests
  runBasicPerftTests();

  // Run validation against known results
  std::cout << "\n=== Perft Validation ===" << std::endl;
  bool validationPassed = runPerftValidation();

  if (validationPassed) {
    std::cout << "\n✓ All perft tests passed!" << std::endl;
  } else {
    std::cout << "\n✗ Some perft tests failed. Check your move generation."
              << std::endl;
  }

  return validationPassed ? 0 : 1;
}
