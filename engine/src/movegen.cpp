#include "../include/movegen.hpp"
#include "../include/utils.hpp"

u64 validMoveBB::kingMoves(u64 kingLoc, u64 ownPieces) {

  u64 clip_file_H = kingLoc & Tables::clearFile[7];
  u64 clip_file_A = kingLoc & Tables::clearFile[0];

  u64 sq_1 = clip_file_H << 7;
  u64 sq_2 = kingLoc << 8;
  u64 sq_3 = clip_file_H << 9;
  u64 sq_4 = clip_file_H << 1;

  u64 sq_5 = clip_file_A >> 7;
  u64 sq_6 = kingLoc >> 8;
  u64 sq_7 = clip_file_A >> 9;
  u64 sq_8 = clip_file_A >> 1;

  u64 kingValid =
      (sq_1 | sq_2 | sq_3 | sq_4 | sq_5 | sq_6 | sq_7 | sq_8) & ~(ownPieces);
  return kingValid;
}

std::vector<Move> MoveGeneration::generateKingMoves(Board &board,
                                                    bool isWhite) {

  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 kingLoc = isWhite ? board.getWhiteKing() : board.getBlackKing();
  u64 kingValid = validMoveBB::kingMoves(kingLoc, ownPieces);

  // Get king position
  Square kingSquare = Utils::bitboardToSquare(kingLoc);
  PieceType kingPiece = isWhite ? WHITE_KING : BLACK_KING;

  while (kingValid) {
    Square toSquare = Utils::popLSB(kingValid); // Get square and remove bit

    // Check if it's a capture
    u64 toSquareBitboard = Utils::squareToBitboard(toSquare);
    bool isCapture = (enemyPieces & toSquareBitboard) != 0;

    if (isCapture) {
      // Find what piece we're capturing
      PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
      moves.push_back(Move(kingSquare, toSquare, kingPiece, capturedPiece,
                           false, false, false, false, EMPTY)); // Capture move
    } else {
      moves.push_back(Move(kingSquare, toSquare, kingPiece, EMPTY, false, false,
                           false, false, EMPTY)); // Normal move
    }

    // todo: add castling logic(both king and queenside)
  }
  return moves;
}
