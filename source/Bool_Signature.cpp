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

#include "Bool_Signature.h"

#include <iostream>
using namespace std;
#include <ostream>

#include <cstdlib>

bool*** Bool_Signature::construct(int n) {
    bool*** out = new bool**[n];
    for(int i = 0; i < n; i++) {
        out[i] = new bool*[n];
        for(int j = 0; j < n; j++) {
            out[i][j] = new bool[n];
            for(int k = 0; k < n; k++) {
                out[i][j][k] = 0;
            }
        }
    }
    return out;
}

void Bool_Signature::destruct(bool*** S, int n) {
    if(S) {
        for(int i = 0; i < n; i++) {
            if(S[i]) {
                for(int j = 0; j < n; j++) {
                    if(S[i][j]) {
                        delete [] S[i][j];
                        S[i][j] = NULL;
                    }
                }
                delete [] S[i];
                S[i] = NULL;
            }
        }
        delete [] S;
        S = NULL;
    }
}

void Bool_Signature::copy(bool*** S, int n, bool*** O) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                O[i][j][k] = S[i][j][k];
            }
        }
    }
}

void Bool_Signature::print(bool*** S, int n, char* pre, ostream& inOS) {
    if(pre) inOS << pre;
    inOS << "n " << n << endl;
    for(int i = 0; i < n; i++) {
        if(S[i][i][i]) {
            inOS << "a " << i << endl;
        }
        if(i<(n-1)) {
            for(int j = (i+1); j < n; j++) {
                if(S[i][j][j]) inOS << "b " << i << " " << j << endl;
                if(j<(n-1)) {
                    for(int k = (j+1); k < n; k++) {
                        if(S[i][j][k]) inOS << "c " << i << " " << j << " " << k << endl;
                    }
                }
            }
        }

    }
}

void Bool_Signature::set(bool*** S, int i, int j, int k, bool val) {
    if((i==j)&&(i==k)) {
        S[i][i][i] = val;
    } else if(i==j) {
        S[i][i][k] = val;
        S[i][k][i] = val;
        S[k][i][i] = val;
        S[k][k][i] = val;
        S[k][i][k] = val;
        S[i][k][k] = val;
    } else if(i==k) {
        S[i][i][j] = val;
        S[i][j][i] = val;
        S[j][i][i] = val;
        S[j][j][i] = val;
        S[j][i][j] = val;
        S[i][j][j] = val;
    } else if(j==k) {
        S[i][i][j] = val;
        S[i][j][i] = val;
        S[j][i][i] = val;
        S[j][j][i] = val;
        S[j][i][j] = val;
        S[i][j][j] = val;
    } else {
        S[i][j][k] = val;
        S[j][k][i] = val;
        S[k][i][j] = val;
        S[i][k][j] = val;
        S[k][j][i] = val;
        S[j][i][k] = val;
    }
}

void Bool_Signature::from_A_matrix(bool*** S, bool** A, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < n; k++) {
                int sum = 0;
                for(int h = 0; h < m; h++) {
                    sum += A[i][h]*A[j][h]*A[k][h];
                }
                S[i][j][k] = (sum%2);
            }
        }
    }
}

void Bool_Signature::random(bool*** S, int n) {
    for(int i = 0; i < n; i++) {
        Bool_Signature::set(S,i,i,i,rand()%2);
        if(i<(n-1)) {
            for(int j = (i+1); j < n; j++) {
                Bool_Signature::set(S,i,j,j,rand()%2);
                if(j<(n-1)) {
                    for(int k = (j+1); k < n; k++) {
                        Bool_Signature::set(S,i,j,k,rand()%2);
                    }
                }
            }
        }

    }
}

bool Bool_Signature::equals(bool*** S1, bool*** S2, int n) {
    bool out = 1;
    for(int i = 0; out&&(i < n); i++) {
        for(int j = 0; out&&(j < n); j++) {
            for(int k = 0; out&&(k < n); k++) {
                out = (S1[i][j][k]==S2[i][j][k]);
                if(!out) {
                    cout << "Failure at " << i << ", " << j << ", " << k << endl;
                }
            }
        }
    }
    return out;
}
