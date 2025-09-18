// 1K5k/8/8/8/8/8/P7/8 w - - 0 1

#include "../include/ai.hpp"
#include "../include/evaluation.hpp"
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

  // Promotion selection state
  bool showingPromotionDialog;
  int promotionFromSquare;
  int promotionToSquare;
  std::vector<Move> promotionMoves; // The 4 promotion move options
  sf::RectangleShape promotionDialog;
  std::vector<sf::RectangleShape> promotionButtons;

  ChessAI ai;
  bool aiPlaysBlack = true;

public:
  ChessGUI()
      : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
               "Chess Debug GUI") {
    lightSquare = sf::Color(240, 217, 181);
    darkSquare = sf::Color(181, 136, 99);
    selectedSquare = sf::Color(255, 255, 0, 128);
    legalMoveSquare = sf::Color(0, 255, 0, 128);
    selectedSquareIndex = -1;

    // Initialize promotion dialog state
    showingPromotionDialog = false;
    promotionFromSquare = -1;
    promotionToSquare = -1;
    setupPromotionDialog();

    board = Board("4k3/8/8/8/8/8/PPPPPPPP/4K3 w KQkq - 0 1");

    loadTextures();
  }

  void run() {
    while (window.isOpen()) {
      handleEvents();
      render();
    }
  }

private:
  void setupPromotionDialog() {
    // Create dialog background
    promotionDialog.setSize({320.0f, 100.0f});
    promotionDialog.setPosition(
        {(WINDOW_WIDTH - 320) / 2.0f, (WINDOW_HEIGHT - 100) / 2.0f});
    promotionDialog.setFillColor(sf::Color(200, 200, 200, 240));
    promotionDialog.setOutlineThickness(2);
    promotionDialog.setOutlineColor(sf::Color::Black);

    // Create 4 buttons for piece selection
    promotionButtons.clear();
    for (int i = 0; i < 4; i++) {
      sf::RectangleShape button;
      button.setSize({70.0f, 70.0f});
      button.setPosition({(WINDOW_WIDTH - 320) / 2.0f + 15 + i * 75.0f,
                          (WINDOW_HEIGHT - 100) / 2.0f + 15});
      button.setFillColor(sf::Color::White);
      button.setOutlineThickness(1);
      button.setOutlineColor(sf::Color::Black);
      promotionButtons.push_back(button);
    }
  }

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
          if (showingPromotionDialog) {
            handlePromotionClick(mouseButtonPressed->position.x,
                                 mouseButtonPressed->position.y);
          } else {
            handleMouseClick(mouseButtonPressed->position.x,
                             mouseButtonPressed->position.y);
          }
        }
      }

      if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (!showingPromotionDialog) { // Don't handle keyboard when promotion
                                       // dialog is open
          if (keyPressed->code == sf::Keyboard::Key::R) {
            resetBoard();
          }
          if (keyPressed->code == sf::Keyboard::Key::U) {
            undoMove();
          }
        }
      }
    }
  }

  void handlePromotionClick(int x, int y) {

    // Check if click is on one of the promotion buttons
    for (int i = 0; i < 4; i++) {
      sf::RectangleShape &button = promotionButtons[i];

      if (x >= button.getPosition().x &&
          x <= button.getPosition().x + button.getSize().x &&
          y >= button.getPosition().y &&
          y <= button.getPosition().y + button.getSize().y) {

        // Find the corresponding promotion move
        PieceType selectedPromotion;
        bool isWhite = board.getWhiteToMove();

        switch (i) {
        case 0:
          selectedPromotion = isWhite ? WHITE_QUEEN : BLACK_QUEEN;
          break;
        case 1:
          selectedPromotion = isWhite ? WHITE_ROOK : BLACK_ROOK;
          break;
        case 2:
          selectedPromotion = isWhite ? WHITE_BISHOP : BLACK_BISHOP;
          break;
        case 3:
          selectedPromotion = isWhite ? WHITE_KNIGHT : BLACK_KNIGHT;
          break;
        }

        std::cout << "Selected promotion piece type: " << selectedPromotion
                  << std::endl;
        std::cout << "Available promotion moves (" << promotionMoves.size()
                  << "):" << std::endl;

        // Find and execute the matching promotion move
        for (const Move &move : promotionMoves) {

          if (move.getPromotionPiece() == selectedPromotion) {
            board.makeMove(move);
            promotionMoves.clear();
            std::cout << "Evaluation: "
                      << -(static_cast<float>(Evaluation::evaluate(board)) /
                           100)
                      << std::endl;

            // Close promotion dialog
            showingPromotionDialog = false;
            deselectSquare();

            // Make AI move after player's promotion
            makeAIMove();
            return;
          }
        }

        std::cout << "ERROR: No matching promotion move found for piece type "
                  << selectedPromotion << std::endl;
        break;
      }
    }

    // If clicked outside dialog, cancel promotion
    if (x < promotionDialog.getPosition().x ||
        x > promotionDialog.getPosition().x + promotionDialog.getSize().x ||
        y < promotionDialog.getPosition().y ||
        y > promotionDialog.getPosition().y + promotionDialog.getSize().y) {
      std::cout << "Clicked outside promotion dialog - cancelling" << std::endl;
      showingPromotionDialog = false;
      deselectSquare();
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

    if (selectedSquareIndex == -1) {
      // Check what piece is at this square
      PieceType piece =
          Utils::getPieceTypeAt(board, Utils::makeSquare(rank, file));

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

  void makeAIMove() {

    Move aiMove = ai.getBestMove(board);

    if (aiMove.getFromSquare() != aiMove.getToSquare()) {
      board.makeMove(aiMove);
      std::cout << "\n\nMade move: " << aiMove << "\nEvaluation: "
                << static_cast<float>(Evaluation::evaluate(board)) / 100 << "\n"
                << std::endl;
      if (ai.isCheckmate(board, false))
        std::cout << "****** Checkmate! AI wins! ******\n\n" << std::endl;
    }
  }

  void attemptMove(int fromSquare, int toSquare) {
    std::cout << "Attempting move from square " << fromSquare << " to "
              << toSquare << std::endl;

    bool playerPlayed = false;
    std::vector<Move> possiblePromotions;

    std::cout << "Checking " << legalMoves.size()
              << " legal moves:" << std::endl;

    // Check if this is a promotion move
    for (const Move &move : legalMoves) {

      if (move.getFromSquare() == fromSquare &&
          move.getToSquare() == toSquare) {

        if (move.getIsPromotion()) {
          possiblePromotions.push_back(move);
        } else {
          // Regular move (non-promotion)
          board.makeMove(move);
          playerPlayed = true;
          std::cout << "Made move: " << move << std::endl;
          std::cout << "Evaluation: "
                    << -(static_cast<float>(Evaluation::evaluate(board)) / 100)
                    << std::endl;
          if (ai.isCheckmate(board, true))
            std::cout << "****** Checkmate! You win! ******\n\n" << std::endl;

          if (playerPlayed) {
            // Clear selection after regular move
            selectedSquareIndex = -1;
            highlightedSquares.clear();
            legalMoves.clear();

            makeAIMove();
            playerPlayed = false;
          }
          return;
        }
      }
    }

    // If we found promotion moves, show dialog
    if (!possiblePromotions.empty()) {
      promotionMoves = possiblePromotions;
      promotionFromSquare = fromSquare;
      promotionToSquare = toSquare;
      showingPromotionDialog = true;
      std::cout << "Showing promotion dialog with " << possiblePromotions.size()
                << " options" << std::endl;
      return;
    }

    std::cout << "Illegal move attempted: " << fromSquare << " to " << toSquare
              << std::endl;
  }

  void resetBoard() {
    board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    showingPromotionDialog = false;
    promotionMoves.clear(); // Clear promotion moves when resetting
    deselectSquare();
    std::cout << "Board reset to starting position" << std::endl;
  }

  void undoMove() {
    board.undoMove();
    showingPromotionDialog = false;
    promotionMoves.clear(); // Clear promotion moves when undoing
    deselectSquare();
    std::cout << "Move undone" << std::endl;
  }

  void render() {
    window.clear();

    drawBoard();
    drawPieces();
    drawHighlights();

    // Draw promotion dialog on top if active
    if (showingPromotionDialog) {
      drawPromotionDialog();
    }

    window.display();
  }

  void drawPromotionDialog() {
    // Draw semi-transparent overlay
    sf::RectangleShape overlay(
        {static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw dialog background
    window.draw(promotionDialog);

    // Draw buttons and piece icons
    bool isWhite = board.getWhiteToMove();
    PieceType pieces[4] = {isWhite ? WHITE_QUEEN : BLACK_QUEEN,
                           isWhite ? WHITE_ROOK : BLACK_ROOK,
                           isWhite ? WHITE_BISHOP : BLACK_BISHOP,
                           isWhite ? WHITE_KNIGHT : BLACK_KNIGHT};

    for (int i = 0; i < 4; i++) {
      // Draw button background
      window.draw(promotionButtons[i]);

      // Draw piece sprite on button
      sf::Sprite pieceSprite(pieceTextures[pieces[i]]);
      sf::Vector2f buttonPos = promotionButtons[i].getPosition();
      sf::Vector2f buttonSize = promotionButtons[i].getSize();

      // Scale and center the piece on the button
      pieceSprite.setScale(
          {buttonSize.x * 0.8f / pieceSprite.getTexture().getSize().x,
           buttonSize.y * 0.8f / pieceSprite.getTexture().getSize().y});

      sf::Vector2f spriteSize = {
          pieceSprite.getTexture().getSize().x * pieceSprite.getScale().x,
          pieceSprite.getTexture().getSize().y * pieceSprite.getScale().y};

      pieceSprite.setPosition(
          {buttonPos.x + (buttonSize.x - spriteSize.x) / 2.0f,
           buttonPos.y + (buttonSize.y - spriteSize.y) / 2.0f});

      window.draw(pieceSprite);
    }
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
