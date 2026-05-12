#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstdint>
#include <vector>

enum Piece
{
    EMPTY = 0,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
};

struct Move;

class Board
{
  public:
    // individual bitboards
    uint64_t pieces[13]; // WP..BK
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t occupied;


    int white_king_sq; 
    int black_king_sq;
    int en_passant_sq = -1;

    Board();

    void clear();
    void update_occupancy();

    // Utility
    Piece piece_at(int sq) const;
    Piece squares[64];   
    void set_piece(Piece p, int sq);
    void remove_piece(int sq);
    bool white_to_move;
    void make_move(const Move& m);
    void unmake_move(const Move& m);

    // castling
    bool WK_moved = false;
    bool WR_K_moved = false;
    bool WR_Q_moved = false;

    bool BK_moved = false;
    bool BR_K_moved = false;
    bool BR_Q_moved = false;
    void verify_state();

    // draw by repetition
    std::vector<uint64_t> position_history;  
    
    uint64_t compute_hash() const;  
    void push_history();            
    bool is_threefold_repetition() const; 
    

};

void load_pieces(Board& board);



#endif
