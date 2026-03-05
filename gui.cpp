#include "gui.h"
#include "bitboard.h"
#include "board.h"
#include "movegen.h"
#include "move.h"

ChessGUI::ChessGUI(Board& b) : board(b)
{
    window.create(sf::VideoMode(tileSize * 8, tileSize * 8), "Chess", sf::Style::Default);
    window.setVisible(true);
    window.setPosition(sf::Vector2i(200, 200));
    window.setFramerateLimit(60);
    load_textures();
}

void ChessGUI::run()
{
    sf::Event event; // sfml variable that stores most recent user interaction

    while (window.isOpen())
    {
        while (window.pollEvent(event)) // loop to process user event/interaction, if no event, skip
        {
            if (event.type == sf::Event::Closed)
                window.close(); // close window if user clicks the 'X' button

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) // checks for left mouse button press
            {

                int sq =
                    mouse_to_square(event.mouseButton.x,
                                    event.mouseButton.y); // convert mouse coords to square index

                if (sq == -1)
                    continue; // click was outside the board

                Piece p = board.piece_at(sq); // get piece at clicked square

                // --- SELECT PIECE --- (first click)
                if (!pieceSelected) // checks for first piece selection
                {
                    if (p != EMPTY)
                    {
                        // turn based system validation
                        if ((board.white_to_move && p >= BP) ||
                            (!board.white_to_move && p <= WK))
                        {
                            continue; // wrong colour piece
                        }


                        selectedSquare = sq;
                        selectedPiece = p;    // assigns piece type
                        pieceSelected = true; // updates state

                        // highlight moves
                        auto moves = generate_moves(board);

                        highlightedMoves = 0ULL;
                        highlightedAttacks = 0ULL;

                        for (const auto& m : moves)
                        {
                            if (m.from == sq)
                            {
                                if (m.captured == EMPTY || m.is_en_passant)
                                    highlightedMoves |= (1ULL << m.to);
                                else
                                    highlightedAttacks |= (1ULL << m.to);
                            }
                        }

                    }
                    
                }
                // --- MOVE PIECE --- (second click)
                else
                {
                    auto moves = generate_moves(board);

                    for (const auto& m : moves)
                    {
                        if (m.from == selectedSquare && m.to == sq)
                        {
                            board.make_move(m);  // ✅ handles EP, sets en_passant_sq, switches turn
                            break;
                        }
                    }

                    pieceSelected = false;
                    selectedSquare = -1;
                    selectedPiece = EMPTY;
                    highlightedMoves = 0ULL;
                    highlightedAttacks = 0ULL;
                }
            }
        }

        // Draw
        window.clear(sf::Color::White);
        draw_board();
        draw_highlights(); // you can enable this now
        draw_pieces();
        window.display();
    }
}

void ChessGUI::draw_board()
{
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));
            square.setPosition(file * tileSize, (7 - rank) * tileSize);

            bool dark = (rank + file) % 2;
            square.setFillColor(dark ? sf::Color(118, 150, 86) : sf::Color(238, 238, 210));

            window.draw(square);
        }
    }
}



void ChessGUI::draw_pieces()
{
    for (int sq = 0; sq < 64; sq++)
    {
        Piece p = board.piece_at(sq);
        if (p == EMPTY)
            continue;

        sf::Sprite sprite(textures[p]);

        int rank = sq / 8;
        int file = sq % 8;

        sprite.setPosition(file * tileSize - 5, (7 - rank) * tileSize - 5);


        
        window.draw(sprite);
    }
}

int ChessGUI::mouse_to_square(int mouseX, int mouseY)
{
    int file = mouseX / tileSize;
    int rank = 7 - (mouseY / tileSize);

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1;

    return rank * 8 + file;
}

void ChessGUI::draw_highlights()
{
    // grey circles for moves
    sf::CircleShape highlight(tileSize / 2 - 20);
    highlight.setFillColor(sf::Color(100, 100, 200, 100));

    for (int sq = 0; sq < 64; sq++)
    {
        if (highlightedMoves & (1ULL << sq))
        {
            int rank = sq / 8;
            int file = sq % 8;

            highlight.setPosition(file * tileSize + 20, (7 - rank) * tileSize + 20);

            window.draw(highlight);
        }
    }

    // red squares for attacks
    sf::RectangleShape attackHighlight(sf::Vector2f(tileSize, tileSize));
    attackHighlight.setFillColor(sf::Color(255, 0, 0, 100));

    for (int sq = 0; sq < 64; sq++)
    {
        if (highlightedAttacks & (1ULL << sq))
        {
            int rank = sq / 8;
            int file = sq % 8;

            attackHighlight.setPosition(file * tileSize, (7 - rank) * tileSize);
            window.draw(attackHighlight);
        }
    }
}

void ChessGUI::load_textures() {
    std::string colours[2] = {"w", "b"};
    std::string letters[6] = {"P", "N", "B", "R", "Q", "K"};
    for (int p = 1; p <= 12; p++)
    {
        std::string colour = colours[p <= 6 ? 0 : 1];
        std::string letter = letters[(p - 1) % 6];
        textures[p].loadFromFile("pieces/" + colour + letter + ".png");
    }
}
