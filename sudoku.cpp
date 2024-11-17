/*
 *  This file is part of Christian's OpenMP parallel Sudoku Solver
 *
 *  Copyright (C) 2013 by Christian Terboven <christian@terboven.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "SudokuBoard.h"

/**
 * Number of sudoku grid's cells to fill with the permutations of the numbers 1 to field_size in order to compute some starting
 * partial solutions used to parallelize the resolution of the sudoku among different tasks.
 */
#define CELLS_TO_PERMUTE 7

/**
 * Global variable that stores the number of sudoku's solutions found.
 */
int found_sudokus = 0;

/**
 * Solve the sudoku puzzle in parallel by recursively trying all possible numbers for each cell.
 * @param x The row of the cell to solve.
 * @param y The column of the cell to solve.
 * @param sudoku The sudoku board to solve.
 * @param printAnyFoundSolution A flag to print the found solutions (default value: true).
 */
bool solve_parallel(int x, int y, CSudokuBoard &sudoku, bool printAnyFoundSolution = true)
{
	if (x == sudoku.getFieldSize())
	{ // end of column
		y++;
		x = 0;
		if (y == sudoku.getFieldSize()) // end of grid
		{
			return true;
		}
	}

	if (sudoku.get(x, y) > 0) // cell already set, no need to recursively solve it
	{
		return solve_parallel(x + 1, y, sudoku, printAnyFoundSolution);
	}

	for (int i = 1; i <= sudoku.getFieldSize(); i++) // try all numbers
	{
		if (sudoku.isInBitmask(x, y, i)) // Check if the number is insertable in the cell by checking the bitmask
		{
#pragma omp task firstprivate(i, x, y, sudoku) final(y > 0)
			{

				sudoku.set(x, y, i); // Set the cell's value with the number i

				if (solve_parallel(x + 1, y, sudoku, printAnyFoundSolution))
				{
#pragma omp atomic
					found_sudokus++;		   // Increment the number of found solutions
					if (printAnyFoundSolution) // Print the solution if the flag is set to true
					{
						std::cout << "The following is a valid solution:" << std::endl;
						sudoku.printBoard();
						std::cout << std::endl;
					}
				}
			}
		}
	}

#pragma omp taskwait
	return false;
}

/**
 * Calculate the permutations of the numbers 1 to field_size in order to compute some starting partial solutions
 * used to parallelize the resolutions of the sudoku among different tasks.
 * @param x The starting row.
 * @param y The starting column.
 * @param sudoku The sudoku board of which we need to compute the first partial solutions.
 * @param counter The number of cells already set.
 * @param permutations The list of permutations computed.
 */
void calculatePermutations(int x, int y, CSudokuBoard &sudoku, int counter, std::list<CSudokuBoard> &permutations)
{
	if (counter >= CELLS_TO_PERMUTE) // If we have already computed the number of permutations we need, we can add the sudoku board to the list of starting partial solutions
	{
		permutations.push_back(sudoku);
		return;
	}

	if (x == sudoku.getFieldSize()) // end of column
	{
		y++;
		x = 0;
		if (y == sudoku.getFieldSize()) // end of grid
		{
			return;
		}
	}

	if (sudoku.get(x, y) > 0) // If cell already set, no need to recursively call the function in order to fill it
	{
		return calculatePermutations(x + 1, y, sudoku, counter, permutations);
	}

	for (int i = 1; i <= sudoku.getFieldSize(); i++) // Try all numbers
	{
		if (sudoku.isInBitmask(x, y, i)) // Check if the number is insertable in the cell by checking the bitmask
		{
			CSudokuBoard copy(sudoku); // Copy the sudoku board
			copy.set(x, y, i);		   // Set the cell's value with the number i
			counter++;				   // Increment the number of cells computed
			calculatePermutations(x + 1, y, copy, counter, permutations);
		}
	}
}

/**
 * Solve the sudoku puzzle recursively and in parallel. Each task starts from a partial solution of a list made of all the possible
 * permutations of the numbers 1 to field_size in the first CELL_TO_PERMUTE cells of the sudoku grid.
 * @param sudoku The sudoku board to solve.
 * @param printAnyFoundSolution A flag to print the found solutions (default value: true).
 * @see calculatePermutations()
 * @see solve_parallel()
 * @see field_size
 * @see CELL_TO_PERMUTE
 */
void solve(CSudokuBoard &sudoku, bool printAnyFoundSolution = true)
{

	std::list<CSudokuBoard> permutations;
	calculatePermutations(0, 0, sudoku, 0, permutations); // Compute the permutations of the numbers 1 to 9 in the first CELL_TO_PERMUTE cells of the sudoku grid
#pragma omp parallel
#pragma omp single
	{
		for (auto it = permutations.begin(); it != permutations.end(); ++it)
#pragma omp task firstprivate(it)
			solve_parallel(0, 0, *it, printAnyFoundSolution); // Solve the sudoku puzzle in parallel by recursively trying all possible numbers for each cell
#pragma omp taskwait										  // Each task start from a different partial solution
	}
}

int main(int argc, char *argv[])
{
	// Vars. to store the time
	double start, end;

	// Expect three command line arguments: field size, block size, and input file
	if (argc != 4)
	{
		std::cout << "Usage: sudoku.exe <field size> <block size> <input filename>" << std::endl;
		std::cout << std::endl;
		return -1;
	}
	else
	{
		CSudokuBoard sudoku1(atoi(argv[1]), atoi(argv[2]));
		if (!sudoku1.loadFromFile(argv[3]))
		{
			std::cout << "There was an error reading a Sudoku template from " << argv[3] << std::endl;
			std::cout << std::endl;
			return -1;
		}

		// print the Sudoku board template
		std::cout << "Given Sudoku template" << std::endl;
		sudoku1.printBoard();

		// solve the Sudoku by finding (and printing) all solutions
		start = omp_get_wtime();

		// #pragma omp parallel
		// 		{
		// #pragma omp master
		// 			solve_parallel(0, 0, sudoku1, false);
		// 		}
		solve(sudoku1, false);

		end = omp_get_wtime();
	}

	// print the time
	std::cout << "Parallel computation took " << end - start << " seconds ("
			  << omp_get_max_threads() << " threads)." << std::endl
			  << std::endl;

	// print the number of solutions
	std::cout << "Number of solutions found: " << found_sudokus << std::endl;

	return 0;
}
