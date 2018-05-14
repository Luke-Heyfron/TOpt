#ifndef TESTS_HEADER
#define TESTS_HEADER

#include <iostream>
using namespace std;

#include "TO_Decoder.h"
#include "Signature.h"
#include "GateStringSparse.h"
#include "GateSigInterface.h"
#include "LCL_Maths.h"
#include "LCL_Bool.h"
#include "BoolMat.h"
#include "BMSparse.h"
#include "Interface_SigBMS.h"
#include "Interface_BMSGSS.h"
#include "LCL_Int.h"
#include "Bool_Signature.h"
#include "LCL_Mat_GF2.h"
#include "GateSynthesisMatrix.h"
#include "SQC_Circuit.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;
#include "PhasePolynomial.h"
#include "TO_Maps.h"
#include "WeightedPolynomial.h"
#include "Utils.h"
#include "TO_CircuitGenerators.h"

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

void test_TO_maps() {

    SQC_Circuit* blah = SQC_Circuit::LoadTFCFile("test.tfc");
    blah->ConvertFromToffoli();
    blah->n = 3;
    int gate[] = {SQC_OPERATOR_T,3};
    //blah->AddOperator(gate,2);
    blah->Print();

    PhasePolynomial blee = TO_Maps::SQC_Circuit_to_PhasePolynomial(*blah);
    blee.print();

    SQC_Circuit bloy = TO_Maps::PhasePolynomial_to_SQC_Circuit(blee);
    bloy.Print();

    WeightedPolynomial bewd = TO_Maps::PhasePolynomial_to_WeightedPolynomial(blee);
    bewd.print();

    //GateStringSparse bess = TO_Maps::PhasePolynomial_to_GateStringSparse(blee);
    //bess.print();

    PhasePolynomial brec = TO_Maps::WeightedPolynomial_to_PhasePolynomial(bewd);
    brec.print();

    Signature bsfa = TO_Maps::WeightedPolynomial_to_Signature(bewd);
    bsfa.print();
}

void test_GSS_set_boolvec(int n = 10, int d = 100) {
    GateStringSparse blah(n);
    for(int i = 0; i < d; i++) {
        bool x[n];
        for(int j = 0; j < n; j++) x[j] = Utils::rand_i(2);
        blah.set(x);
    }
    blah.print();
}

void test_RM(int n = 6, int d = 10) {
    Signature blah = generate_signature(n,d);
    cout << "blah" << endl;
    blah.print();

    GateStringSparse blee = ReedMullerSynthesis(blah);
    cout << "blee" << endl;
    blee.print();

    bool success = synthesis_success(blah,blee);
    cout << "success = " << success << endl;
}

void test_sig_to_GSS(int n = 10, int d = 4) {
    Signature blah = generate_signature(n,d);
    blah.print();
    GateStringSparse blee = GateSigInterface::SigToGSS(blah);
    blee.print();
    Signature recovered = GateSigInterface::expandGSSTerm(blee);
    recovered.print();
    bool success = (blah+recovered).isEmpty();
    cout << "Success = " << success << endl;
}

void test_GSS_add(int n = 10, int d = 4) {
    GateStringSparse blah = generate_GSS(n,d);
    cout << "blah:" << endl;
    blah.print();
    GateStringSparse blee = generate_GSS(n,d);
    cout << "blee:" << endl;
    blee.print();

    blah = blah + blee;
    cout << "blah + blee:" << endl;
    blah.print();
}

void test_expandLinQuadCube(int n = 10, int x = 1, int y = 2, int z = 3) {
    GateStringSparse blah = GateStringSparse::expandLin(n,x);
    blah.print();
    GateStringSparse blee = GateStringSparse::expandQuad(n,x,y);
    blee.print();
    GateStringSparse bloy = GateStringSparse::expandCube(n,x,y,z);
    bloy.print();
}

void test_GSS_PP_and_map_for_large_n(int n = 2000) {
    GateStringSparse blah(n);
    vector<bool> ind;
    ind.resize(n);
    ind[n-1] = 1;
    blah.set(ind);
    ind[0] = ind[1] = 1;
    blah.set(ind);
    ind[4] = ind[5] = 1;
    blah.set(ind);
    ind[8] = ind[9] = 1;
    blah.set(ind);
    blah.print();

    cout << "Weight = " << blah.weight(true) << endl;

    PhasePolynomial blee = TO_Maps::GateStringSparse_to_PhasePolynomial(blah);
    blee.print();
}

Signature generate_signature(int n, int d) {
    Signature blah(n);
    for(int i = 0; i < d; i++) {
        switch(Utils::rand_i(3)) {
        case 0:
            blah.set(Utils::rand_i(n)+1);
            break;
        case 1:
            blah.set(Utils::rand_i(n)+1,Utils::rand_i(n)+1);
            break;
        case 2:
            blah.set(Utils::rand_i(n)+1,Utils::rand_i(n)+1,Utils::rand_i(n)+1);
            break;
        }
    }
    return blah;
}

GateStringSparse generate_GSS(int n, int d) {
    GateStringSparse out(n);
    for(int i = 0; i < d; i++) {
        vector<bool> x(n);
        for(int j = 0; j < n; j++) x[j] = Utils::rand_i(2);
        out.set(x);
    }
    return out;
}

#endif // TESTS_HEADER
