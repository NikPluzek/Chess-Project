#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <cstdint>

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
    // Individual bitboards
    uint64_t pieces[13]; // WP..BK
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t occupied;
    uint64_t white_pawns;
    uint64_t black_pawns;

    int white_king_sq; 
    int black_king_sq;

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

  };

#endif
