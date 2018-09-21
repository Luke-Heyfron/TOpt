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

#ifndef TO_MAPS_HEADER
#define TO_MAPS_HEADER

#include "PhasePolynomial.h"
#include "SQC_Circuit.h"
#include "GateStringSparse.h"
#include "WeightedPolynomial.h"
#include "Signature.h"
#include "Matrix.h"
#include "CTX_Circuit.h"

namespace TO_Maps {
    // PhasePolynomial <-> SQC_Circuit
    PhasePolynomial SQC_Circuit_to_PhasePolynomial(const SQC_Circuit& in);
    PhasePolynomial SQC_Circuit_to_PhasePolynomial2(const SQC_Circuit& in);
    SQC_Circuit PhasePolynomial_to_SQC_Circuit(const PhasePolynomial& in);

    // PhasePolynomial <-> GateStringSparse
    GateStringSparse PhasePolynomial_to_GateStringSparse(const PhasePolynomial& in);
    PhasePolynomial GateStringSparse_to_PhasePolynomial(const GateStringSparse& in);

    // PhasePolynomial <-> WeightedPolynomial
    WeightedPolynomial PhasePolynomial_to_WeightedPolynomial(const PhasePolynomial& in);
    PhasePolynomial WeightedPolynomial_to_PhasePolynomial(const WeightedPolynomial& in);

    // WeightedPolynomia <-> Signature
    Signature WeightedPolynomial_to_Signature(const WeightedPolynomial& in);

    // SQC_Circuit -> Matrix
    Matrix SQC_Circuit_to_Matrix(const SQC_Circuit& in, int in_n = -1);

    // SQC_Circuit <-> CTX_Circuit
    CTX_Circuit SQC_Circuit_to_CTX_Circuit(const SQC_Circuit& in); /// ASSUMES that `in' is a <CNOT,T^k,X> circuit
    SQC_Circuit CTX_Circuit_to_SQC_Circuit(const CTX_Circuit& in);

}

#endif // TO_MAPS_HEADER
