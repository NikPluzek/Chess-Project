#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "board.h"

class ChessGUI {
public:
    ChessGUI(Board& board);
    void run();

private:
    Board& board;
    sf::RenderWindow window;
    int tileSize = 80; 

    int selectedSquare = -1;
    uint64_t highlightedMoves = 0ULL;

    void draw_board();
    void draw_pieces();
    void draw_highlights();
    int mouse_to_square(int x, int y);
};


#endif
