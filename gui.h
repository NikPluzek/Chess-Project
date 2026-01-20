#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "board.h"

class ChessGUI {
public:
    ChessGUI(Board& board);
    void run();

private:
    void draw_board();
    void draw_pieces();

    Board& board;
    int tileSize = 80;          
    sf::RenderWindow window;
};


#endif
