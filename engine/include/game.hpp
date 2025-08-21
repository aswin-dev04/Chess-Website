#ifndef GAME_H
#define GAME_H

#include "board.hpp"

class Game {
private:
  Board &board;
  bool playerTurn;
  bool whiteInCheck;
  bool blackInCheck;
  bool checkMate;
  bool draw;

public:
  Game(Board &board)
      : board(board), playerTurn(true), whiteInCheck(false),
        blackInCheck(false), checkMate(false), draw(false) {};
  inline void switchTurn() { playerTurn = !playerTurn; }
  ~Game() = default;

  // getters and setters for Game state
  inline bool isPlayerTurn() const { return playerTurn; }
  inline void setPlayerTurn(bool playerTurn) { this->playerTurn = playerTurn; }

  inline bool isWhiteInCheck() const { return whiteInCheck; }
  inline void setWhiteInCheck(bool whiteInCheck) {
    this->whiteInCheck = whiteInCheck;
  }

  inline bool isBlackInCheck() const { return blackInCheck; }
  inline void setBlackInCheck(bool blackInCheck) {
    this->blackInCheck = blackInCheck;
  }

  inline bool isCheckMate() const { return checkMate; }
  inline void setCheckMate(bool checkMate) { this->checkMate = checkMate; }

  inline bool isDraw() const { return draw; }
  inline void setDraw(bool draw) { this->draw = draw; }
};
#endif
