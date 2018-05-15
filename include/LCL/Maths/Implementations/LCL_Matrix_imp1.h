#ifndef LCL_MATRIX_IMP1_HEADER
#define LCL_MATRIX_IMP1_HEADER
#include "LCL/Maths/LCL_Matrix.h"

#include "LCL/Maths/LCL_FiniteField.h"

template<class T>
T LCL_Matrix<T>::LCL_Matrix_Bad_Val;

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include <iostream>
using namespace std;

#include <utility>

// CONSTRUCTORS

/*template <class T>
LCL_Matrix<T>::LCL_Matrix() {
    ;
}

template <class T>
LCL_Matrix<T>::LCL_Matrix(const LCL_Matrix& in) {
    r() = in.r();
    c() = in.c();
    alloc();
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            operator()(i,j) = in(i,j);
        }
    }
}

template <class T>
LCL_Matrix<T>::LCL_Matrix(int in_r, int in_c) {
    r() = in_r;
    c() = in_c;
    alloc();
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            operator()(i,j) = 0;
        }
    }
}

template <class T>
void LCL_Matrix<T>::alloc() {
    ;
}

template <class T>
void LCL_Matrix<T>::dealloc() {
    ;
}*/



// ACCESSORS
/*
template <class T>
const T& LCL_Matrix<T>::operator()(int i, int j) const {
    if((i>=0)&&(i<r())&&(j>=0)&&(j<c())) {
        warning("Should override this function with a derived class!", "operator()", "LCL_Matrix");
        return LCL_Matrix_Bad_Val;
    } else {
        error("Index out of bounds.", "operator()", "LCL_Matrix");
        return LCL_Matrix_Bad_Val;
    }
}

template <class T>
T& LCL_Matrix<T>::operator()(int i, int j) {
    if((i>=0)&&(i<r())&&(j>=0)&&(j<c())) {
        warning("Should override this function with a derived class!", "operator()", "LCL_Matrix");
        return LCL_Matrix_Bad_Val;
    } else {
        error("Index out of bounds.", "operator()", "LCL_Matrix");
        return LCL_Matrix_Bad_Val;
    }
}
*/

template <class T>
void LCL_Matrix<T>::copy(const LCL_Matrix<T>& in) {
    resize(in.r(),in.c());
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            operator()(i,j) = in(i,j);
        }
    }
}

// ASSIGNMENT

template <class T>
LCL_Matrix<T>& LCL_Matrix<T>::operator=(const LCL_Matrix<T>& in) {
    if((r()!=in.r())||(c()!=in.c())) {
        resize(in.r(), in.c());
    }
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            operator()(i,j) = in(i,j);
        }
    }
    return (*this);
}


template <class T>
int LCL_Matrix<T>::r() const {
    return get<0>(_dims);
}



template <class T>
int& LCL_Matrix<T>::r() {
    return get<0>(_dims);
}



template <class T>
int LCL_Matrix<T>::c() const {
    return get<1>(_dims);
}


template <class T>
int& LCL_Matrix<T>::c() {
    return get<1>(_dims);
}

template <class T>
LCL_Mat_Dims LCL_Matrix<T>::dims() const {
    return _dims;
}

template <class T>
LCL_Mat_Dims& LCL_Matrix<T>::dims() {
    return _dims;
}



// ARITHMETIC

template<class T>
void LCL_Matrix<T>::addition(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>&B) {
    if((A.r()==B.r())&&(A.c()==B.c())) {
        if(out.dims()!=A.dims()) {
            out.resize(A.r(), A.c());
        }
        for(int i = 0; i < out.r(); i++) {
            for(int j = 0; j < out.c(); j++) {
                out(i,j) = (A(i,j) + B(i,j));
            }
        }
    } else {
        error("Operand dimensions must match.", "addition", "LCL_Matrix.h");
    }
}

template<class T>
void LCL_Matrix<T>::subtraction(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>&B) {
    if((A.r()==B.r())&&(A.c()==B.c())) {
        if(out.dims()!=A.dims()) {
            out.resize(A.r(), A.c());
        }
        for(int i = 0; i < out.r(); i++) {
            for(int j = 0; j < out.c(); j++) {
                out(i,j) = (A(i,j) - B(i,j));
            }
        }
    } else {
        error("Operand dimensions must match.", "addition", "LCL_Matrix.h");
    }
}

template<class T>
void LCL_Matrix<T>::multiplication(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>& B) {
    /// Only does something if inner dimensions of A and B match.
    if(A.c()==B.r()) {
        /// If the output's dimensions are wrong, make them right.
        if((out.r()!=A.r())||(out.c()!=B.c())) {
            out.resize(A.r(),B.c());
        }
        /// Set output's elements to zero.
        out.reset();
        /// Perform matrix multiplication.
        for(int i = 0; i < out.r(); i++) {
            for(int j = 0; j < out.c(); j++) {
                for(int k = 0; k < A.c(); k++) {
                    out(i,j) += A(i,k)*B(k,j);
                }
            }
        }
    } else {
        error("Inner dimensions must match.", "operator*", "LCL_Matrix");
    }
}

template <class T>
void LCL_Matrix<T>::scalar_multiplication(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const T& b) {
    /// If the output's dimensions are wrong, make them right.
    if((out.dims()!=A.dims())) {
        out.resize(A.r(),A.c());
    }
    for(int i = 0; i < out.r(); i++) {
        for(int j = 0; j < out.c(); j++) {
            out(i,j) = A(i,j)*b;
        }
    }
}

template<class T>
void LCL_Matrix<T>::assignment(LCL_Matrix<T>& dest, const LCL_Matrix<T>& src) {
    if((dest.r()==src.r())&&(dest.c()==src.c())) {
        for(int i = 0; i < dest.r(); i++) {
            for(int j = 0; j < dest.c(); j++) {
                dest(i,j) = src(i,j);
            }
        }
    } else {
        error("Matrices dimensions must match.", "assignment", "LCL_Matrix.h");
    }
}

template <class T>
void LCL_Matrix<T>::reset() {
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            operator()(i,j) = 0;
        }
    }
}



/*

template <class T, int N, int M>
LCL_Matrix<T,N,M> LCL_Matrix<T,N,M>::operator+(const LCL_Matrix& b) const {
    LCL_Matrix<T,N,M> out;

    if((_r==b._r)&&(_c==b._c)) {
        out.dims(_r,_c);
        for(int i = 0; i < _r; i++) {
            for(int j = 0; j < _c; j++) {
                out(i,j) = (operator()(i,j) + b(i,j));
            }
        }
    } else {
        error("Operand dimensions must match.", "operator+", "LCL_Matrix");
    }

    return out;
}



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


template<class T>
void LCL_Matrix<T>::print() const {
    for(int i = 0; i < r(); i++) {
        for(int j = 0; j < c(); j++) {
            cout << operator()(i,j) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Misc

template <class T>
LCL_Mat_Flat_Index LCL_Matrix<T>::Index_2D_to_Flat(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const {
    return (c()*i + j);
}

template <class T>
LCL_Mat_2D_Index LCL_Matrix<T>::Index_Flat_to_i(LCL_Mat_Flat_Index in_I) const {
    return in_I/c();
}

template <class T>
LCL_Mat_2D_Index LCL_Matrix<T>::Index_Flat_to_j(LCL_Mat_Flat_Index in_I) const {
    return in_I%c();
}

template <class T>
void LCL_Matrix<T>::transpose(LCL_Matrix<T>& out, const LCL_Matrix<T>& in) {
    if((out.r()==in.c())&&(out.c()==in.r())) {
        for(int i = 0; i < in.r(); i++) {
            for(int j = 0; j < in.c(); j++) {
                out(i,j)=in(j,i);
            }
        }
    } else {
        error("Output dimensions must be the input dimensions swapped.", "transpose", "LCL_Matrix");
    }
}

#endif // LCL_MATRIX_IMP1_HEADER
