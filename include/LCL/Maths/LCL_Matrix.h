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

#ifndef LCL_MATRIX_HEADER
#define LCL_MATRIX_HEADER

#include "LCL/Core/LCL_Array.h"
#include <iostream>
using namespace std;

#include <utility>

/// First element is the number of rows, second is the number of columns.
typedef int LCL_Mat_Size;

typedef int LCL_Mat_Flat_Index;
typedef int LCL_Mat_2D_Index;

//typedef pair<int, int> LCL_Mat_Dims;
class LCL_Mat_Dims : public pair<LCL_Mat_Size,LCL_Mat_Size> {
    public:
        bool operator==(const LCL_Mat_Dims& B) const {return (r()==B.r())&&(c()==B.c());}
        bool operator!=(const LCL_Mat_Dims& B) const {return !((*this)==B);}
        LCL_Mat_Size r() const {return get<0>(*this);}
        LCL_Mat_Size c() const {return get<1>(*this);}
};

/// A class that represents the mathematical objects commonly known as matrices.
/**
    The key ideas behind this system of classes are:

    1) Operational compatibility between different types of physical representations of matrices.
    I.e. a dense matrix on the stack can multiply with a sparse matrix on the heap etc.

    2) Template system to allow matrices on ANY field be easily defined without lots of trivial code repetition.

    Notes:
        - The template parameter T should ideally be a class derived from LCL_Field. The ensures operators +,-,*,/ are well defined.
        - Each derived class of LCL_Matrix, LCL_Matrix_Stack_2D and LCL_Matrix_Heap_Sparse should implement arithmetic operator overrides whose return type should be it's own class.
        - These arithmetic overrides should use the static arithmetic operator implementations found in LCL_Matrix.
*/
template<class T>
class LCL_Matrix {
    protected:
        LCL_Mat_Dims _dims;
    public:
        /*LCL_Matrix();
        LCL_Matrix(const LCL_Matrix& in);
        LCL_Matrix(int in_r, int in_c);*/
        // virtual void alloc() {}
        // virtual void dealloc() {}
        /// A child class that needs to (re)allocate memory on the fly should override this.
        virtual void resize(LCL_Mat_Size in_r, LCL_Mat_Size in_c) {r() = in_r; c() = in_c;}
        virtual void copy(const LCL_Matrix<T>& in);

        // Accessors
        virtual T operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const = 0;
        virtual T& operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) = 0;

        // Assignment
        static void assignment(LCL_Matrix<T>& dest, const LCL_Matrix<T>& src); /// dest = src;
        virtual LCL_Matrix& operator=(const LCL_Matrix& in);

        // Dimensions
        virtual LCL_Mat_Size r() const;
        virtual LCL_Mat_Size c() const;
        virtual LCL_Mat_Size& r();
        virtual LCL_Mat_Size& c();
        virtual LCL_Mat_Dims dims() const;
        virtual LCL_Mat_Dims& dims();

        // Element setters
        void reset();

        // Input/Output
            // Save
            // Load

        // Arithmetic operators (base)
        static void addition(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>&B); /// out = A + B;
        static void subtraction(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>&B); /// out = A - B;
        static void multiplication(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const LCL_Matrix<T>&B); /// out = A * B;
        static void scalar_multiplication(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const T& b); /// out = A * b;
        static void scalar_division(LCL_Matrix<T>& out, const LCL_Matrix<T>& A, const T& b); /// out = A / b;
        static void additive_inverse(LCL_Matrix<T>& out, const LCL_Matrix<T>& A); /// out = -A;
        static void multiplicative_inverse(LCL_Matrix<T>& out, const LCL_Matrix<T>& A); /// out = A^{-1};

        // Templates for child class arithmetic operators
        /*
        virtual LCL_Matrix operator+(const LCL_Matrix& b) const;
        virtual LCL_Matrix operator-() const;
        virtual LCL_Matrix operator-(const LCL_Matrix& b) const;
        virtual LCL_Matrix operator*(const T& b) const;
        virtual LCL_Matrix operator/(const T& b) const;
        virtual LCL_Matrix operator*(const LCL_Matrix& b) const;
        */

        // Arithmetic assignment operators
        // TODO Implement these
        /*virtual LCL_Matrix& operator+=(const LCL_Matrix& b) const;
        virtual LCL_Matrix& operator-=(const LCL_Matrix& b) const;
        virtual LCL_Matrix& operator*=(const LCL_Matrix& b) const;
        virtual LCL_Matrix& operator*=(const T& b) const;
        virtual LCL_Matrix& operator/=(const T& b) const;*/

        // Matrix operations
            static void transpose(LCL_Matrix<T>& out, const LCL_Matrix<T>& in); /// out = in.T

        // Composition operators
            // Left-right concat
            static void concatenate_vertical(LCL_Matrix<T>& out, const LCL_Matrix<T>& top, const LCL_Matrix<T>& bottom);
            // Top-bottom concat

        // Row/column operators
            // Get/set row/col

        // stdout
        // TODO Implement this:
        /*template <class Y, int J, int K>
        friend ostream& operator<<(ostream&, const LCL_Matrix<Y,J,K>& in);*/
        void print() const;

        // Badval
        static T LCL_Matrix_Bad_Val;

        // Misc
        LCL_Mat_Flat_Index Index_2D_to_Flat(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const;
        LCL_Mat_2D_Index Index_Flat_to_i(LCL_Mat_Flat_Index in_I) const;
        LCL_Mat_2D_Index Index_Flat_to_j(LCL_Mat_Flat_Index in_I) const;


};

#include "LCL/Maths/Implementations/LCL_Matrix_imp1.h"

#endif // LCL_MATRIX_HEADER
