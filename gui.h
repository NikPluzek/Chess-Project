#ifndef GUI_H
#define GUI_H

#include "board.h"
#include <SFML/Graphics.hpp>
#include "move.h"

enum ScreenState {
    MENU,
    SIDE_SELECTION,
    PLAYING,
    GAME_OVER
};

enum class GameOutcome {
    Playing,
    Checkmate,
    Stalemate
};

const int WINDOW_WIDTH = 640;   // 8 tiles * 80 pixels
const int WINDOW_HEIGHT = 640;  // 8 tiles * 80 pixels

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

    ScreenState current_state;
    bool player_is_white;

    void render_menu();
    void render_side_selection();
    void render_game();
    void render_game_over();

    void handle_menu_click(int mouse_x, int mouse_y);
    void handle_side_selection_click(int mouse_x, int mouse_y);

    //loading piece PNGs
    sf::Texture textures[13];
    void load_textures();

    //promotion menu
    bool awaiting_promotion = false;
    Move pending_promotion_move;

    void draw_promotion_picker();
    bool handle_promotion_click(int sq);

    //gameover
    GameOutcome gameOutcome = GameOutcome::Playing;
    sf::Font font;
    void draw_game_over();

    //last move highlight
    int from_square = -1;
    int to_square = -1;
};

#endif
