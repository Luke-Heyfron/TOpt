#include "LCL_RealMatrix.h"

#include <iostream>
using namespace std;

#include <ostream>

#include "LCL_ConsoleOut.h"
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
