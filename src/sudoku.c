#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

//STATIC FUNCTIONS DECLARATION

/* Returns the number of ones in the number
 */
static int num_ones(uint16_t n);

/* Gets next free space on the board with the least amoung of possibilities
 * and saves the row and coloumn to 'row' and 'col'.
 * Returns true if a square is found else false
 */
static bool get_next_square(sudoku_t const *board, size_t *row, size_t *col);

/*Returns a bitmap of possible value for the given position
 */
static uint16_t get_possible_values(sudoku_t const *board, size_t row, size_t col);

/*Change value on the board and changes the rows_memo, cols_memo and squares_memo variables
 */
static void set_value(sudoku_t *board, size_t row, size_t col, uint8_t value);

/*Remove value from the board and revert changes from rows_memo, cols_memo and squares_memo
 */
static void remove_value(sudoku_t *board, size_t row, size_t col);


//FUNCTION IMPLEMENTATION

void get_board( FILE *infile, sudoku_t *board )
{
	//Zero initialize
	*board = (sudoku_t) {0};

	for (int i=0; i<9; ++i) {
		for (int j=0; j<9; ++j) {
			int c = fgetc(infile);

			//Check if character is a valid digit
			if ( (c - '0') < 0 || (c - '0') > 9 ) {
				fprintf(stderr, "ERROR: invalid character given: %d\n", c);
				exit(EXIT_FAILURE);
			}
			
			set_value( board, i, j , (uint8_t)( c - '0') );
		}
		//Ignore '\n' a the end
		fgetc(infile);
	}
}

void print_board( FILE *outfile, sudoku_t const *board, const bool format)
{
	const int COLS = 25;

	for (int i=0; i<9; i++) {
		if (format) {
			// print horizontal line
			if ( i%3 == 0) {
				for (int i=0; i<COLS; i++) {
					fprintf(outfile, "-");
				}
				fprintf(outfile, "\n");
			}
		}

		for (int j=0; j<9; j++) {
			uint8_t elem = board->board[i*9 + j];

			if (format) {
				// print vertical lines
				if (j%3==0) {
					fprintf(outfile, "| ");
				}

				if (elem==0) {
					fprintf(outfile, "  ");
				} else {
					fprintf(outfile, "%d ", elem);
				}
			} else {
				fprintf(outfile, "%d", elem);
			}
		}

		if (format) {
			fprintf(outfile, "|\n");
		} else {
			fprintf(outfile, "\n");
		}
	}

	// print base horizontal line
	if (format) {
		for ( int i=0; i<COLS; i++ ) {
			fprintf(outfile, "-");
		}
		fprintf(outfile, "\n");
	}
}

static int num_ones( uint16_t n )
{
	// Brian Kernighan's Algorithm
	int c = 0;
	while ( n ) {
		n = n & (n-1);
		c++;
	}
	return c;
}

static bool get_next_square( sudoku_t const *board, size_t *row, size_t *col )
{
	int min_pos = 9; // minimum number of possibilities found
	for (int i=0; i<9; ++i) {
		for (int j=0; j<9; ++j) {
			if ( board->board[i*9 + j] != 0 )
				continue;

			int pos = get_possible_values(board, i, j);
			int num_pos = num_ones(pos);
			// if only one possibility, return immediately
			if (num_pos == 1) {
				*row = i;
				*col = j;
				return true;
			} else if (num_pos < min_pos) {
				*row = i;
				*col = j;
				min_pos = num_pos;
			}
		}	
	}

	// min_pos == 9 indicates nothing has been found
	return (min_pos != 9);
}

static uint16_t get_possible_values( sudoku_t const *board, size_t row, size_t col )
{
	//This function should only be called for non-filled(O) places on the board
	assert( board->board[row*9 + col] == 0 );

	//OR the memos together to get a bitmap of which numbers are taken
	//Flip the bits to get te complement and take only the first 9 bits
	uint16_t possibilities = (uint16_t) 0b111111111 & ~(board->rows_memo[row] | board->cols_memo[col] | board->squares_memo[(row/3)*3 + (col/3)]);

	return possibilities;
}

static void set_value( sudoku_t *board, size_t row, size_t col, uint8_t value )
{
	board->board[row*9 + col] = value;

	//OR the new value onto the bitmaps
	//000100111 | 10000000 = 100100111
	//^ When 1 is added to a row
	if ( value != 0 ) {
		board->rows_memo[row] |= ( 1 << (value-1) );
		board->cols_memo[col] |= ( 1 << (value-1) );
		board->squares_memo[(row/3)*3 + (col/3)] |= ( 1 << (value-1) );
	}
}
 
static void remove_value( sudoku_t *board, size_t row, size_t col )
{
	uint8_t value = board->board[row*9 + col];
	
	board->board[row*9 + col] = 0;

	//Mask the output to get only the desired values
	board->rows_memo[row] &= ( (uint16_t)0b111111111 & ~( 1 << (value-1) ) );
	board->cols_memo[col] &= ( (uint16_t)0b111111111 & ~( 1 << (value-1) ) );
	board->squares_memo[(row/3)*3 + (col/3)] &= ( (uint16_t)0b111111111 & ~( 1 << (value-1) ) );
}

bool solve_board( sudoku_t *board )
{
	size_t row, col;

	while ( get_next_square( board, &row, &col ) ) {
		uint16_t possibilities = get_possible_values( board, row, col );
		for ( int value=1; value<=9; ++value ) {
			// Check if 'value' is a possible value for the current square
			if (  possibilities & 1  ) {
				set_value( board, row, col, value );
				if ( solve_board( board ) )
					return true;
				remove_value( board, row, col );
			}
			// Rightshift possibilities to check next value
			possibilities >>= 1;
		}
		return false;
	}

	return true;
}
