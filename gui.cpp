#include "gui.h"

ChessGUI::ChessGUI(Board& b)
    : board(b),
      window(sf::VideoMode(tileSize * 8, tileSize * 8), "Chess",
             sf::Style::Titlebar | sf::Style::Close) {

    window.setFramerateLimit(60);
}

void ChessGUI::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        draw_board();
        draw_pieces();
        window.display();
    }
}

void ChessGUI::draw_board() {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));
            square.setPosition(file * tileSize, (7 - rank) * tileSize);

            bool dark = (rank + file) % 2;
            square.setFillColor(
                dark ? sf::Color(118, 150, 86)
                     : sf::Color(238, 238, 210)
            );

            window.draw(square);
        }
    }
}

void ChessGUI::draw_pieces() {
    sf::CircleShape piece(tileSize / 2 - 10);
    piece.setFillColor(sf::Color::Red);

    for (int sq = 0; sq < 64; sq++) {
        Piece p = board.piece_at(sq);
        if (p == EMPTY) continue;

        int rank = sq / 8;
        int file = sq % 8;

        piece.setPosition(
            file * tileSize + 10,
            (7 - rank) * tileSize + 10
        );

        window.draw(piece);
    }
}
