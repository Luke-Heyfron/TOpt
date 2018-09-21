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

#ifndef CTX_CIRCUIT_HEADER
#define CTX_CIRCUIT_HEADER

#include "PhasePolynomial.h"
#include "TO_Matrix.h"

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

/**
    A class that represents a <CNOT,T,X> as a three-part data structure that consists of: a PhasePolynomial, f(x);
    an invertible binary matrix, E; and a binary column vector, b. These data completely describe the action of a <CNOT,T,X>
    unitary, U, in the following sense:
        U\ket{x} = \omega^{f(x)}\ket{Ex + b}.
**/
class CTX_Circuit {
    private:
        int _n;
        PhasePolynomial* _f_x = NULL;
        TO_Matrix _E;
        TO_Matrix _b;
    public:
        // Constructors & Destructors
        CTX_Circuit(int in_n);
        ~CTX_Circuit();

        // Accessors
        const PhasePolynomial& f_x() const {
            if(_f_x) return (*_f_x); error("Attempt to access non-initialized object of type PhasePolynomial", "f_x", "CTX_Circuit");
            throw "Attempt to access non-initialized object of type PhasePolynomial";
        }
        const TO_Matrix& E() const {return _E;}
        const TO_Matrix& b() const {return _b;}

        PhasePolynomial& f_x(){
            if(_f_x) return (*_f_x); error("Attempt to access non-initialized object of type PhasePolynomial", "f_x", "CTX_Circuit");
            throw "Attempt to access non-initialized object of type PhasePolynomial";
        }
        TO_Matrix& E() {return _E;}
        TO_Matrix& b() {return _b;}

        int n() const {return _n;};
};

#endif // CTX_CIRCUIT_HEADER
