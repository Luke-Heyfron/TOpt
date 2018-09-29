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

#include "LCL/LCL_BooleanMatrix.h"

#include <iostream>
using namespace std;

#include <ostream>

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

bool LCL_BOOLEAN_MATRIX_DUMMY = 0;

template <int N, int M>
LCL_BooleanMatrix<N,M>::LCL_BooleanMatrix() {
    r(0,N-1);
    c(0,M-1);
    r = 0;
    c = 0;

    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            data[i][j]=0;
        }
    }
}

template <int N, int M>
LCL_BooleanMatrix<N,M>::LCL_BooleanMatrix(int in_r, int in_c) {
    r(0,N-1);
    c(0,M-1);
    r = in_r;
    c = in_c;
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            data[i][j]=0;
        }
    }
}

template <int N, int M>
bool LCL_BooleanMatrix<N,M>::operator()(int i ,int j) const {
    bool out = 0;
    if((i>=0) && (i<r) && (j>=0) && (j<c) &&(i<N) && (j<M)) {
        out = data[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_BooleanMatrix");
    }
    return out;
}

template <int N, int M>
bool& LCL_BooleanMatrix<N,M>::operator()(int i ,int j) {
    if((i>=0) && (i<r) && (j>=0)&&(j<c) &&(i<N) && (j<M)) {
        return data[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_BooleanMatrix");
        return LCL_BOOLEAN_MATRIX_DUMMY;
    }
}

template <int N, int M>
void LCL_BooleanMatrix<N,M>::print(ostream& in_OS) const {
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            in_OS << data[i][j] << " ";
        }
        in_OS << endl;
    }
}

template <int N, int M>
LCL_BooleanMatrix<N,M> LCL_BooleanMatrix<N,M>::operator+(const LCL_BooleanMatrix& in_Mat) const {
    LCL_BooleanMatrix<N,M> out;

    // Output only defined if operand dimensions match.
    if((r==in_Mat.r)&&(c==in_Mat.c)) {
        out.r = r;
        out.c = c;
        for(int i = 0; i < out.r; i++) {
            for(int j = 0; j < out.c; j++) {
                out(i,j) = (operator()(i,j)+in_Mat(i,j))%2;
            }
        }
    } else {
        LCL_ConsoleOut::warning("Operand dimensions must match.", "operator+", "LCL_BooleanMatrix");
    }

    return out;
}

template <int N, int M>
LCL_BooleanMatrix<N,M>& LCL_BooleanMatrix<N,M>::operator+=(const LCL_BooleanMatrix& in_Mat) {
    if((r==in_Mat.r)&&(c==in_Mat.c)) {
        for(int i = 0; i < r; i++) {
            for(int j = 0; j < c; j++) {
                operator()(i,j) = (operator()(i,j)+in_Mat(i,j))%2;
            }
        }
    } else {
        LCL_ConsoleOut::warning("Operand dimensions must match.", "operator+=", "LCL_BooleanMatrix");
    }

    return (*this);
}

template <int N, int M>
LCL_BooleanMatrix<N,M> LCL_BooleanMatrix<N,M>::operator*(const LCL_BooleanMatrix& in_Mat) const {
    LCL_BooleanMatrix<N,M> out;

    // Output only defined if inner dimensions match.
    if(c==in_Mat.r) {
        out.r = r;
        out.c = in_Mat.c;
        for(int i = 0; i < out.r; i++) {
            for(int j = 0; j < out.c; j++) {
                int this_sum = 0;
                for(int k = 0; k < c; k++) {
                    this_sum += operator()(i,k)&in_Mat(k,j);
                }
                out(i,j) = (this_sum%2);
            }
        }
    } else {
        LCL_ConsoleOut::warning("Operand dimensions must match.", "operator*", "LCL_BooleanMatrix");
    }

    return out;
}
