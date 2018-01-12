#include "LCL_BooleanMatrix.h"

#include <iostream>
using namespace std;

#include <ostream>

#include "LCL_ConsoleOut.h"
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
                out(i,j) = operator()(i,j)^in_Mat(i,j);
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
                operator()(i,j) ^= in_Mat(i,j);
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
                for(int k = 0; k < c; k++) {
                    out(i,j) ^= operator()(i,k)&in_Mat(k,j);
                }
            }
        }
    } else {
        LCL_ConsoleOut::warning("Operand dimensions must match.", "operator*", "LCL_BooleanMatrix");
    }

    return out;
}
