#ifndef MOVE_H
#define MOVE_H

#include "board.h"

struct Move {
    int from;
    int to;
    Piece piece;
    Piece captured;
    bool is_en_passant = false;
    int prev_en_passant_sq = -1;
};

#endif