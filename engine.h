#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "movegen.h"
#include "move.h"

int evaluate(const Board& board);
void init_pst();

// minimax
int minimax(Board& board, int depth, bool white_turn);
Move get_best_move(Board& board, int depth);

// Alpha-beta pruning
int minimax_ab(Board& board, int depth, bool white_turn, int alpha, int beta);
Move get_best_move_ab(Board& board, int depth);

// move ordering
int score_move(const Move& m);

//quiescence search
int quiescence(Board& board, int alpha, int beta, bool white_turn, int q_depth = 0);

#endif