#include "../include/board.hpp"


Board::Board()
{
  setWhitePawns();
  setBlackPawns();
  setWhiteKnights();
  setBlackKnights();
  setWhiteBishops();
  setBlackBishops();
  setWhiteRooks();
  setBlackRooks();
  setWhiteQueens();
  setBlackQueens();
  setWhiteKing();
  setBlackKing();
}

Board::Board(u64 wPawns, u64 bPawns,
      u64 wKnights, u64 bKnights,
      u64 wBishops, u64 bBishops,
      u64 wRooks,   u64 bRooks,
      u64 wQueens,  u64 bQueens,
      u64 wKing,    u64 bKing)
{
  setWhitePawns(wPawns);
  setBlackPawns(bPawns);
  setWhiteKnights(wKnights);
  setBlackKnights(bKnights);
  setWhiteBishops(wBishops);
  setBlackBishops(bBishops);
  setWhiteRooks(wRooks);
  setBlackRooks(bRooks);
  setWhiteQueens(wQueens);
  setBlackQueens(bQueens);
  setWhiteKing(wKing);
  setBlackKing(bKing);
}
