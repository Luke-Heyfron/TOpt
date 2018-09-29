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

#ifndef LCL_MATRIX_STACK_2D_HEADER
#define LCL_MATRIX_STACK_2D_HEADER

#include "LCL/Maths/LCL_Matrix.h"

template<class T, int N, int M>
class LCL_Matrix_Stack_2D : public LCL_Matrix<T> {
    protected:
        T _e[N][M];
        LCL_Mat_Size _N;
        LCL_Mat_Size _M;
    public:
        // Constructors
        LCL_Matrix_Stack_2D();
        LCL_Matrix_Stack_2D(const LCL_Matrix<T>& in);
        LCL_Matrix_Stack_2D(LCL_Mat_Size in_r, LCL_Mat_Size in_c);

        // Accessors
        const T& operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const override;
        T& operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) override;

        // Arithmetic
            // +
        LCL_Matrix_Stack_2D operator+(const LCL_Matrix<T>& B) const;
        static LCL_Matrix_Stack_2D<T,N,M> add(const LCL_Matrix<T>& A, const LCL_Matrix<T>& B);
            // * (mat)
        LCL_Matrix_Stack_2D operator*(const LCL_Matrix<T>& B) const;
        static LCL_Matrix_Stack_2D<T,N,M> mult(const LCL_Matrix<T>& A, const LCL_Matrix<T>& B);
            // * (sca)
        LCL_Matrix_Stack_2D operator*(const T& b) const;
        static LCL_Matrix_Stack_2D<T,N,M> mult(const LCL_Matrix<T>& A, const T& b);
};

#include "LCL_Matrix_Stack_2D_imp1.h"

#endif // LCL_MATRIX_STACK_2D_HEADER
