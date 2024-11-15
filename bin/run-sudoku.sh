#!/bin/bash

export IFS=";"

THREADS="01;02;04;06;08;10;12;14;16;18;20;22;24;26;28;30;32;34;36;38;40;42;44;46;48"

for threads in $THREADS; do
  OMP_NUM_THREADS=$threads ./sudoku 9 3 sudoku-9x9.txt # 1 solution
  #OMP_NUM_THREADS=$threads ./sudoku 16 4 sudoku-16x16.txt # 80 solutions
  #OMP_NUM_THREADS=$threads ./sudoku 25 5 sudoku-25x25.txt # 1 solution
done
