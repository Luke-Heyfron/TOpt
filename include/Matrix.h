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

#ifndef MATRIX_CLASS
#define MATRIX_CLASS

#include "Complex.h"
#include <cstdlib>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
//#include "State.h"

const double L_PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989380952572010654858632788659361533818279682303019520353018529689957736225994138912497217752834791315155748572424541506959508295331168617278558890750983817546374649393192550604009277016711390098488240128583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912933136770289891521047521620569660240580381501935112533824300355876402474964732639141992726042699227967823547816360093417216412199245863150302861829745557067498385054945885869269956909272107975;

class Matrix {
	private:
		int rows;
		int cols;
		Complex ***data;
		bool notamatrix;
		//void reshape(int,int);
	public:
		//Constructors and destructors
		Matrix(int inRows, int inCols);
		Matrix(const Matrix&);
		//Matrix(const State*);
		~Matrix();

		//Members functions
		Complex E(int,int) const;						//Get value at given indices
		Complex& E(int,int);
		void E(int,int,Complex);						//Assign value at given indices with Complex number
		void E(int,int,double);							//Assign value at given indices with single double. Real part remains unchanged
		void E(int,int,double,double);					//Assign value at given indices with two doubles. Both real and imaginary parts are assigned.
		void E(int,int,bool,double);					//Assign value at given indices with effectively one double. Assigns only imaginary part and bool is ignored. (By convention use true)
		int getRows() const;
		int getCols() const;
		bool isSquare() const;
		int isVec() const;								//Returns -1 for row vec, 0 for not a vec, 1 for col vec
		double magnitude() const;							//Returns length if matrix is a vector (row or col)
		int length() const;								//Returns number of elements on main axis if vector
		bool multCompatible(const Matrix&) const;
		Matrix multiply(const Matrix&) const;
		//Vector multiply(Vector&);
		Matrix inverse() const;
		Matrix transpose() const;
		Matrix adjoint() const;
		Complex trace() const;
		Matrix partialTrace(int q_i, Complex* tr_i = NULL) const; //Valid only for 1 <= q_i <= n where this is a square 2^n X 2^n matrix. Returns the matrix where qubit i has been traced out. tr_i is container for the renormalization scalar. In this case returns unitary.
		Complex determinant() const;
		bool isNAM() const;
		Matrix add(const Matrix& inM) const;
		Matrix tensorProduct(const Matrix&) const;
		static Matrix nTensorM(const Matrix&, int inN);
		void clone(const Matrix& inM);
		Matrix submatrix(int r_0, int c_0, int n_r, int n_c) const;

		//Display functions
		void print() const;

		//Operator overloads
			//Assignment
		void operator=(const Matrix&);
			//Addition and subtraction
		Matrix operator+(const Matrix&) const;
		Matrix operator-(const Matrix&) const;
			//Scalar and matrix multiplication
		Matrix operator*(double) const;						//Note: This means scalar multiplication always has to be at the END of a statement involving matrices
		Matrix operator*(const Matrix&) const;
		Matrix operator*(const Complex& in) const;
		//Vector operator*(Vector&);
			//Tensor product
		Matrix operator^(const Matrix&) const;
		Matrix operator^=(const Matrix&);
			//Vector element access
		Complex operator[](int index) const;
		Complex& operator[](int index);

		//Matrix generators
		static Matrix colVec(int);
		static Matrix rowVec(int);
		static Matrix identity(int);
		static Matrix identity();
		static Matrix X();
		static Matrix Y();
		static Matrix Z();
		static Matrix H();
		static Matrix S();
		static Matrix T();
		static Matrix M(); // Post-select +1 outcome of Pauli-Z measurement
		static Matrix R(double theta);
		static Matrix CNOT();
		static Matrix CMAT(const Matrix& inMat);
		static Matrix X(int Nbit,int nth);		//All nths or bit indices start from 1 in this context
		static Matrix Y(int Nbit,int nth);
		static Matrix Z(int Nbit,int nth);
		static Matrix H(int Nbit,int nth);
		static Matrix S(int Nbit,int nth);
		static Matrix T(int Nbit,int nth);
		static Matrix M(int Nbit,int nth);
		static Matrix R(int Nbit,int nth, double theta);
		static Matrix O(int Nbit,int nth);
		static Matrix CNOT(int Nbit, int cbit, int obit);
		static Matrix CMAT(const Matrix& inMat, int Nbit, int cbit, int obit);
		static Matrix PERM(int Nbits, int cbit, int Mbits, int inA, int inC);
        static Matrix CNOT(int Nbit, int* qargs, int nargs = -1);//qargs[0] = target, quargs[1 -> (n-1)] = controls
        static Matrix CS(int Nbit, int c1, int c2);
        static Matrix CZ(int Nbit, int c1, int c2);
        static Matrix CCZ(int Nbit, int c1, int c2, int c3);

		static Matrix loadMatrix(const char* filename);

		//Shorthand
		//Matrix T();
};

#endif
