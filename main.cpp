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

    board.set_piece(WP, square_index(1, 0)); // pawn A2
    board.set_piece(WP, square_index(1, 1)); // pawn B2
    board.set_piece(WP, square_index(1, 2)); // pawn C2
    board.set_piece(WP, square_index(1, 3)); // pawn D2
    board.set_piece(WP, square_index(1, 4)); // pawn E2
    board.set_piece(WP, square_index(1, 5)); // pawn F2
    board.set_piece(WP, square_index(1, 6)); // pawn G2
    board.set_piece(WP, square_index(1, 7)); // pawn H2

    // BLACK PIECES
    board.set_piece(BR, square_index(7, 0)); // rook A8
    board.set_piece(BN, square_index(7, 1)); // knight B8
    board.set_piece(BB, square_index(7, 2)); // bishop C8
    board.set_piece(BQ, square_index(7, 3)); // queen D8
    board.set_piece(BK, square_index(7, 4)); // king E8
    board.set_piece(BB, square_index(7, 5)); // bishop F8
    board.set_piece(BN, square_index(7, 6)); // knight G8
    board.set_piece(BR, square_index(7, 7)); // rook H8

    board.set_piece(BP, square_index(6, 0)); // pawn A7
    board.set_piece(BP, square_index(6, 1)); // pawn B7
    board.set_piece(BP, square_index(6, 2)); // pawn C7
    board.set_piece(BP, square_index(6, 3)); // pawn D7
    board.set_piece(BP, square_index(6, 4)); // pawn E7
    board.set_piece(BP, square_index(6, 5)); // pawn F7
    board.set_piece(BP, square_index(6, 6)); // pawn G7
    board.set_piece(BP, square_index(6, 7)); // pawn H7

    ChessGUI gui(board);
    gui.run();

    return 0;
}