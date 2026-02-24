#ifndef MOVE_H
#define MOVE_H

#include "board.h"

struct Move {
    int from;
    int to;
    Piece piece;
    Piece captured;
};

#endif