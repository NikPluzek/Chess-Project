#include "bitboard.h"
#include "board.h"
#include "gui.h"

int main()
{
    init_knight_attacks();
    init_king_attacks();

    Board board;

    // WHITE PIECES
    board.set_piece(WR, square_index(0, 0)); // rook A1
    board.set_piece(WN, square_index(0, 1)); // knight B1
    board.set_piece(WB, square_index(0, 2)); // bishop C1
    board.set_piece(WQ, square_index(0, 3)); // queen D1
    board.set_piece(WK, square_index(0, 4)); // king E1
    board.set_piece(WB, square_index(0, 5)); // bishop F1
    board.set_piece(WN, square_index(0, 6)); // knight G1
    board.set_piece(WR, square_index(0, 7)); // rook H1

    // BLACK PIECES
    board.set_piece(BR, square_index(7, 0)); // rook A8
    board.set_piece(BN, square_index(7, 1)); // knight B8
    board.set_piece(BB, square_index(7, 2)); // bishop C8
    board.set_piece(BQ, square_index(7, 3)); // queen D8
    board.set_piece(BK, square_index(7, 4)); // king E8
    board.set_piece(BB, square_index(7, 5)); // bishop F8
    board.set_piece(BN, square_index(7, 6)); // knight G8
    board.set_piece(BR, square_index(7, 7)); // rook H8

    ChessGUI gui(board);
    gui.run();

    return 0;
}