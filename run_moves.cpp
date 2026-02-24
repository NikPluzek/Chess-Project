#include <iostream>
#include "bitboard.h"
#include "board.h"
#include "movegen.h"

int main()
{
    std::cout << "Starting run_moves test\n";
    init_knight_attacks();
    init_king_attacks();

    Board board;

    // Set up a simple initial position
    board.set_piece(WR, square_index(0, 0));
    board.set_piece(WN, square_index(0, 1));
    board.set_piece(WB, square_index(0, 2));
    board.set_piece(WQ, square_index(0, 3));
    board.set_piece(WK, square_index(0, 4));
    board.set_piece(WB, square_index(0, 5));
    board.set_piece(WN, square_index(0, 6));
    board.set_piece(WR, square_index(0, 7));

    board.set_piece(WP, square_index(1, 0));
    board.set_piece(WP, square_index(1, 1));
    board.set_piece(WP, square_index(1, 2));
    board.set_piece(WP, square_index(1, 3));
    board.set_piece(WP, square_index(1, 4));
    board.set_piece(WP, square_index(1, 5));
    board.set_piece(WP, square_index(1, 6));
    board.set_piece(WP, square_index(1, 7));

    board.set_piece(BR, square_index(7, 0));
    board.set_piece(BN, square_index(7, 1));
    board.set_piece(BB, square_index(7, 2));
    board.set_piece(BQ, square_index(7, 3));
    board.set_piece(BK, square_index(7, 4));
    board.set_piece(BB, square_index(7, 5));
    board.set_piece(BN, square_index(7, 6));
    board.set_piece(BR, square_index(7, 7));

    board.set_piece(BP, square_index(6, 0));
    board.set_piece(BP, square_index(6, 1));
    board.set_piece(BP, square_index(6, 2));
    board.set_piece(BP, square_index(6, 3));
    board.set_piece(BP, square_index(6, 4));
    board.set_piece(BP, square_index(6, 5));
    board.set_piece(BP, square_index(6, 6));
    board.set_piece(BP, square_index(6, 7));

    auto moves = generate_moves(board);

    std::cout << "Generated moves: " << moves.size() << "\n";
    for (auto &m : moves)
    {
        if (m.piece == WP || m.piece == BP)
        {
            std::cout << "Pawn from " << m.from << " to " << m.to << "\n";
        }
    }

    return 0;
}
