#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    uint64_t player1_pieces;
    uint64_t player1_kings;
    uint64_t player2_pieces;
    uint64_t player2_kings;
    int current_turn;
    int middle_target;
} CheckersGame;

// Basic bit operations
unsigned int SetBit(unsigned int value, int position); // Set bit to 1
unsigned int ClearBit(unsigned int value, int position); // Set bit to 0
unsigned int ToggleBit(unsigned int value, int position); // Flip bit
int GetBit(unsigned int value, int position); // Get bit value

// Counting and finding
int CountBits(unsigned int value); // Count 1s

// Shift operations
unsigned int ShiftLeft(unsigned int value, int positions); // Shift left
unsigned int ShiftRight(unsigned int value, int positions); // Shift right

// Display functions
void PrintBinary(unsigned int value); // Print in binary format
void PrintHex(unsigned int value); // Print in hex format

void TestFunctions(); // Debugging function to test all bit manipulation functions

int mapToBitboardIndex(int row, int col);

void StartGame(CheckersGame* game);
void PrintBoard(CheckersGame* game);

int MovePiece(CheckersGame *game, int from, int to, int row);




int main() {

  
  bool TestingFlag = true; // Setting this to false will skip the bit manipulation testing process

  if (TestingFlag) {
    TestFunctions();
  }

  int rowFrom, rowTo, colFrom, colTo, exitInput;
  char colLetterF, colLetterT;

  CheckersGame *game = malloc(sizeof(CheckersGame));
  StartGame(game);
  printf("Welcome to Bitboard Checkers.\n");
  printf("On your turn, input your move by specifying the piece to move and its destination.\n");
  printf("Format: (col_from)(row_from) (col_to)(row_to) [e.g., B2 A3].\n");
  printf("If you wish to exit the game prematurely, type -1 to do so.\n\n");

  while (true) {
    PrintBoard(game);
    printf("Player %d's Turn [%s] ", game->current_turn == 0 ? 1 : 2, game->current_turn == 0 ? "BLACK" : "WHITE");
    if (scanf(" %d", &exitInput) == 1) {
      if (exitInput == -1) {
        printf("You have quit the game.");
        break;
      }
      else {
        printf("Invalid input, please try again\n");
        while (getchar() != '\n');
        continue;
      }
    }

    if (scanf(" %c%d %c%d", &colLetterF, &rowFrom, &colLetterT, &rowTo) == 4) {
      colFrom = colLetterF - 'A';
      colTo = colLetterT - 'A';
      int from = mapToBitboardIndex(rowFrom, colFrom);
      int to = mapToBitboardIndex(rowTo, colTo);

      int moveAttempt = MovePiece(game, from, to, rowTo);
      if (moveAttempt == 1) {
        printf("Movement successful!\n");
        game->current_turn = 1 - game->current_turn;
      }
      else {
        printf("Attempted move is not legal, please try again\n");
        continue;
      }
    }
    else {
      printf("Invalid input, please try again\n");
      while (getchar() != '\n');
      continue;
    }

    if (game->player1_pieces == 0 && game->player1_kings == 0) {
      printf("Game over: Player 2 has won!");
      break;
    }
    else if (game->player2_pieces == 0 && game->player2_kings == 0) {
      printf("Game over: Player 1 has won!");
      break;
    }
  }
}

// Bit Manipulation Test
void TestFunctions() {
  unsigned int test = 0;
  printf("----- BIT MANIPULATION TEST -----\n");
  // Display test value before making modifications
  printf("Initialization: ");
  PrintBinary(test);

  // Test SetBit
  test = SetBit(test, 3); // Should set bit 3
  printf("After setting bit 3: ");
  PrintBinary(test); // Should show: 0000 0000 0000 1000

  // Test CountBits
  printf("Number of 1s: %d\n", CountBits(test)); // Should print 1

  // Test ToggleBit
  test = ToggleBit(test, 3); // Toggle bit 3
  test = ToggleBit(test, 9); // Toggle bit 9
  test = ToggleBit(test, 15); // Toggle bit 15

  printf("After toggling bits 5, 9, and 15: ");
  PrintBinary(test); // Should show: 1000 0010 0000 0000

  // Test ClearBit
  test = ToggleBit(test, 15); // Clear bit 15
  printf("After clearing bit 15: ");
  PrintBinary(test); // Should show: 0000 0010 0000 0000

  // Test PrintHex
  printf("Value in Hex format: ");
  PrintHex(test); // Should show: 200

  // Test ShiftLeft and ShiftRight
  test = ShiftLeft(test, 3); // Shift left by 3
  printf("After shifting left by 3 positions: ");
  PrintBinary(test); // Should show: 0001 0000 0000 0000

  test = ShiftRight(test, 5); // Shift right by 5
  printf("After shifting right by 5 positions: ");
  PrintBinary(test); // Should show: 0000 0000 1000 0000

  // Test GetBit
  int bit_at_pos = GetBit(test, 7);
  printf("Bit value at position 7: %d\n", bit_at_pos); // Should print 1
  printf("----- END TEST -----\n\n");
}
