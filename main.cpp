#include "bitboard.h"
#include "board.h"
#include "gui.h"
#include "engine.h"

int main()
{
    init_knight_attacks();
    init_king_attacks();
    init_pst();

    Board board;

    load_pieces(board);

    ChessGUI gui(board);
    gui.run();

    return 0;
}