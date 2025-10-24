#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    uint64_t player1_pieces;
    uint64_t player1_kings;
    uint64_t player2_pieces;
    uint64_t player2_kings;
    int current_turn;
    int middle_target;
} CheckersGame;

int isLegalMove(CheckersGame *game, int from, int to);

unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;

    return value | (1 << position);
}

unsigned int ClearBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;

    return value & ~(1 << position);
}

unsigned int ToggleBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;

    return value ^ (1 << position);
}

int GetBit(unsigned int value, int position) {
    return (value >> position) & 1ULL;
}

int CountBits(unsigned int value) {
    int count = 0;
    while (value) {
        value &= (value - 1);
        count++;
    }

    return count;
}

unsigned int ShiftLeft(unsigned int value, int positions) {
    return value << positions;
}

unsigned int ShiftRight(unsigned int value, int positions) {
    return value >> positions;
}

void PrintBinary(unsigned int value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);

        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void PrintHex(unsigned int value) {
    printf("%X \n", value);
}

int mapToBitboardIndex(int row, int col) {
    // Only black squares are used, so we map (row, col) to a 0-31 index
    if ((row + col) % 2 == 0) {
        return -1;  // Invalid for white squares
    }
    return (row * 4) + (col / 2);
}

void StartGame(CheckersGame* game) {
    game->player1_pieces = 0x00000FFF;
    game->player1_kings = 0;
    game->player2_pieces = 0xFFF00000;
    game->player2_kings = 0;

    game->current_turn = 0;
    game->middle_target = 0;
}

void PrintBoard(CheckersGame* game) {
  printf("Current Board:\n");
  printf("   A  B  C  D  E  F  G  H\n  -----------------------\n");
    for (int row = 0; row < 8; row++) {
        printf("%d|", (row));
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 0) {
                // White square
                printf(" . ");
            } else {
                // Black square
                int index = mapToBitboardIndex(row, col);
                uint32_t mask = 1 << index;
                if (game->player1_pieces & mask) {
                    printf(" b ");
                } else if (game->player2_pieces & mask) {
                    printf(" w ");
                } else if (game->player1_kings & mask) {
                    printf(" B ");
                } else if (game->player2_kings & mask) {
                    printf(" W ");
                } else {
                    printf(" . ");
                }
            }
        }
        printf("\n");
    }
}

int MovePiece(CheckersGame *game, int from, int to, int row) {
    int moveType = isLegalMove(game, from, to);

    // BLACK MOVEMENT
    if (game->current_turn == 0) {
        switch (moveType) {
            case 0:
                return 0;
            break;

            case 1:
                if (GetBit(game->player1_pieces, from)) {
                    game->player1_pieces = ClearBit(game->player1_pieces, from);
                    game->player1_pieces = SetBit(game->player1_pieces, to);
                    if (row == 7) {
                        game->player1_pieces = ClearBit(game->player1_pieces, from);
                        game->player1_kings = SetBit(game->player1_kings, to);
                    }
                    else {
                        game->player1_pieces = SetBit(game->player1_pieces, to);
                    }
                    return 1;
                }
                else if (GetBit(game->player1_kings, from)) {
                    game->player1_kings = ClearBit(game->player1_kings, from);
                    game->player1_kings = SetBit(game->player1_kings, to);
                    return 1;
                }
            break;

            case 2:
                if (GetBit(game->player1_pieces, from)) {
                    game->player1_pieces = ClearBit(game->player1_pieces, from);
                    game->player1_kings = ClearBit(game->player1_kings, from);
                    game->player2_pieces = ClearBit(game->player2_pieces, game->middle_target);
                    game->player2_kings = ClearBit(game->player2_kings, game->middle_target);
                    game->player1_pieces = SetBit(game->player1_pieces, to);
                    if (row == 7) {
                        game->player1_pieces = ClearBit(game->player1_pieces, to);
                        game->player1_kings = SetBit(game->player1_kings, to);
                    }
                    return 1;
                }
                else if (GetBit(game->player1_kings, from)) {
                    game->player1_kings = ClearBit(game->player1_kings, from);
                    game->player2_pieces = ClearBit(game->player2_pieces, game->middle_target);
                    game->player2_kings = ClearBit(game->player2_kings, game->middle_target);
                    game->player1_kings = SetBit(game->player1_kings, to);
                    return 1;
                }
            break;
            
            // For debugging purposes, the isLegalMove() function returns varying integer values to denote which legality check failed.
            // Values of 1 and 2 are considered valid [normal and capturing respectively], while everything else is considered invalid.
            default:
                printf("Invalid move [%d]\n", moveType);
                return 0;
            break;
        }
        
    // WHITE MOVEMENT
    } else {
        switch (moveType) {
            case 0:
                return 0;
            break;

            case 1:
                if (GetBit(game->player2_pieces, from)) {
                    game->player2_pieces = ClearBit(game->player2_pieces, from);
                    game->player2_pieces = SetBit(game->player2_pieces, to);
                    if (row == 0) {
                        game->player2_pieces = ClearBit(game->player2_pieces, to);
                        game->player2_kings = SetBit(game->player2_kings, to);
                    }
                    return 1;
                }
                else if (GetBit(game->player2_kings, from)) {
                    game->player2_kings = ClearBit(game->player2_kings, from);
                    game->player2_kings = SetBit(game->player2_kings, to);
                    return 1;
                }
            break;

            case 2:
                if (GetBit(game->player2_pieces, from)) {
                    game->player2_pieces = ClearBit(game->player2_pieces, from);
                    game->player2_kings = ClearBit(game->player2_kings, from);
                    game->player1_pieces = ClearBit(game->player1_pieces, game->middle_target);
                    game->player1_kings = ClearBit(game->player1_kings, game->middle_target);
                    game->player2_pieces = SetBit(game->player2_pieces, to);
                    if (row == 0) {
                        game->player2_pieces = ClearBit(game->player2_pieces, to);
                        game->player2_kings = SetBit(game->player2_kings, to);
                    }
                    return 1;
                }
                else if (GetBit(game->player2_kings, from)) {
                    game->player1_kings = ClearBit(game->player1_kings, from);
                    game->player1_pieces = ClearBit(game->player1_pieces, game->middle_target);
                    game->player1_kings = ClearBit(game->player1_kings, game->middle_target);
                    game->player2_kings = SetBit(game->player2_kings, to);
                    return 1;
                }
            break;
            
            // For debugging purposes, the isLegalMove() function returns varying integer values to denote which legality check failed.
            // Values of 1 and 2 are considered valid [normal and capturing respectively], while everything else is considered invalid.
            default:
                printf("Invalid move [%d]\n", moveType);
                return 0;
            break;
        }
    }
}

int isLegalMove(CheckersGame *game, int from, int to) {
    switch (game->current_turn)
    {
        // PLAYER 1 [BLACK]
        case 0:
            // Check if the specified piece is valid
            if (!(game->player1_pieces & (1 << from))) {
                if (!(game->player1_kings & (1 << from))) {
                    return 0;
                }
            }

            // Check if the destination is vacant
            if ((game->player1_pieces & (1 << to)) || (game->player2_pieces & (1 << to) || game->player1_kings & (1 << to)) || (game->player2_kings & (1 << to))) return 4;

            int rowDiff = (to / 4) - (from / 4);
            int colDiff = (to % 4) - (from % 4);

            // Normal move check
            if (abs(rowDiff) == 1) {
                if (!(game->player1_kings & (1 << from))) {
                    if (rowDiff != 1) return 6;
                }
                return 1;  // Valid normal move
            }

            // Capturing move check
            if (abs(rowDiff) == 2) {
                game->middle_target = from + (to - from) / 2;
                if (!(game->player2_pieces & (1 << game->middle_target))) {
                    if (!(game->player2_kings & (1 << game->middle_target))) {
                        return 8;
                    }
                    return 7;
                }
                return 2;  // Valid capture move
            }
        break;
        
        // PLAYER 1 [BLACK]
        case 1:
            // Check if the specified piece is valid
            if (!(game->player2_pieces & (1 << from))) {
                if (!(game->player2_kings & (1 << from))) {
                    return 8;
                }
            }

            // Check if the destination is vacant
            if ((game->player2_pieces & (1 << to)) || (game->player1_pieces & (1 << to) || game->player2_kings & (1 << to)) || (game->player1_kings & (1 << to))) return 10;

            rowDiff = (to / 4) - (from / 4);
            colDiff = (to % 4) - (from % 4);

            // Normal move check
            if (abs(rowDiff) == 1) {
                if (!(game->player2_kings & (1 << from))) {
                    if (rowDiff != -1) return 12;
                }
                return 1;  // Valid normal move
            }

            // Capturing move check
            if (abs(rowDiff) == 2) {
                game->middle_target = from + (to - from) / 2;
                if (!(game->player1_pieces & (1 << game->middle_target))) {
                    if (!(game->player1_kings & (1 << game->middle_target))) {
                        return 13;
                    }
                    return 14;
                }
                return 2;  // Valid capture move
            }
        break;
    }

    // Default invalid case
    return 16;
}