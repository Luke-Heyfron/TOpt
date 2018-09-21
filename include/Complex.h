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

#ifndef COMPLEX
#define COMPLEX

#include <string>

class Complex {
	private:
		double real, imaginary;
	public:
		//Construstors
		Complex();
		Complex(double, double);
		Complex(double*);

		//Getters + setters
		void setComplex(double, double);
		void setComplex(double*);
		void setReal(double);
		void setImaginary(double);
		double getReal();
		double getImaginary();
		void toArray(double*);

		//Member functions
		double magnitude();
		double magnitude2(); //Magnitude squared
		Complex conjugate();
		Complex multiply(Complex);
		Complex divide(Complex);

		//Output functions
		void print(); //Print the complex number to the standard out
		std::string toString();

		//Operator overload
			//Assignment
		void operator=(Complex);
		void operator=(double);
		Complex& operator=(const double*);
			//Multiplication
		Complex operator*(double);
		Complex operator*=(double);
		Complex operator*(Complex);
		Complex operator*=(Complex);
			//Division
		Complex operator/(double);
		Complex operator/=(double);
		Complex operator/(Complex);
		Complex operator/=(Complex);
			//Addition
		Complex operator+(double);
		Complex operator+=(double);
		Complex operator+(Complex);
		Complex operator+=(Complex);
			//Subtraction
		Complex operator-(double);
		Complex operator-=(double);
		Complex operator-(Complex);
		Complex operator-=(Complex);


		const static Complex NACN;
};

#endif
