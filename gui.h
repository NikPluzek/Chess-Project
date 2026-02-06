#ifndef GUI_H
#define GUI_H

#include "board.h"
#include <SFML/Graphics.hpp>

class ChessGUI
{
  public:
    ChessGUI(Board& board);
    void run();

  private:
    Board& board;
    sf::RenderWindow window;
    int tileSize = 80;

    int selectedSquare = -1;
    uint64_t highlightedMoves = 0ULL;
    uint64_t highlightedAttacks = 0ULL;
    uint64_t legalMoves = 0ULL;

    bool pieceSelected = false;
    Piece selectedPiece = EMPTY;

    void draw_board();
    void draw_pieces();
    void draw_highlights();
    int mouse_to_square(int x, int y);
};

#endif
