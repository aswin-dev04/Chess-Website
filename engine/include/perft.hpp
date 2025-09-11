// perft.hpp
#ifndef PERFT_HPP
#define PERFT_HPP

#include "movegen.hpp"
#include <chrono>
#include <iostream>

class Perft {
public:
  static u64 perft(Board &board, int depth);
  static u64 perftDivide(Board &board, int depth);
  static void runPerftTest(const std::string &fen, int maxDepth);

private:
  static u64 perftRecursive(Board &board, int depth, bool isWhite);
};

#endif // PERFT_HPP
