#include "board.h"

Board::Board() {
    clear();
}

void Board::clear() {
    for (int i = 0; i < 12; i++)
        pieces[i] = 0ULL;

    white_pieces = 0ULL;
    black_pieces = 0ULL;
    occupied = 0ULL;
}

void Board::update_occupancy() {
    white_pieces = pieces[WP] | pieces[WN] | pieces[WB] | pieces[WR] | pieces[WQ] | pieces[WK];
    black_pieces = pieces[BP] | pieces[BN] | pieces[BB] | pieces[BR] | pieces[BQ] | pieces[BK];
    occupied = white_pieces | black_pieces;
}

Piece Board::piece_at(int sq) const {
    uint64_t mask = 1ULL << sq;

    for (int p = WP; p <= BK; p++) {
        if (pieces[p] & mask)
            return static_cast<Piece>(p);
    }

    return EMPTY;
}

void Board::set_piece(Piece p, int sq) {
    pieces[p] |= (1ULL << sq);
    update_occupancy();
}

void Board::remove_piece(int sq) {
    uint64_t mask = ~(1ULL << sq);

    for (int p = WP; p <= BK; p++) {
        pieces[p] &= mask;
    }

    update_occupancy();
}
