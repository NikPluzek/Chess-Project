#include "movegen.h"

std::vector<Move>  generate_moves_pseudolegal(const Board& board)
{
    std::vector<Move> moves;

    uint64_t friendly = board.white_to_move ? board.white_pieces : board.black_pieces;
    uint64_t enemy    = board.white_to_move ? board.black_pieces : board.white_pieces;

    // Handle pawns via pop-lsb iteration for efficiency
    if (board.white_to_move)
    {
        uint64_t pawns = board.white_pawns;
        while (pawns)
        {
            int from = pop_lsb(pawns);
            uint64_t targets = pawn_moves(from, true, board.occupied, enemy) & ~friendly;
            while (targets)
            {
                int to = pop_lsb(targets);
                Move m; m.from = from; m.to = to; m.piece = WP; m.captured = board.piece_at(to);
                moves.push_back(m);
            }
        }
    }
    else
    {
        uint64_t pawns = board.black_pawns;
        while (pawns)
        {
            int from = pop_lsb(pawns);
            uint64_t targets = pawn_moves(from, false, board.occupied, enemy) & ~friendly;
            while (targets)
            {
                int to = pop_lsb(targets);
                Move m; m.from = from; m.to = to; m.piece = BP; m.captured = board.piece_at(to);
                moves.push_back(m);
            }
        }
    }

    // Handle non-pawn pieces by scanning squares (keep existing logic)
    for (int sq = 0; sq < 64; sq++)
    {
        Piece p = board.piece_at(sq);
        if (p == EMPTY) continue;
        // skip pawns (already handled)
        if (p == WP || p == BP) continue;

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

// Check if a square is attacked by pieces of a given side
bool is_square_attacked_by(const Board& board, int target_sq, bool by_white)
{
    // Check pawn attacks
    int sq_rank = target_sq / 8;
    int sq_file = target_sq % 8;
    
    if (by_white) {
        if (sq_rank > 0) {
            if (sq_file > 0 && board.piece_at(square_index(sq_rank - 1, sq_file - 1)) == WP) return true;
            if (sq_file < 7 && board.piece_at(square_index(sq_rank - 1, sq_file + 1)) == WP) return true;
        }
    } else {
        if (sq_rank < 7) {
            if (sq_file > 0 && board.piece_at(square_index(sq_rank + 1, sq_file - 1)) == BP) return true;
            if (sq_file < 7 && board.piece_at(square_index(sq_rank + 1, sq_file + 1)) == BP) return true;
        }
    }
    
    // Check knights
    uint64_t enemy_knights = by_white ? board.pieces[WN] : board.pieces[BN];
    if (knight_attacks[target_sq] & enemy_knights) return true;
    
    // Check king
    uint64_t enemy_king = by_white ? board.pieces[WK] : board.pieces[BK];
    if (king_attacks[target_sq] & enemy_king) return true;
    
    // Check rooks and queens
    uint64_t rook_attackers = rook_attacks(target_sq, board.occupied);
    uint64_t enemy_rooks = by_white ? board.pieces[WR] : board.pieces[BR];
    uint64_t enemy_queens = by_white ? board.pieces[WQ] : board.pieces[BQ];
    if (rook_attackers & (enemy_rooks | enemy_queens)) return true;
    
    // Check bishops and queens
    uint64_t bishop_attackers = bishop_attacks(target_sq, board.occupied);
    uint64_t enemy_bishops = by_white ? board.pieces[WB] : board.pieces[BB];
    if (bishop_attackers & (enemy_bishops | enemy_queens)) return true;
    
    return false;
}

// Check if a king is in check
bool is_in_check(const Board& board, bool is_white)
{
    int king_sq = is_white ? board.white_king_sq : board.black_king_sq;
    
    if (king_sq == -1) return false;
    
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
        temp.make_move(m);
        
        // Check if the side that just moved has their king in check
        // board.white_to_move tells us who was moving
        // After make_move(), temp.white_to_move is the opposite
        // So we check if board.white_to_move's king is safe on the modified board
        if (!is_in_check(temp, board.white_to_move))
        {
            legal.push_back(m);
        }
    }
    
    return legal;
}