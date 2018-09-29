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

#ifndef LCL_MATRIX_STACK_2D_IMP1_HEADER
#define LCL_MATRIX_STACK_2D_IMP1_HEADER

#include "LCL/Maths/LCL_Matrix.h"

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include <iostream>
using namespace std;

#include <utility>

// CONSTRUCTORS

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T,N,M>::LCL_Matrix_Stack_2D() {
    this->resize(0,0);
    _N = N;
    _M = M;
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T,N,M>::LCL_Matrix_Stack_2D(const LCL_Matrix<T>& in) {
    LCL_Matrix<T>::r() = in.r();
    LCL_Matrix<T>::c() = in.c();
    _N = N;
    _M = M;
    LCL_Matrix<T>::assignment(*this,in);
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T,N,M>::LCL_Matrix_Stack_2D(LCL_Mat_Size in_r, LCL_Mat_Size in_c) {
    LCL_Matrix<T>::r() = in_r;
    LCL_Matrix<T>::c() = in_c;
    _N = N;
    _M = M;
    LCL_Matrix<T>::reset();
}

// ACCESSORS

template <class T, int N, int M>
const T& LCL_Matrix_Stack_2D<T,N,M>::operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const {
    if((i>=0)&&(i<LCL_Matrix<T>::r())&&(j>=0)&&(j<LCL_Matrix<T>::c())) {
        return _e[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_Matrix");
        // Throw an exception instead
        return LCL_Matrix<T>::LCL_Matrix_Bad_Val;
    }
}

template <class T, int N, int M>
T& LCL_Matrix_Stack_2D<T,N,M>::operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) {
    if((i>=0)&&(i<LCL_Matrix<T>::r())&&(j>=0)&&(j<LCL_Matrix<T>::c())) {
        return _e[i][j];
    } else {
        error("Index out of bounds.", "operator()", "LCL_Matrix");
        // Throw an exception instead
        return LCL_Matrix<T>::LCL_Matrix_Bad_Val;
    }
}

// ARITHMETIC

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T,N,M> LCL_Matrix_Stack_2D<T,N,M>::operator+(const LCL_Matrix<T>& b) const {
    LCL_Matrix_Stack_2D<T,N,M> out;

    LCL_Matrix<T>::addition(out,*this,b);

    return out;
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T,N,M> LCL_Matrix_Stack_2D<T,N,M>::add(const LCL_Matrix<T>& A, const LCL_Matrix<T>& B) {
    LCL_Matrix_Stack_2D<T,N,M> out(A.r(), B.c());

    LCL_Matrix<T>::addition(out,A,B);

    return out;
}

template<class T, int N, int M>
LCL_Matrix_Stack_2D<T, N, M> LCL_Matrix_Stack_2D<T, N, M>::operator*(const LCL_Matrix<T>& in) const {
    LCL_Matrix_Stack_2D<T, N, M> out(this->r(),in.c());

    LCL_Matrix<T>::multiplication(out,(*this), in);

    return out;
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T, N, M> LCL_Matrix_Stack_2D<T,N,M>::mult(const LCL_Matrix<T>& A, const LCL_Matrix<T>& B) {
    LCL_Matrix_Stack_2D<T, N, M> out(A.r(), B.c());

    LCL_Matrix<T>::multiplication(out, A, B);

    return out;
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T, N, M> LCL_Matrix_Stack_2D<T,N,M>::operator*(const T& b) const {
    LCL_Matrix_Stack_2D<T, N, M> out(this->r(), this->c());

    LCL_Matrix<T>::scalar_multiplication(out, *this, b);

    return out;
}

template <class T, int N, int M>
LCL_Matrix_Stack_2D<T, N, M> LCL_Matrix_Stack_2D<T,N,M>::mult(const LCL_Matrix<T>& A, const T& b) {
    LCL_Matrix_Stack_2D<T, N, M> out(A.r(), A.c());

    LCL_Matrix<T>::scalar_multiplication(out, A, b);

    return out;
}

/*
template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator*(const LCL_Matrix& b) const {
    LCL_Matrix<T,N,M> out;

    if(_c==b._r) {
        out.dims(_r,b._c);
        for(int i = 0; i < _r; i++) {
            for(int j = 0; j < b._c; j++) {
                for(int k = 0; k < _c; k++) {
                    out(i,j) += operator()(i,k)*b(k,j);
                }
            }
        }
    } else {
        error("Inner dimensions must match.", "operator*", "LCL_Matrix");
    }

    return out;
}

template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator-() const {
    LCL_Matrix<T,N,M> out(_r,_c);

    for(int i = 0; i < _r; i++) {
        for(int j = 0; j < _c; j++) {
            out(i,j) = (-out(i,j));
        }
    }

    return out;
}

template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator-(const LCL_Matrix& b) const {
    return operator+(-b);
}

template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator*(const T& b) const {
    LCL_Matrix<T,N,M> out(_r,_c);

    for(int i = 0; i < _r; i++) {
        for(int j = 0; j < _c; j++) {
            out(i,j) = (operator()(i,j)*b);
        }
    }

    return out;
}

template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator/(const T& b) const {
    LCL_Matrix<T,N,M> out(_r,_c);

    for(int i = 0; i < _r; i++) {
        for(int j = 0; j < _c; j++) {
            out(i,j) = (operator()(i,j)/b);
        }
    }

    return out;
}

template <class T, int N, int M>
ostream& operator<<(ostream& os, const LCL_Matrix<T,N,M>& in) {
    for(int i = 0; i < in._r; i++) {
        for(int j = 0; j < in._c; j++) {
            os << in._e[i][j] << " ";
        }
        os << endl;
    }
    return os;
}
*/

#endif // LCL_MATRIX_STACK_2D_IMP1_HEADER
