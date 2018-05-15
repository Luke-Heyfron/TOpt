#include "CTX_Circuit.h"

CTX_Circuit::CTX_Circuit(int in_n) {
    _n = in_n;
    _f_x = new PhasePolynomial(_n);
    _E.resize(_n,_n);
    _b.resize(_n,1);

    TO_Matrix::identity(_E);
}

CTX_Circuit::~CTX_Circuit() {
    if(_f_x) delete _f_x;
    _f_x = NULL;
}
