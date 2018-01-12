#include "CNOTOptimize.h"

#include <iostream>
using namespace std;

#include "SQC_Circuit.h"
#include "BMSparse.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include "TO_Maps.h"

SQC_Circuit CNOTOptimize::CNOT_Synth(BMSparse& A, int m) {
    SQC_Circuit out;
    if(A.get_n()==A.get_m()) {
        int n = A.get_n();
        out.n = n;
        out.Construct();

        cout << "A:" << endl;
        A.printFull();

        BMSparse Acopy(A.get_n(),A.get_m());
        Acopy = A;

        // Synthesis lower/upper triangular part
        SQC_Circuit circuit_l = CNOTOptimize::Lwr_CNOT_Synth(A,m);
        A = A.T();
        SQC_Circuit circuit_u = CNOTOptimize::Lwr_CNOT_Synth(A,m);

        // combine lower/upper triangular synthesis
        for(int t = 0; t < circuit_u.m; t++) {
            swap(circuit_u.operator_list[t][1], circuit_u.operator_list[t][2]);
        }
        out += circuit_u;
        out += circuit_l;

        BMSparse Ap = TO_Maps::SQC_Circuit_to_BMSparse(out);
        cout << "Ap:" << endl;
        Ap.printFull();

        cout << "A + Ap = " << endl;
        (Acopy+Ap).printFull();
        bool success = !(bool)((Acopy+Ap).sum());
        cout << "success = " << success << endl;
    } else {
        error("Input matrix <A> must be square.", "CNOT_Synth", "CNOTOptimize");
    }

    return out;
}

SQC_Circuit CNOTOptimize::Lwr_CNOT_Synth(BMSparse& A, int m) {
    SQC_Circuit out;

    // A must be a square (invertible) matrix.
    if(A.get_n()==A.get_m()) {
        int n = A.get_n();
        out.n = n;
        out.Construct();
        int patt[(int)pow(2,m)];
        bool subrowpatt[m];
        for(int sec=1; sec <= ceil((double)n/m); sec++) {
            // Remove duplicate sub-rows in section sec
            for(int i = 0; i < (int)pow(2,m); i++) {
                patt[i] = -1; // Marker for first positions of sub-row patterns
            }
            for(int r = (sec-1)*m; r < n; r++) {
                //sub-row patt = A[row, (sec-1)*m:sec*m-1
                for(int j = 0; j < m; j++) subrowpatt[j] = A.E(r,(sec-1)*m+j);
                // if first copy of pattern save, otherwise remove
                int srp = LCL_Bool::BoolVecToInt(subrowpatt,m);
                if(patt[srp] == -1) {
                    patt[srp] = r;
                } else {
                    for(int j = 0; j < n; j++) A.E(r,j,(A.E(r,j)+A.E(patt[srp],j))%2);
                    int this_op[] = {SQC_OPERATOR_CNOT, (r+1), (patt[srp]+1)};
/*STEP A*/          out.AddOperator(this_op,3); // Reverse out once at the end.
                }
            }

            // Use Gaussian elimination for remaining entries for remaining entries in column section.
            for(int c = (sec-1)*m; c < (sec*m-1); c++) {
                // Check for 1 on diagonal
                bool diag_one = A.E(c,c);

                // Remove ones in rows below column col
                for(int r = (c+1); r < n; r++) {
                    if(A.E(r,c)) {
                        if(!diag_one) {
                            for(int j = 0; j < n; j++) {
                                A.E(c,j, (A.E(c,j)+A.E(r,j))%2);

                            }
                            int this_op[] = {SQC_OPERATOR_CNOT, (c+1), (r+1)};
/*STEP B*/                  out.AddOperator(this_op,3); // Reverse out once at the end.
                            diag_one = 1;
                        }
                        for(int j = 0; j < n; j++) {
                            A.E(r,j, (A.E(r,j)+A.E(c,j))%2);
                        }
                        int this_op[] = {SQC_OPERATOR_CNOT, (r+1), (c+1)};
/*STEP C*/              out.AddOperator(this_op,3); // Reverse out once at the end.
                    }
                }
            }
        }
    } else {
        error("Input matrix <A> must be square.", "Lwr_CNOT_Synth", "CNOTOptimize");
    }

    out.Copy(!out);

    return out;
}
