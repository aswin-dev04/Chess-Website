#ifndef BOARD_HPP
#define BOARD_HPP

#include "move.hpp"
#include <cstdint>
#include <memory>
#include <sys/types.h>

using u64 = uint64_t;

class Board {
private:
  u64 whitePawns;
  u64 whiteKnights;
  u64 whiteBishops;
  u64 whiteRooks;
  u64 whiteQueens;
  u64 whiteKing;
  u64 blackPawns;
  u64 blackKnights;
  u64 blackBishops;
  u64 blackRooks;
  u64 blackQueens;
  u64 blackKing;
  u64 allWhitePieces;
  u64 allBlackPieces;
  u64 allPieces;

  std::unique_ptr<Board> prevState;

public:
  Board();
  Board(u64 wPawns, u64 bPawns, u64 wKnights, u64 bKnights, u64 wBishops,
        u64 bBishops, u64 wRooks, u64 bRooks, u64 wQueens, u64 bQueens,
        u64 wKing, u64 bKing);
  ~Board() = default;

  Board &operator=(const Board &other);

  void makeMove(const Move &move);
  inline void undoMove() {
    if (prevState)
      *this = *prevState;
  }

  bool isKingChecked(bool isWhite);

  int getAttackersCount(bool isWhite);

  // getters and setters for pawns
  inline u64 getWhitePawns() const { return whitePawns; }
  inline u64 getBlackPawns() const { return blackPawns; }

  inline void setWhitePawns() { whitePawns = 0x000000000000FF00ULL; }
  inline void setWhitePawns(u64 value) { whitePawns = value; }

  inline void setBlackPawns() { blackPawns = 0x00FF000000000000ULL; }
  inline void setBlackPawns(u64 value) { blackPawns = value; }

  // getters and setters for knights
  inline u64 getWhiteKnights() const { return whiteKnights; }
  inline u64 getBlackKnights() const { return blackKnights; }

  inline void setWhiteKnights() { whiteKnights = 0x0000000000000042ULL; }
  inline void setWhiteKnights(u64 value) { whiteKnights = value; }

  inline void setBlackKnights() { blackKnights = 0x4200000000000000ULL; }
  inline void setBlackKnights(u64 value) { blackKnights = value; }

  // getters and setters for bishops
  inline u64 getWhiteBishops() const { return whiteBishops; }
  inline u64 getBlackBishops() const { return blackBishops; }

  inline void setWhiteBishops() { whiteBishops = 0x0000000000000024ULL; }
  inline void setWhiteBishops(u64 value) { whiteBishops = value; }

  inline void setBlackBishops() { blackBishops = 0x2400000000000000ULL; }
  inline void setBlackBishops(u64 value) { blackBishops = value; }

  // getters and setters for rooks
  inline u64 getWhiteRooks() const { return whiteRooks; }
  inline u64 getBlackRooks() const { return blackRooks; }

  inline void setWhiteRooks() { whiteRooks = 0x0000000000000081ULL; }
  inline void setWhiteRooks(u64 value) { whiteRooks = value; }

  inline void setBlackRooks() { blackRooks = 0x8100000000000000ULL; }
  inline void setBlackRooks(u64 value) { blackRooks = value; }

  // getters and setters for queens
  inline u64 getWhiteQueens() const { return whiteQueens; }
  inline u64 getBlackQueens() const { return blackQueens; }

  inline void setWhiteQueens() { whiteQueens = 0x0000000000000008ULL; }
  inline void setWhiteQueens(u64 value) { whiteQueens = value; }

  inline void setBlackQueens() { blackQueens = 0x0800000000000000ULL; }
  inline void setBlackQueens(u64 value) { blackQueens = value; }

  // getters and setters for kings
  inline u64 getWhiteKing() const { return whiteKing; }
  inline u64 getBlackKing() const { return blackKing; }

  inline void setWhiteKing() { whiteKing = 0x0000000000000010ULL; }
  inline void setWhiteKing(u64 value) { whiteKing = value; }

  inline void setBlackKing() { blackKing = 0x1000000000000000ULL; }
  inline void setBlackKing(u64 value) { blackKing = value; }

  // getters and setters for additional bitboards
  inline u64 getAllWhitePieces() const { return allWhitePieces; }
  inline u64 getAllBlackPieces() const { return allBlackPieces; }

  inline void setAllWhitePieces() {
    allWhitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks |
                     whiteQueens | whiteKing;
  }

  inline void setAllBlackPieces() {
    allBlackPieces = blackPawns | blackKnights | blackBishops | blackRooks |
                     blackQueens | blackKing;
  }

  inline u64 getAllPieces() const { return allPieces; }
  inline void setAllPieces() { allPieces = allWhitePieces | allBlackPieces; }
  inline void setALLPiecesAggregate() {
    setAllWhitePieces();
    setAllBlackPieces();
    setAllPieces();
  }
};

#endif
