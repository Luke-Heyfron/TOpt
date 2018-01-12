#ifndef CNOT_OPTIMIZE_HEADER
#define CNOT_OPTIMIZE_HEADER

#include <iostream>
using namespace std;

#include "SQC_Circuit.h"
#include "BMSparse.h"

namespace CNOTOptimize {
     // Following two functions optimize CNOT circuits correspond to those in algorithm 1 of Patel, Markov, Hayes (2008) Quant. Inf. Comput.
    SQC_Circuit CNOT_Synth(BMSparse& A, int in_m);
    SQC_Circuit Lwr_CNOT_Synth(BMSparse& A, int in_m); // NOTE: Changes value of A.
}

#endif // CNOT_OPTIMIZE_HEADER
