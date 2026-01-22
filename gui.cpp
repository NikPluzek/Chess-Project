#include "gui.h"
#include "bitboard.h"
#include "board.h"

ChessGUI::ChessGUI(Board& b) : board(b) {
    window.create(sf::VideoMode(tileSize * 8, tileSize * 8), "Chess", sf::Style::Default);
    window.setVisible(true);
    window.setPosition(sf::Vector2i(200, 200));
    window.setFramerateLimit(60);
}


void ChessGUI::run() {
    sf::Event event;

    while (window.isOpen()) {

        // Events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                int sq = mouse_to_square(event.mouseButton.x,
                                         event.mouseButton.y);

                if (sq != -1) {
                    Piece p = board.piece_at(sq);

                    if (p != EMPTY) {
                        selectedSquare = sq;

                        // TEMP: knight only
                        if (p == BN || p == WN) {
                            highlightedMoves = knight_attacks[sq];
                        } else {
                            highlightedMoves = 0ULL;
                        }
                    } else {
                        selectedSquare = -1;
                        highlightedMoves = 0ULL;
                    }
                }
            }
        }

        // Draw
        window.clear(sf::Color::White);
        draw_board();
        draw_highlights();   // you can enable this now
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
    //piece.setFillColor(sf::Color::Red);

    for (int sq = 0; sq < 64; sq++) {
        Piece p = board.piece_at(sq);
        if (p == EMPTY) continue;

        //defining piece colour
        if (p >= WP && p <= WK)
            piece.setFillColor(sf::Color::White);
        else
            piece.setFillColor(sf::Color::Black);

        int rank = sq / 8;
        int file = sq % 8;

        piece.setPosition(
            file * tileSize + 10,
            (7 - rank) * tileSize + 10
        );

        window.draw(piece);
    }
}

int ChessGUI::mouse_to_square(int mouseX, int mouseY) {
    int file = mouseX / tileSize;
    int rank = 7 - (mouseY / tileSize);

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1;

    return rank * 8 + file;
}

void ChessGUI::draw_highlights() {
    sf::RectangleShape highlight(
        sf::Vector2f(tileSize, tileSize)
    );
    highlight.setFillColor(sf::Color(0, 0, 255, 100));

    for (int sq = 0; sq < 64; sq++) {
        if (highlightedMoves & (1ULL << sq)) {
            int rank = sq / 8;
            int file = sq % 8;

            highlight.setPosition(
                file * tileSize,
                (7 - rank) * tileSize
            );

            window.draw(highlight);
        }
    }
}

