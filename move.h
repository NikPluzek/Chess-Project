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
    Piece promotion = EMPTY;
    bool prev_WK_moved = false;
    bool prev_WR_K_moved = false;
    bool prev_WR_Q_moved = false;
    bool prev_BK_moved = false;
    bool prev_BR_K_moved = false;
    bool prev_BR_Q_moved = false;
};

#endif