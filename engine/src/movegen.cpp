#include "../include/movegen.hpp"
#include "../include/utils.hpp"

u64 validMoveBB::kingMoves(u64 kingLoc, u64 ownPieces) {

  u64 clip_file_H = kingLoc & Tables::clearFile[7];
  u64 clip_file_A = kingLoc & Tables::clearFile[0];

  u64 sq_1 = clip_file_A << 7;
  u64 sq_2 = kingLoc << 8;
  u64 sq_3 = clip_file_H << 9;
  u64 sq_4 = clip_file_H << 1;

  u64 sq_5 = clip_file_H >> 7;
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

u64 validMoveBB::knightMoves(u64 knightLoc, u64 ownPieces) {

  u64 clip_file_H = Tables::clearFile[7]; // Can't move right from H file
  u64 clip_file_A = Tables::clearFile[0]; // Can't move left from A file
  u64 clip_file_G = Tables::clearFile[6]; // Can't move 2 right from G file
  u64 clip_file_B = Tables::clearFile[1]; // Can't move 2 left from B file

  u64 sq_1 = (clip_file_A & clip_file_B & knightLoc) << 6;  // up 1, left 2
  u64 sq_2 = (clip_file_A & knightLoc) << 15;               // up 2, left 1
  u64 sq_3 = (clip_file_H & knightLoc) << 17;               // up 2, right 1
  u64 sq_4 = (clip_file_H & clip_file_G & knightLoc) << 10; // up 1, right 2
  u64 sq_5 = (clip_file_H & clip_file_G & knightLoc) >> 6;  // down 1, right 2
  u64 sq_6 = (clip_file_H & knightLoc) >> 15;               // down 2, right 1
  u64 sq_7 = (clip_file_A & knightLoc) >> 17;               // down 2, left 1
  u64 sq_8 = (clip_file_A & clip_file_B & knightLoc) >> 10; // down 1, left 2

  u64 knightValid =
      (sq_1 | sq_2 | sq_3 | sq_4 | sq_5 | sq_6 | sq_7 | sq_8) & ~(ownPieces);

  return knightValid;
}

std::vector<Move> MoveGeneration::generateKnightMoves(Board &board,
                                                      bool isWhite) {

  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 knightLoc = isWhite ? board.getWhiteKnights() : board.getBlackKnights();

  PieceType knightPiece = isWhite ? WHITE_KNIGHT : BLACK_KNIGHT;

  while (knightLoc) {
    Square currKnightSquare = Utils::popLSB(knightLoc);
    u64 currKnight = Utils::squareToBitboard(currKnightSquare);
    u64 knightValid = validMoveBB::knightMoves(currKnight, ownPieces);
    std::cout << knightValid << std::endl;
    while (knightValid) {
      Square toSquare = Utils::popLSB(knightValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.push_back(Move(currKnightSquare, toSquare, knightPiece,
                             capturedPiece, false, false, false, false,
                             EMPTY)); // Capture move
      } else {
        moves.push_back(Move(currKnightSquare, toSquare, knightPiece, EMPTY,
                             false, false, false, false, EMPTY)); // Normal move
      }
    }
  }
  return moves;
}

u64 validMoveBB::whitePawnMoves(u64 whitePawns, u64 ownPieces, u64 allPieces,
                                u64 enemyPieces) {

  u64 one_square_forward = (whitePawns << 8) & ~allPieces;
  u64 two_square_forward =
      ((one_square_forward & Tables::maskRank[2]) << 8) & ~allPieces;
  u64 validMoves = one_square_forward | two_square_forward;

  u64 white_pawn_attack_left = (whitePawns & Tables::clearFile[0]) << 7;
  u64 white_pawn_attack_right = (whitePawns & Tables::clearFile[7]) << 9;
  u64 allAttacks = white_pawn_attack_left | white_pawn_attack_right;

  u64 validAttacks = allAttacks & enemyPieces;

  return validMoves | validAttacks;
}

u64 validMoveBB::blackPawnMoves(u64 blackPawns, u64 ownPieces, u64 allPieces,
                                u64 enemyPieces) {

  u64 one_square_forward = (blackPawns >> 8) & ~allPieces;
  u64 two_square_forward =
      ((one_square_forward & Tables::maskRank[5]) >> 8) & ~allPieces;
  u64 validMoves = one_square_forward | two_square_forward;

  u64 black_pawn_attack_left = (blackPawns & Tables::clearFile[0]) >> 9;
  u64 black_pawn_attack_right = (blackPawns & Tables::clearFile[7]) >> 7;
  u64 allAttacks = black_pawn_attack_left | black_pawn_attack_right;

  u64 validAttacks = allAttacks & enemyPieces;

  return validMoves | validAttacks;
}

std::vector<Move> MoveGeneration::generatePawnMoves(Board &board,
                                                    bool isWhite) {

  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 pawnLoc = isWhite ? board.getWhitePawns() : board.getBlackPawns();
  u64 allPieces = board.getAllPieces();

  PieceType pawnPiece = isWhite ? WHITE_PAWN : BLACK_PAWN;

  while (pawnLoc) {
    Square currPawnSquare = Utils::popLSB(pawnLoc);
    u64 currPawn = Utils::squareToBitboard(currPawnSquare);
    u64 pawnValid = isWhite
                        ? validMoveBB::whitePawnMoves(currPawn, ownPieces,
                                                      allPieces, enemyPieces)
                        : validMoveBB::blackPawnMoves(currPawn, ownPieces,
                                                      allPieces, enemyPieces);
    while (pawnValid) {
      Square toSquare = Utils::popLSB(pawnValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.push_back(Move(currPawnSquare, toSquare, pawnPiece, capturedPiece,
                             false, false, false, false,
                             EMPTY)); // Capture move
      } else {
        moves.push_back(Move(currPawnSquare, toSquare, pawnPiece, EMPTY, false,
                             false, false, false, EMPTY)); // Normal move
      }
    }
  }
  return moves;
}
