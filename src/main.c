#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const char *HELP_STR = 
	"USAGE:\n"
	"  command | sudoku [OPTIONS]\n"
	"  sudoku [OPTIONS] <FILE>\n"
	"  sudoku [OPTIONS] <<FILE>\n"
	"\n"
	"A command line tool to solve sudoku puzzles and write the solution to stdout.\n"
	"\n"
	"ARGUMENTS\n"
	"  <FILE>              The file to take input from. If this argument is not\n"
	"                      provided, input is taken from stdin.\n"
	"\n"
	"OPTIONS:\n"
	"  -f, --format        Turn formatting on\n"
	"  -h, --help          Display help\n"
	"\n"
	"INPUT FORMAT:\n"
	"  Input must be a a list of 9 lines, separated by a single ASCII character(eg:'\\n')\n"
	"  Each line must contain any of the ASCII characters from '0' to '9'\n";


static inline void print_help()
{
	printf(HELP_STR);
}

/*A funtion to check if an argument is present in the arguments list
 * Returns  a boolean value
 */
static bool args_contain( int argc, char* argv[], const char* arg)
{
	for ( int i=1; i<argc; ++i) {
		if ( strcmp( arg, argv[i] ) == 0 )
			return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	sudoku_t board;
	FILE *infile, *outfile;
	bool format, infile_given;

	outfile = stdout;
	format = false;

	int argn = 0;
	if (argc==1) {
		infile = stdin;
	} else {
		if ( args_contain( argc, argv, "-h" ) || args_contain( argc, argv, "--help" ) ) {
			print_help();
			return 0;
		} else if ( args_contain(argc, argv, "-f") || args_contain(argc, argv, "--format") ) {
			format = true;
			++argn;
		}

		//Check if an input file was given
		if ( argn+2 == argc ) {
			infile_given = true;
			char* filename = argv[argc-1];
			infile = fopen(filename, "r");
			if ( infile == NULL ) {
				fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
				exit(EXIT_FAILURE);
			}
		} else {
			infile = stdin;
		}
	
	}

	
		
	get_board(infile, &board);

	if (!solve_board(&board)) {
		fprintf(stderr, "Could not solve puzzle\n");
		return EXIT_FAILURE;
	}

	print_board(outfile, &board, format);

	if ( infile_given ) {
		fclose(infile);
	}
	
	return 0;
}
