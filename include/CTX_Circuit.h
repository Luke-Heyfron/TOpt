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
