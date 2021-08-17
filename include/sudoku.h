#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Sudoku board type 
 */
typedef struct {
	//Representation of the board
	uint8_t board[9*9];

	//Bitmap of values allowed on each row, column and square
	uint16_t rows_memo[9], cols_memo[9], squares_memo[9];
} sudoku_t;

/* Get the nth board from sudoku.txt and store in into the address provided by board
 * Exits the program and prints error to stderr if error was encountered
 */
void get_board( FILE *infile, sudoku_t *board );

/*Print the board to stdout
 */
void print_board( FILE *outfile, sudoku_t const *board, bool format );

/*Solve the puzzle recursively
 *Return true if solved, false if not
 */
bool solve_board( sudoku_t *board );

#endif // SUDOKU_H
