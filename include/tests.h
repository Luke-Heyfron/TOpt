#ifndef TESTS_HEADER
#define TESTS_HEADER

#include <iostream>
using namespace std;

// TOpt Headers
#include "TO_Maps.h"
#include "TO_CircuitGenerators.h"
#include "TO_Decoder.h"
#include "TO_Matrix.h"
#include "Signature.h"
#include "GateStringSparse.h"
#include "GateSigInterface.h"
#include "BoolMat.h"
#include "BMSparse.h"
#include "Interface_SigBMS.h"
#include "Interface_BMSGSS.h"
#include "Bool_Signature.h"
#include "GateSynthesisMatrix.h"
#include "SQC_Circuit.h"
#include "CTX_Circuit.h"
#include "PhasePolynomial.h"
#include "WeightedPolynomial.h"
#include "Utils.h"

// Misc./Deprecated Headers
#include "Utils.h"
#include "BoolMat.h"
#include "BMSparse.h"

// LCL Headers
#include "LCL/Modules/LCL_Maths.h"
#include "LCL/LCL_Bool.h"
#include "LCL/LCL_Int.h"
#include "LCL/LCL_Mat_GF2.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;
#include "LCL/Core/LCL_Utils.h"

// Standard Library Headers
#include <climits>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <utility>
#include <climits>

Signature generate_signature(int n, int d);
GateStringSparse generate_GSS(int n, int d);
SQC_Circuit generate_SQC_Clifford_T(int n, int m, double* weights = NULL);
TO_Matrix generate_TO_Matrix(int n, int m, double p);

void test_TO_Matrix(int n=6, int m=20, double p=0.3);

void test_circuit(int n=4, int m=10, int d=1, bool v = true);

void test_TO_maps();

void test_GSS_set_boolvec(int n = 10, int d = 100);

void test_RM(int n = 6, int d = 10);

void test_sig_to_GSS(int n = 10, int d = 4);

void test_GSS_add(int n = 10, int d = 4);

void test_expandLinQuadCube(int n = 10, int x = 1, int y = 2, int z = 3);

void test_GSS_PP_and_map_for_large_n(int n = 2000);

Signature generate_signature(int n, int d);

GateStringSparse generate_GSS(int n, int d);

SQC_Circuit generate_SQC_Clifford_T(int n, int m, double* weights);



#endif // TESTS_HEADER
