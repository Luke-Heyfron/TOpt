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

#include <iostream>
#include "Complex.h"
#include <cmath>
#include <sstream>
#include <string>

using namespace std;

Complex::Complex() {
	real = 0.0;
	imaginary = 0.0;
}

Complex::Complex(double inR, double inI) {
	real = inR;
	imaginary = inI;
}

Complex::Complex(double* inArr) {
	real = inArr[0];
	imaginary = inArr[1];
}

void Complex::setComplex(double inR, double inI) {
	real = inR;
	imaginary = inI;
}

void Complex::setComplex(double* inArr) {
	real = inArr[0];
	imaginary = inArr[1];
}

void Complex::setReal(double inR) {
	real = inR;
}

void Complex::setImaginary(double inI) {
	imaginary = inI;
}

double Complex::getReal() {
	return real;
}

double Complex::getImaginary() {
	return imaginary;
}

void Complex::toArray(double* inArr) {
	inArr[0] = real;
	inArr[1] = imaginary;
}

double Complex::magnitude() {
	return sqrt(real*real + imaginary*imaginary);
}

double Complex::magnitude2() {
	return real*real + imaginary*imaginary;
}

Complex Complex::conjugate() {
	return Complex(real, (-1.0)*imaginary);
}

void Complex::print() {
	cout << "Complex number" << endl;
	cout << "\tvalue = " << real << " ";
	if(imaginary >= 0.0) {
		cout << "+";
	} else {
		cout << "-";
	}
	cout << " i*" << fabs(imaginary) << endl;
}

string Complex::toString() {
	string out;
	ostringstream convert;
	if(fabs(real) > 0.0) convert << real;

	if((fabs(real) > 0.0)&&(fabs(imaginary) > 0.0)) {
		convert << " ";
		if(imaginary >= 0.0) {
			convert << "+ ";
		} else {
			convert << "- ";
		}
	} else if(imaginary < 0.0) convert << "-";

	if(fabs(imaginary) > 0.0) {
		if(fabs(imaginary)==1.0) {
			convert << "i";
		} else {
			convert << "i*" << fabs(imaginary);
		}
	}
	out = convert.str();
	return out;
}

void Complex::operator=(Complex inC) {
	real = inC.getReal();
	imaginary = inC.getImaginary();
}

void Complex::operator=(double inR) {
	real = inR;
	imaginary = 0.0;
}

Complex Complex::multiply(Complex inC) {
	double newReal = real*inC.getReal() - imaginary*inC.getImaginary();
	double newImag = real*inC.getImaginary() + inC.getReal()*imaginary;
	return Complex(newReal, newImag);
}

Complex Complex::operator*(double inA) {
	return Complex(inA*this->getReal(), inA*this->getImaginary());
}

Complex Complex::operator*=(double inA) {
	real *= inA;
	imaginary *= inA;
	return *this;
}

Complex Complex::operator*(Complex inC) {
	return multiply(inC);
}

Complex Complex::operator*=(Complex inC) {
	Complex temp = multiply(inC);
	*this = temp;
	return *this;
}

Complex Complex::divide(Complex inC) {
	return multiply(inC.conjugate())/inC.magnitude2();
}

Complex Complex::operator/(double inD) {
	return (*this)*(1.0/inD);
}

Complex Complex::operator/=(double inD) {
	return (*this)*=(1.0/inD);
}

Complex Complex::operator/(Complex inC) {
	return divide(inC);
}

Complex Complex::operator/=(Complex inC) {
	*this = divide(inC);
	return *this;
}

Complex Complex::operator+(double inD) {
	return Complex(real+inD, imaginary);
}

Complex Complex::operator+=(double inD) {
	real += inD;
	return *this;
}

Complex Complex::operator+(Complex inC) {
	return Complex(real+inC.getReal(), imaginary+inC.getImaginary());
}

Complex Complex::operator+=(Complex inC) {
	real += inC.getReal();
	imaginary += inC.getImaginary();
	return *this;
}

Complex Complex::operator-(double inD) {
	return Complex(real-inD, imaginary);
}

Complex Complex::operator-=(double inD) {
	real -= inD;
	return *this;
}

Complex Complex::operator-(Complex inC) {
	return Complex(real-inC.getReal(), imaginary-inC.getImaginary());
}

Complex Complex::operator-=(Complex inC) {
	real -= inC.getReal();
	imaginary -= inC.getImaginary();
	return *this;
}

//Complex Complex::NACN(0,0);

Complex& Complex::operator=(const double* in) {
    real = in[0];
    imaginary = in[1];

    return (*this);
}
