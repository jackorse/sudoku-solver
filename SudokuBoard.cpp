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

CSudokuBoard::CSudokuBoard(int fsize, int bsize)
	: field_size(fsize), block_size(bsize), solutions(-1)
{
	field = new int[field_size*field_size];
}


CSudokuBoard::CSudokuBoard(const CSudokuBoard& other)
	: field_size(other.getFieldSize()), block_size(other.getBlockSize()), solutions(other.getNumSolutions())
{
	field = new int[field_size*field_size];
	std::memcpy(field, other.field, sizeof(int) * field_size*field_size);
}


CSudokuBoard::~CSudokuBoard(void)
{
	delete[] field;
}


bool CSudokuBoard::loadFromFile(char *filename)
{
  std::ifstream ifile(filename);
  
  if (!ifile) {
    std::cout << "There was an error opening the input file " << filename << std::endl;
    std::cout << std::endl;
    return false;
  }

  for (int i = 0; i < this->field_size; i++) {
    for (int j = 0; j < this->field_size; j++) {
	  ifile >> this->field[ACCESS(i,j)];
    }
  }

  return true;
}


void CSudokuBoard::printBoard()
{
	for(int i = 0; i < field_size; i++) {
		for(int j = 0; j < field_size; j++) {
			std::cout << std::setw(3) << 
				this->field[ACCESS(i,j)] 
				<< " ";
		}
		std::cout << std::endl;
	}
}