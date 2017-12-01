#include "TO_Maps.h"

#include <iostream>
using namespace std;

#include "PhasePolynomial.h"
#include "SQC_Circuit.h"
#include "GateStringSparse.h"
#include "WeightedPolynomial.h"

#include "BoolMat.h"
#include "LCL_Bool.h"
#include "LCL_Mat_GF2.h"
#include "LCL_ConsoleOut.h"
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
                }
                break;
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_T_DAG:
            case SQC_OPERATOR_S_DAG:
                {
                    int q = in.operator_list[t][1]-1;
                    bool x[n];
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

    bool x[n];
    for(int t = 0; t < in.T(); t++) {
        int this_m_t = in.get_m_at(t);
        while(this_m_t<0) this_m_t += 8;
        this_m_t %= 8;
        if(this_m_t!=0) {
            LCL_Bool::IntToBoolVec(x,in.get_a_at(t),n);
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

    return out;
}

GateStringSparse TO_Maps::PhasePolynomial_to_GateStringSparse(const PhasePolynomial& in) {
    int n = in.get_n();
    GateStringSparse out(n);

    for(int t = 0; t < in.T(); t++) {
        int this_m_t = in.get_m_at(t);
        int this_a_t = in.get_a_at(t);
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

    unordered_set<int> data = in.get_data();
    for(unordered_set<int>::iterator it = data.begin(); it != data.end(); it++) {
        int thisI = *it;
        out[thisI] = 1;
    }

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
    bool x[n];
    int j = 0;

    for(int t = 0; t < in.T(); t++) {
        int this_m = in.get_m_at(t);
        int this_a = in.get_a_at(t);
        while(this_m<0) this_m += 8;
        this_m %= 8;
        if(this_m) {
            LCL_Bool::IntToBoolVec(x,this_a,n);
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

    return out;
}


PhasePolynomial TO_Maps::WeightedPolynomial_to_PhasePolynomial(const WeightedPolynomial& in) {
    int n = in.get_n();
    PhasePolynomial out(n);

    bool x[n];

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
