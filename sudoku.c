#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

sudoku_t get_puzzle( int n )
{
	//Zero initialize
	sudoku_t puzzle = {0};

	if (n < 1 || n > 50) {
		fprintf(stderr, "n must be a number from 1 to 50\n");
		exit(EXIT_FAILURE);
	}

	FILE* file = fopen("sudoku.txt", "r");
	if (file == NULL) {
		fprintf(stderr, "Cannot open sudoku.txt: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	fseek(file, 98*(n-1)+8, SEEK_SET);

	for (int i=0; i<9; ++i) {
		for (int j=0; j<9; ++j) {
			set_value( &puzzle, i, j , fgetc(file) - '0' );
		}
		//Ignore '\n' a the end
		fgetc(file);
	}

	return puzzle;
}

void print_board( sudoku_t *puzzle )
{
	const int COLS = 25;
	for (int i=0; i<9; i++) {
		// print horizontal line
		if (i%3==0) {
			for (int i=0; i<COLS; i++) {
				printf("-");
			}
			printf("\n");
		}
    
		for (int j=0; j<9; j++) {
			uint8_t elem = puzzle->board[i][j];
			// print vertical lines
			if (j%3==0) {
				printf("| ");
			}
      
			if (elem==0) {
				printf("  ");
			} else {
				printf("%d ", puzzle->board[i][j]);
			}
		}
		printf("|\n");
	}
  
	// print base horizontal line
	for ( int i=0; i<COLS; i++ ) {
		printf("-");
	}
	printf("\n");
}

bool get_next_free( sudoku_t *puzzle, size_t *row, size_t *col )
{
	for (int i=0; i<9; ++i) {
		for (int j=0; j<9; ++j) {
			if ( ( puzzle->board[i][j] == 0 )) {
				*row = i;
				*col = j;
				return true;
			}
		}	
	}

	return false;
}

uint16_t get_possible_values( sudoku_t *puzzle, size_t row, size_t col )
{
	//This function should only be called for non-filled(O) places on the board
	assert( puzzle->board[row][col] == 0 );

	//OR the memos together to get a bitmap of which numbers are taken
	//Flip the bits to get te complement and take only the first 9 bits
	uint16_t possibilities = (uint16_t)0b111111111 & ~(puzzle->rows_memo[row] | puzzle->cols_memo[col] | puzzle->squares_memo[(row/3)*3 + (col/3)]);

	return possibilities;
}

void set_value( sudoku_t *puzzle, size_t row, size_t col, uint8_t value )
{
	puzzle->board[row][col] = value;

	//OR the new value onto the bitmaps
	//000100111 | 10000000 = 100100111
	//^ When 1 is added to a row
	if ( value != 0 ) {
		puzzle->rows_memo[row] |= ( 1 << (value-1) );
		puzzle->cols_memo[col] |= ( 1 << (value-1) );
		puzzle->squares_memo[(row/3)*3 + (col/3)] |= ( 1 << (value-1) );
	}
}
 
void remove_value( sudoku_t *puzzle, size_t row, size_t col )
{
	uint8_t value = puzzle->board[row][col];
	
	puzzle->board[row][col] = 0;

	//Mask the output to get only the desired values
	puzzle->rows_memo[row] &= ((uint16_t)0b111111111 & ~( 1 << (value-1) ));
	puzzle->cols_memo[col] &= ((uint16_t)0b111111111 & ~( 1 << (value-1) ));
	puzzle->squares_memo[(row/3)*3 + (col/3)] &= ((uint16_t)0b111111111 & ~( 1 << (value-1) ));
}

bool solve_puzzle( sudoku_t *puzzle )
{
	size_t row, col;

	while ( get_next_free( puzzle, &row, &col ) ) {
		uint16_t possibilities = get_possible_values( puzzle, row, col );
		for ( int value=1; value<=9; ++value ) {
			// Check if 'value' is a possible value for the current square
			if ( ( (possibilities >> (value-1)) & 1 ) == 1 ) {
				set_value( puzzle, row, col, value );
				if ( solve_puzzle( puzzle ) ) {
					return true;
				} else {
					remove_value( puzzle, row, col );
				}
			}
		}
		return false;
	}

	return true;
}

