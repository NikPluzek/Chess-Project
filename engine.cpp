#include "engine.h"
#include <algorithm>  
#include <cstdlib> 

int piece_values[13] = {
    0,   // EMPTY
    100, // WP
    300, // WN
    300, // WB
    500, // WR
    900, // WQ
    1000,// WK
    
    -100, // BP
    -300, // BN
    -300, // BB
    -500, // BR
    -900, // BQ
    -1000 // BK
};

int pst[13][64] = {
    // EMPTY
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,

    // white pawn
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 30, 30,  0,  0,  0,
    5, -5,-10, 10, 10,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0,

    // white knight
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,

    // whitebishop
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,

    // white rook
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  15, 10, 15,  0,  0,

    // white queen
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,

    // white king
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20,


    // pawn
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0,

    // knight
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,

    // bishop
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,

    // rook
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0,

    // queen
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,

    // king 
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

void init_pst()
{
    // black pieces (flip white pieces)
    for (int sq = 0; sq < 64; sq++) {
        pst[BP][sq] = -pst[WP][sq ^ 56];
    }
    
    for (int sq = 0; sq < 64; sq++) {
        pst[BN][sq] = -pst[WN][sq ^ 56];
    }
    
    for (int sq = 0; sq < 64; sq++) {
        pst[BB][sq] = -pst[WB][sq ^ 56];
    }
    
    for (int sq = 0; sq < 64; sq++) {
        pst[BR][sq] = -pst[WR][sq ^ 56];
    }
    
    for (int sq = 0; sq < 64; sq++) {
        pst[BQ][sq] = -pst[WQ][sq ^ 56];
    }
    
    for (int sq = 0; sq < 64; sq++) {
        pst[BK][sq] = -pst[WK][sq ^ 56];
    }
}

int score_move(const Move& m){
    // implementing MVV-LVA
    if (m.captured != EMPTY){
        int victim_value = std::abs(piece_values[m.captured]);
        int attacker_value = std::abs(piece_values[m.piece]);

        // prioritise valuable victim and weak attacker
        return 10000 + (victim_value *10) - attacker_value;
    }

    if (m.is_en_passant){
        return 10000 + (100 * 10) - 100;  
    }

    if (m.promotion != EMPTY){
        return 9000 + std::abs(piece_values[m.promotion]);
    }

    return 0; // non-capture moves are least valuable
}

int evaluate(const Board& board)
{
    int score = 0;

    //endgame detection
    int white_material = 0;
    int black_material = 0;
    white_material += __builtin_popcountll(board.pieces[WN]) * 300;
    white_material += __builtin_popcountll(board.pieces[WB]) * 300;
    white_material += __builtin_popcountll(board.pieces[WR]) * 500;
    white_material += __builtin_popcountll(board.pieces[WQ]) * 900;
    black_material += __builtin_popcountll(board.pieces[BN]) * 300;
    black_material += __builtin_popcountll(board.pieces[BB]) * 300;
    black_material += __builtin_popcountll(board.pieces[BR]) * 500;
    black_material += __builtin_popcountll(board.pieces[BQ]) * 900;

    bool endgame = (white_material + black_material) <= 2000;

    for (int i = 1; i <= 12; i++)
    {
        uint64_t pieces = board.pieces[i];
        while (pieces)
        {
            int sq = pop_lsb(pieces);
            score += piece_values[i];  //material score
            score += pst[i][sq];       //positional score

            //penalty for pieces on back rank (development bonus)
            int rank = sq / 8;
            if (i == WN || i == WB || i == WR) { 
                if (rank == 0) score -= 15;  //white pieces on rank 1
            }
            if (i == BN || i == BB || i == BR) {  
                if (rank == 7) score += 15;  // black pieces on rank 8
            }

            //pushing pawns in endgame
            if (endgame)
            {
                if (i == WP)
                {
                    //bonus increases the further the pawn has advanced
                    score += rank * 15; 
                }
                if (i == BP)
                {
                    //black pawns advance toward rank 1
                    score -= (7 - rank) * 15;
                }
            }
        }
    }
    return score;
}

Move get_best_move(Board& board, int depth)
{
    auto moves = generate_moves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return score_move(a) > score_move(b);
    });

    Move best_move = moves[0]; // default to first move
    bool is_white = board.white_to_move;
    int best_score = is_white ? -99999 : 99999;

    for (const Move& m : moves)
    {
        board.make_move(m);
        int score = minimax(board, depth - 1, !is_white);
        board.unmake_move(m);
        if (is_white){
            if (score > best_score){
                best_score = score;
                best_move = m;
            }
        }
        else{
            if (score < best_score){
                best_score = score;
                best_move = m;
            }
        }

        // update best_move and best_score if this move is better
    }

    return best_move;
}

int minimax(Board& board, int depth, bool white_turn)
{
    // base case 1: depth reached
    if (depth == 0)
        return evaluate(board);

    // generate moves
    auto moves = generate_moves(board);
    
    // base case 2 & 3: no legal moves
    if (moves.empty())
    {
        if (is_in_check(board, white_turn))
            return white_turn ? -100000 : 100000; // checkmate
        else
            return 0; // stalemate  
    }
    
    if (white_turn)
    {
        int best = -99999;
        for (const Move& m : moves)
        {
            board.make_move(m);
            int score = minimax(board, depth - 1, false);
            board.unmake_move(m);
            if (score > best)
                best = score;
        }
        return best;
    }
    else
    {
        int best = 99999;
        for (const Move& m : moves)
        {
            board.make_move(m);
            int score = minimax(board, depth - 1, true);
            board.unmake_move(m);
            if (score < best)
                best = score;
        }
        return best;
    }
}

// alpha beta pruning 
Move get_best_move_ab(Board& board, int depth)
{
    auto moves = generate_moves(board);
    Move best_move = moves[0]; // default to first move
    bool is_white = board.white_to_move;
    int alpha = -99999;
    int beta = 99999;

    for (const Move& m : moves)
    {
        board.make_move(m);
        int score = minimax_ab(board, depth - 1, !is_white, alpha, beta);
        board.unmake_move(m);
        if (is_white){
            if (score > alpha){
                alpha = score;
                best_move = m;
            }
        }
        else{
            if (score < beta){
                beta = score;
                best_move = m;
            }
        }
    }

    return best_move;
}

int minimax_ab(Board& board, int depth, bool white_turn, int alpha, int beta)
{
    
    // base case 1: depth reached
    if (depth == 0)
        return quiescence(board, alpha, beta, white_turn);

    // generate moves
    auto moves = generate_moves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return score_move(a) > score_move(b);
    });
    
    // base case 2 & 3: no legal moves
    if (moves.empty())
    {
        if (is_in_check(board, white_turn))
            return white_turn ? -100000 : 100000; // checkmate
        else
            return 0; // stalemate  
    }
    
    if (white_turn)
    {
        for (const Move& m : moves)
        {
            board.make_move(m);
            int score = minimax_ab(board, depth-1, false, alpha, beta);
            board.unmake_move(m);

            if (score > alpha)
                alpha = score;
            if (alpha >= beta)
                break;
        }
        return alpha;
    }
    else
    {
        for (const Move& m : moves)
        {
            board.make_move(m);
            int score = minimax_ab(board, depth-1, true, alpha, beta);
            board.unmake_move(m);

            if (score < beta)
                beta = score;
            if (beta <= alpha)
                break;
        }
        return beta;
    }
}

int quiescence(Board& board, int alpha, int beta, bool white_turn, int q_depth){
    
    //safety limit 
    if (q_depth > 6)
        return evaluate(board);
    
    //stand pat score
    int stand_pat = evaluate(board);

    if (white_turn){
        if (stand_pat >= beta)
            return beta;
        if (stand_pat > alpha)
            alpha = stand_pat;
    }
    else{
        if (stand_pat <= alpha)
            return alpha;
        if (stand_pat < beta)
            beta = stand_pat;
    }
    
    auto moves = generate_moves(board);

    std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
        return score_move(a) > score_move(b);
    });

    for (const Move& m : moves){
        if (m.captured == EMPTY && !m.is_en_passant)
            continue;
        
        board.make_move(m);
        int score = quiescence(board, alpha, beta, !white_turn, q_depth + 1);
        board.unmake_move(m);

        if (white_turn)
        {
            if (score > alpha)
                alpha = score;
            if (alpha >= beta)
                return beta;
        }
        else
        {
            if (score < beta)
                beta = score;
            if (beta <= alpha)
                return alpha;
        }
    }

    return white_turn ? alpha : beta;
}