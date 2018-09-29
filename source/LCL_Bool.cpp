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

#include "LCL/LCL_Bool.h"

#include <iostream>
using namespace std;

#include <cmath>
#include <utility>
#include <cstring>

int LCL_Bool::IntToBoolVec(bool* out, int I, int m) {
    /*
    Turns integer into a corresponding bit string.

    Args:
        - Out: boolean array in which output is to be stored. Should be allocated memory outside function.
        - I: Integer to be turned into bit string.
        - m: Number of bits. If left default, only necessary bits will be set.
    Return:
        - 1 if successful, -1 if array not large enough.
    */

    int N = m;
    if(N<0) {
        N = LCL_Bool::BitSize(I);
    }
    int temp = I;
    for(int i = 0; i < N; i++) {
        bool thisBit = temp%2;
        out[i] = thisBit;
        temp /= 2;
    }
    return 1;
}

int LCL_Bool::BitSize(int I) {
    int out = 0;
    while((pow(2,out)-1)<I) {
        out++;
    }
    return out;
}

int LCL_Bool::Inner(bool* x1, bool* x2, int len) {
    int out = 0;
    for(int i = 0; i < len; i++) {
        out += (x1[i]&&x2[i]);
    }
    return out;
}

void LCL_Bool::print(bool* x, int len, const char* pre) {
    if(pre) cout << pre;
    for(int i = 0; i < len; i++) cout << x[len-i-1];
    cout << endl;
}

void LCL_Bool::BitwiseAnd(bool* x1, bool* x2, bool* out, int len) {
    for(int i = 0; i < len; i++) {
        out[i] = x1[i]*x2[i];
    }
}

void LCL_Bool::BitwiseXor(bool* x1, bool* x2, bool* out, int len) {
    for(int i = 0; i < len; i++) {
        out[i] = x1[i]^x2[i];
    }
}

void LCL_Bool::copy(bool* src, bool* dst, int len) {
    for(int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

bool LCL_Bool::increment(bool* x, int len) {
    bool out;
    int i = 0;
    bool exit;
    do {
    exit = !x[i];
    x[i] = !x[i];
    i++;
    } while((!exit)&&(i<len));
    out = exit;
    return out;
}

int LCL_Bool::BoolVecToInt(const bool* x, int len) {
    int out = 0;
    for(int i = 0; i < len; i++) {
        out += x[i]*pow(2,i);
    }
    return out;
}

void LCL_Bool::zeros(bool* x, int len) {
    for(int i = 0; i < len; i++) {
        x[i] = false;
    }
}

bool LCL_Bool::nextUniquePerm(bool* out, const bool* in, int len) {
    bool outB = false;
    int W = 0;
    for(int i = 0; i < len; i++) {
        W += in[i];
        out[i] = 0;
    }
    if(W) {
        int m = 1;
        do {
            bool moveleft;
            int foundpos = -1;
            {
                int foundcount = 0;
                for(int i = 0; (i < len)&&(foundcount!=m); i++) {
                    if(in[i]) {
                        foundcount++;
                        if(foundcount==m) foundpos = i;
                    }
                }
                if((foundpos<(len-1))&&(!in[foundpos+1])) {
                    moveleft = true;
                } else {
                    moveleft = false;
                }
            }
            if(moveleft) {
                for(int i = 0; i < (m-1); i++) {
                    out[i] = 1;
                }
                out[foundpos+1] = 1;
                for(int i = (foundpos+2); i < len; i++) {
                    out[i] = in[i];
                }
                outB = true;
            } else {
                m++;
            }
        } while((m<=W)&&(!outB));
    }
    return outB;
}

int LCL_Bool::Weight(bool* x, int len) {
    int out = 0;

    for(int i = 0; i < len; i++) {
        out += x[i];
    }

    return out;
}

void LCL_Bool::fromString(bool* out, const char* in_s) {
    int n = strlen(in_s);
    for(int i = 0; i < n; i++) {
        if(in_s[i]=='0') {
            out[i] = 0;
        } else if(in_s[i]=='1') {
            out[i] = 1;
        }
    }
}

int LCL_Bool::ReedDecoder(bool* x, int R, int M, bool* c, bool* e) {
    /*
        Performs the Reed Decoder algorithm on input vector x for RM(r,m).
        The recovered codeword is stored in c and the error string in e.
    */
    int out = 0;

    int n = (int)pow(2,M);

    // Initialize b = x
    bool* b = new bool[n];
    LCL_Bool::copy(x,b,n);
    LCL_Bool::print(b,n,"Init b: ");

    if(c) {
        for(int i = 0; i < n; i++) c[i] = 0;
    }

    for(int r = R; r >= 0; r--) {
        cout << "r = " << r << endl;
        // Perform majority vote decoding on all subspaces of F_2^M of size r
            // Construct incidence vector I
        bool* b_r = new bool[n];
        for(int i = 0; i < n; i++) b_r[i] = 0;
        bool* I = new bool[M];
            // Initialize I to state with 1's for first r elements and zeros elsewhere
        for(int i = 0; i < M; i++) I[i] = (i<r);
        bool exit = true;
        do {
            LCL_Bool::print(I,M,"I: ");
            // Set vote counter to zero
            int votes = 0;
            // For each vote, v, contruct a different translation vector on F_2^(M-r)
            for(int v = 0; v < (int)pow(2,M-r); v++) {
                cout << "v = " << v << endl;
                bool* v_vec = new bool[M-r];
                LCL_Bool::IntToBoolVec(v_vec, v, M-r);
                LCL_Bool::print(v_vec,M-r,"v: ");
                // For each term in v, construct each vector in F_2^r
                int this_vote = 0;
                if(r) {
                    for(int t = 0; t < (int)pow(2,r); t++) {
                            cout << "t = " << t << endl;
                            bool* t_vec = new bool[r];
                            LCL_Bool::IntToBoolVec(t_vec,t,r);
                            LCL_Bool::print(t_vec,r,"t: ");
                            // Make the vector k_vec on F_2^M where k_1 = t_1 if I_1 = 1 OR k_1 = v_1 if I_1 = 0 etc.
                            bool* k_vec = new bool[M];
                            int i_v = 0;
                            int i_t = 0;
                            for(int i = 0; i < M; i++) {
                                if(I[i]==0) {
                                    k_vec[i] = v_vec[i_v];
                                    i_v++;
                                } else {
                                    k_vec[i] = t_vec[i_t];
                                    i_t ++;
                                }
                            }
                            int k = LCL_Bool::BoolVecToInt(k_vec, M);
                            this_vote += b[k];

                            delete [] k_vec;
                            delete [] t_vec;

                    }
                    this_vote = (this_vote%2);
                } else {
                    this_vote = b[v];
                }

                votes += this_vote;

                delete [] v_vec;
            }

            // If votes > (1/2)2^(M-r) then add corresonding RM basis vector to b_r
            if(votes > (int)(0.5*pow(2,M-r))) {
                for(int i = 0; i < n; i++) {
                    bool* i_vec = new bool[M];
                    LCL_Bool::IntToBoolVec(i_vec,i,M);
                    bool this_f = 1;
                    for(int j = 0; j < M; j++) {
                        this_f *= (bool)pow(i_vec[j], I[j]);
                    }
                    if(this_f) {
                        b_r[i] = !b_r[i];
                    }

                    delete [] i_vec;
                }
                cout << "Term present" << endl;
            } else if(votes == (int)(0.5*pow(2,M-r))) {
                cout << "No majority" << endl;
            }

            // Increment incidence vector for next RM coefficient
            bool* temp = new bool[M];
            exit = !LCL_Bool::nextUniquePerm(temp,I,M);
            LCL_Bool::copy(temp,I,M);
            delete [] temp;
        } while(!exit);

        // Construct the order r sub-code word, b_r, and subtract it from b
        bool* temp = new bool[n];
        LCL_Bool::BitwiseXor(b,b_r,temp,n);
        LCL_Bool::copy(temp,b,n);
        LCL_Bool::print(b,n,"new b: ");
        if(c) {
            LCL_Bool::BitwiseXor(c,b_r,temp,n);
            LCL_Bool::copy(temp,c,n);
        }
        delete [] temp;

        delete [] I;
        delete [] b_r;
    }

    if(e) {
        LCL_Bool::copy(b,e,n);
    }
    out = LCL_Bool::Weight(b,n);

    delete [] b;

    return out;
}
