#include "gui.h"
#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "movegen.h"

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

                if (awaiting_promotion)
                {
                    handle_promotion_click(sq);
                    continue; // don't process this click as a normal move
                }

                // --- SELECT PIECE --- (first click)
                if (!pieceSelected) // checks for first piece selection
                {
                    if (p != EMPTY)
                    {
                        // turn based system validation
                        if ((board.white_to_move && p >= BP) || (!board.white_to_move && p <= WK))
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
                            if ((m.piece == WP && m.to / 8 == 7) ||
                                (m.piece == BP && m.to / 8 == 0))
                            {
                                // dont make_move yet, wait for promotion choice
                                awaiting_promotion = true;
                                pending_promotion_move = m; // store the queen version for now
                                break;
                            }
                            else
                            {
                                board.make_move(m);
                                auto moves = generate_moves(board);
                                if (moves.empty())
                                {
                                    if (is_in_check(board, board.white_to_move))
                                    {
                                        gameState = GameState::Checkmate;
                                    }
                                    else
                                    {
                                        gameState = GameState::Stalemate;
                                    }
                                }
                                break;
                            }
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
        if (awaiting_promotion)
        {
            draw_promotion_picker();
        }
        if ( gameState != GameState::Playing)
        {
            draw_game_over();
        }
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

void ChessGUI::load_textures()
{
    std::string colours[2] = {"w", "b"};
    std::string letters[6] = {"P", "N", "B", "R", "Q", "K"};
    for (int p = 1; p <= 12; p++)
    {
        std::string colour = colours[p <= 6 ? 0 : 1];
        std::string letter = letters[(p - 1) % 6];
        textures[p].loadFromFile("pieces/" + colour + letter + ".png");
    }
    font.loadFromFile("Roboto-Regular.ttf");
}

void ChessGUI::draw_promotion_picker()
{
    int file = pending_promotion_move.to % 8;
    bool is_white = (pending_promotion_move.piece == WP);

    Piece options[4];
    if (is_white)
    {
        options[0] = WQ;
        options[1] = WR;
        options[2] = WB;
        options[3] = WN;
    }
    else
    {
        options[0] = BQ;
        options[1] = BR;
        options[2] = BB;
        options[3] = BN;
    }

    for (int i = 0; i < 4; i++)
    {
        int rank = is_white ? (7 - i) : i;

        // background square
        sf::RectangleShape bg(sf::Vector2f(tileSize, tileSize));
        bg.setPosition(file * tileSize, (7 - rank) * tileSize);
        bg.setFillColor(sf::Color(200, 200, 200, 230));
        window.draw(bg);

        // piece sprite
        sf::Sprite sprite(textures[options[i]]);
        sprite.setPosition(file * tileSize - 5, (7 - rank) * tileSize - 5);
        window.draw(sprite);
    }
}

bool ChessGUI::handle_promotion_click(int sq)
{
    int file = sq % 8;
    int rank = sq / 8;
    
    if (file != pending_promotion_move.to % 8)
        return false; // click was outside the promotion options

    if (pending_promotion_move.piece == WP)
    {
        if (rank == 7 - 0) // queen
            pending_promotion_move.promotion = WQ;
        else if (rank == 7 - 1) // rook
            pending_promotion_move.promotion = WR;
        else if (rank == 7 - 2) // bishop
            pending_promotion_move.promotion = WB;
        else if (rank == 7 - 3) // knight
            pending_promotion_move.promotion = WN;
        else
            return false;
    }
    else if (pending_promotion_move.piece == BP)
    {
        if (rank == 0) // queen
            pending_promotion_move.promotion = BQ;
        else if (rank == 1) // rook
            pending_promotion_move.promotion = BR;
        else if (rank == 2) // bishop
            pending_promotion_move.promotion = BB;
        else if (rank == 3) // knight
            pending_promotion_move.promotion = BN;
        else
            return false;
    }
    else
        return false;

    board.make_move(pending_promotion_move);
    awaiting_promotion = false;
    return true;
}

void ChessGUI::draw_game_over()
{
    // dark overlay
    sf::RectangleShape overlay(sf::Vector2f(tileSize * 8, tileSize * 8));
    overlay.setPosition(0, 0);
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    // main text
    sf::Text text;
    text.setFont(font);
    text.setString(gameState == GameState::Checkmate ? "Checkmate!" : "Stalemate");
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    text.setPosition(tileSize * 4, tileSize * 3.5f);
    window.draw(text);

    // subtext
    sf::Text sub;
    sub.setFont(font);
    if (gameState == GameState::Checkmate)
        sub.setString(board.white_to_move ? "Black wins!" : "White wins!");
    else
        sub.setString("Draw");
    sub.setCharacterSize(28);
    sub.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect subBounds = sub.getLocalBounds();
    sub.setOrigin(subBounds.width / 2.f, subBounds.height / 2.f);
    sub.setPosition(tileSize * 4, tileSize * 4.5f);
    window.draw(sub);
}