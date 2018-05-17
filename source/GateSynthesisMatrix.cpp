#include "GateSynthesisMatrix.h"

#include <iostream>
using namespace std;

#include <cmath>
#include "LCL/LCL_Mat_GF2.h"
#include "LCL/Core/LCL_ConsoleOut.h"
#include "LCL/LCL_Int.h"
using namespace LCL_ConsoleOut;

bool** GateSynthesisMatrix::from_signature(bool*** S, int n, int& mp) {
    int N = (int)pow(2,n);
    bool* a = new bool[N];
    for(int i = 0; i < N; i++) a[i] = 0;

    int I;
    for(int i = 0; i < n; i++) {
        if(S[i][i][i]) {
            I = (int)pow(2,i);
            a[I] = !a[I];
        }
        if(i<(n-1)) {
            for(int j = (i+1); j < n; j++) {
                if(S[i][j][j]) {
                    I = (int)pow(2,i);
                    a[I] = !a[I];
                    I = (int)pow(2,j);
                    a[I] = !a[I];
                    I = (int)pow(2,i)+(int)pow(2,j);
                    a[I] = !a[I];
                }
                if(j<(n-1)) {
                    for(int k = (j+1); k < n; k++) {
                        if(S[i][j][k]) {
                            I = (int)pow(2,i);
                            a[I] = !a[I];
                            I = (int)pow(2,j);
                            a[I] = !a[I];
                            I = (int)pow(2,k);
                            a[I] = !a[I];
                            I = (int)pow(2,i)+(int)pow(2,j);
                            a[I] = !a[I];
                            I = (int)pow(2,i)+(int)pow(2,k);
                            a[I] = !a[I];
                            I = (int)pow(2,j)+(int)pow(2,k);
                            a[I] = !a[I];
                            I = (int)pow(2,i)+(int)pow(2,j)+(int)pow(2,k);
                            a[I] = !a[I];
                        }
                    }
                }
            }
        }
    }
    int m = 0;
    for(int i = 0; i < N; i++) {
        if(a[i]) {
            m++;
        }
    }
    bool** out = LCL_Mat_GF2::construct(n,m);
    int j = 0;
    for(int I = 0; I < N; I++) {
        if(a[I]) {
            int Ip = I;
            int i = 0;
            while(Ip>0) {
                out[i][j] = (Ip%2);
                Ip /= 2;
                i++;
            }
            j++;
        }
    }

    delete [] a;

    mp = m;
    return out;
}

void GateSynthesisMatrix::cleanup(bool** A, int n, int m, int& mp) {
    for(int j1 = 0; j1 < (m-1); j1++) {
        for(int j2 = (j1+1); j2 < m; j2++) {
            bool same = 1;
            for(int i = 0; same&&(i < n); i++) {
                same *= (A[i][j1]==A[i][j2]);
            }
            if(same) {
                for(int i = 0; i < n; i++) {
                    A[i][j1]=0;
                    A[i][j2]=0;
                }
            }
        }
    }

    int j_end = (m-1);
    int non_zero_count = 0;
    for(int j = 0; j < m; j++) {
        int sum = 0;
        for(int i = 0; i < n; i++) {
            sum += A[i][j];
        }
        if(!sum) {
            bool found = false;
            while((!found)&&(j_end>j)) {
                for(int i = 0; (!found)&&(i < n); i++) {
                    found = A[i][j_end];
                }
                if(!found) j_end--;
            }
            if(found) {
                LCL_Mat_GF2::swapcol(A,n,m,j,j_end);
                non_zero_count++;
            }
        } else {
            non_zero_count++;
        }
    }
    mp = non_zero_count;
}

void GateSynthesisMatrix::LempelX(bool** A, int n, int m, int& omp) {
    int this_m = m;
    bool** x = LCL_Mat_GF2::construct(n,1);
    bool** nv = LCL_Mat_GF2::construct(1,m+1);
    bool** xnv = LCL_Mat_GF2::construct(n,m+1);
    bool** A_xnv = LCL_Mat_GF2::construct(n,m+1);
    int n_ext = n+((n*(n-1)*(n-2))/6);
    bool** A_ext = LCL_Mat_GF2::construct(n_ext,m+1);

    /*bool __x[n][1];
    bool __nv[1][m+1];
    bool __xnv[n][m+1];
    bool __A_xnv[n][m+1];
    bool __A_ext[n_ext][m+1];

    bool* _x = (bool*)&__x;
    bool* _nv = (bool*)&__nv;
    bool* _xnv = (bool*)&__xnv;
    bool* _A_xnv = (bool*)&__A_xnv;
    bool* _A_ext = (bool*)&__A_ext;

    bool** x = (bool**)&_x;
    bool** nv = (bool**)&_nv;
    bool** xnv = (bool**)&_xnv;
    bool** A_xnv = (bool**)&_A_xnv;
    bool** A_ext = (bool**)&_A_ext;*/

    bool found = 1;
    int round = 0;
    while(found&&(round<m)) {
        //cout << "A for round " << round << ":" << endl;
        //LCL_Mat_GF2::print(A,n,this_m);
        found = 0;
        LOut(); cout << "Round = " << round << endl;
        LCL_Mat_GF2::copy((const bool**)A,n,this_m,A_ext);
		int col_perm[this_m]; for(int i = 0; i < this_m; i++) col_perm[i]=i;
		LCL_Int::randperm(col_perm,this_m);
        for(int j1_ind = 0; (!found)&&(j1_ind < (this_m-1)); j1_ind++) {
            for(int j2_ind = (j1_ind+1); (!found)&&(j2_ind < this_m); j2_ind++) {
				int j1 = col_perm[j1_ind];
				int j2 = col_perm[j2_ind];
                for(int i = 0; i < n; i++) {
                    x[i][0] = (A[i][j1] + A[i][j2])%2;
                }
                int I = 0;
                for(int a = 0; a < (n-2); a++) {
                    for(int b = (a+1); b < (n-1); b++) {
                        for(int c = (b+1); c < n; c++) {
                            for(int j = 0; j < this_m; j++) {
                                A_ext[n+I][j] = (x[a][0]*A[b][j]*A[c][j] + x[b][0]*A[c][j]*A[a][j] + x[c][0]*A[a][j]*A[b][j])%2;
                            }
                            I++;
                        }
                    }
                }
                int d=-1;
                int m_NS = this_m;
                bool** NS = LCL_Mat_GF2::nullspace((const bool**)A_ext,n_ext,m_NS,d);
                found = 0;
                int nsv = -1;
                for(int h = 0; (!found)&&(h < d); h++) {
                    found = (NS[j1][h]+NS[j2][h])%2;
                    if(found) {
                        nsv = h;
                    }
                }
                if(found) {
                    int weight_nv = 0;
                    for(int h = 0; h < this_m; h++) {
                        nv[0][h] = NS[h][nsv];
                        weight_nv += nv[0][h];
                    }
                    if(weight_nv%2) {
                        nv[0][this_m]=1;
                        for(int h = 0; h < n; h++) A[h][this_m]=0;
                        this_m++;
                    }
                    LCL_Mat_GF2::times((const bool**)x,(const bool**)nv,n,1,this_m,xnv);
                    LCL_Mat_GF2::add((const bool**)A,(const bool**)xnv,n,this_m,A_xnv);
                    LCL_Mat_GF2::copy((const bool**)A_xnv,n,this_m,A);
                    int mp=-1;
                    GateSynthesisMatrix::cleanup(A,n,this_m,mp);
                    this_m = mp;
                }
                if(NS) LCL_Mat_GF2::destruct(NS,m_NS,d);
            }
        }
        round++;
        //cout << "Round end" << endl;
    }
    //cout << "OUT OF LOOP" << endl;

    /*LCL_Mat_GF2::print((const bool**)A,n,this_m,"A\n");
    LCL_Mat_GF2::print((const bool**)x,n,1,"x\n");
    LCL_Mat_GF2::print((const bool**)A_ext,n_ext,m+1,"A_ext\n");
    LCL_Mat_GF2::print((const bool**)nv,1,m+1,"nv\n");
    LCL_Mat_GF2::print((const bool**)xnv,n,m+1,"xnv\n");
    LCL_Mat_GF2::print((const bool**)A_xnv,n,m+1,"A_xnv\n");*/

    //cout << "Destroying x" << endl;
    LCL_Mat_GF2::destruct(x,n,1);
    //cout << "Destroying A_ext" << endl;
    LCL_Mat_GF2::destruct(A_ext,n_ext,m+1);
    //cout << "Destroying nv" << endl;
    LCL_Mat_GF2::destruct(nv,1,m+1);
    //cout << "Destroying xnv" << endl;
    LCL_Mat_GF2::destruct(xnv,n,m+1);
    //cout << "Destroying A_xnv" << endl;
    LCL_Mat_GF2::destruct(A_xnv,n,m+1);
    omp = this_m;
    //cout << "END OF LEMPELX" << endl;
}

void GateSynthesisMatrix::Chi(bool** A, bool** x, int n, int m, bool** Aext) {
    int i = 0;
    int temp = 0;
    for(int alpha = 0; alpha < n; alpha++) {
        for(int beta = 0; beta < n; beta++) {
            for(int gamma = 0; gamma < n; gamma++) {
                for(int j = 0; j < m; j++) {
                    temp = x[alpha][0]*x[beta][0]*x[gamma][0]
                        + x[alpha][0]*x[beta][0]*A[gamma][j]
                        + x[beta][0]*x[gamma][0]*A[alpha][j]
                        + x[gamma][0]*x[alpha][0]*A[beta][j]
                        + x[alpha][0]*A[beta][j]*A[gamma][j]
                        + x[beta][0]*A[gamma][j]*A[alpha][j]
                        + x[gamma][0]*A[alpha][j]*A[beta][j];
                    Aext[i][j] = (temp%2);
                }
                i++;
            }
        }
    }
}


void GateSynthesisMatrix::ChiPrime(bool** A, bool** x, int n, int m, bool** Aext) {
    int i = 0;
    int temp = 0;
    for(int alpha = 0; alpha < n; alpha++) {
        for(int beta = 0; beta < n; beta++) {
            for(int gamma = 0; gamma < n; gamma++) {
                for(int j = 0; j < m; j++) {
                    temp = x[alpha][0]*x[beta][0]*A[gamma][j]
                        + x[beta][0]*x[gamma][0]*A[alpha][j]
                        + x[gamma][0]*x[alpha][0]*A[beta][j]
                        + x[alpha][0]*A[beta][j]*A[gamma][j]
                        + x[beta][0]*A[gamma][j]*A[alpha][j]
                        + x[gamma][0]*A[alpha][j]*A[beta][j];
                    Aext[i][j] = (temp%2);
                }
                i++;
            }
        }
    }
}

void GateSynthesisMatrix::LempelX2(bool** A, int n, int m, int& omp) {
    int this_m = m;
    bool** x = LCL_Mat_GF2::construct(n,1);
    bool** y = LCL_Mat_GF2::construct(m+1,1);
    bool** xyT = LCL_Mat_GF2::construct(n,m+1);
    bool** A_xyT = LCL_Mat_GF2::construct(n,m+1);
    int n_chi_A = n*n*n;
    bool** chi_A = LCL_Mat_GF2::construct(n_chi_A,m+1);

    // Column randomizers
    int* r_j1 = new int[m];
    int* r_j2 = new int[m];

    // Provisional Anew
    bool** Anew = LCL_Mat_GF2::construct(n,m+1);
    LCL_Mat_GF2::copy((const bool**)A,n,m,Anew);
    int m_new = m;

    // Current best A
    bool** Abest = LCL_Mat_GF2::construct(n,m+1);
    LCL_Mat_GF2::copy((const bool**)A,n,m,Abest);
    int m_best = m;

    bool found = 1;
    int round = 0;
    while(found&&(round<m)) {
        found = 0;
        LOut(); cout << "Round = " << round << endl;
        //LCL_Mat_GF2::print(A,n,this_m,"A: ");
        LCL_Int::randperm(r_j1,this_m-1);
        for(int j1 = 0; (!found)&&(j1 < (this_m-1)); j1++) {
            int this_col_1 = r_j1[j1];
            LCL_Int::randperm(r_j2,this_m-1-j1,j1+1);
            for(int j2 = 0; (!found)&&(j2 < (this_m-1-j1)); j2++) {
                int this_col_2 = r_j2[j2];

                for(int i = 0; i < n; i++) {
                    x[i][0] = (A[i][this_col_1] + A[i][this_col_2])%2;
                }

                // Check chi.
                GateSynthesisMatrix::Chi(A,x,n,this_m,chi_A);
                //LCL_Mat_GF2::print(chi_A,n*n*n,this_m);
                int d = 0;
                bool** NS = LCL_Mat_GF2::nullspace((const bool**)chi_A,n_chi_A,this_m,d);
                //LCL_Mat_GF2::print(NS,this_m,d,"NS: ");
                found = 0;
                int nsv = -1;
                for(int h = 0; (!found)&&(h < d); h++) {
                    found = (NS[this_col_1][h]+NS[this_col_2][h])%2;
                    if(found) {
                        nsv = h;
                        //cout << "j1 = " << this_col_1 << ", j2 = " << this_col_2 << endl;
                    }
                }
                if(found) {
                    //for(int j = 0; j < this_m; j++) y[j][0] = NS[j][nsv];
                    for(int i = 0; i < n; i++) {
                        for(int j = 0; j < this_m; j++) {
                            int temp = (A[i][j] + x[i][0]*NS[j][nsv])%2;
                            Anew[i][j] = temp;
                        }
                    }
                    int mp;
                    GateSynthesisMatrix::cleanup(Anew,n,this_m,mp);
                    //this_m = mp;
                    if(mp<m_best) {
                        LCL_Mat_GF2::copy((const bool**)Anew,n,mp,Abest);
                        m_best = mp;
                    }
                } else {
                    if(false/*Conditions for chi prime*/) {
                        // Do chi prime
                    } else {
                        // Do single column
                    }
                }
                if(d) LCL_Mat_GF2::destruct(NS,this_m,d);
            }
        }
        LCL_Mat_GF2::copy((const bool**)Abest,n,m_best,A);
        this_m = m_best;
        round++;
    }

    delete [] r_j1; r_j1 = NULL;
    delete [] r_j2; r_j2 = NULL;
    LCL_Mat_GF2::destruct(x,n,1);
    LCL_Mat_GF2::destruct(chi_A,n_chi_A,m+1);
    LCL_Mat_GF2::destruct(y,m+1,1);
    LCL_Mat_GF2::destruct(xyT,n,m+1);
    LCL_Mat_GF2::destruct(A_xyT,n,m+1);
    LCL_Mat_GF2::destruct(Anew,n,m+1);
    LCL_Mat_GF2::destruct(Abest,n,m+1);
    omp = this_m;
}
