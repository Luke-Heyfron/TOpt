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

#ifndef TO_MATRIX_HEADER
#define TO_MATRIX_HEADER

#include "LCL/Maths/LCL_Matrix_Heap_Sparse.h"
#include "LCL/Maths/LCL_FiniteField.h"

using TO_Matrix = LCL_Matrix_Heap_Sparse<F2>;

TO_Matrix& R(TO_Matrix& mat, int c, int t, int n=-1);

TO_Matrix& add_rows(TO_Matrix& A, int t, int s);
TO_Matrix& swap_rows(TO_Matrix& mat, int r1, int r2);
TO_Matrix& rowechelon(TO_Matrix& out, const TO_Matrix& in);
TO_Matrix& nullspace(TO_Matrix& out, const TO_Matrix& in);
//TO_Matrix& chi(TO_Matrix& out, const TO_Matrix& A, const TO_Matrix& z);


TO_Matrix& swap_columns(TO_Matrix& mat, int c1, int c2);
TO_Matrix& proper(TO_Matrix& mat);

#endif // TO_MATRIX_HEADER
