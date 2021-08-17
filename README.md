# sudoku

A sudoku solver written in C using backtracking algorithm

### Instructions:

1. Clone the repository locally and move into the directory

    ```sh
    $ git clone https://github.com/aadilshabier/sudoku.git && cd sudoku
    ```
1. There are two different ways to build the project:  
    I. Meson
      ```sh
      $ meson build && ninja -C build         
      ```
      The binary executable will be generated in the `build` directory.  
    II. This one liner
      ```sh
      $ cc -Wall -Wextra -pedantic --std=c11 -O3 -Iinclude -o main src/main.c src/sudoku.c
      ```
      The binary exectable will be generated in the parent directory.
1. The `example` script can be used to get some example puzzles to test the program, it contains 50 puzzles 
    ```sh
    $ ./example 12 | ./sudoku
    ``` 
