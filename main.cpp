#include "bitboard.h"
#include "board.h"
#include "gui.h"

int main() {
    init_knight_attacks();

    Board board;

    // TEMP: test position
    board.set_piece(WR, square_index(3, 3)); // rook d4
    board.set_piece(WP, square_index(5, 3)); // pawn d6
    board.set_piece(BN, square_index(3, 5)); // knight f4

    ChessGUI gui(board);
    gui.run();

    return 0;
}
