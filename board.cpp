#include "board.h"
#include "move.h"

Board::Board() { clear(); }

void Board::clear()
{
    for (int i = 0; i <= 12; i++)
        pieces[i] = 0ULL;

    for (int i = 0; i < 64; i++)
        squares[i] = EMPTY;

    white_pieces = 0ULL;
    black_pieces = 0ULL;
    occupied = 0ULL;
    white_to_move = true;
}

void Board::update_occupancy()
{
    white_pieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
    black_pieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
    occupied = white_pieces | black_pieces;
}

Piece Board::piece_at(int sq) const
{
    return squares[sq];
}

void Board::set_piece(Piece p, int sq)
{
    pieces[p] |= (1ULL << sq);
    squares[sq] = p;   // ⭐ NEW

    if (p >= WP && p <= WK)
        white_pieces |= (1ULL << sq);
    else if (p >= BP && p <= BK)
        black_pieces |= (1ULL << sq);

    if (p == WP)
        white_pawns |= (1ULL << sq);
    if (p == BP)
        black_pawns |= (1ULL << sq);

    occupied |= (1ULL << sq);

    if (p == WK) white_king_sq = sq;
    if (p == BK) black_king_sq = sq;
}

void Board::remove_piece(int sq)
{
    Piece p = squares[sq];
    if (p == EMPTY) return;

    if (p == WK) white_king_sq = -1;
    if (p == BK) black_king_sq = -1;

    pieces[p] &= ~(1ULL << sq);
    squares[sq] = EMPTY; 

    if (p >= WP && p <= WK)
        white_pieces &= ~(1ULL << sq);
    else if (p >= BP && p <= BK)
        black_pieces &= ~(1ULL << sq);

    if (p == WP)
        white_pawns &= ~(1ULL << sq);
    if (p == BP)
        black_pawns &= ~(1ULL << sq);

    occupied &= ~(1ULL << sq);
}

void Board::make_move(const Move& m)
{
    // Step 1: Remove piece from source square
    remove_piece(m.from);
    
    // Step 2: If there's a piece to capture, remove it
    if (m.captured != EMPTY)
    {
        remove_piece(m.to);
    }
    
    // Step 3: Place the moving piece at destination
    set_piece(m.piece, m.to);
    
    // Step 4: Switch whose turn it is
    white_to_move = !white_to_move;
}

void Board::unmake_move(const Move& m)
{
    // Step 1: Remove the piece from destination
    remove_piece(m.to);
    
    // Step 2: Place it back at source
    set_piece(m.piece, m.from);
    
    // Step 3: If a piece was captured, restore it at destination
    if (m.captured != EMPTY)
    {
        set_piece(m.captured, m.to);
    }
    
    // Step 4: Switch whose turn it is back
    white_to_move = !white_to_move;
}
