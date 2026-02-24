#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "board.h"
#include "bitboard.h"
#include "move.h"

std::vector<Move> generate_moves(const Board& board);

#endif