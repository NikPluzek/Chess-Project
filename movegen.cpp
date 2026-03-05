#include "movegen.h"

std::vector<Move> generate_moves_pseudolegal(const Board& board)
{
    std::vector<Move> moves;

    uint64_t friendly = board.white_to_move ? board.white_pieces : board.black_pieces;
    uint64_t enemy = board.white_to_move ? board.black_pieces : board.white_pieces;

    // PAWNS 
    if (board.white_to_move)
    {
        uint64_t pawns = board.pieces[WP];
        while (pawns)
        {
            int from = pop_lsb(pawns);
            uint64_t targets = pawn_moves(from, true, board.occupied, enemy) & ~friendly;
            while (targets)
            {
                int to = pop_lsb(targets);
                Move m;
                m.from = from;
                m.to = to;
                m.piece = WP;
                m.captured = board.piece_at(to);

                int to_rank = to / 8;
                if (to_rank == 7)
                {
                    m.promotion = WQ;
                    moves.push_back(m);
                    m.promotion = WR;
                    moves.push_back(m);
                    m.promotion = WN;
                    moves.push_back(m);
                    m.promotion = WB;
                    moves.push_back(m);
                }
                else
                {
                    moves.push_back(m);
                }
            }

            if (board.en_passant_sq != -1)
            {
                uint64_t ep_attacks = pawn_attacks(from, true) & (1ULL << board.en_passant_sq);
                while (ep_attacks)
                {
                    int to = pop_lsb(ep_attacks);
                    Move m;
                    m.from = from;
                    m.to = to;
                    m.piece = WP;
                    m.captured = BP;
                    m.is_en_passant = true;
                    moves.push_back(m);
                }
            }
        }
    }
    else
    {
        uint64_t pawns = board.pieces[BP];
        while (pawns)
        {
            int from = pop_lsb(pawns);
            uint64_t targets = pawn_moves(from, false, board.occupied, enemy) & ~friendly;
            while (targets)
            {
                int to = pop_lsb(targets);
                Move m;
                m.from = from;
                m.to = to;
                m.piece = BP;
                m.captured = board.piece_at(to);

                int to_rank = to / 8;
                if (to_rank == 0)
                {
                    m.promotion = BQ;
                    moves.push_back(m);
                    m.promotion = BR;
                    moves.push_back(m);
                    m.promotion = BN;
                    moves.push_back(m);
                    m.promotion = BB;
                    moves.push_back(m);
                }
                else
                {
                    moves.push_back(m);
                }
            }

            if (board.en_passant_sq != -1)
            {
                uint64_t ep_attacks = pawn_attacks(from, false) & (1ULL << board.en_passant_sq);
                while (ep_attacks)
                {
                    int to = pop_lsb(ep_attacks);
                    Move m;
                    m.from = from;
                    m.to = to;
                    m.piece = BP;
                    m.captured = WP;
                    m.is_en_passant = true;
                    moves.push_back(m);
                }
            }
        }
    }

    // KNIGHTS 
    uint64_t knights = board.white_to_move ? board.pieces[WN] : board.pieces[BN];
    while (knights)
    {
        int sq = pop_lsb(knights);
        uint64_t attackMask = knight_attacks[sq] & ~friendly;
        while (attackMask)
        {
            int to = pop_lsb(attackMask);
            Move m;
            m.from = sq;
            m.to = to;
            m.piece = board.white_to_move ? WN : BN;
            m.captured = board.piece_at(to);
            moves.push_back(m);
        }
    }

    // ROOKS 
    uint64_t rooks = board.white_to_move ? board.pieces[WR] : board.pieces[BR];
    while (rooks)
    {
        int sq = pop_lsb(rooks);
        uint64_t attackMask = rook_attacks(sq, board.occupied) & ~friendly;
        while (attackMask)
        {
            int to = pop_lsb(attackMask);
            Move m;
            m.from = sq;
            m.to = to;
            m.piece = board.white_to_move ? WR : BR;
            m.captured = board.piece_at(to);
            moves.push_back(m);
        }
    }

    // BISHOPS
    uint64_t bishops = board.white_to_move ? board.pieces[WB] : board.pieces[BB];
    while (bishops)
    {
        int sq = pop_lsb(bishops);
        uint64_t attackMask = bishop_attacks(sq, board.occupied) & ~friendly;
        while (attackMask)
        {
            int to = pop_lsb(attackMask);
            Move m;
            m.from = sq;
            m.to = to;
            m.piece = board.white_to_move ? WB : BB;
            m.captured = board.piece_at(to);
            moves.push_back(m);
        }
    }

    // QUEENS 
    uint64_t queens = board.white_to_move ? board.pieces[WQ] : board.pieces[BQ];
    while (queens)
    {
        int sq = pop_lsb(queens);
        uint64_t attackMask = queen_attacks(sq, board.occupied) & ~friendly;
        while (attackMask)
        {
            int to = pop_lsb(attackMask);
            Move m;
            m.from = sq;
            m.to = to;
            m.piece = board.white_to_move ? WQ : BQ;
            m.captured = board.piece_at(to);
            moves.push_back(m);
        }
    }

    // KINGS 
    uint64_t kings = board.white_to_move ? board.pieces[WK] : board.pieces[BK];
    while (kings)
    {
        int sq = pop_lsb(kings);
        uint64_t attackMask = king_attacks[sq] & ~friendly;
        while (attackMask)
        {
            int to = pop_lsb(attackMask);
            Move m;
            m.from = sq;
            m.to = to;
            m.piece = board.white_to_move ? WK : BK;
            m.captured = board.piece_at(to);
            moves.push_back(m);
        }
    }

    return moves;
}

// Check if a square is attacked by pieces of a given side
bool is_square_attacked_by(const Board& board, int target_sq, bool by_white)
{
    // Check pawn attacks
    int sq_rank = target_sq / 8;
    int sq_file = target_sq % 8;

    if (by_white)
    {
        if (sq_rank > 0)
        {
            if (sq_file > 0 && board.piece_at(square_index(sq_rank - 1, sq_file - 1)) == WP)
                return true;
            if (sq_file < 7 && board.piece_at(square_index(sq_rank - 1, sq_file + 1)) == WP)
                return true;
        }
    }
    else
    {
        if (sq_rank < 7)
        {
            if (sq_file > 0 && board.piece_at(square_index(sq_rank + 1, sq_file - 1)) == BP)
                return true;
            if (sq_file < 7 && board.piece_at(square_index(sq_rank + 1, sq_file + 1)) == BP)
                return true;
        }
    }

    // Check knights
    uint64_t enemy_knights = by_white ? board.pieces[WN] : board.pieces[BN];
    if (knight_attacks[target_sq] & enemy_knights)
        return true;

    // Check king
    uint64_t enemy_king = by_white ? board.pieces[WK] : board.pieces[BK];
    if (king_attacks[target_sq] & enemy_king)
        return true;

    // Check rooks and queens
    uint64_t rook_attackers = rook_attacks(target_sq, board.occupied);
    uint64_t enemy_rooks = by_white ? board.pieces[WR] : board.pieces[BR];
    uint64_t enemy_queens = by_white ? board.pieces[WQ] : board.pieces[BQ];
    if (rook_attackers & (enemy_rooks | enemy_queens))
        return true;

    // Check bishops and queens
    uint64_t bishop_attackers = bishop_attacks(target_sq, board.occupied);
    uint64_t enemy_bishops = by_white ? board.pieces[WB] : board.pieces[BB];
    if (bishop_attackers & (enemy_bishops | enemy_queens))
        return true;

    return false;
}

// Check if a king is in check
bool is_in_check(const Board& board, bool is_white)
{
    int king_sq = is_white ? board.white_king_sq : board.black_king_sq;

    if (king_sq == -1)
        return false;

    bool enemy_white = !is_white;
    return is_square_attacked_by(board, king_sq, enemy_white);
}

std::vector<Move> generate_moves(const Board& board)
{
    std::vector<Move> pseudolegal = generate_moves_pseudolegal(board);
    std::vector<Move> legal;

    for (const Move& m : pseudolegal)
    {
        // Make a copy of the board
        Board temp = board;

        // Execute the move on the copy
        Move m_copy = m;
        m_copy.prev_en_passant_sq = board.en_passant_sq; // save it
        temp.make_move(m_copy);

        // Check if the side that just moved has their king in check
        // board.white_to_move tells us who was moving
        // After make_move(), temp.white_to_move is the opposite
        // So we check if board.white_to_move's king is safe on the modified board
        if (!is_in_check(temp, board.white_to_move))
        {
            legal.push_back(m_copy);
        }
    }

    return legal;
}