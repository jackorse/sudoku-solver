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

#include "SudokuBoard.h"

#include <cstring>

/**
 * Constructor of the Sudoku board.
 * @param fsize The size of the field.
 * @param bsize The size of the block.
 */
CSudokuBoard::CSudokuBoard(int fsize, int bsize)
	: field_size(fsize), block_size(bsize)
{
	field = new int[field_size * field_size];
	mask = new bool[field_size * field_size * field_size];
}

/**
 * Copy constructor of the Sudoku board.
 * @param other The Sudoku board to copy.
 */
CSudokuBoard::CSudokuBoard(const CSudokuBoard &other)
	: field_size(other.getFieldSize()), block_size(other.getBlockSize())
{
	field = new int[field_size * field_size];
	mask = new bool[field_size * field_size * field_size];
	std::memcpy(field, other.field, sizeof(int) * field_size * field_size);
	std::memcpy(mask, other.mask, sizeof(bool) * field_size * field_size * field_size);
}

/**
 * Destructor of the Sudoku board.
 */
CSudokuBoard::~CSudokuBoard(void)
{
	delete[] field;
	delete[] mask;
}

bool CSudokuBoard::loadFromFile(char *filename)
{
	std::ifstream ifile(filename);

	if (!ifile)
	{
		std::cout << "There was an error opening the input file " << filename << std::endl;
		std::cout << std::endl;
		return false;
	}

	for (int i = 0; i < this->field_size; i++)
	{
		for (int j = 0; j < this->field_size; j++)
		{
			ifile >> this->field[ACCESS(i, j)];
		}
	}

	calculateMask();

	return true;
}

void CSudokuBoard::printBoard()
{
	for (int i = 0; i < field_size; i++)
	{
		for (int j = 0; j < field_size; j++)
		{
			std::cout << std::setw(3) << this->field[ACCESS(i, j)]
					  << " ";
		}
		std::cout << std::endl;
	}
}

bool CSudokuBoard::isInsertable(int x, int y, int value)
{
	return isInsertableHorizontal(x, value) && isInsertableVertical(y, value) && isInsertableBox(x, y, value);
}

bool CSudokuBoard::isInsertableHorizontal(int x, int value)
{
	for (int i = 0; i < field_size; i++)
		if (field[ACCESS(x, i)] == value)
			return false;
	return true;
}

bool CSudokuBoard::isInsertableVertical(int y, int value)
{
	for (int i = 0; i < field_size; i++)
		if (field[ACCESS(i, y)] == value)
			return false;
	return true;
}

bool CSudokuBoard::isInsertableBox(int x, int y, int value)
{
	int x_box = x - x % block_size; // x coordinate of the top left corner of the box
	int y_box = y - y % block_size; // y coordinate of the top left corner of the box

	for (int i = x_box; i < x_box + block_size; i++)
		for (int j = y_box; j < y_box + block_size; j++)
			if (field[ACCESS(i, j)] == value) // if the value is already present in the box return false
				return false;

	return true;
}

void CSudokuBoard::calculateMask(int x, int y)
{
	for (int i = 1; i <= field_size; i++)
	{
		mask[ACCESS_MASK(x, y, i)] = isInsertable(x, y, i);
	}
}

void CSudokuBoard::calculateMask()
{
	for (int x = 0; x < field_size; x++)
		for (int y = 0; y < field_size; y++)
			if (field[ACCESS(x, y)] == 0)
				calculateMask(x, y);
}
