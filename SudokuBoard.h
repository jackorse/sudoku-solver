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
#include <iomanip>
#include <fstream>
#include <list>
#include <vector>

/**
 * Macro to access the linearized field array.
 */
#define ACCESS(x, y) (this->field_size * (x) + (y))
/**
 * Macro to access the linearized mask array.
 */
#define ACCESS_MASK(x, y, value) (this->field_size * this->field_size * (x) + this->field_size * (y) + (value - 1))

/**
 * @brief Representation of the Sudoku board, includes utility functions
 */
class CSudokuBoard
{
public:
	CSudokuBoard(int fsize, int bsize);
	CSudokuBoard(const CSudokuBoard &other);
	~CSudokuBoard(void);

	/**
	 * Get the size of the field
	 * @return The size of the field
	 */
	inline int getFieldSize() const
	{
		return this->field_size;
	}

	/**
	 * Get the size of the block
	 * @return The size of the block
	 */
	inline int getBlockSize() const
	{
		return this->block_size;
	}

	/**
	 * Get the value of a cell
	 * @param x The row of the cell
	 * @param y The column of the cell
	 * @return The value of the cell
	 */
	inline int get(int x, int y) const
	{
		return this->field[ACCESS(x, y)];
	}

	/**
	 * Set the value of a cell
	 * @param x The row of the cell
	 * @param y The column of the cell
	 * @param value The value to set
	 */
	inline void set(int x, int y, int value)
	{
		this->field[ACCESS(x, y)] = value; // Set the value of the cell

		// Remove the value from the bitmask at the index corresponding to the value inserted in all the cells of the same
		// row and column
		for (int i = 0; i < field_size; i++)
		{
			if (field[ACCESS(x, i)] == 0)
				resetBitOfMask(x, i, value);
			if (field[ACCESS(i, y)] == 0)
				resetBitOfMask(i, y, value);
		}

		int x_box = (int)(x / block_size) * block_size; // x coordinate of the top left corner of the box
		int y_box = (int)(y / block_size) * block_size; // y coordinate of the top left corner of the box

		// Remove the value from the bitmask at the index corresponding to the value inserted in all the cells of the same block.
		for (int i = x_box; i < x_box + block_size; i++)
			for (int j = y_box; j < y_box + block_size; j++)
				if (field[ACCESS(i, j)] == 0)
					resetBitOfMask(i, j, value);
	}

	/**
	 * Check if a number is insertable in a cell
	 * @param x The row of the cell to check
	 * @param y The column of the cell to check
	 * @param value The value to check
	 * @return true if the value is insertable, false otherwise
	 */
	inline bool isInBitmask(int x, int y, int value) const
	{
		return mask[ACCESS_MASK(x, y, value)];
	}

	/**
	 * Read Sudoku template from file
	 * @param filename name of file to read input from
	 * @return true if reading file was successful, false if not
	 */
	bool loadFromFile(char *filename);

	/**
	 * Print the Sudoku board to stdout
	 */
	void printBoard();

private:
	/**
	 * Check if a number is insertable in a cell by checking if the number is already present in the same row of the cell
	 * @param x The row of the cell to check
	 * @param value The value to check
	 */
	bool isInsertableHorizontal(int x, int value);

	/**
	 * Check if a number is insertable in a cell by checking if the number is already present in the same column of the cell
	 * @param y The column of the cell to check
	 * @param value The value to check
	 */
	bool isInsertableVertical(int y, int value);

	/**
	 * Check if a number is insertable in a cell by checking if the number is already present in the same block of the cell
	 * @param x The row of the cell to check
	 * @param y The column of the cell to check
	 * @param value The value to check
	 */
	bool isInsertableBox(int x, int y, int value);

	/**
	 * Check if a number is insertable in a cell
	 * @param x The row of the cell to check
	 * @param y The column of the cell to check
	 * @param value The value to check
	 */
	bool isInsertable(int x, int y, int value);

	/**
	 * Compute the bitmask of a cell.
	 * @param x The row of the cell.
	 * @param y The column of the cell.
	 */
	void calculateMask(int x, int y);

	/**
	 * Compute the bitmask (an array of booleans in which, each boolean indicates if the number corresponding to the index is
	 * insertable in the cell) of the whole Sudoku board.
	 */
	void calculateMask();

	/**
	 * Remove a bit from the bitmask of a cell.
	 * @param x The row of the cell.
	 * @param y The column of the cell.
	 * @param value The value to reset from the bitmask.
	 */
	inline void resetBitOfMask(int x, int y, int value)
	{
		mask[ACCESS_MASK(x, y, value)] = 0;
	}

	/**
	 * The size of the field
	 */
	int field_size;

	/**
	 * The size of the block
	 */
	int block_size;

	/**
	 * Array of elements of the Sudoku board
	 */
	int *field;

	/**
	 * Array of booleans representing the bitmasks of the Sudoku board cells
	 */
	bool *mask;
};
