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

#include "Matrix.h"

#include "Complex.h"
#include "Utils.h"
#include "LCL/LCL_Bool.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

Matrix::Matrix(int inRows, int inCols) {
	rows = inRows;
	cols = inCols;
	if((rows*cols) > 0) {
		data = new Complex**[rows];
		for(int i = 0; i < rows; i++) {
			data[i] = new Complex*[cols];
			for(int j = 0; j < cols; j++) {
				data[i][j] = new Complex();
			}
		}
		notamatrix = false;
	} else {

		notamatrix = true;
	}
}

Matrix::Matrix(const Matrix& inMat) {
	rows = inMat.getRows();
	cols = inMat.getCols();
	if((rows*cols) > 0) {
		data = new Complex**[rows];
		for(int i = 0; i < rows; i++) {
			data[i] = new Complex*[cols];
			for(int j = 0; j < cols; j++) {
				data[i][j] = new Complex();
			}
		}

		notamatrix = false;
	} else {

		notamatrix = true;
	}
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			E(i,j,inMat.E(i,j));
		}
	}
}

Matrix::~Matrix() {
	if(!isNAM()) {
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				delete data[i][j];
			}
			delete [] data[i];
		}
		delete[] data;
	}
}

Complex Matrix::E(int inRow, int inCol) const {
	return *data[inRow][inCol];
}

Complex& Matrix::E(int inRow, int inCol) {
	return *data[inRow][inCol];
}

bool Matrix::isNAM() const{
	return notamatrix;
}

void Matrix::print() const {
	if(!isNAM()) {
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				Complex thisC = *data[i][j];
				if(thisC.magnitude() == 0.0) {
					cout << 0.0 << "\t";
				} else {
					cout << data[i][j]->toString() << "\t";
				}
			}
			cout << endl;
		}
	} else {
		cout << "This is not a matrix" << endl;
	}

}

void Matrix::E(int inRow, int inCol, double inVal) {
	data[inRow][inCol]->setReal(inVal);
}

void Matrix::E(int inRow, int inCol, double inR, double inI) {
	data[inRow][inCol]->setComplex(inR,inI);
}

void Matrix::E(int inRow, int inCol, Complex inC) {
	*data[inRow][inCol] = inC;
}

void Matrix::E(int inRow, int inCol, bool redundant, double inImag) {
	data[inRow][inCol]->setImaginary(inImag);
}

int Matrix::getRows() const {
	return rows;
}

int Matrix::getCols() const {
	return cols;
}

bool Matrix::isSquare() const {
	return rows==cols;
}

bool Matrix::multCompatible(const Matrix& inM) const {
	return cols==inM.getRows();
}




void Matrix::operator=(const Matrix& inM) {
	//cout << "Assigning" << endl;

	if(!isNAM()) {
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				delete data[i][j];
			}
			delete [] data[i];
		}
		delete[] data;
	}


	rows = inM.getRows();
	cols = inM.getCols();

	if((rows*cols) > 0) {
		data = new Complex**[rows];
		for(int i = 0; i < rows; i++) {
			data[i] = new Complex*[cols];
			for(int j = 0; j < cols; j++) {
				data[i][j] = new Complex();
			}
		}
		notamatrix = false;
	} else {

		notamatrix = true;
	}
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			E(i,j,inM.E(i,j));
		}
	}
}

void Matrix::clone(const Matrix& inM) {


	if(!isNAM()) {
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				delete data[i][j];
			}
			delete [] data[i];
		}
		delete[] data;
	}

	rows = inM.getRows();
	cols = inM.getCols();

	if((rows*cols) > 0) {
		data = new Complex**[rows];
		for(int i = 0; i < rows; i++) {
			data[i] = new Complex*[cols];
			for(int j = 0; j < cols; j++) {
				data[i][j] = new Complex();
			}
		}
		notamatrix = false;
	} else {

		notamatrix = true;
	}
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			E(i,j,inM.E(i,j));
		}
	}
}

Matrix Matrix::add(const Matrix& inM) const {
	if(((rows==inM.getRows())&&(cols==inM.getCols()))) {
		Matrix out(rows,cols);
		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < cols; j++) {
				out.E(i,j,E(i,j)+inM.E(i,j));
			}
		}
		return out;
	} else {
		Matrix out(0,0);
		return out;
	}
}

Matrix Matrix::operator+(const Matrix& inM) const {
	return add(inM);
}

Matrix Matrix::operator-(const Matrix& inM) const {
	if((rows==inM.getRows())&&(cols==inM.getCols())); else cout << "Warning: subtraction of matrices not same size" << endl;
	Matrix out(rows,cols);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			out.E(i,j,E(i,j)-inM.E(i,j));
		}
	}
	return out;
}

Matrix Matrix::operator*(double inD) const {
	Matrix out(rows, cols);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			out.E(i,j,E(i,j)*inD);
		}
	}
	return out;
}


Matrix Matrix::multiply(const Matrix& inM) const {
	Matrix* out = NULL;
	if(multCompatible(inM)) {
		int thisRows = rows;
		int thisCols = inM.getCols();
		Matrix tempM(thisRows,thisCols);
		for(int i = 0; i < thisRows; i++) {
			for(int j = 0; j < thisCols; j++) {
				Complex temp;
				for(int k = 0; k < cols; k++) {
					temp += E(i,k)*inM.E(k,j);
				}
				tempM.E(i,j,temp);
			}
		}
		out = new Matrix(tempM);
	} else {
		out = new Matrix(0,0);
	}
	Matrix outM(*out);

	delete out;
	return outM;
}

Matrix Matrix::operator*(const Matrix& inM) const {
	return multiply(inM);
}

Complex Matrix::determinant() const {
	if(!isSquare()) cout << "WARNING! Can't find determinant of a non-square matrix" << endl;
	Complex out;
	int order = rows;
	if(order == 2) {
		return E(0,0)*E(1,1) - E(1,0)*E(0,1);
	} else {
		for(int i = 0; i < order; i++) {
			Matrix temp(order-1,order-1);
			for(int j = 1; j < (order); j++) {
				int k_count = 0;
				for(int k = 0; k < (order); k++) {
					if(k!=i) {
						temp.E(j-1,k_count,E(j,k));
						k_count++;
					}
				}
			}
			out += E(0,i)*temp.determinant()*pow(-1.0,i);
		}
	}
	return out;
}

Matrix Matrix::transpose() const {
	Matrix out(cols, rows);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			out.E(j, i, E(i, j));
		}
	}
	return out;
}

Matrix Matrix::adjoint() const {
	Matrix out(cols, rows);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			out.E(j, i, E(i, j).conjugate());
		}
	}
	return out;
}

Matrix Matrix::tensorProduct(const Matrix& inM) const {
	Matrix out(rows*inM.getRows(), cols*inM.getCols());
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			for(int k = 0; k < inM.getRows(); k++) {
				for(int l = 0; l < inM.getCols(); l++) {
					out.E(i*inM.getRows()+k,j*inM.getCols()+l,E(i,j)*inM.E(k,l));
				}
			}
		}
	}
	return out;
}

Matrix Matrix::operator^(const Matrix& inM) const {
	return tensorProduct(inM);
}


Matrix Matrix::operator^=(const Matrix& inM) {
	Matrix temp = tensorProduct(inM);
	clone(temp);
	return (*this);
}


Matrix Matrix::identity(int inN) {
	Matrix out(inN, inN);
	for(int i = 0; i < inN; i++) out.E(i,i,1.0);
	return out;
}

Matrix Matrix::identity() {
	return identity(2);
}

Matrix Matrix::X() {
	Matrix out(2,2);
	out.E(0,1,1.0);
	out.E(1,0,1.0);
	return out;
}

Matrix Matrix::Y() {
	Matrix out(2,2);
	out.E(0,1,0.0,-1.0);
	out.E(1,0,0.0,1.0);
	return out;
}

Matrix Matrix::Z() {
	Matrix out(2,2);
	out.E(0,0,1.0);
	out.E(1,1,-1.0);
	return out;
}

Matrix Matrix::H() {
	Matrix out(2,2);
	out.E(0,0,1.0);
	out.E(0,1,1.0);
	out.E(1,0,1.0);
	out.E(1,1,-1.0);
	out = out*pow(2,-0.5);
	return out;
}

Matrix Matrix::S() {
    Matrix out(2,2);
    out.E(0,0,1.0);
    out.E(1,1,0.0,1.0);
    return out;
}

Matrix Matrix::T() {
    return R(L_PI/4.0);
}

Matrix Matrix::M() {
    Matrix out(2,2);
    out.E(0,0,1.0);
    return out;
}

Matrix Matrix::R(double theta) {
	while(theta >= 360.0) theta -= 360.0;
	Matrix out(2,2);
	out.E(0,0,1.0);
	if(theta==(L_PI/2.0)) {
		out.E(1,1,0.0,1.0);
	} else if(theta==(L_PI/4.0)) {
		double temp = sqrt(2)/2;
		out.E(1,1,temp,temp);
	} else if(theta==(L_PI)) {
		out.E(1,1,-1.0,0.0);
	} else if(theta==(3.0*L_PI/2.0)) {
		out.E(1,1,0.0,-1.0);
	} else if(theta==(3.0*L_PI/4.0)) {
		double temp = sqrt(2)/2;
		out.E(1,1,-temp,temp);
	} else if(theta==(5.0*L_PI/4.0)) {
		double temp = sqrt(2)/2;
		out.E(1,1,-temp,-temp);
	} else if(theta==(7.0*L_PI/4.0)) {
		double temp = sqrt(2)/2;
		out.E(1,1,temp,-temp);
	} else {
		out.E(1,1,cos(theta),sin(theta));
	}
	return out;
}

Matrix Matrix::CNOT() {
	Matrix out(4,4);
	out.E(0,0,1.0);
	out.E(1,1,1.0);
	out.E(3,2,1.0);
	out.E(2,3,1.0);
	return out;
}

Matrix Matrix::CMAT(const Matrix& inMat) {
	Matrix out(4,4);
	out.E(0,0,1.0);
	out.E(1,1,1.0);
	out.E(2,2,inMat.E(0,0));
	out.E(2,3,inMat.E(0,1));
	out.E(3,2,inMat.E(1,0));
	out.E(3,3,inMat.E(1,1));
	return out;
}

/*Matrix Matrix::T() {
	return transpose();
}*/

int Matrix::isVec() const {
	if((rows>1)&&(cols==1)) return 1;
	else if((rows==1)&&(cols>1)) return -1;
	else return 0;
}

double Matrix::magnitude() const {
	double out = 0.0;
	switch(isVec()) {
		case 1:
			for(int i = 0; i < rows; i++) {
				out += E(i,0).magnitude2();
			}
			return sqrt(out);
			break;
		case -1:
			for(int i = 0; i < rows; i++) {
				out += E(0,i).magnitude2();
			}
			return sqrt(out);
			break;
		default:
			cout << "ERROR! Can not find length of a non-vector matrix" << endl;
			return out;
			break;
	}
}

int Matrix::length() const {
	switch(isVec()) {
		case 1:
			return rows;
			break;
		case -1:
			return cols;
			break;
		default:
			cout << "ERROR! Can not find length of a non-vector matrix" << endl;
			return 0;
			break;
	}
}

Complex& Matrix::operator[](int index) {
	switch(isVec()) {
		case 1:
			return E(index,0);
			break;
		case -1:
			return E(0,index);
			break;
		default:
			cout << "ERROR! Can not find length of a non-vector matrix" << endl;
			throw "ERROR! Can not find length of a non-vector matrix";
			break;
	}
}

Complex Matrix::operator[](int index) const {
	switch(isVec()) {
		case 1:
			return E(index,0);
			break;
		case -1:
			return E(0,index);
			break;
		default:
			cout << "ERROR! Can not find length of a non-vector matrix" << endl;
			throw "ERROR! Can not find length of a non-vector matrix";
			break;
	}
}

Matrix Matrix::colVec(int len) {
	return Matrix(len,1);
}

Matrix Matrix::rowVec(int len) {
	return Matrix(1,len);
}

Matrix Matrix::X(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::X();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::X();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::Y(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::Y();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::Y();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::Z(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::Z();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::Z();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::H(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::H();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::H();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::S(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::S();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::S();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::T(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::T();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::T();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::M(int Nbit, int nth) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::M();
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::M();
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::R(int Nbit, int nth, double theta) {
	Matrix* outP = NULL;
	Matrix tempX = Matrix::R(theta);
	Matrix tempI = Matrix::identity();
	if(Nbit==nth) {
		outP = new Matrix(tempX);
	} else {
		outP = new Matrix(tempI);
	}
	for(int i = 1; i < Nbit; i++) {
		if(i == (Nbit-nth)) {
			Matrix temp = Matrix::R(theta);
			(*outP)^=temp;
		} else {
			Matrix temp = Matrix::identity();
			(*outP)^=temp;
		}
	}
	Matrix out(*outP);
	delete outP;
	return out;
}

Matrix Matrix::CNOT(int Nbit, int cbit, int obit) {
	int N = pow(2,Nbit);
	Matrix out(N,N);
	Matrix tempCNOT = Matrix::CNOT();
	for(int i = 0; i < N; i++) {
		bool binRows[Nbit];
		for(int k = 0; k < Nbit; k++) {
			binRows[k] = Utils::Bn(i,k);
		}
		for(int j = 0; j < N; j++) {
			bool binCols[Nbit];
			for(int k = 0; k < Nbit; k++) {
				binCols[k] = Utils::Bn(j,k);
			}
			Complex thisE = tempCNOT.E(2*binRows[cbit-1]+binRows[obit-1],2*binCols[cbit-1]+binCols[obit-1]);
			for(int k = 0; k < Nbit; k++) {
				if((k!=(cbit-1))&&(k!=(obit-1))) {
					thisE *= (double)(binRows[k]==binCols[k]);
				}
			}
			out.E(i, j, thisE);
		}
	}
	return out;
}

Matrix Matrix::CMAT(const Matrix& inMat, int Nbit, int cbit, int obit) {
	int N = pow(2,Nbit);
	Matrix out(N,N);
	Matrix tempCMAT = Matrix::CMAT(inMat);
	Complex thisE;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			thisE = tempCMAT.E(2*Utils::Bn(i,cbit-1)+Utils::Bn(i,obit-1),2*Utils::Bn(j,cbit-1)+Utils::Bn(j,obit-1));
			for(int k = 0; k < Nbit; k++) {
				if((k!=(cbit-1))&&(k!=(obit-1))) {
					thisE *= (double)(Utils::Bn(i,k)==Utils::Bn(j,k));
				}
			}
			out.E(i, j, thisE);
		}
	}
	return out;
}

Matrix Matrix::PERM(int Nbits, int lbit, int Mbits, int inA, int inC) {
	int N = pow(2,Nbits);
	Matrix out(N,N);
	for(int k = 0; k < N; k++) {
		if(!Utils::Bn(k,Mbits+lbit)) {
			out.E(k,k,1.0);
		} else {
			int f = 0;
			for(int m = 0; m < Mbits; m++) {
				f += Utils::Bn(k, m)*pow(2,m);
			}
			if(f>=inC) {
				out.E(k,k,1.0);
			} else {
				int An_modC = (int)pow(inA,pow(2,lbit)) % inC;
				int f_primed = An_modC*f % inC;
				int j = f_primed;
				for(int i = Mbits; i < Nbits; i++) {
					j += Utils::Bn(k, i)*pow(2,i);
				}
				out.E(j,k,1.0);
			}
		}
	}
	return out;
}

Matrix Matrix::nTensorM(const Matrix& inM, int inN) {
	Matrix out(inM);
	for(int i = 0; i < (inN-1); i++) {
		out^=inM;
	}
	return out;
}

Matrix Matrix::loadMatrix(const char* filename) {
	int inRows, inCols;
	ifstream myFile(filename);
	myFile >> inRows;
	myFile >> inCols;
	Matrix out(inRows, inCols);
	for(int i = 0; i < inRows; i++) {
		for(int j = 0; j < inCols; j++) {
			char thisStr[100];
			myFile >> thisStr;
			double thisReal, thisImag;
			char arbChar;
			stringstream tempSS;
			tempSS.str(thisStr);
			tempSS >> thisReal;
			tempSS >> arbChar;
			tempSS >> thisImag;
			out.E(i,j,thisReal,thisImag);
		}
	}
	return out;
}

Matrix Matrix::O(int Nbit, int nth) {
	int N = pow(2,Nbit);
	Matrix out = Matrix::identity(N);
	out.E(nth,nth,-1);
	return out;
}

Matrix Matrix::CNOT(int Nbit, int* quargs, int nargs) {
    // E.g. quargs = {1,5,6,7,0,0,0,0} for Toffoli-4 on 8-qubit Hilbert space with target of 1 and controls 5, 6 and 7
    int N = pow(2,Nbit);
    Matrix out(N,N);

    if(nargs==-1) nargs = Nbit;

    // First translate into t_ind and c_vec
    int t_ind = quargs[0]-1;
    bool c_vec[Nbit];
    for(int i = 0; i < Nbit; i++) c_vec[i] = 0;
    for(int i = 1; i < nargs; i++) {
        if(quargs[i]) {
            c_vec[quargs[i]-1] = 1;
        }
    }

    for(int i = 0; i < N; i++) {
        bool b[Nbit];

        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        bool flip = 1;
        for(int k = 0; (flip)&&(k < Nbit); k++) {
            if(c_vec[k]) {
                if(!b[k]) flip = 0;
            }
        }
        if(flip) {
            b[t_ind] = !b[t_ind];
        }
        int this_i = LCL_Bool::BoolVecToInt(b,Nbit);
        out.E(this_i,i,1);
    }

    return out;
}

Matrix Matrix::CS(int Nbit, int c1, int c2) {
    int N = pow(2,Nbit);
    Matrix out = Matrix::identity(N);

    for(int i = 0; i < N; i++) {
        bool b[Nbit];
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        if(b[c1-1]&&b[c2-1]) {
            out.E(i,i,0.0,1.0);
        }
    }

    return out;
}

Matrix Matrix::CZ(int Nbit, int c1, int c2) {
    int N = pow(2,Nbit);
    Matrix out = Matrix::identity(N);

    for(int i = 0; i < N; i++) {
        bool b[Nbit];
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        if(b[c1-1]&&b[c2-1]) {
            out.E(i,i,-1.0,0.0);
        }
    }

    return out;
}

Matrix Matrix::CCZ(int Nbit, int c1, int c2, int c3) {
    int N = pow(2,Nbit);
    Matrix out = Matrix::identity(N);

    for(int i = 0; i < N; i++) {
        bool b[Nbit];
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        for(int k = 0; k < Nbit; k++) {
            b[k] = Utils::Bn(i,k);
        }
        if(b[c1-1]&&b[c2-1]&&b[c3-1]) {
            out.E(i,i,-1.0);
        }
    }

    return out;
}

Matrix Matrix::operator*(const Complex& in) const {
    Matrix out(rows,cols);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            out.E(i,j,E(i,j)*in);
        }
    }

    return out;
}

Complex Matrix::trace() const {
    Complex out;

    for(int i = 0; i < getRows(); i++) {
        out += E(i,i);
    }

    return out;
}

Matrix Matrix::partialTrace(int q_i, Complex* tr_i) const {
    Matrix out(getRows()/2,getCols()/2);

    int n = log2(getRows());
    bool valid = isSquare()*(log2(getRows())==n)*(q_i>=1)*(q_i<=n);

    if(valid) {
        {
            Matrix tempR = Matrix::identity(1);
            Matrix this_basis = Matrix::colVec(2);
            Matrix this_id = Matrix::identity(2);
            this_basis.E(0,0,1);
            for(int i = 0; i < n; i++) {
                if(i==(n-q_i)) {
                    tempR ^= this_basis;
                } else {
                    tempR ^= this_id;
                }
            }
            Matrix tempL = tempR.adjoint();
            Matrix temp_result = (tempL*(*this)*tempR);

            out = (out + temp_result);
        }
        {
            Matrix tempR = Matrix::identity(1);
            Matrix this_basis = Matrix::colVec(2);
            Matrix this_id = Matrix::identity(2);
            this_basis.E(1,0,1);
            for(int i = 0; i < n; i++) {
                if(i==(n-q_i)) {
                    tempR ^= this_basis;
                } else {
                    tempR ^= this_id;
                }
            }
            Matrix tempL = tempR.adjoint();
            Matrix temp_result = (tempL*(*this)*tempR);

            out = (out + temp_result);
        }
    }

    return out;
}

Matrix Matrix::submatrix(int r_0, int c_0, int n_r, int n_c) const {
    Matrix out(n_r,n_c);

    for(int i = r_0; (i<n_r)&&(i<rows); i++) {
        for(int j = c_0; (j<n_c)&&(j<cols); j++) {
            out.E(i-r_0,j-c_0,E(i,j));
        }
    }

    return out;
}
