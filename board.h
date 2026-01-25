#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>

enum Piece {
    EMPTY = 0,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
};

class Board {
public:
    // Individual bitboards
    uint64_t pieces[13];   // WP..BK
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t occupied;

    Board();

    void clear();
    void update_occupancy();

    // Utility
    Piece piece_at(int sq) const;
    void set_piece(Piece p, int sq);
    void remove_piece(int sq);
};

#endif
