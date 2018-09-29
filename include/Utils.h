/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke E. Heyfron, Earl T. Campbell
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef UTILS
#define UTILS

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

namespace Utils {
	double rand_d();
	int rand_i(int min, int max);
	int rand_i(int num);
	int flipBitN(int index, int Nbit);
	
	int kDelta(int i, int j);
	bool Bn(int index, int bit); //bit starts from 0 (least significant bit)
	void getComment(istream& inStr);
	int GCD(int inA, int inB);
	int factorize(int inC, int* inFactors);
}

#endif
