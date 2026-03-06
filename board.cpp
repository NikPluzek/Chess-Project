#include "board.h"
#include "move.h"
#include "bitboard.h"

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
    en_passant_sq = -1;
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


    occupied &= ~(1ULL << sq);
}

void Board::make_move(const Move& m)
{
    // Step 1: Remove piece from source square
    remove_piece(m.from);
    
    //en passant
    if (m.is_en_passant)
    {
        int captured_pawn_sq = m.to + (white_to_move ? -8 : 8); // capture the pawn behind the destination square
        remove_piece(captured_pawn_sq);
    }
    // Step 2: If there's a piece to capture, remove it
    else if (m.captured != EMPTY)
    {
        remove_piece(m.to);
    }
    
    // Step 3: Place the moving piece at destination
    if (m.promotion == EMPTY)
    {  
        set_piece(m.piece, m.to);  
    }  
    else
    {  
        set_piece(m.promotion, m.to);  
    }
    
    // Step 4: Handle en passant target square
    bool is_pawn = (m.piece == WP || m.piece == BP);
    int rank_diff = (m.to / 8) - (m.from / 8);
    if (is_pawn && (rank_diff == 2 || rank_diff == -2)){
        en_passant_sq = (m.from + m.to) / 2; // the skipped square
    }
    else {
        en_passant_sq = -1; // reset if not a double pawn move
    }

    // step 5: handle castling rights
    if (m.piece == WK){
        WK_moved = true;
    }
    if(m.piece == BK){
        BK_moved = true;
    }
    if (m.piece == WR){
        if (m.from == square_index(0, 0)) WR_Q_moved = true;
        else if (m.from == square_index(0, 7)) WR_K_moved = true;
    }
    if (m.piece == BR){
        if (m.from == square_index(7, 0)) BR_Q_moved = true;
        else if (m.from == square_index(7, 7)) BR_K_moved = true;
    }

    // Handle castling move
    if (m.is_castling)
    {
        if (m.to == 6)  // white kingside
        {
            remove_piece(7);
            set_piece(WR, 5);
        }
        else if (m.to == 2)  // white queenside
        {
            remove_piece(0);
            set_piece(WR, 3);
        }
        else if (m.to == 62)  // black kingside
        {
            remove_piece(63);
            set_piece(BR, 61);
        }
        else if (m.to == 58)  // black queenside
        {
            remove_piece(56);
            set_piece(BR, 59);
        }
    }

    // Step 6: Switch whose turn it is
    white_to_move = !white_to_move;
}

void Board::unmake_move(const Move& m)
{
    white_to_move = !white_to_move; // restore turn first

    remove_piece(m.to);
    set_piece(m.piece, m.from);

    if (m.is_en_passant)
    {
        // Restore the captured pawn on its original square
        int captured_pawn_sq = m.to + (white_to_move ? -8 : 8);
        Piece captured_pawn = white_to_move ? BP : WP; // opponent's pawn
        set_piece(captured_pawn, captured_pawn_sq);
    }
    else if (m.captured != EMPTY)
    {
        set_piece(m.captured, m.to);
    }

    if (m.to == 6)  // white kingside
    {
        remove_piece(5);    
        set_piece(WR, 7);   
    }
    else if (m.to == 2)  // white queenside
    {
        remove_piece(3);
        set_piece(WR, 0);
    }
    else if (m.to == 62)  // black kingside
    {
        remove_piece(61);
        set_piece(BR, 63);
    }
    else if (m.to == 58)  // black queenside
    {
        remove_piece(59);
        set_piece(BR, 56);
    }


    en_passant_sq = m.prev_en_passant_sq; // restore previous EP state
    WK_moved = m.prev_WK_moved;
    WR_K_moved = m.prev_WR_K_moved;
    WR_Q_moved = m.prev_WR_Q_moved;
    BK_moved = m.prev_BK_moved;
    BR_K_moved = m.prev_BR_K_moved;
    BR_Q_moved = m.prev_BR_Q_moved;
}

void load_pieces(Board& board) {

    // Back rank piece order
    Piece back_rank[8] = { WR, WN, WB, WQ, WK, WB, WN, WR };

    for (int file = 0; file < 8; file++)
    {
        // White back rank
        board.set_piece(back_rank[file], square_index(0, file));
        // White pawns
        board.set_piece(WP, square_index(1, file));
        // Black pawns
        board.set_piece(BP, square_index(6, file));
        // Black back rank (same order, just different colour)
        Piece black_piece = (Piece)(back_rank[file] + 6); // offset by 6 to get black equivalent
        board.set_piece(black_piece, square_index(7, file));
    }
}
