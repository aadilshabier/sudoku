#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>

#define NOPRINT

int main(int argc, char* argv[])
{
	sudoku_t board;
	int n;
	if (argc==2) {
		sscanf(argv[1], "%d", &n);
	} else {
		n = 1;
	}

	board = get_puzzle(n);

#ifndef NOPRINT
	printf("Before solving puzzle:\n");
	print_board(&board);
#endif
	
	if (!solve_puzzle(&board)) {
		fprintf(stderr, "Could not solve puzzle\n");
		return 1;
	}

#ifndef NOPRINT
	printf("After solving puzzle:\n");
	print_board(&board);
#endif
	
	return 0;
}
