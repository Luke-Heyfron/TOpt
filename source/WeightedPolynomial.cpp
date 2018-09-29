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

#include "WeightedPolynomial.h"

#include <iostream>

using namespace std;

#include <utility>
#include <cmath>

WeightedPolynomial::WeightedPolynomial(int in_n) {
    n = in_n;
    N_l = get_N_l();
    N_q = get_N_q();
    N_c = get_N_c();

    if(n>0) {
        _l = new int[n];
        for(int i = 0; i < n; i++) {
            _l[i] = 0;
        }
    }
    if(n>1) {
        _q = new int*[n-1];
        for(int i = 0; i < (n-1); i++) {
            _q[i] = new int[n-1-i];
            for(int j = 0; j < (n-1-i); j++) {
                _q[i][j] = 0;
            }
        }
    }
    if(n>2) {
        int n_c = 0;
        _c = new int**[n-2];
        for(int i = 0; i < (n-2); i++) {
            _c[i] = new int*[n-2-i];
            for(int j = 0; j < (n-2-i); j++) {
                _c[i][j] = new int[n-2-i-j];
                for(int k = 0; k < (n-2-i-j); k++) {
                    _c[i][j][k] = 0;
                    n_c++;
                }
            }
        }
    }
}

WeightedPolynomial::WeightedPolynomial(const WeightedPolynomial& in) {
    n = in.get_n();
    N_l = get_N_l();
    N_q = get_N_q();
    N_c = get_N_c();

    if(n>0) {
        _l = new int[n];
        for(int i = 0; i < n; i++) {
            _l[i] = 0;
        }
    }
    if(n>1) {
        _q = new int*[n-1];
        for(int i = 0; i < (n-1); i++) {
            _q[i] = new int[n-1-i];
            for(int j = 0; j < (n-1-i); j++) {
                _q[i][j] = 0;
            }
        }
    }
    if(n>2) {
        int n_c = 0;
        _c = new int**[n-2];
        for(int i = 0; i < (n-2); i++) {
            _c[i] = new int*[n-2-i];
            for(int j = 0; j < (n-2-i); j++) {
                _c[i][j] = new int[n-2-i-j];
                for(int k = 0; k < (n-2-i-j); k++) {
                    _c[i][j][k] = 0;
                    n_c++;
                }
            }
        }
    }
    operator=(in);
}

WeightedPolynomial::~WeightedPolynomial() {
    if(_c) {
        for(int i = 0; i < (n-2); i++) {
            if(_c[i]) {
                for(int j = 0; j < (n-2-i); j++) {
                    if(_c[i][j]) {
                        delete [] _c[i][j];
                        _c[i][j] = NULL;
                    }
                }
                delete [] _c[i];
                _c[i] = NULL;
            }
        }
        delete [] _c;
        _c = NULL;
    }
}

int WeightedPolynomial::get_n() const {
    int out = 0;

    out = n;

    return out;
}

int WeightedPolynomial::get_N_l() const {
    int out = 0;

    out = n;

    return out;
}

int WeightedPolynomial::get_N_q() const {
    int out = 0;

    out = (n*(n-1))/2;

    return out;
}
int WeightedPolynomial::get_N_c() const {
    int out = 0;

    out = (n*(n-1)*(n-2))/6;

    return out;
}

int& WeightedPolynomial::operator()(int a) {
    int i = a - 1;
    if(i < N_l) {
        return _l[i];
    } else {
        cout << "Error!" << endl;
        throw "Error!";
    }
}

const int& WeightedPolynomial::operator()(int a) const {
    int i = a - 1;
    if((i < N_l)&&(i>=0)) {
        return _l[i];
    } else {
        cout << "Error!" << endl;
        throw "Error!";
    }
}

int& WeightedPolynomial::operator()(int a, int b) {
    if(a>b) swap(a,b);
    if(a==b) {
        return operator()(a,a);
    } else {
        int i = a - 1;
        int j = b - a - 1;

        if((i < N_q)&&(i>=0)&&(j<N_q)&&(j>=0)) {
            return _q[i][j];
        } else {
            cout << "Error!" << endl;
            throw "Error!";
        }
    }
}

const int& WeightedPolynomial::operator()(int a, int b) const {
    if(a>b) swap(a,b);
    if(a==b) {
        return operator()(a,a);
    } else {
        int i = a - 1;
        int j = b - a - 1;

        if((i < N_q)&&(i>=0)&&(j<N_q)&&(j>=0)) {
            return _q[i][j];
        } else {
            cout << "Error!" << endl;
            throw "Error!";
        }
    }
}

int& WeightedPolynomial::operator()(int a, int b, int c) {
    if(a>b) swap(a,b);
    if(b>c) swap(b,c);
    if(a>b) swap(a,b);
    if(a==b) {
        return operator()(a,c);
    } else if((b==c)||(a==c)) {
        return operator()(a,b);
    } else {
        int i = a - 1;
        int j = b - a - 1;
        int k = c - b - 1;

        if((i < N_c)&&(i>=0)&&(j<N_c)&&(j>=0)&&(k<N_c)&&(k>=0)) {
            return _c[i][j][k];
        } else {
            cout << "Error!" << endl;
            throw "Error!";
        }
    }
}

const int& WeightedPolynomial::operator()(int a, int b, int c) const {
    if(a>b) swap(a,b);
    if(b>c) swap(b,c);
    if(a>b) swap(a,b);
    if(a==b) {
        return operator()(a,c);
    } else if((b==c)||(a==c)) {
        return operator()(a,b);
    } else {
        int i = a - 1;
        int j = b - a - 1;
        int k = c - b - 1;

        if((i < N_c)&&(i>=0)&&(j<N_c)&&(j>=0)&&(k<N_c)&&(k>=0)) {
            return _c[i][j][k];
        } else {
            cout << "Error!" << endl;
            throw "Error!";
        }
    }
}

void WeightedPolynomial::print() const {
    cout << "f(x) =";

    bool first_term = 1;
    for(int i = 0; i < N_l; i++) {
        int a = i + 1;
        if(_l[i]!=0) {
            cout << " ";
            if((!first_term)&&(_l[i]>0)) {
                cout << "+ ";
            }
            if(_l[i]<0) {
                cout << "- ";
            }
            if((int)fabs(_l[i])!=1) cout << ((int)fabs(_l[i]));
            cout << "(x_" << a << ")";
            if(first_term) {
                first_term = 0;
            }
        }
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            if(_q[i][j]!=0) {
                cout << " ";
                if((!first_term)&&(_q[i][j]>0)) {
                    cout << "+ ";
                }
                if(_q[i][j]<0) {
                    cout << "- ";
                }
                if(((int)fabs(_q[i][j]))!=1) cout << ((int)fabs(_q[i][j]));
                cout << "(2*x_" << a << "*x_" << b << ")";
                if(first_term) {
                    first_term = 0;
                }
            }
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                if(_c[i][j][k]!=0) {
                    cout << " ";
                    if((!first_term)&&(_c[i][j][k]>0)) {
                        cout << "+ ";
                    }
                    if(_c[i][j][k]<0) {
                        cout << "- ";
                    }
                    if(((int)fabs(_c[i][j][k]))!=1) cout << ((int)fabs(_c[i][j][k]));
                    cout << "(4*x_" << a << "*x_" << b << "*x_" << c << ")";
                    if(first_term) {
                        first_term = 0;
                    }
                }
            }
        }
    }
    cout << endl;
}

void WeightedPolynomial::operator+=(const WeightedPolynomial& in) {
    for(int i = 0; i < n; i++) {
        int a = i + 1;
        operator()(a) += in(a);
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            operator()(a,b) += in(a,b);
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                operator()(a,b,c) += in(a,b,c);
            }

        }
    }
}

void WeightedPolynomial::operator*=(int in) {
    for(int i = 0; i < n; i++) {
        int a = i + 1;
        operator()(a) *= in;
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            operator()(a,b) *= in;
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                operator()(a,b,c) *= in;
            }

        }
    }
}

void WeightedPolynomial::operator-=(const WeightedPolynomial& in) {
    for(int i = 0; i < n; i++) {
        int a = i + 1;
        operator()(a) -= in(a);
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            operator()(a,b) -= in(a,b);
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                operator()(a,b,c) -= in(a,b,c);
            }

        }
    }
}

void WeightedPolynomial::operator=(const WeightedPolynomial& in) {
    for(int i = 0; i < n; i++) {
        int a = i + 1;
        operator()(a) = in(a);
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            operator()(a,b) = in(a,b);
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                operator()(a,b,c) = in(a,b,c);
            }

        }
    }
}

void WeightedPolynomial::operator%=(int in) {
    for(int i = 0; i < n; i++) {
        int a = i + 1;
        while(operator()(a)<0) operator()(a) += in;
        operator()(a) %= in;
    }
    for(int i = 0; i < (n-1); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-1-i); j++) {
            int b = i + j + 2;
            while(operator()(a,b)<0) operator()(a,b) += in;
            operator()(a,b) %= in;
        }
    }
    for(int i = 0; i < (n-2); i++) {
        int a = i + 1;
        for(int j = 0; j < (n-2-i); j++) {
            int b = i + j + 2;
            for(int k = 0; k < (n-2-i-j); k++) {
                int c = i + j + k + 3;
                while(operator()(a,b,c)<0) operator()(a,b,c) += in;
                operator()(a,b,c) %= in;
            }

        }
    }
}

void WeightedPolynomial::ijk_to_abc(int i, int j, int k, int* a, int* b,int* c) const {
    if(a) *a = i + 1;
    if(b) *b = i + j + 2;
    if(c) *c = i + j + k + 3;
}

void WeightedPolynomial::abc_to_ijk(int a, int b, int c, int* i, int* j, int* k) const {
    if(i) *i = a - 1;
    if(j) *j = b - a - 1;
    if(k) *k = c - b - 1;
}

void WeightedPolynomial::dims_l(int* N_i) const {
    if(N_i) *N_i = n;
}

void WeightedPolynomial::dims_q(int i, int* N_i, int* N_j) const {
    if(N_i) *N_i = (n-1);
    if(N_j) *N_j = (n-1-i);
}

void WeightedPolynomial::dims_c(int i, int j, int* N_i, int* N_j, int* N_k) const {
    if(N_i) *N_i = (n-2);
    if(N_j) *N_j = (n-2-i);
    if(N_k) *N_k = (n-2-i-j);
}
