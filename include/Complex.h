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
