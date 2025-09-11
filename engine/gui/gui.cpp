#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 80;
const int WINDOW_WIDTH = BOARD_SIZE * SQUARE_SIZE;
const int WINDOW_HEIGHT = BOARD_SIZE * SQUARE_SIZE;

class ChessGUI {
private:
  sf::RenderWindow window;
  Board board;
  sf::Color lightSquare;
  sf::Color darkSquare;
  sf::Color selectedSquare;
  sf::Color legalMoveSquare;

  std::map<PieceType, sf::Texture> pieceTextures;

  int selectedSquareIndex;
  std::vector<Move> legalMoves;
  std::vector<int> highlightedSquares;

public:
  ChessGUI()
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
               "Chess Debug GUI") {
    lightSquare = sf::Color(240, 217, 181);
    darkSquare = sf::Color(181, 136, 99);
    selectedSquare = sf::Color(255, 255, 0, 128);
    legalMoveSquare = sf::Color(0, 255, 0, 128);
    selectedSquareIndex = -1;

    board = Board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

    loadTextures();
  }

  void run() {
    while (window.isOpen()) {
      handleEvents();
      render();
    }
  }

private:
  void loadTextures() {
    auto load = [&](PieceType type, const std::string &filename) {
      sf::Texture texture;
      if (!texture.loadFromFile("../assets/" + filename)) {
        std::cout << "Failed to load " << filename << std::endl;
      }
      texture.setSmooth(true);
      pieceTextures[type] = std::move(texture);
    };

    load(WHITE_PAWN, "wpawn.png");
    load(WHITE_ROOK, "wrook.png");
    load(WHITE_KNIGHT, "wknight.png");
    load(WHITE_BISHOP, "wbishop.png");
    load(WHITE_QUEEN, "wqueen.png");
    load(WHITE_KING, "wking.png");

    load(BLACK_PAWN, "bpawn.png");
    load(BLACK_ROOK, "brook.png");
    load(BLACK_KNIGHT, "bknight.png");
    load(BLACK_BISHOP, "bbishop.png");
    load(BLACK_QUEEN, "bqueen.png");
    load(BLACK_KING, "bking.png");
  }

  void handleEvents() {
    while (const auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }

      if (const auto *mouseButtonPressed =
              event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
          handleMouseClick(mouseButtonPressed->position.x,
                           mouseButtonPressed->position.y);
        }
      }

      if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::R) {
          resetBoard();
        }
        if (keyPressed->code == sf::Keyboard::Key::U) {
          undoMove();
        }
      }
    }
  }

  void handleMouseClick(int x, int y) {
    if (y >= BOARD_SIZE * SQUARE_SIZE || x >= BOARD_SIZE * SQUARE_SIZE) {
      return;
    }

    int file = x / SQUARE_SIZE;
    int rank = 7 - (y / SQUARE_SIZE);
    int squareIndex = rank * 8 + file;

    // Bounds check
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
      return;
    }

    std::cout << "Clicked at screen (" << x << ", " << y << ") -> file:" << file
              << " rank:" << rank << " square:" << squareIndex << std::endl;

    if (selectedSquareIndex == -1) {
      // Check what piece is at this square
      PieceType piece =
          Utils::getPieceTypeAt(board, Utils::makeSquare(rank, file));
      std::cout << "Piece at position (" << rank << "," << file
                << "): " << piece << std::endl;

      if (piece != EMPTY && isCorrectColorPiece(piece)) {
        selectSquare(squareIndex);
      }
    } else {
      if (squareIndex == selectedSquareIndex) {
        deselectSquare();
      } else {
        attemptMove(selectedSquareIndex, squareIndex);
        deselectSquare();
      }
    }
  }

  bool isCorrectColorPiece(PieceType piece) {
    bool isWhiteTurn = board.getWhiteToMove();
    bool isWhitePiece = (piece >= WHITE_PAWN && piece <= WHITE_KING);
    std::cout << "White to move: " << isWhiteTurn
              << ", White piece: " << isWhitePiece << std::endl;
    return isWhiteTurn == isWhitePiece;
  }

  void selectSquare(int squareIndex) {
    selectedSquareIndex = squareIndex;
    std::cout << "Selected square: " << squareIndex << std::endl;

    // Generate legal moves for current player
    legalMoves =
        MoveGeneration::generateAllMoves(board, board.getWhiteToMove());
    highlightedSquares.clear();

    for (const Move &move : legalMoves) {
      if (move.getFromSquare() == squareIndex) {
        highlightedSquares.push_back(move.getToSquare());
        std::cout << "  Legal move to square: " << move.getToSquare()
                  << std::endl;
      }
    }

    std::cout << "Found " << highlightedSquares.size()
              << " legal moves from square " << squareIndex << std::endl;
  }

  void deselectSquare() {
    selectedSquareIndex = -1;
    highlightedSquares.clear();
    legalMoves.clear();
  }

  void attemptMove(int fromSquare, int toSquare) {
    std::cout << "Attempting move from square " << fromSquare << " to "
              << toSquare << std::endl;

    for (const Move &move : legalMoves) {
      if (move.getFromSquare() == fromSquare &&
          move.getToSquare() == toSquare) {
        board.makeMove(move);
        std::cout << "Made move: " << move << std::endl;
        return;
      }
    }

    std::cout << "Illegal move attempted: " << fromSquare << " to " << toSquare
              << std::endl;
  }

  void resetBoard() {
    board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    deselectSquare();
    std::cout << "Board reset to starting position" << std::endl;
  }

  void undoMove() {
    board.undoMove();
    deselectSquare();
    std::cout << "Move undone" << std::endl;
  }

  void render() {
    window.clear();

    drawBoard();
    drawPieces();
    drawHighlights();

    window.display();
  }

  void drawBoard() {
    for (int rank = 0; rank < 8; rank++) {
      for (int file = 0; file < 8; file++) {
        sf::RectangleShape square(
            {static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)});

        // Screen position: rank 7 at top (y=0), rank 0 at bottom
        // (y=7*SQUARE_SIZE)
        square.setPosition({static_cast<float>(file * SQUARE_SIZE),
                            static_cast<float>((7 - rank) * SQUARE_SIZE)});

        // Color pattern
        if ((rank + file) % 2 == 0) {
          square.setFillColor(lightSquare);
        } else {
          square.setFillColor(darkSquare);
        }

        window.draw(square);
      }
    }
  }

  void drawHighlights() {
    // Highlight selected square
    if (selectedSquareIndex != -1) {
      int file = selectedSquareIndex % 8;
      int rank = selectedSquareIndex / 8;

      sf::RectangleShape highlight(
          {static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE)});
      highlight.setPosition({static_cast<float>(file * SQUARE_SIZE),
                             static_cast<float>((7 - rank) * SQUARE_SIZE)});
      highlight.setFillColor(selectedSquare);
      window.draw(highlight);
    }

    // Highlight legal move squares
    for (int squareIndex : highlightedSquares) {
      int file = squareIndex % 8;
      int rank = squareIndex / 8;

      sf::CircleShape dot(10);
      dot.setPosition(
          {static_cast<float>(file * SQUARE_SIZE + SQUARE_SIZE / 2.0f - 10),
           static_cast<float>((7 - rank) * SQUARE_SIZE + SQUARE_SIZE / 2.0f -
                              10)});
      dot.setFillColor(legalMoveSquare);
      window.draw(dot);
    }
  }

  void drawPieces() {
    for (int chessRank = 0; chessRank < 8; chessRank++) {
      for (int file = 0; file < 8; file++) {
        PieceType piece =
            Utils::getPieceTypeAt(board, Utils::makeSquare(chessRank, file));

        if (piece != EMPTY) {
          sf::Sprite sprite(pieceTextures[piece]);
          sprite.setScale({static_cast<float>(SQUARE_SIZE) /
                               sprite.getTexture().getSize().x,
                           static_cast<float>(SQUARE_SIZE) /
                               sprite.getTexture().getSize().y});

          // Screen position: chess rank 7 at screen y=0, chess rank 0 at screen
          // y=7*SQUARE_SIZE
          int screenY = (7 - chessRank) * SQUARE_SIZE;
          sprite.setPosition({static_cast<float>(file * SQUARE_SIZE),
                              static_cast<float>(screenY)});

          window.draw(sprite);
        }
      }
    }
  }
};

int main() {
  ChessGUI gui;
  gui.run();
  return 0;
}
