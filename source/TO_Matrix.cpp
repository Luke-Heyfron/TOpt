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

TO_Matrix& add_rows(TO_Matrix& A, int i_t, int i_s) {
    for(int j = 0; j < A.c(); j++) {
        F2 temp = (A(i_t,j) + A(i_s,j));
        A(i_t,j) = temp;
    }

    return A;
}

/*TO_Matrix& swap_rows(TO_Matrix& A, int r1, int r2) {

}*/
