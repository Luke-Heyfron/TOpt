#ifndef TO_MAPS_HEADER
#define TO_MAPS_HEADER

#include "PhasePolynomial.h"
#include "SQC_Circuit.h"
#include "GateStringSparse.h"
#include "WeightedPolynomial.h"
#include "Signature.h"
#include "Matrix.h"
#include "BMSparse.h"

namespace TO_Maps {
    // PhasePolynomial <-> SQC_Circuit
    PhasePolynomial SQC_Circuit_to_PhasePolynomial(const SQC_Circuit& in);
    SQC_Circuit PhasePolynomial_to_SQC_Circuit(const PhasePolynomial& in);
    SQC_Circuit PhasePolynomial_to_SQC_Circuit2(const PhasePolynomial& in);

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

    // SQC_Circuit(CNOTS ONLY) -> BMSparse
    BMSparse SQC_Circuit_to_BMSparse(const SQC_Circuit& in);

}

#endif // TO_MAPS_HEADER
