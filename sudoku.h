#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Sudoku board type 
 */
typedef struct {
	//Representation of the board
	uint8_t board[9][9];

	//Bitmap of values allowed on each row, column and square
	uint16_t rows_memo[9], cols_memo[9], squares_memo[9];
} sudoku_t;

/* Get the nth board from sudoku.txt
 */
sudoku_t get_puzzle(int n);

/*Print the board to stdout
 */
void print_board(sudoku_t *puzzle);

/*Gets next free space on the board and saves the row and coloumn to 'row'
 * and 'col'. Returns true if found else false
 */
bool get_next_free(sudoku_t *puzzle, size_t *row, size_t *col);

/*Returns a bitmap of possible value for the given position
 */
uint16_t get_possible_values(sudoku_t *puzzle, size_t row, size_t col);

/*Change value on the board and changes the rows_memo, cols_memo and squares_memo variables
 */
void set_value(sudoku_t *puzzle, size_t row, size_t col, uint8_t value);

/*Remove value from the board and revert changes from rows_memo, cols_memo and squares_memo
 */
void remove_value(sudoku_t *puzzle, size_t row, size_t col);

/*Solve the puzzle recursively
 *Return true if solved, false if not
 */
bool solve_puzzle(sudoku_t *puzzle);

#endif // SUDOKU_H
