/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke E. Heyfron, Earl T. Campbell
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

#include "TO_Maps.h"

#include <iostream>
using namespace std;

#include "PhasePolynomial.h"
#include "SQC_Circuit.h"
#include "GateStringSparse.h"
#include "WeightedPolynomial.h"

#include "BoolMat.h"
#include "LCL/LCL_Bool.h"
#include "LCL/LCL_Mat_GF2.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

PhasePolynomial TO_Maps::SQC_Circuit_to_PhasePolynomial(const SQC_Circuit& in) {
    int n = in.n;
    PhasePolynomial out(n);

    BoolMat E_mat(n,n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            E_mat.E(i,j) = (i==j);
        }
    }

    for(int t = 0; t < in.m; t++) {
        switch(in.operator_list[t][0]) {
            case SQC_OPERATOR_CNOT:
                {
                    int c = in.operator_list[t][2]-1;
                    int d = in.operator_list[t][1]-1;
                    BoolMat this_CNOT(n,n);
                    for(int i = 0; i < n; i++) {
                        for(int j = 0; j < n; j++) {
                            this_CNOT.E(i,j) = ((i==j)+(i==d)*(j==c))%2;
                        }
                    }
                    E_mat = (this_CNOT*E_mat);
					/*if(false&&((c>=31)||(d>=31))) {
						cout << "CNOT INVOLVING QUBIT 32 OR GREATER" << endl;
						E_mat.print();
					}*/
                }
                break;
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_T_DAG:
            case SQC_OPERATOR_S_DAG:
                {
                    int q = in.operator_list[t][1]-1;
                    //bool x[n];
                    bool* x = new bool[n];
                    for(int j = 0; j < n; j++) x[j] = E_mat.E(q,j);
                    int m = 0;
                    switch(in.operator_list[t][0]) {
                        case SQC_OPERATOR_Z:
                            m = 4;
                            break;
                        case SQC_OPERATOR_S:
                            m = 2;
                            break;
                        case SQC_OPERATOR_T:
                            m = 1;
                            break;
                        case SQC_OPERATOR_T_DAG:
                            m = 7;
                            break;
                        case SQC_OPERATOR_S_DAG:
                            m = 6;
                            break;
                    }
                    out[x] += m;
					/*out.print();
					for(int j = 0; j < n; j++) cout << x[j];
					cout << endl;
					cout << endl;*/
					delete [] x;
					x = NULL;
                }
                break;
            default:
                cout << "Warning in TO_Maps::PhasePolynomial_to_SQC_Circuit! Expected gates are {CNOT, Z, S, T} only." << endl;
                break;
        }
    }

    out %= 8;

	//out.print();

	//cout << "ADFADFASDF" << endl;

    return out;
}

PhasePolynomial TO_Maps::SQC_Circuit_to_PhasePolynomial2(const SQC_Circuit& in) {
    int n = in.n;
    PhasePolynomial out(n);

    BMSparse E_mat(n,n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            E_mat.E(i,j,(i==j));
        }
    }

    for(int t = 0; t < in.m; t++) {
        switch(in.operator_list[t][0]) {
            case SQC_OPERATOR_CNOT:
                {
                    int c = in.operator_list[t][2]-1;
                    int d = in.operator_list[t][1]-1;
                    BMSparse this_CNOT(n,n);
                    for(int i = 0; i < n; i++) {
                        for(int j = 0; j < n; j++) {
                            this_CNOT.E(i,j,((i==j)+(i==d)*(j==c))%2);
                        }
                    }
                    E_mat = (this_CNOT*E_mat);
                }
                break;
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_T_DAG:
            case SQC_OPERATOR_S_DAG:
                {
                    int q = in.operator_list[t][1]-1;
                    //bool x[n];
                    bool* x = new bool[n];
                    for(int j = 0; j < n; j++) x[j] = E_mat.E(q,j);
                    int m = 0;
                    switch(in.operator_list[t][0]) {
                        case SQC_OPERATOR_Z:
                            m = 4;
                            break;
                        case SQC_OPERATOR_S:
                            m = 2;
                            break;
                        case SQC_OPERATOR_T:
                            m = 1;
                            break;
                        case SQC_OPERATOR_T_DAG:
                            m = 7;
                            break;
                        case SQC_OPERATOR_S_DAG:
                            m = 6;
                            break;
                    }
                    out[x] += m;
                    delete [] x;
                    x = NULL;
                }
                break;
            default:
                cout << "Warning in TO_Maps::PhasePolynomial_to_SQC_Circuit! Expected gates are {CNOT, Z, S, T} only." << endl;
                break;
        }
    }

    out %= 8;

    return out;
}

SQC_Circuit TO_Maps::PhasePolynomial_to_SQC_Circuit(const PhasePolynomial& in) {
    int n = in.get_n();
    SQC_Circuit out(n);

    //bool x[n];
    bool* x = new bool[n];
    for(int t = 0; t < in.T(); t++) {
        int this_m_t = in.get_m_at(t);
        while(this_m_t<0) this_m_t += 8;
        this_m_t %= 8;
        if(this_m_t!=0) {
            in.get_a_at(x,t);
            int i_0 = -1;
            for(int i = 0; (i_0==(-1))&&(i < n); i++) if(x[i]) i_0 = i;
            if(i_0>=0) {
                for(int i = (i_0 + 1); i < n; i++) {
                    if(x[i]) {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, i_0 + 1, i+1};
                        out.AddOperator(this_CNOT,3);
                    }
                }
                if(this_m_t%2) {
                    int this_T[] = {SQC_OPERATOR_T,i_0+1};
                    out.AddOperator(this_T,2);
                }
                this_m_t /= 2;
                if(this_m_t%2) {
                    int this_S[] = {SQC_OPERATOR_S,i_0+1};
                    out.AddOperator(this_S,2);
                }
                this_m_t /= 2;
                if(this_m_t%2) {
                    int this_Z[] = {SQC_OPERATOR_Z,i_0+1};
                    out.AddOperator(this_Z,2);
                }
                for(int i = (n-1); i > i_0; i--) {
                    if(x[i]) {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, i_0 + 1, i+1};
                        out.AddOperator(this_CNOT,3);
                    }
                }
            }
        }
    }

    delete [] x;
    x = NULL;

    return out;
}

GateStringSparse TO_Maps::PhasePolynomial_to_GateStringSparse(const PhasePolynomial& in) {
    int n = in.get_n();
    GateStringSparse out(n);

    for(int t = 0; t < in.T(); t++) {
        int this_m_t = in.get_m_at(t);
        vector<bool> this_a_t = in.get_a_at(t);
        while(this_m_t<0) this_m_t+=2;
        this_m_t%=2;
        if(this_m_t%2) {
            if(out.E(this_a_t)) {
                out.clear(this_a_t);
            } else {
                out.set(this_a_t);
            }
        }
    }

    return out;
}

PhasePolynomial TO_Maps::GateStringSparse_to_PhasePolynomial(const GateStringSparse& in) { // Quite expensive
    int n = in.get_n();
    PhasePolynomial out(n);

	vector<vector<bool>> data = in.get_data();

	for(int t = 0; t < (int)data.size(); t++) {
		out[data[t]]=1;
	}

    /*unordered_set<int> data = in.get_data();
    for(unordered_set<int>::iterator it = data.begin(); it != data.end(); it++) {
        int thisI = *it;
        out[thisI] = 1;
    }*/

    return out;
}

WeightedPolynomial TO_Maps::PhasePolynomial_to_WeightedPolynomial(const PhasePolynomial& in) {
    int n = in.get_n();
    WeightedPolynomial out(n);

    int m = 0;
    for(int t = 0; t < in.T(); t++) {
        int this_m = in.get_m_at(t);
        while(this_m<0) this_m += 8;
        this_m %= 8;
        m += this_m;
    }
    bool** A = LCL_Mat_GF2::construct(n,m);
    //bool x[n];
    bool* x = new bool[n];
    int j = 0;

    for(int t = 0; t < in.T(); t++) {
        int this_m = in.get_m_at(t);
        //int this_a = in.get_a_at(t);
        while(this_m<0) this_m += 8;
        this_m %= 8;
        if(this_m) {
            //LCL_Bool::IntToBoolVec(x,this_a,n);
			in.get_a_at(x,t);
            for(int k = 0; k < this_m; k++) {
                for(int i = 0; i < n; i++) {
                    A[i][j] = x[i];
                }
                j++;
            }
        }
    }

    for(int a = 0; a < n; a++) {
        for(int t = 0; t < m; t++) {
            out(a+1) += A[a][t];
        }
        for(int b = (a+1); (a<(n-1))&&(b < n); b++) {
            for(int t = 0; t < m; t++) {
                out(a+1,b+1) += (A[a][t]*A[b][t]);
            }
            for(int c = (b+1); (a<(n-2))&&(b<(n-1))&&(c < n); c++) {
                for(int t = 0; t < m; t++) {
                    out(a+1,b+1,c+1) += (A[a][t]*A[b][t]*A[c][t]);
                }
            }
        }
    }

    LCL_Mat_GF2::destruct(A,n,m);

    out %= 8;

    delete [] x;
    x = NULL;

    return out;
}


PhasePolynomial TO_Maps::WeightedPolynomial_to_PhasePolynomial(const WeightedPolynomial& in) {
    int n = in.get_n();
    PhasePolynomial out(n);

    //bool x[n];
    bool* x = new bool[n];

    {
        int N_i;
        in.dims_l(&N_i);
        for(int i = 0; i < N_i; i++) {
            int a;
            in.ijk_to_abc(i,0,0,&a);
            //
            for(int t = 0; t < n; t++) x[t] = (t==(a-1));
            out[x] += in(a);
        }
    }
    {
        int N_i,N_j;
        in.dims_q(0,&N_i);
        for(int i = 0; i < N_i; i++) {
            in.dims_q(i,NULL,&N_j);
            int a;
            for(int j = 0; j < N_j; j++) {
                int b;
                in.ijk_to_abc(i,j,0,&a,&b);
                //
                for(int t = 0; t < n; t++) x[t] = (t==(a-1));
                out[x] += in(a,b);
                for(int t = 0; t < n; t++) x[t] = (t==(b-1));
                out[x] += in(a,b);
                for(int t = 0; t < n; t++) x[t] = (t==(a-1))||(t==(b-1));
                out[x] -= in(a,b);
            }


        }
    }
    {
        int N_i,N_j,N_k;
        in.dims_c(0,0,&N_i);
        for(int i = 0; i < N_i; i++) {
            in.dims_c(i,0,NULL,&N_j);
            int a;
            for(int j = 0; j < N_j; j++) {
                in.dims_c(i,j,NULL,NULL,&N_k);
                int b;
                for(int k = 0; k < N_k; k++) {
                    int c;
                    in.ijk_to_abc(i,j,k,&a,&b,&c);
                    //
                    for(int t = 0; t < n; t++) x[t] = (t==(a-1));
                    out[x] += in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(b-1));
                    out[x] += in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(c-1));
                    out[x] += in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(a-1))||(t==(b-1));
                    out[x] -= in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(b-1))||(t==(c-1));
                    out[x] -= in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(c-1))||(t==(a-1));
                    out[x] -= in(a,b,c);
                    for(int t = 0; t < n; t++) x[t] = (t==(a-1))||(t==(b-1))||(t==(c-1));
                    out[x] += in(a,b,c);
                }
            }
        }
    }



    out %= 8;

    delete [] x;
    x = NULL;

    return out;
}

Signature TO_Maps::WeightedPolynomial_to_Signature(const WeightedPolynomial& in) {
    int n = in.get_n();
    Signature out(n);

    for(int i = 1; i <= n; i++) {
        int this_l = in(i);
        while(this_l<0) this_l += 8;
        this_l %= 8;
        if(this_l%2) out.set(i);
        if(i<n) {
            for(int j = (i+1); j <= n; j++) {
                int this_q = in(i,j);
                while(this_q<0) this_q += 8;
                this_q %= 8;
                if(this_q%2) out.set(i,j);
                if(j<n) {
                    for(int k = (j+1); k <= n; k++) {
                        int this_c = in(i,j,k);
                        while(this_c<0) this_c += 8;
                        this_c %= 8;
                        if(this_c%2) out.set(i,j,k);
                    }
                }
            }
        }

    }

    return out;
}

Matrix TO_Maps::SQC_Circuit_to_Matrix(const SQC_Circuit& in, int in_n) {
    LOut() << "Calculating unitary for circuit..." << endl;
    LOut_Pad++;
    int n = in.n;
    if(in_n!=-1) n = in_n;
    int N = (int)pow(2,n);
    if(!N) N = 1;
    Matrix out = Matrix::identity(N);

    for(int t = 0; t < in.m; t++) {
        LOut() << "Converting gate " << (t+1) << " of " << in.m << endl;
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_X:
                {
                    Matrix this_mat = Matrix::X(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_Y:
                {
                    Matrix this_mat = Matrix::Y(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_HADAMARD:
                {
                    Matrix this_mat = Matrix::H(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_CNOT:
                {
                    Matrix this_mat = Matrix::CNOT(n,this_op[2],this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_T:
                {
                    Matrix this_mat = Matrix::T(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_T_DAG:
                {
                    Matrix this_mat = Matrix::T(n,this_op[1]);
                    this_mat = (this_mat.adjoint());
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_CS:
                {
                    Matrix this_mat = Matrix::CS(n,this_op[1],this_op[2]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_CS_DAG:
                {
                    Matrix this_mat = Matrix::CS(n,this_op[1],this_op[2]);
                    this_mat = (this_mat.adjoint());
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_CZ:
                {
                    Matrix this_mat = Matrix::CZ(n,this_op[1],this_op[2]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_CCZ:
                {
                    Matrix this_mat = Matrix::CCZ(n,this_op[1],this_op[2],this_op[3]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_S:
                {
                    Matrix this_mat = Matrix::S(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_S_DAG:
                {
                    Matrix this_mat = Matrix::S(n,this_op[1]);
                    this_mat = (this_mat.adjoint());
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_Z:
                {
                    Matrix this_mat = Matrix::Z(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_TOFFOLI:
                {
                    int* temp_op = this_op;
                    temp_op++;
                    Matrix this_mat = Matrix::CNOT(n,temp_op,3);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_TOFFOLI_N:
                {
                    int* temp_op = this_op;
                    temp_op++;
                    Matrix this_mat = Matrix::CNOT(n,temp_op);
                    out = (this_mat*out);
                }
                break;
            case SQC_OPERATOR_POST_0:
                {
                    Matrix this_mat = Matrix::M(n,this_op[1]);
                    out = (this_mat*out);
                }
                break;
        }
    }
    LOut_Pad--;
    LOut() << "Conversion complete." << endl;

    return out;
}

CTX_Circuit TO_Maps::SQC_Circuit_to_CTX_Circuit(const SQC_Circuit& in) {
    // ASSUMES that `in' is a <CNOT,T^k,X> circuit
    int n = in.n;
    CTX_Circuit out(n);

    // Allocate a matrix to represent the action of a CNOT on the c.b.s.
    TO_Matrix r(n,n);
    int c,d,p,q,k;

    // Iterate through every gate in the input SQC_Circuit
    for(int t = 0; t < in.m; t++) {
        //cout << "t = " << t << endl;
        SQC_Operator this_op = in.operator_list[t];



        // Query the gate type
        switch(this_op[0]) {
            case SQC_OPERATOR_CNOT:
                {
                    // The control qubit
                    c = (this_op[2]-1);

                    // The target qubit
                    d = (this_op[1]-1);

                    // Set elements of r for this CNOT gate
                    R(r,c,d);

                    // Apply update rules for E and b
                    out.E() = (r*out.E());
                    out.b() = (r*out.b());
                    //cout << "HADGA" << endl;
                    //out.E().print();
                }
                break;
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_T_DAG:
            case SQC_OPERATOR_S_DAG:
                {
                    p = (this_op[1]-1);
                    vector<bool> x(n);
                    for(int i = 0; i < n; i++) x[i] = out.E()(p,i);
                    switch(this_op[0]) {
                        case SQC_OPERATOR_T:
                            k = 1;
                            break;
                        case SQC_OPERATOR_T_DAG:
                            k = 7;
                            break;
                        case SQC_OPERATOR_S:
                            k = 2;
                            break;
                        case SQC_OPERATOR_S_DAG:
                            k = 6;
                            break;
                        case SQC_OPERATOR_Z:
                            k = 4;
                            break;
                        default:
                            error("Unexpected gate.", "SQC_Circuit_to_CTX_Circuit", "TO_Maps");
                            LOut() << "Gate Index = " << this_op[0] << endl;
                            break;
                    }
                    out.f_x()[x] += ((int)out.b()(p,0))?(7*k):k;
                }
                break;
            case SQC_OPERATOR_X:
                {
                    q = (this_op[1]-1);
                    out.b()(q,0) += F2(1);
                }
                break;
            default:
                error("Unexpected gate.", "SQC_Circuit_to_CTX_Circuit", "TO_Maps");
                LOut() << "Gate Index = " << this_op[0] << endl;
                break;
        }
        /*out.f_x().print();
        out.E().print();
        out.b().print();*/
    }

    return out;
}

SQC_Circuit TO_Maps::CTX_Circuit_to_SQC_Circuit(const CTX_Circuit& in) {
    int n = in.n();
    SQC_Circuit out(n);

    SQC_Circuit D3 = TO_Maps::PhasePolynomial_to_SQC_Circuit(in.f_x());
    SQC_Circuit CNOTs(n);
    SQC_Circuit PauliXs(n);

    // Row echelon the E matrix, adding CNOTs to CNOTs as you go
    TO_Matrix E(n,n);
    E = in.E();
    TO_Matrix r(n,n);
    /*cout << "E before start" << endl;
    E.print();*/
    for(int j = 0; j < E.c(); j++) {
        // Make E(j,j) = 1
        if((int)E(j,j)!=1) {
            bool found = 0;
            for(int i = (j+1); (!found)&&(i < E.r()); i++) {
                if((int)E(i,j)==1) {
                    found = 1;
                    //cout << "Pivot found: Adding row " << i << " to row " << j << endl;
                    // Perform row addition R(i,j) and add a CNOT(i,j) gate to CNOT1
                    R(r,i,j);
                    E = r*E;
                    /*cout << "E = " << endl;
                    E.print();*/
                    int this_gate[] = {SQC_OPERATOR_CNOT,(j+1),(i+1)};
                    CNOTs.AddOperator(this_gate,3);
                }
            }
        }

        // Now, make all other E(*,j) = 0
        for(int i = 0; i < E.r(); i++) {
            if((i!=j)&&((int)E(i,j)==1)) {
                //cout << "Non-zero found: Adding row " << j << " to row " << i << endl;
                // Perform row addition R(i,j) and add a CNOT(j,i) gate to CNOT1
                R(r,j,i);
                E = r*E;
                /*cout << "E = " << endl;
                E.print();*/
                int this_gate[] = {SQC_OPERATOR_CNOT,(i+1),(j+1)};
                CNOTs.AddOperator(this_gate,3);
            }
        }
        /*cout << "E at end j = " << j << endl;
        E.print();*/
    }
    /*cout << "THIS E" << endl;
    E.print();*/

    // Add a Pauli X for each non-zero element of b
    for(int i = 0; i < n; i++) {
        if((int)in.b()(i,0)) {
            int this_gate[] = {SQC_OPERATOR_X, (i+1)};
            PauliXs.AddOperator(this_gate,2);
        }
    }

    for(int t = 0; t < D3.m; t++) {
        out.AddOperator(D3.operator_list[t]);
    }
    for(int t = (CNOTs.m-1); t >= 0; t--) {
        out.AddOperator(CNOTs.operator_list[t]);
    }
    for(int t = 0; t < PauliXs.m; t++) {
        out.AddOperator(PauliXs.operator_list[t]);
    }

    out.simplify();

    return out;
}
