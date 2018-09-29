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

#ifndef LCL_FIELD_HEADER
#define LCL_FIELD_HEADER

#include <iostream>
using namespace std;

/**
    Derived classes should define static consts '<Derived class>_ZERO' and '<Derived class>_UNITY'.
    A derived class must implement:
        - operator=
        - operator+
        - operator-
        - operator*
        - operator~
        - operator==
    A derivate class should implement:
        - constructor: <derived class>(const LCL_Field&)
        - operator*(const LCL_Matrix& in) const {return in*(*this);}
*/

class LCL_Field {
    private:

    public:
        // Access value
        virtual operator int () const = 0;
        virtual operator double () const = 0;
        virtual operator bool () const = 0;

        // Assignment
        virtual LCL_Field& operator=(const LCL_Field& b) {return (*this);}
        virtual LCL_Field& operator=(int b) = 0;
        virtual LCL_Field& operator=(double b) = 0;
        virtual LCL_Field& operator=(bool b) = 0;
        /*virtual LCL_Field& operator+=(const LCL_Field& b) {return ((*this)=(*this)+b);}
        virtual LCL_Field& operator*=(const LCL_Field& b) {return ((*this)=(*this)*b);}
        virtual LCL_Field& operator-=(const LCL_Field& b) {return ((*this)=(*this)-b);}
        virtual LCL_Field& operator/=(const LCL_Field& b) {return ((*this)=(*this)/b);}*/

        // Arithmetic Operators
        /*virtual const LCL_Field& operator+(const LCL_Field& b) const = 0;
        virtual const LCL_Field& operator-() const = 0;
        virtual const LCL_Field& operator-(const LCL_Field& b) const {return (*this) + (-b);}
        virtual const LCL_Field& operator*(const LCL_Field& b) const = 0;
        virtual const LCL_Field& operator~() const = 0;    /// Multiplicative inverse
        virtual const LCL_Field& operator/(const LCL_Field& b) const {return (*this) * (~b);}*/

        // Comparison Operators
        virtual bool operator==(const LCL_Field& b) const = 0;
        virtual bool operator!=(const LCL_Field& b) const {return !operator==(b);}
};

// ostream& operator<<(ostream& os, const LCL_Field& f);

#endif // LCL_FIELD_HEADER
