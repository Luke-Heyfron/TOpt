#include "TO_Matrix.h"


TO_Matrix& R(TO_Matrix& mat, int c, int t, int n) {
    if(n<0) {
        n = min(mat.r(),mat.c());
    }
    mat.clear();
    mat.resize(n,n);
    for(int i = 0; i < n; i++) {
        mat(i,i) = 1;
    }
    mat(t,c) = 1;
    return mat;
}
