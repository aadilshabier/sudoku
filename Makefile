CC=gcc
CFLAGS=-Wall -Wextra -ggdb

sudoku: main.o sudoku.o
	$(CC) $(CFLAGS) -o sudoku main.o sudoku.o

main.o: main.c sudoku.h
	$(CC) $(CFLAGS) -c main.c

sudoku.o: sudoku.c sudoku.h
	$(CC) $(CFLAGS) -c sudoku.c

run: sudoku
	./sudoku

clean:
	rm *.o sudoku
