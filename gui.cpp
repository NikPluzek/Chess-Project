#include "gui.h"
#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "engine.h"
#include <chrono>
#include <iostream>

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    sf::FloatRect bounds;
    
    Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, sf::Font& font)
    {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(70, 70, 70));
        shape.setOutlineThickness(3);
        shape.setOutlineColor(sf::Color(40, 40, 40));
        
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
        text.setPosition(position.x + size.x / 2, position.y + size.y / 2 - 5);
        
        bounds = shape.getGlobalBounds();
    }
    
    void setHovered(bool hovered)
    {
        if (hovered)
            shape.setFillColor(sf::Color(100, 100, 100));
        else
            shape.setFillColor(sf::Color(70, 70, 70));
    }
    
    bool contains(int x, int y)
    {
        return bounds.contains(x, y);
    }
    
    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
        window.draw(text);
    }
};

ChessGUI::ChessGUI(Board& b) : board(b)
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess Game");

    current_state = MENU;
    player_is_white = true;

    window.setVisible(true);
    window.setPosition(sf::Vector2i(200, 200));
    window.setFramerateLimit(60);
    load_textures();
}

void ChessGUI::run()
{
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                if (current_state == MENU){
                    handle_menu_click(event.mouseButton.x, event.mouseButton.y);
                    continue;
                }
                else if (current_state == SIDE_SELECTION){
                    handle_side_selection_click(event.mouseButton.x, event.mouseButton.y);
                }
                else if (current_state == PLAYING){

                    int sq = mouse_to_square(event.mouseButton.x, event.mouseButton.y);


                    if (sq == -1)
                        continue;

                    Piece p = board.piece_at(sq);

                    if (awaiting_promotion)
                    {
                        handle_promotion_click(sq);
                        continue;
                    }

                    // --- SELECT PIECE --- (first click)
                    if (!pieceSelected)
                    {
                        if (p != EMPTY)
                        {
                            if ((board.white_to_move && p >= BP) || (!board.white_to_move && p <= WK))
                                continue;

                            selectedSquare = sq;
                            selectedPiece = p;
                            pieceSelected = true;

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
                                    awaiting_promotion = true;
                                    pending_promotion_move = m;
                                    break;
                                }
                                else
                                {
                                    board.make_move(m);
                                    board.push_history();
                                    from_square = m.from;
                                    to_square = m.to;

                                    // check endgame after human move
                                    auto next_moves = generate_moves(board);
                                    if (next_moves.empty())
                                    {
                                        if (is_in_check(board, board.white_to_move))
                                            gameOutcome = GameOutcome::Checkmate;
                                        else
                                            gameOutcome = GameOutcome::Stalemate;
                                    }
                                    else if (board.is_threefold_repetition())  
                                    {
                                        gameOutcome = GameOutcome::Draw;
                                    }

                                    // engine move
                                    bool engine_turn = (board.white_to_move != player_is_white);
                                    if (gameOutcome == GameOutcome::Playing && engine_turn)
                                    {
                                        auto start = std::chrono::high_resolution_clock::now();
                                        Move engine_move = get_best_move_ab(board, 5);
                                        auto end = std::chrono::high_resolution_clock::now();
                                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                                        std::cout << "Engine move time: " << duration.count() << " ms" << std::endl;
                                        // move times printed


                                        board.make_move(engine_move);
                                        board.push_history();
                                        from_square = engine_move.from;
                                        to_square = engine_move.to;


                                        auto engine_next = generate_moves(board);
                                        if (engine_next.empty())
                                        {
                                            if (is_in_check(board, board.white_to_move))
                                                gameOutcome = GameOutcome::Checkmate;
                                            else
                                                gameOutcome = GameOutcome::Stalemate;
                                        }
                                    }
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
        }

        // draw/render 
        window.clear(sf::Color::White);

        if(current_state == MENU){
            render_menu();
        }
        else if (current_state == SIDE_SELECTION){
            render_side_selection();
        }
        else if (current_state == PLAYING){
            draw_board();
            draw_highlights();
            draw_pieces();
            if (awaiting_promotion)
                draw_promotion_picker();
            if (gameOutcome != GameOutcome::Playing)
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
            square.setFillColor(dark ? sf::Color(238, 238, 210) : sf::Color(118, 150, 86));
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
    // last move highlight (drawn first, underneath everything)
    if (from_square != -1 && to_square != -1)
    {
        sf::RectangleShape moveHighlight(sf::Vector2f(tileSize, tileSize));
        moveHighlight.setFillColor(sf::Color(255, 255, 0, 100));

        moveHighlight.setPosition((from_square % 8) * tileSize, (7 - from_square / 8) * tileSize);
        window.draw(moveHighlight);

        moveHighlight.setPosition((to_square % 8) * tileSize, (7 - to_square / 8) * tileSize);
        window.draw(moveHighlight);
    }

    //grey circles for moves
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

    //red squares for attacks
    sf::RectangleShape attackHighlight(sf::Vector2f(tileSize, tileSize));
    attackHighlight.setFillColor(sf::Color(155, 0, 0, 255));

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

        sf::RectangleShape bg(sf::Vector2f(tileSize, tileSize));
        bg.setPosition(file * tileSize, (7 - rank) * tileSize);
        bg.setFillColor(sf::Color(200, 200, 200, 230));
        window.draw(bg);

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
        return false;

    if (pending_promotion_move.piece == WP)
    {
        if (rank == 7) pending_promotion_move.promotion = WQ;
        else if (rank == 6) pending_promotion_move.promotion = WR;
        else if (rank == 5) pending_promotion_move.promotion = WB;
        else if (rank == 4) pending_promotion_move.promotion = WN;
        else return false;
    }
    else if (pending_promotion_move.piece == BP)
    {
        if (rank == 0) pending_promotion_move.promotion = BQ;
        else if (rank == 1) pending_promotion_move.promotion = BR;
        else if (rank == 2) pending_promotion_move.promotion = BB;
        else if (rank == 3) pending_promotion_move.promotion = BN;
        else return false;
    }
    else
        return false;

    board.make_move(pending_promotion_move);
    board.push_history();
    from_square = pending_promotion_move.from;
    to_square = pending_promotion_move.to;
    awaiting_promotion = false;

    // check endgame after promotion
    auto next_moves = generate_moves(board);
    if (next_moves.empty())
    {
        if (is_in_check(board, board.white_to_move))
            gameOutcome = GameOutcome::Checkmate;
        else
            gameOutcome = GameOutcome::Stalemate;
    }

    // engine move after promotion
    bool engine_turn = (board.white_to_move != player_is_white);
    if (gameOutcome == GameOutcome::Playing && engine_turn)
    {
        Move engine_move = get_best_move_ab(board, 5);
        board.make_move(engine_move);
        board.push_history();
        from_square = engine_move.from;
        to_square = engine_move.to;

        auto engine_next = generate_moves(board);
        if (engine_next.empty())
        {
            if (is_in_check(board, board.white_to_move))
                gameOutcome = GameOutcome::Checkmate;
            else
                gameOutcome = GameOutcome::Stalemate;
        }
    }

    return true;
}

void ChessGUI::draw_game_over()
{
    sf::RectangleShape overlay(sf::Vector2f(tileSize * 6, tileSize * 4));
    overlay.setPosition(tileSize, tileSize * 2);
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    sf::Text text;
    text.setFont(font);
    std::string result;
    if (gameOutcome == GameOutcome::Checkmate)
        result = "Checkmate!";
    else if (gameOutcome == GameOutcome::Stalemate)
        result = "Stalemate!";
    else if (gameOutcome == GameOutcome::Draw)
        result = "Draw!";
    text.setString(result);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    text.setPosition(tileSize * 4, tileSize * 3.5f);
    window.draw(text);

    sf::Text sub;
    sub.setFont(font);
    if (gameOutcome == GameOutcome::Checkmate)
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

void ChessGUI::render_menu()
{
    //background gradient effect (two rectangles)
    sf::RectangleShape bg1(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg1.setPosition(0, 0);
    bg1.setFillColor(sf::Color(240, 240, 240));
    window.draw(bg1);
    
   
    
    //title
    sf::Text title;
    title.setFont(font);
    title.setString("Chess");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color(40, 40, 40));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    title.setPosition(WINDOW_WIDTH / 2, 120);
    window.draw(title);
    
    //subtitle
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("AI Chess Engine By Nikodem Pluzek");
    subtitle.setCharacterSize(24);
    subtitle.setFillColor(sf::Color(100, 100, 100));
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setOrigin(subBounds.width / 2.f, subBounds.height / 2.f);
    subtitle.setPosition(WINDOW_WIDTH / 2, 190);
    window.draw(subtitle);
    
    //new game button
    Button newGame("Play Game", sf::Vector2f(220, 300), sf::Vector2f(200, 70), font);
    
    //get mouse position for hover effect
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    newGame.setHovered(newGame.contains(mousePos.x, mousePos.y));
    
    newGame.draw(window);
}

void ChessGUI::handle_menu_click(int mouse_x, int mouse_y){
    current_state = SIDE_SELECTION;
}

void ChessGUI::render_side_selection()
{
    //background
    sf::RectangleShape bg(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    bg.setFillColor(sf::Color(230, 230, 230));
    window.draw(bg);
    
    //title
    sf::Text title;
    title.setFont(font);
    title.setString("Choose Your Colour");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color(40, 40, 40));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    title.setPosition(WINDOW_WIDTH / 2, 80);
    window.draw(title);
    
    //white button (left side)
    Button whiteBtn("Play as White", sf::Vector2f(60, 250), sf::Vector2f(250, 250), font);
    
    //black button (right side)  
    Button blackBtn("Play as Black", sf::Vector2f(340, 250), sf::Vector2f(250, 250), font);
    
    //hover effects
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    whiteBtn.setHovered(whiteBtn.contains(mousePos.x, mousePos.y));
    blackBtn.setHovered(blackBtn.contains(mousePos.x, mousePos.y));
    
    //draw buttons
    whiteBtn.draw(window);
    blackBtn.draw(window);
    
    //draw piece icons on buttons 
    if (textures[WK].getSize().x > 0)
    {
        sf::Sprite whiteKing(textures[WK]);
        whiteKing.setPosition(140, 280);
        window.draw(whiteKing);
        
        sf::Sprite blackKing(textures[BK]);
        blackKing.setPosition(420, 280);
        window.draw(blackKing);
    }
}

void ChessGUI::handle_side_selection_click(int mouse_x, int mouse_y)
{
    //left half = white, right half = black
    if (mouse_x < WINDOW_WIDTH / 2)
    {
        player_is_white = true;
    }
    else
    {
        player_is_white = false;
    }
    
    current_state = PLAYING;

    if (!player_is_white){
        Move engine_move = get_best_move_ab(board, 5);
        board.make_move(engine_move);
        board.push_history();
        from_square = engine_move.from;
        to_square = engine_move.to;
    }
}

