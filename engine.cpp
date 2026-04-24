#include "engine.h"

int piece_values[13] = {
    0,   // EMPTY
    100, // WP
    300, // WN
    300, // WB
    500, // WR
    900, // WQ
    1000,   // WK
    
    -100, // BP
    -300, // BN
    -300, // BB
    -500, // BR
    -900, // BQ
    -1000     // BK
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

    // white king middle game
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

    // king middle game
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
    // Black pawn (flip and negate white pawn)
    for (int sq = 0; sq < 64; sq++) {
        pst[BP][sq] = -pst[WP][sq ^ 56];
    }
    
    // Black knight (flip white knight)
    for (int sq = 0; sq < 64; sq++) {
        pst[BN][sq] = -pst[WN][sq ^ 56];
    }
    
    // Black bishop (flip white bishop)
    for (int sq = 0; sq < 64; sq++) {
        pst[BB][sq] = -pst[WB][sq ^ 56];
    }
    
    // Black rook (flip white rook)
    for (int sq = 0; sq < 64; sq++) {
        pst[BR][sq] = -pst[WR][sq ^ 56];
    }
    
    // Black queen (flip white queen)
    for (int sq = 0; sq < 64; sq++) {
        pst[BQ][sq] = -pst[WQ][sq ^ 56];
    }
    
    // Black king (flip white king)
    for (int sq = 0; sq < 64; sq++) {
        pst[BK][sq] = -pst[WK][sq ^ 56];
    }
}

int evaluate(const Board& board)
{
    int score = 0;
    for (int i = 1; i <= 12; i++)
    {
        uint64_t pieces = board.pieces[i];
        while (pieces)
        {
            int sq = pop_lsb(pieces);
            score += piece_values[i];  // Material score
            score += pst[i][sq];       // Positional score

            // Penalty for pieces on back rank (simple development bonus)
            int rank = sq / 8;
            if (i == WN || i == WB || i == WR) {  // Added WR
                if (rank == 0) score -= 15;  // White pieces on rank 1
            }
            if (i == BN || i == BB || i == BR) {  // Added BR
                if (rank == 7) score += 15;  // Black pieces on rank 8
            }
        }
    }
    return score;
}

Move get_best_move(Board& board, int depth)
{
    auto moves = generate_moves(board);
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