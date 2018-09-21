/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke Heyfron

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

#ifndef LCL_REALMATRIX
#define LCL_REALMATRIX

/*
    LCL_RealMatrix is a class for fast matrix calculations on the real set. It uses double as it's base data type and strictly static allocation for maximum speed.
    Implementations of all functions are optimized for performance. Use a different class if dynamic allocation is necessary.
*/

#include <iostream>
using namespace std;

#include <ostream>

template <int N, int M>
class LCL_RealMatrix {
    private:
        double data[N][M];
        int n = N;
        int m = M;
    public:
    // Constructor
    LCL_RealMatrix();

    // Output
    void print(ostream& in_OS = cout) const;

    // Accessors
    double operator()(int i, int j) const;
    double& operator()(int i, int j);

    // Binary Operators

    // Unary Operators
};

#endif // LCL_REALMATRIX
