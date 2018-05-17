#include "LCL/LCL_Mat_GF2.h"

#include <iostream>
#include <ostream>
using namespace std;

#include <cmath>
#include <cstdlib>

bool** LCL_Mat_GF2::construct(int n, int m) {
    bool** out = NULL;
    if(n>0) out = new bool*[n];
    for(int i = 0; i < n; i++) {
        if(m>0) {
            out[i] = new bool[m];
            for(int j = 0; j < m; j++) {
                out[i][j] = 0;
            }
        } else {
            out[i] = NULL;
        }
    }
    return out;
}

void LCL_Mat_GF2::destruct(bool** A, int n, int m) {
    if(A) {
        //cout << "Destructing outer" << endl;
        for(int i = 0; i < n; i++) {
            if(A[i]) {
                //for(int j = 0; j < m; j++) cout << A[i][j];
                //cout << endl << "Destructing inner " << i << endl;
                delete [] A[i];
                A[i] = NULL;
                //cout << "Inner destruction complete" << i << endl;
            }
        }
        delete [] A;
        A = NULL;
        //cout << "Outer destruction complete" << endl;
    }
}

void LCL_Mat_GF2::copy(bool const** A, int n, int m, bool** O) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            O[i][j] = A[i][j];
        }
    }
}

void LCL_Mat_GF2::print(bool const** A, int n, int m, char* pre, bool header, ostream& inOS) {
    if(pre) inOS << pre;
    if(header) inOS << n << " by " << m << " matrix over GF(2):" << endl;
    else if(pre) inOS << endl;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            inOS << A[i][j];
        }
        inOS << endl;
    }
    inOS << endl;
}

void LCL_Mat_GF2::add(bool const** A, bool const** B, int n, int m, bool** O) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            O[i][j] = (A[i][j]+B[i][j])%2;
        }
    }
}

void LCL_Mat_GF2::times(bool const** A, bool const** B, int n, int m, int p, bool** O) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            int sum = 0;
            for(int k = 0; k < m; k++) {
                sum += A[i][k]*B[k][j];
            }
            O[i][j] = sum%2;
        }
    }
}

void LCL_Mat_GF2::transpose(bool const** A, int n, int m, bool** O) {
    for(int i = 0; i < n; i++) {void addrow(bool** A, int n, int m, int i_t, int i_s);
        for(int j = 0; j < m; j++) {
            O[j][i] = A[i][j];
        }
    }
}

void LCL_Mat_GF2::addrow(bool** A, int n, int m, int i_t, int i_s) {
    for(int j = 0; j < m; j++) {
        A[i_t][j] = (A[i_t][j]+A[i_s][j])%2;
    }
}

void LCL_Mat_GF2::swaprow(bool** A, int n, int m, int i_1, int i_2) {
    for(int j = 0; j < m; j++) {
        bool temp = A[i_1][j];
        A[i_1][j] = A[i_2][j];
        A[i_2][j] = temp;
    }
}

void LCL_Mat_GF2::addcol(bool** A, int n, int m, int j_t, int j_s) {
    for(int i = 0; i < n; i++) {
        A[i][j_t] = (A[i][j_t]+A[i][j_s])%2;
    }
}

void LCL_Mat_GF2::swapcol(bool** A, int n, int m, int j_1, int j_2) {
    for(int i = 0; i < n; i++) {
        bool temp = A[i][j_1];
        A[i][j_1] = A[i][j_2];
        A[i][j_2] = temp;
    }
}

void LCL_Mat_GF2::rowechelon(bool** A, int n, int m) {
    int i_0 = 0;
    int j_0 = 0;
    while((i_0<n) && (j_0<m)) {
        // Search for non-zero element in column j_0
        bool found = 0;
        for(int i = i_0; (!found)&&(i < n); i++) {
            if(A[i][j_0]) {
                // If found swap i and i_0
                if(i!=i_0) LCL_Mat_GF2::swaprow(A,n,m,i,i_0);
                found = 1;
            }
        }
        if(found) {
            // Update the lower rows
            for(int i = (i_0+1); i < n; i++) {
                if(A[i][j_0]) {
                    LCL_Mat_GF2::addrow(A,n,m,i,i_0);
                }
            }
            i_0++;
        } else {
            j_0++;
        }
    }
}

bool** LCL_Mat_GF2::nullspace(bool const** A, int n, int m, int& out_d) {
    bool** out = NULL;

    bool** A_copy = NULL;
    A_copy = construct(n,m);
    LCL_Mat_GF2::copy(A,n,m, A_copy);
    LCL_Mat_GF2::rowechelon(A_copy, n, m);

    bool* S = NULL;
    S = new bool[m];
    for(int i = 0; i < m; i++) S[i] = 0;

    int A_rank = 0;
    for(int i = 0; i < n; i++) {
        bool found = 0;
        int j_found = -1;
        for(int j = 0; (!found)&&(j<m); j++) {
            found = A_copy[i][j];
            if(found) j_found = j;
        }
        if(found) {
            A_rank++;
            S[j_found] = 1;
        }
    }

    int d = (A_rank>0)?(m - A_rank):0;

    int* L = NULL;
    int* F = NULL;
    int* L_pos = NULL;
    L_pos = new int[m];
    int* F_pos = NULL;
    F_pos = new int[m];

    if(d*A_rank) {
        out = LCL_Mat_GF2::construct(m,d);

        L = new int[A_rank];
        F = new int[d];
        int L_count = 0;
        int F_count = 0;
        for(int j = 0; j < m; j++) {
            if(S[j]) {
                L[L_count] = j;
                L_pos[j] = L_count;
                F_pos[j] = -1;
                L_count++;
            } else {
                F[F_count] = j;
                F_pos[j] = F_count;
                L_pos[j] = -1;
                F_count++;
            }
        }

        for(int i = (m-1); i >= 0; i--) {
            if(!S[i]) {
                // If its a free parameter
                out[i][F_pos[i]] = 1;
            } else {
                // If its a leading 1 column
                for(int j = (A_rank-1); j > L_pos[i]; j--) {
                    if(A_copy[L_pos[i]][L[j]]) {
                        LCL_Mat_GF2::addrow(A_copy,n,m,L_pos[i],j);
                    }

                }
                for(int k = 0; k < d; k++) {
                    out[i][k] = A_copy[L_pos[i]][F[k]];
                }
            }
        }

    }


    LCL_Mat_GF2::destruct(A_copy,n,m);
    if(S) {
        delete [] S;
        S=NULL;
    }
    if(L) {
        delete [] L;
        L=NULL;
    }
    if(F) {
        delete [] F;
        F=NULL;
    }
    if(L_pos) {
        delete [] L_pos;
        L_pos = NULL;
    }
    if(F_pos) {
        delete [] F_pos;
        F_pos = NULL;
    }

    out_d = d;
    return out;
}

void LCL_Mat_GF2::eye(bool** A, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            A[i][j] = (i==j);
        }
    }
}

void LCL_Mat_GF2::zeros(bool** A, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            A[i][j] = 0;
        }
    }
}

void LCL_Mat_GF2::random(bool** A, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            A[i][j] = rand()%2;
        }
    }
}
