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
#include <vector>
#include <list>


#define ACCESS(x, y) (this->field_size * (x) + (y))
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

	inline int getNumSolutions() const
	{
		return this->solutions;
	}

	inline void incrementSolutionCounter()
	{
		if (this->solutions == -1)
			this->solutions = 1;
		else
			this->solutions++;
	}

	inline int getFieldSize() const
	{
		return this->field_size;
	}

	inline int getBlockSize() const
	{
		return this->block_size;
	}

	inline int get(int x, int y) const
	{
		return this->field[ACCESS(x, y)];
	}

	inline void set(int x, int y, int value)
	{
		this->field[ACCESS(x, y)] = value;

		for (int i = 0; i < field_size; i++)
		{
			if (field[ACCESS(x, i)] == 0)
				removeBitFromMask(x, i, value);
			if (field[ACCESS(i, y)] == 0)
				removeBitFromMask(i, y, value);
		}

		int x_box = (int)(x / block_size) * block_size;
		int y_box = (int)(y / block_size) * block_size;

		for (int i = x_box; i < x_box + block_size; i++)
			for (int j = y_box; j < y_box + block_size; j++)
				if (field[ACCESS(i, j)] == 0)
					removeBitFromMask(i, j, value);
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
	bool isInBitmask(int x, int y, int value);

private:
	bool isInsertableHorizontal(int y, int value);
	bool isInsertableVertical(int x, int value);
	bool isInsertableBox(int x, int y, int value);
	bool isInsertable(int x, int y, int value);
	void calculateMask(int x, int y);
	void calculateMask();
	void removeBitFromMask(int x, int y, int value);

	int field_size;
	int block_size;

	int *field;
	bool *mask;

	int solutions;
};
