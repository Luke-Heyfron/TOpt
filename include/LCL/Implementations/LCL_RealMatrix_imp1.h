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

#include "LCL/LCL_RealMatrix.h"

#include <iostream>
using namespace std;

#include <ostream>

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

template <int N, int M>
LCL_RealMatrix<N,M>::LCL_RealMatrix() {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            data[i][j]=0.0;
        }
    }
}

template <int N, int M>
double LCL_RealMatrix<N,M>::operator()(int i ,int j) const {
    double out = 0.0;
    if((i>=0) && (i<n) && (j>=0)&&(j<m)) {
        out = data[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_RealMatrix");
    }
    return out;
}

template <int N, int M>
double& LCL_RealMatrix<N,M>::operator()(int i ,int j) {
    if((i>=0) && (i<n) && (j>=0)&&(j<m)) {
        return data[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_RealMatrix");
        double dummy = 0.0;
        return dummy;
    }
}

template <int N, int M>
void LCL_RealMatrix<N,M>::print(ostream& in_OS) const {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            in_OS << data[i][j] << " ";
        }
        in_OS << endl;
    }
}
