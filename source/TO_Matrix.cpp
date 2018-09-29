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
