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

#ifndef LCL_UTILS
#define LCL_UTILS

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

/// Contains handy functions.
namespace LCL_Utils {
    /// Returns a random double between 0 and 1.
	double rand_d();

	/// Returns a random int between min and max.
	int rand_i(int min, int max);

	/// Returns a random int between 0 and num.
	int rand_i(int num);

	/// Flips the Nth bit of binary expansion of index.
	int flipBitN(int index, int Nbit);

    /// Returns 1 if and only if i==j.
	int kDelta(int i, int j);

	/// Returns the Nth bit of binary expansion of index.
	bool Bn(int index, int bit); // bit starts from 0 (least significant bit)

	/// Displays a commented out line from an input stream if it begins with one.
	void getComment(istream& inStr);

	/// (I think) GCD calculates the Greatest Common Divisor between inA and inB.
	int GCD(int inA, int inB);

	/// Factorizes an integer inC and places the factors in an array inFactors.
	int factorize(int inC, int* inFactors);
}

#endif
