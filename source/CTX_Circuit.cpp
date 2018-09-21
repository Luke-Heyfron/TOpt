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
