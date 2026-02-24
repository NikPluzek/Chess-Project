#include "movegen.h"

std::vector<Move> generate_moves(const Board& board)
{
    std::vector<Move> moves;

    uint64_t friendly = board.white_to_move ? board.white_pieces : board.black_pieces;
    uint64_t enemy    = board.white_to_move ? board.black_pieces : board.white_pieces;

    for (int sq = 0; sq < 64; sq++)
    {
        Piece p = board.piece_at(sq);
        if (p == EMPTY) continue;

        // only current side
        if (board.white_to_move && p >= BP) continue;
        if (!board.white_to_move && p <= WK) continue;

        uint64_t attackMask = 0ULL;

        if (p == WN || p == BN)
            attackMask = knight_attacks[sq];
        else if (p == WR || p == BR)
            attackMask = rook_attacks(sq, board.occupied);
        else if (p == WB || p == BB)
            attackMask = bishop_attacks(sq, board.occupied);
        else if (p == WQ || p == BQ)
            attackMask = queen_attacks(sq, board.occupied);
        else if (p == WK || p == BK)
            attackMask = king_attacks[sq];
        else if (p == WP || p == BP)
            attackMask = pawn_moves(
                sq,
                p == WP,
                board.occupied,
                p == WP ? board.black_pieces : board.white_pieces
            );

        attackMask &= ~friendly;

        for (int to = 0; to < 64; to++)
        {
            if (attackMask & (1ULL << to))
            {
                Move m;
                m.from = sq;
                m.to = to;
                m.piece = p;
                m.captured = board.piece_at(to);
                moves.push_back(m);
            }
        }
    }

    return moves;
}