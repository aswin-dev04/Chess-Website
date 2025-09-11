#include "../include/board.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include "test.hpp"

// Test 1: King cannot move into pawn attacks
bool test_king_legal_pawn_attacks() {
  Board board;
  // Clear all pieces first
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL); // E4
  // Black pawns on D6 and F6 (attack C5, E5 and G5)
  board.setBlackPawns(0x0000280000000000ULL); // D6, F6
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  // King should have 7 moves (8 total - 1 square attacked by both pawns: E5)
  ASSERT_EQ(7, moves.size());
  return true;
}

// Test 2: King cannot move into knight attacks
bool test_king_legal_knight_attacks() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL); // E4
  // Black knight on G6 (attacks F4, H4, F8, H8, E5, E7 - only E5 and F4 affect
  // king)
  board.setBlackKnights(0x0000400000000000ULL); // G6
  board.setBlackKing(0x8000000000000000ULL);    // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  // King should have 6 moves (8 total - 2 squares attacked by knight: E5, F4)
  ASSERT_EQ(6, moves.size());
  return true;
}

// Test 3: King cannot move into bishop attacks
bool test_king_legal_bishop_attacks() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL);    // E4
  board.setBlackBishops(0x0000000000000001ULL); // A1
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  ASSERT_EQ(6, moves.size());
  return true;
}

// Test 4: King cannot move into rook attacks
bool test_king_legal_rook_attacks() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL);  // E4
  board.setBlackRooks(0x0000000000000008ULL); // D1
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  ASSERT_EQ(5, moves.size());
  return true;
}

// Test 5: King can capture attacking piece (if safe)
bool test_king_legal_capture_undefended() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL); // E4
  // Black knight on F5 (adjacent to king, can be captured)
  board.setBlackKnights(0x0000002000000000ULL); // F5
  board.setBlackKing(0x8000000000000000ULL);    // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  // King should have 6 moves (8 total - 2 squares attacked by knight: E5, F4)
  ASSERT_EQ(6, moves.size());

  // Should include capture of knight on F5
  bool foundCapture = false;
  for (const Move &move : moves) {
    if (move.getToSquare() == Utils::bitboardToSquare(0x0000002000000000ULL)) {
      foundCapture = true;
      ASSERT_TRUE(move.getCapturedPiece() != EMPTY);
    }
  }
  ASSERT_TRUE(foundCapture);
  return true;
}
bool test_king_legal_defended_piece() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL); // E4
  // Black knight on F5 defended by black pawn on G6
  board.setBlackKnights(0x0000002000000000ULL); // F5
  board.setBlackPawns(0x0000400000000000ULL);   // G6 (defends F5)
  board.setBlackKing(0x8000000000000000ULL);    // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  ASSERT_EQ(5, moves.size());

  // Verify no move to F5 exists
  for (const Move &move : moves) {
    ASSERT_NE(move.getToSquare(),
              Utils::bitboardToSquare(0x0000002000000000ULL));
  }
  return true;
}
// Test 6: King blocked by own pieces but can't move to attacked squares
bool test_king_legal_own_pieces_and_attacks() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king on E4
  board.setWhiteKing(0x0000000010000000ULL); // E4
  // Own pawn blocking one square
  board.setWhitePawns(0x0000000008000000ULL); // D4
  // Enemy queen attacking other squares
  board.setBlackQueens(0x0000000020000000ULL); // F4 (attacks many squares)
  board.setBlackKing(0ULL);
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  ASSERT_EQ(3, moves.size());
  return true;
}

// Test 7: Black king legal moves
bool test_black_king_legal_moves() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // Black king on E5
  board.setBlackKing(0x0000001000000000ULL); // E5
  // White pawns attacking some squares around black king
  board.setWhitePawns(0x0000000028000000ULL); // D4, F4 (attack D5, E5, F5, G5)
  board.setWhiteKing(0x0000000000000001ULL);  // A1
  board.setALLPiecesAggregate();

  std::vector<Move> moves =
      MoveGeneration::generateKingLegalMoves(board, false); // false for black

  ASSERT_EQ(8, moves.size());
  return true;
}

// Test 8: King with only one legal move
bool test_king_one_legal_move() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king in corner A1
  board.setWhiteKing(0x0000000000000001ULL); // A1
  // Black pieces attacking most escape squares
  board.setBlackRooks(0x0100000000000000ULL);
  board.setBlackBishops(
      0x0000000000000004ULL); // C1 (attacks diagonal, blocking B2)
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  // King should have only 1 legal move (B1)
  ASSERT_EQ(1, moves.size());

  // Verify the move is to B1
  ASSERT_EQ(moves[0].getToSquare(),
            Utils::bitboardToSquare(0x0000000000000002ULL));
  return true;
}

// Test 9: King with no legal moves
bool test_king_no_legal_moves() {
  Board board;
  // Clear all pieces
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // White king in corner A1
  board.setWhiteKing(0x0000000000000001ULL); // A1
  // Black knight on C3 (attacks A2, B1)
  board.setBlackKnights(0x0000000000040000ULL); // C3
  // Black queen on B3 (attacks A2, B2, and other squares)
  board.setBlackQueens(0x0000000000020000ULL); // B3
  board.setBlackKing(0x8000000000000000ULL);   // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingLegalMoves(board, true);

  // King should have 0 legal moves
  ASSERT_EQ(0, moves.size());
  return true;
}

// Test 10: King in center
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

// Test 11: King in corner
bool test_king_corner_moves() {
  Board board;
  // Clear all pieces and place white king in corner A1
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setWhiteKing(0x0000000000000001ULL); // A1
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8 - far away
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // King in corner should only have 3 moves: A2, B1, B2
  ASSERT_EQ(3, moves.size());
  return true;
}

// Test 12: King blocked by own pieces
bool test_king_blocked_by_own_pieces() {
  Board board;
  // Place king in center, surround with own pieces
  board.setWhiteKing(0x0000000010000000ULL); // E4
  board.setWhitePawns(
      0x000000003C000000ULL); // D4, E3, F4, G4 (some squares blocked)
  board.setWhiteKnights(0x0000003C00000000ULL); // D5, E5, F5, G5 (more blocked)
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // Should have fewer than 8 moves due to own pieces blocking
  ASSERT_LT(8, moves.size());
  return true;
}

// Test 13: King can capture enemy pieces
bool test_king_capture_moves() {
  Board board;
  // Place king in center with enemy pieces around
  board.setWhiteKing(0x0000000010000000ULL); // E4
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0x0000000008000000ULL);   // D4 - capturable
  board.setBlackKnights(0x0000001000000000ULL); // E5 - capturable
  board.setBlackBishops(0x0000000020000000ULL); // F4 - capturable
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // Should be able to move to empty squares + capture enemy pieces
  ASSERT_EQ(8, moves.size()); // 5 empty squares + 3 captures

  // Use your capture count utility function
  int captureCount = Utils::getCaptureCount(board, WHITE_KING, true);
  ASSERT_EQ(3, captureCount);
  return true;
}

// Test 14: King on edge of board
bool test_king_edge_moves() {
  Board board;
  // Place king on edge (E1)
  board.setWhiteKing(0x0000000000000010ULL); // E1
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // King on edge should have 5 moves
  ASSERT_EQ(5, moves.size());
  return true;
}

// Test 15: Black king moves
bool test_black_king_moves() {
  Board board;
  // Test black king in center
  board.setWhiteKing(0x0000000000000001ULL); // A1 - far away
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x0000001000000000ULL); // E5
  board.setALLPiecesAggregate();

  std::vector<Move> moves =
      MoveGeneration::generateKingMoves(board, false); // false for black

  // Black king in center should have 8 moves
  ASSERT_EQ(8, moves.size());
  return true;
}

// Test 16: Mixed scenario - own pieces, enemy pieces, empty squares
bool test_king_mixed_scenario() {
  Board board;
  // Complex scenario with various blocking/capture situations
  board.setWhiteKing(0x0000000010000000ULL);  // E4
  board.setWhitePawns(0x0000000008000000ULL); // D4 - blocks king
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0x0000000020000000ULL);   // F4 - capturable
  board.setBlackKnights(0x0000000000001000ULL); // E3 - capturable
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // Should have 7 moves (8 total - 1 blocked by own pawn)
  ASSERT_EQ(7, moves.size());
  return true;
}

// Test 17: King with no valid moves (completely surrounded)
bool test_king_no_moves() {
  Board board;
  // Clear all pieces first
  board.setWhitePawns(0ULL);
  board.setWhiteKnights(0ULL);
  board.setWhiteBishops(0ULL);
  board.setWhiteRooks(0ULL);
  board.setWhiteQueens(0ULL);
  board.setBlackPawns(0ULL);
  board.setBlackKnights(0ULL);
  board.setBlackBishops(0ULL);
  board.setBlackRooks(0ULL);
  board.setBlackQueens(0ULL);

  // Place king at E4 (bit 28)
  board.setWhiteKing(0x0000000010000000ULL); // E4

  u64 surroundingSquares = 0;
  surroundingSquares |= (1ULL << 19); // D3
  surroundingSquares |= (1ULL << 20); // E3
  surroundingSquares |= (1ULL << 21); // F3
  surroundingSquares |= (1ULL << 27); // D4
  surroundingSquares |= (1ULL << 29); // F4
  surroundingSquares |= (1ULL << 35); // D5
  surroundingSquares |= (1ULL << 36); // E5
  surroundingSquares |= (1ULL << 37); // F5

  board.setWhitePawns(surroundingSquares);
  board.setBlackKing(0x8000000000000000ULL); // H8
  board.setALLPiecesAggregate();

  std::vector<Move> moves = MoveGeneration::generateKingMoves(board, true);

  // Should have 0 moves when completely surrounded by own pieces
  ASSERT_EQ(0, moves.size());
  return true;
}

bool test_kingside_castling_valid() {
  // Use FEN constructor instead
  // White king on E1, white rook on H1, kingside castling allowed
  Board board("8/8/8/8/8/8/8/4K2R w K - 0 1");

  std::vector<Move> moves = MoveGeneration::generateCastlingMoves(board, true);
  ASSERT_EQ(1, moves.size()); // Should have kingside castling
  return true;
}

// Test: Castling blocked - pieces between king and rook
bool test_castling_blocked() {
  // White king on E1, white rook on H1, white knight on G1 blocking
  Board board("8/8/8/8/8/8/8/4K1NR w K - 0 1");

  std::vector<Move> moves = MoveGeneration::generateCastlingMoves(board, true);
  ASSERT_EQ(0, moves.size()); // No castling possible
  return true;
}

// Test: King in check - cannot castle
bool test_castling_king_in_check() {
  // White king on E1, white rook on H1, black rook on E8 attacking king
  Board board("4r3/8/8/8/8/8/8/4K2R w K - 0 1");

  std::vector<Move> moves = MoveGeneration::generateCastlingMoves(board, true);
  ASSERT_EQ(0, moves.size()); // Cannot castle when in check
  return true;
}

// Test: King passes through attacked square
bool test_castling_through_attack() {
  // White king on E1, white rook on H1, black rook on F8 attacking F1
  Board board("5r2/8/8/8/8/8/8/4K2R w K - 0 1");

  std::vector<Move> moves = MoveGeneration::generateCastlingMoves(board, true);
  ASSERT_EQ(0, moves.size()); // Cannot castle through attacked F1
  return true;
}

// Test: Queenside castling valid
bool test_queenside_castling_valid() {
  // White king on E1, white rook on A1, queenside castling allowed
  Board board("8/8/8/8/8/8/8/R3K3 w Q - 0 1");

  std::vector<Move> moves = MoveGeneration::generateCastlingMoves(board, true);
  ASSERT_EQ(1, moves.size()); // Should have queenside castling
  return true;
}

int main() {
  std::cout << "Running Legal King Move Tests..." << std::endl;

  // RUN_TEST(test_king_legal_pawn_attacks);
  // RUN_TEST(test_king_legal_knight_attacks);
  // RUN_TEST(test_king_legal_bishop_attacks);
  // RUN_TEST(test_king_legal_rook_attacks);
  // RUN_TEST(test_king_legal_capture_undefended);
  // RUN_TEST(test_king_legal_defended_piece);
  // RUN_TEST(test_king_legal_own_pieces_and_attacks);
  // RUN_TEST(test_black_king_legal_moves);
  // RUN_TEST(test_king_one_legal_move);
  // RUN_TEST(test_king_no_legal_moves);
  //
  // RUN_TEST(test_king_moves_basic);
  // RUN_TEST(test_king_corner_moves);
  // RUN_TEST(test_king_blocked_by_own_pieces);
  // RUN_TEST(test_king_capture_moves);
  // RUN_TEST(test_king_edge_moves);
  // RUN_TEST(test_black_king_moves);
  // RUN_TEST(test_king_mixed_scenario);
  // RUN_TEST(test_king_no_moves);

  RUN_TEST(test_kingside_castling_valid);
  RUN_TEST(test_castling_blocked);
  RUN_TEST(test_castling_king_in_check);
  RUN_TEST(test_castling_through_attack);
  RUN_TEST(test_queenside_castling_valid);
  std::cout << "Tests completed!" << std::endl;
  return 0;
}
