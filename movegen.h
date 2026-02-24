#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "board.h"
#include "bitboard.h"
#include "move.h"

std::vector<Move> generate_moves(const Board& board);

bool is_square_attacked_by(const Board& board, int target_sq, bool by_white);
bool is_in_check(const Board& board, bool is_white);

#endif