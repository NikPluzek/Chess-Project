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


    //Perft Testing

    // for (int d = 1; d <= 5; d++)
    // {
    //     uint64_t nodes = perft(board, d);
    //     std::cout << "Depth " << d << ": " << nodes << "\n";
    // }

    ChessGUI gui(board);
    gui.run();

    return 0;
}