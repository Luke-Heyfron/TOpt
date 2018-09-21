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

#include "tests.h"

#include "SQC_Circuit.h"

void test_TO_Matrix(int n, int m, double p) {
    srand(time(NULL));
    TO_Matrix blah = generate_TO_Matrix(n,m,p);
    TO_Matrix blee(n,m);
    int t = 0;
    int s = 1;
    /*for(int i = 0; i < blah.r(); i++) {
        for(int j = 0; j < blah.c(); j++) {
            cout << blah(i,j);
        }
        cout << endl;
    }*/
    blah.print();
    add_rows(blah,t,s);
    blah.print();
    /*for(int i = 0; i < blah.r(); i++) {
        for(int j = 0; j < blah.c(); j++) {
            cout << blah(i,j);
        }
        cout << endl;
    }*/
}

void test_circuit(int n, int m, int d, bool v) {
    double weights[] = {40,20,5,35};
    double total_fidelity = 0.0;
    for(int i = 0; i < d; i++) {
        SQC_Circuit pre = generate_SQC_Clifford_T(n,m,weights);

        LOut() << "Pre-optimize circuit:" << endl;
        pre.Print();

        TO_Decoder this_decoder = TODD;

        SQC_Circuit post = SQC_Circuit::UniversalOptimize(pre,this_decoder);
        LOut() << "Post-optimize circuit:" << endl;
        post.Print();

        if(v) {
            total_fidelity += VerifyOptimization2(pre,post);
        }
    }
    total_fidelity /= d;
    LOut() << endl << "Average Fidelity over " << d << " trails = " << total_fidelity << endl << endl;
}

void test_TO_maps() {

    SQC_Circuit* blah = SQC_Circuit::LoadTFCFile("test.tfc");
    blah->ConvertFromToffoli();
    blah->n = 3;
    //int gate[] = {SQC_OPERATOR_T,3};
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

void test_GSS_set_boolvec(int n, int d) {
    GateStringSparse blah(n);
    for(int i = 0; i < d; i++) {
        bool x[n];
        for(int j = 0; j < n; j++) x[j] = Utils::rand_i(2);
        blah.set(x);
    }
    blah.print();
}

void test_RM(int n, int d) {
    Signature blah = generate_signature(n,d);
    cout << "blah" << endl;
    blah.print();

    GateStringSparse blee = ReedMullerSynthesis(blah);
    cout << "blee" << endl;
    blee.print();

    bool success = synthesis_success(blah,blee);
    cout << "success = " << success << endl;
}

void test_sig_to_GSS(int n, int d) {
    Signature blah = generate_signature(n,d);
    blah.print();
    GateStringSparse blee = GateSigInterface::SigToGSS(blah);
    blee.print();
    Signature recovered = GateSigInterface::expandGSSTerm(blee);
    recovered.print();
    bool success = (blah+recovered).isEmpty();
    cout << "Success = " << success << endl;
}

void test_GSS_add(int n, int d) {
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

void test_expandLinQuadCube(int n, int x, int y, int z) {
    GateStringSparse blah = GateStringSparse::expandLin(n,x);
    blah.print();
    GateStringSparse blee = GateStringSparse::expandQuad(n,x,y);
    blee.print();
    GateStringSparse bloy = GateStringSparse::expandCube(n,x,y,z);
    bloy.print();
}

void test_GSS_PP_and_map_for_large_n(int n) {
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

SQC_Circuit generate_SQC_Clifford_T(int n, int m, double* weights) {
    SQC_Circuit out(n,n);

    int gate_set[] = {SQC_OPERATOR_IDENTITY,
        SQC_OPERATOR_X,
        SQC_OPERATOR_Y,
        SQC_OPERATOR_HADAMARD,
        SQC_OPERATOR_CNOT,
        SQC_OPERATOR_T,
        SQC_OPERATOR_T_DAG,
        SQC_OPERATOR_CS,
        SQC_OPERATOR_CS_DAG,
        SQC_OPERATOR_CZ,
        SQC_OPERATOR_CCZ,
        SQC_OPERATOR_S,
        SQC_OPERATOR_S_DAG,
        SQC_OPERATOR_Z,
        SQC_OPERATOR_TOFFOLI,
        SQC_OPERATOR_TOFFOLI_4,
        SQC_OPERATOR_TOFFOLI_N,
        SQC_OPERATOR_PARTITION,
        SQC_OPERATOR_POST_0};
    int n_gates = SQC_OPERATOR_N;

    int this_gate[n];

    // Copy weights
    double w[n_gates];

    if(weights) {
        for(int i = 0; i < n_gates; i++) {
            w[i] = weights[i];
        }
    } else {
        for(int i = 0; i < n_gates; i++) {
            w[i] = 1.0;
        }
    }

    // Zero undesired gates
    w[SQC_OPERATOR_TOFFOLI_4] = 0.0;
    w[SQC_OPERATOR_IDENTITY] = 0.0;
    w[SQC_OPERATOR_PARTITION] = 0.0;
    w[SQC_OPERATOR_POST_0] = 0.0;
    if(n<3) {
        w[SQC_OPERATOR_TOFFOLI] = 0.0;
        w[SQC_OPERATOR_CCZ] = 0.0;
    }
    if(n<2) {
        w[SQC_OPERATOR_CNOT] = 0.0;
        w[SQC_OPERATOR_CZ] = 0.0;
        w[SQC_OPERATOR_CS] = 0.0;
        w[SQC_OPERATOR_CS_DAG] = 0.0;
    }

    // Normalize weights
    double sum = 0.0;
    for(int i = 0; i < n_gates; i++) {
        sum += w[i];
    }
    for(int i = 0; i < n_gates; i++) {
        w[i] /= sum;
    }

    while(out.m<m) {
        this_gate[0] = -1;
        double x = LCL_Utils::rand_d();
        for(int i = 0; (this_gate[0]==(-1))&&(i<n_gates); i++) {
            if(x<w[i]) this_gate[0] = gate_set[i];
            else x -= w[i];
        }
        switch(this_gate[0]) {
            // Single qubit gates
            case SQC_OPERATOR_X:
            case SQC_OPERATOR_Y:
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_HADAMARD:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_S_DAG:
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_T_DAG:
                this_gate[1] = (LCL_Utils::rand_i(n) + 1);
                out.AddOperator(this_gate,2);
                break;
            // Two qubit gates
            case SQC_OPERATOR_CNOT:
            case SQC_OPERATOR_CS:
            case SQC_OPERATOR_CS_DAG:
            case SQC_OPERATOR_CZ:
                this_gate[1] = (LCL_Utils::rand_i(n) + 1);
                this_gate[2] = (LCL_Utils::rand_i(n-1) + 1);
                if(this_gate[2]>=this_gate[1]) this_gate[2]++;
                out.AddOperator(this_gate,3);
                break;
            // Three qubit gates
            case SQC_OPERATOR_TOFFOLI:
            case SQC_OPERATOR_CCZ:
                {
                    for(int a = 0; a < 3; a++) {
                        this_gate[a+1] = (LCL_Utils::rand_i(n-a) + 1);
                        bool shift = 0;
                        do {
                            shift = 0;
                            for(int i = 0; (!shift)&&(i < a); i++) shift = (this_gate[a+1]==this_gate[i+1]);
                            if(shift) this_gate[a+1]++;
                        } while(shift);
                    }
                    out.AddOperator(this_gate,4);
                }
                break;
            // N qubit gates
            case SQC_OPERATOR_TOFFOLI_N:
                {
                    int n_args = (LCL_Utils::rand_i(n)+1);
                    for(int a = 0; a < n_args; a++) {
                        this_gate[a+1] = (LCL_Utils::rand_i(n-a) + 1);
                        bool shift = 0;
                        do {
                            shift = 0;
                            for(int i = 0; (!shift)&&(i < a); i++) shift = (this_gate[a+1]==this_gate[i+1]);
                            if(shift) this_gate[a+1]++;
                        } while(shift);
                    }
                    out.AddOperator(this_gate,n_args+1);
                }
                break;
        }
        out.simplify();
    }

    return out;
}

TO_Matrix generate_TO_Matrix(int n, int m, double p) {
    TO_Matrix out(n,m);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            out(i,j) = LCL_Utils::rand_d()<p;
        }
    }
    return out;
}
