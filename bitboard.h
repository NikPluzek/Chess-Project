#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <iostream>
#include <string>

// Convert rank/file to square index
constexpr int square_index(int rank, int file) { return rank * 8 + file; }

// Bit manipulation helpers
constexpr uint64_t set_bit(int sq) { return 1ULL << sq; }

// Visualize a bitboard
inline void print_bitboard(uint64_t bb)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int sq = square_index(rank, file);
            std::cout << ((bb >> sq) & 1ULL ? "1 " : ". ");
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Knight attacks lookup table
inline uint64_t knight_attacks[64];

// Precompute all knight moves
inline void init_knight_attacks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        uint64_t attacks = 0ULL;
        int rank = sq / 8;
        int file = sq % 8;

        // All possible knight moves
        int moves[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};

        for (auto& m : moves)
        {
            int r = rank + m[0];
            int f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                attacks |= set_bit(square_index(r, f));
        }

        knight_attacks[sq] = attacks;
    }
}

inline uint64_t rook_attacks(int sq, uint64_t occupied)
{
    uint64_t attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // Directions: N, S, E, W

    // North (+ rank)
    for (int r = rank + 1; r < 8; r++)
    {
        int s = r * 8 + file;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // South (- rank)
    for (int r = rank - 1; r >= 0; r--)
    {
        int s = r * 8 + file;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // East (+ file)
    for (int f = file + 1; f < 8; f++)
    {
        int s = rank * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // West (- file)
    for (int f = file - 1; f >= 0; f--)
    {
        int s = rank * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    return attacks;
}

inline uint64_t bishop_attacks(int sq, uint64_t occupied)
{
    uint64_t attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // NE
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++)
    {
        int s = r * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // NW
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--)
    {
        int s = r * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // SE
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++)
    {
        int s = r * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    // SW
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--)
    {
        int s = r * 8 + f;
        attacks |= (1ULL << s);
        if (occupied & (1ULL << s))
            break;
    }

    return attacks;
}

inline uint64_t queen_attacks(int sq, uint64_t occupied)
{
    return rook_attacks(sq, occupied) | bishop_attacks(sq, occupied);
}

inline uint64_t king_attacks[64];

inline void init_king_attacks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        uint64_t attacks = 0ULL;
        int rank = sq / 8;
        int file = sq % 8;

        // All possible king moves
        int moves[8][2] = {
            {1, 1},   {1, 0},  {1, -1}, // above
            {0, -1},  {0, 1},           // adjacent
            {-1, -1}, {-1, 0}, {-1, 1}  // below
        };

        for (auto& m : moves)
        {
            int r = rank + m[0];
            int f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8)
                attacks |= set_bit(square_index(r, f));
        }

        king_attacks[sq] = attacks;
    }
}

#endif
