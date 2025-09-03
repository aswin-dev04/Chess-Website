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
      moves.emplace_back(kingSquare, toSquare, kingPiece, capturedPiece, false,
                         false, false, false, EMPTY); // Capture move
    } else {
      moves.emplace_back(kingSquare, toSquare, kingPiece, EMPTY, false, false,
                         false, false, EMPTY); // Normal move
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
    while (knightValid) {
      Square toSquare = Utils::popLSB(knightValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.emplace_back(currKnightSquare, toSquare, knightPiece,
                           capturedPiece, false, false, false, false,
                           EMPTY); // Capture move
      } else {
        moves.emplace_back(currKnightSquare, toSquare, knightPiece, EMPTY,
                           false, false, false, false, EMPTY); // Normal move
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
        moves.emplace_back(currPawnSquare, toSquare, pawnPiece, capturedPiece,
                           false, false, false, false, EMPTY); // Capture move
      } else {
        moves.emplace_back(currPawnSquare, toSquare, pawnPiece, EMPTY, false,
                           false, false, false, EMPTY); // Normal move
      }
    }
  }
  return moves;
}

u64 validMoveBB::whitePawnAttacks(u64 pawnLoc) {
  u64 attacks = 0ULL;
  // Left attacks (northwest)
  attacks |= (pawnLoc << 7) & Tables::clearFile[0];
  // Right attacks (northeast)
  attacks |= (pawnLoc << 9) & Tables::clearFile[7];
  return attacks;
}

u64 validMoveBB::blackPawnAttacks(u64 pawnLoc) {
  u64 attacks = 0ULL;
  // Left attacks (southwest)
  attacks |= (pawnLoc >> 9) & Tables::clearFile[0];
  // Right attacks (southeast)
  attacks |= (pawnLoc >> 7) & Tables::clearFile[7];
  return attacks;
}

u64 validMoveBB::allEnemyAttacks(Board &board, bool isWhite) {

  u64 enemyAttacks = 0ULL;

  u64 ownPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 enemyPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();

  u64 enemyPawns = isWhite ? board.getWhitePawns() : board.getBlackPawns();
  u64 enemyKnights =
      isWhite ? board.getWhiteKnights() : board.getBlackKnights();
  u64 enemyBishops =
      isWhite ? board.getWhiteBishops() : board.getBlackBishops();
  u64 enemyRooks = isWhite ? board.getWhiteRooks() : board.getBlackRooks();
  u64 enemyQueens = isWhite ? board.getWhiteQueens() : board.getBlackQueens();
  u64 enemyKing = isWhite ? board.getWhiteKing() : board.getBlackKing();

  enemyAttacks |= isWhite ? validMoveBB::whitePawnAttacks(enemyPawns)
                          : validMoveBB::blackPawnAttacks(enemyPawns);
  enemyAttacks |= validMoveBB::knightMoves(enemyKnights, enemyPieces);
  enemyAttacks |=
      validMoveBB::bishopMoves(enemyBishops, enemyPieces, ownPieces);
  enemyAttacks |= validMoveBB::rookMoves(enemyRooks, enemyPieces, ownPieces);
  enemyAttacks |= validMoveBB::queenMoves(enemyQueens, enemyPieces, ownPieces);
  enemyAttacks |= validMoveBB::kingMoves(enemyKing, enemyPieces);

  return enemyAttacks;
}

u64 validMoveBB::rookMoves(u64 rookLoc, u64 ownPieces, u64 enemyPieces) {

  if (rookLoc == 0ULL)
    return 0ULL;

  u64 moves = 0ULL;
  u64 blockers = ownPieces | enemyPieces;

  Square sq = Utils::bitboardToSquare(rookLoc);

  int rank = sq / 8;
  int file = sq % 8;

  // North (increasing rank)
  for (int r = rank + 1; r < 8; r++) {
    int targetSq = r * 8 + file;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;         // add move
    if (blockers & targetBB) { // stop if blocked
      break;
    }
  }

  // South (decreasing rank)
  for (int r = rank - 1; r >= 0; r--) {
    int targetSq = r * 8 + file;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  // East (increasing file)
  for (int f = file + 1; f < 8; f++) {
    int targetSq = rank * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  // West (decreasing file)
  for (int f = file - 1; f >= 0; f--) {
    int targetSq = rank * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  moves &= ~ownPieces;

  return moves;
}

std::vector<Move> MoveGeneration::generateRookMoves(Board &board,
                                                    bool isWhite) {

  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 rookLoc = isWhite ? board.getWhiteRooks() : board.getBlackRooks();

  PieceType rookPiece = isWhite ? WHITE_ROOK : BLACK_ROOK;

  while (rookLoc) {
    Square currRookSquare = Utils::popLSB(rookLoc);
    u64 currRook = Utils::squareToBitboard(currRookSquare);
    u64 rookValid = validMoveBB::rookMoves(currRook, ownPieces, enemyPieces);
    while (rookValid) {
      Square toSquare = Utils::popLSB(rookValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.emplace_back(currRookSquare, toSquare, rookPiece, capturedPiece,
                           false, false, false, false, EMPTY); // Capture move
      } else {
        moves.emplace_back(currRookSquare, toSquare, rookPiece, EMPTY, false,
                           false, false, false, EMPTY); // Normal move
      }
    }
  }
  return moves;
}

u64 validMoveBB::bishopMoves(u64 bishopLoc, u64 ownPieces, u64 enemyPieces) {

  if (bishopLoc == 0)
    return 0ULL;

  u64 moves = 0ULL;
  u64 blockers = ownPieces | enemyPieces;

  Square sq = Utils::bitboardToSquare(bishopLoc);

  int rank = sq / 8;
  int file = sq % 8;

  // Northeast (increasing rank and file)
  for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++) {
    int targetSq = r * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  // Northwest (increasing rank and decreasing file)
  for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--) {
    int targetSq = r * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  // Southeast (decreasing rank and increasing file)
  for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++) {
    int targetSq = r * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  // Southwest (decreasing rank and decreasing file)
  for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
    int targetSq = r * 8 + f;
    u64 targetBB = Utils::squareToBitboard(targetSq);

    moves |= targetBB;
    if (blockers & targetBB) {
      break;
    }
  }

  moves &= ~ownPieces;

  return moves;
}

std::vector<Move> MoveGeneration::generateBishopMoves(Board &board,
                                                      bool isWhite) {
  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 bishopLoc = isWhite ? board.getWhiteBishops() : board.getBlackBishops();

  PieceType bishopPiece = isWhite ? WHITE_BISHOP : BLACK_BISHOP;

  while (bishopLoc) {
    Square currBishopSquare = Utils::popLSB(bishopLoc);
    u64 currBishop = Utils::squareToBitboard(currBishopSquare);
    u64 bishopValid =
        validMoveBB::bishopMoves(currBishop, ownPieces, enemyPieces);
    while (bishopValid) {
      Square toSquare = Utils::popLSB(bishopValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.emplace_back(currBishopSquare, toSquare, bishopPiece,
                           capturedPiece, false, false, false, false,
                           EMPTY); // Capture move
      } else {
        moves.emplace_back(currBishopSquare, toSquare, bishopPiece, EMPTY,
                           false, false, false, false, EMPTY); // Normal move
      }
    }
  }
  return moves;
}

u64 validMoveBB::queenMoves(u64 queenLoc, u64 ownPieces, u64 enemyPieces) {
  return validMoveBB::bishopMoves(queenLoc, ownPieces, enemyPieces) |
         validMoveBB::rookMoves(queenLoc, ownPieces, enemyPieces);
}

std::vector<Move> MoveGeneration::generateQueenMoves(Board &board,
                                                     bool isWhite) {
  std::vector<Move> moves;

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 enemyPieces =
      isWhite ? board.getAllBlackPieces() : board.getAllWhitePieces();
  u64 queenLoc = isWhite ? board.getWhiteQueens() : board.getBlackQueens();

  PieceType queenPiece = isWhite ? WHITE_QUEEN : BLACK_QUEEN;

  while (queenLoc) {
    Square currQueenSquare = Utils::popLSB(queenLoc);
    u64 currQueen = Utils::squareToBitboard(currQueenSquare);
    u64 queenValid = validMoveBB::queenMoves(currQueen, ownPieces, enemyPieces);
    while (queenValid) {
      Square toSquare = Utils::popLSB(queenValid);
      bool isCapture = (enemyPieces & Utils::squareToBitboard(toSquare)) != 0;
      if (isCapture) {
        // Find what piece we're capturing
        PieceType capturedPiece = Utils::getPieceTypeAt(board, toSquare);
        moves.emplace_back(currQueenSquare, toSquare, queenPiece, capturedPiece,
                           false, false, false, false,
                           EMPTY); // Capture move
      } else {
        moves.emplace_back(currQueenSquare, toSquare, queenPiece, EMPTY, false,
                           false, false, false, EMPTY); // Normal move
      }
    }
  }
  return moves;
}

u64 validMoveBB::kingLegalMoves(Board &board, bool isWhite) {

  // relevant bitboards
  u64 ownPieces =
      isWhite ? board.getAllWhitePieces() : board.getAllBlackPieces();
  u64 kingLoc = isWhite ? board.getWhiteKing() : board.getBlackKing();
  u64 pseudoLegalMoves = validMoveBB::kingMoves(kingLoc, ownPieces);
  u64 enemyAttacks = allEnemyAttacks(board, !isWhite);

  u64 legalMoves = pseudoLegalMoves & ~enemyAttacks;
  return legalMoves;
}

std::vector<Move> MoveGeneration::generateKingLegalMoves(Board &board,
                                                         bool isWhite) {

  std::vector<Move> legalKingMoves;

  std::vector<Move> pseudoLegalKingMoves = generateKingMoves(board, isWhite);
  for (const Move &move : pseudoLegalKingMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalKingMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalKingMoves;
}

std::vector<Move> MoveGeneration::generateKnightLegalMoves(Board &board,
                                                           bool isWhite) {

  std::vector<Move> legalKnightMoves;

  std::vector<Move> pseudoLegalKnightMoves =
      generateKnightMoves(board, isWhite);
  for (const Move &move : pseudoLegalKnightMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalKnightMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalKnightMoves;
}

std::vector<Move> MoveGeneration::generatePawnLegalMoves(Board &board,
                                                         bool isWhite) {

  std::vector<Move> legalPawnMoves;

  std::vector<Move> pseudoLegalPawnMoves = generatePawnMoves(board, isWhite);
  for (const Move &move : pseudoLegalPawnMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalPawnMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalPawnMoves;
}

std::vector<Move> MoveGeneration::generateRookLegalMoves(Board &board,
                                                         bool isWhite) {

  std::vector<Move> legalRookMoves;

  std::vector<Move> pseudoLegalRookMoves = generateRookMoves(board, isWhite);
  for (const Move &move : pseudoLegalRookMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalRookMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalRookMoves;
}

std::vector<Move> MoveGeneration::generateBishopLegalMoves(Board &board,
                                                           bool isWhite) {

  std::vector<Move> legalBishopMoves;

  std::vector<Move> pseudoLegalBishopMoves =
      generateBishopMoves(board, isWhite);
  for (const Move &move : pseudoLegalBishopMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalBishopMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalBishopMoves;
}

std::vector<Move> MoveGeneration::generateQueenLegalMoves(Board &board,
                                                          bool isWhite) {

  std::vector<Move> legalQueenMoves;

  std::vector<Move> pseudoLegalQueenMoves = generateQueenMoves(board, isWhite);
  for (const Move &move : pseudoLegalQueenMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalQueenMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalQueenMoves;
}

std::vector<Move> MoveGeneration::generateAllMoves(Board &board, bool isWhite) {

  std::vector<Move> allMoves;

  std::vector<Move> pawnMoves = generatePawnLegalMoves(board, isWhite);
  std::vector<Move> knightMoves = generateKnightLegalMoves(board, isWhite);
  std::vector<Move> bishopMoves = generateBishopLegalMoves(board, isWhite);
  std::vector<Move> rookMoves = generateRookLegalMoves(board, isWhite);
  std::vector<Move> queenMoves = generateQueenLegalMoves(board, isWhite);
  std::vector<Move> kingMoves = generateKingLegalMoves(board, isWhite);

  // Append all to one vector
  allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
  allMoves.insert(allMoves.end(), knightMoves.begin(), knightMoves.end());
  allMoves.insert(allMoves.end(), bishopMoves.begin(), bishopMoves.end());
  allMoves.insert(allMoves.end(), rookMoves.begin(), rookMoves.end());
  allMoves.insert(allMoves.end(), queenMoves.begin(), queenMoves.end());
  allMoves.insert(allMoves.end(), kingMoves.begin(), kingMoves.end());

  if (board.isKingChecked(isWhite)) {
    if (board.getAttackersCount(isWhite) > 1)
      return kingMoves;
    return generateLegalMovesWhileInCheck(board, isWhite, allMoves);
  }

  return allMoves;
}

std::vector<Move>
MoveGeneration::generateLegalMovesWhileInCheck(Board &board, bool isWhite,
                                               std::vector<Move> allMoves) {

  std::vector<Move> legalMoves;

  for (const Move &move : allMoves) {
    board.makeMove(move);
    if (!board.isKingChecked(isWhite)) {
      legalMoves.push_back(move);
    }
    board.undoMove();
  }
  return legalMoves;
}
