#include "PhasePolynomial.h"

#include <iostream>
using namespace std;

#include <cmath>
#include <unordered_set>
#include <vector>

#include "LCL/LCL_Bool.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;


PhasePolynomial::PhasePolynomial(int in_n) {
    n = in_n;
    N = (int)pow(2,n);
}


PhasePolynomial::~PhasePolynomial() {
    ;
}

void PhasePolynomial::print() const {
    cout << "f(x) =";

    for(int t = 0; t < T(); t++) {
        if((m[t]<0)||(t>0)) {
            if(m[t]<0) cout << " -";
            else cout << " +";
        }
        cout << " ";
        if(((int)fabs(m.at(t))!=1)) cout << (int)fabs(m[t]);
        cout << "(";
        //bool x[n];
        //LCL_Bool::IntToBoolVec(x,a[t],n);
        bool first_var = 1;
        for(int i = 0; i < n; i++) {
            if(a[t][i]) {
                if(first_var) {
                    first_var = 0;
                } else {
                    cout << "^";
                }
                cout << "x_" << (i+1);
            }
        }
        cout << ")";
    }
    cout << endl;
}

int PhasePolynomial::get_n() const {
    return n;
}

int PhasePolynomial::get_N() const {
    return N;
}

int PhasePolynomial::T() const {
    int out = 0;

    if(m.size()!=a.size()) {
        warning("m and a should be the same length.", "getNoTerms", "PhasePolynomial");
    }

    out = m.size();

    return out;
}

int PhasePolynomial::get_m_at(const int in_t) const {
    return m.at(in_t);
}

vector<bool> PhasePolynomial::get_a_at(const int in_t) const {
    return a.at(in_t);
}

void PhasePolynomial::get_a_at(bool* out_x, const int in_t) const {
	for(int i = 0; i < n; i++) {
		out_x[i]=a[in_t][i];
	}
}

void PhasePolynomial::operator+=(const PhasePolynomial& inPP) {
    if(inPP.get_n()==n) {
        for(int t = 0; t < inPP.T(); t++) {
            operator[](inPP.a.at(t)) += inPP.m.at(t);
        }
    } else {
        error("Operands should have matching n.", "operator+=", "PhasePolynomial");
    }
}

void PhasePolynomial::operator*=(const int in_m) {
    for(int t = 0; t < T(); t++) {
        m.at(t) *= in_m;
    }
}

void PhasePolynomial::operator-=(const PhasePolynomial& inPP) {
    if(inPP.get_n()==n) {
        for(int t = 0; t < inPP.T(); t++) {
            operator[](inPP.a.at(t)) -= inPP.m.at(t);
        }
    } else {
        error("Operands should have matching n.", "operator-=", "PhasePolynomial");
    }
}

void PhasePolynomial::operator=(const PhasePolynomial& inPP) {
    if(inPP.get_n()==n) {
        m.clear();
        a.clear();
        for(int t = 0; t < inPP.T(); t++) {
            operator[](inPP.a.at(t)) = inPP.m.at(t);
        }
    } else {
        error("Operands should have matching n.", "operator=", "PhasePolynomial");
    }
}

void PhasePolynomial::operator%=(const int in_m) {
    if(in_m>0) {
        for(int t = 0; t < T(); t++) {
            while(m.at(t)<0) m.at(t) += in_m;
            m.at(t) %= in_m;
        }
    } else {
        error("Modulus should be a positive, non-zero integer.", "operator%=", "PhasePolynomial");
    }
}

int PhasePolynomial::operator[](const vector<bool>& in_v) const {
	int out = 0;

	if(in_v.size()==n) {
		bool found = 0;
		for(int t = 0; (!found)&&(t<T()); t++) {
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (a[t][i]==in_v[i]);
			}
			if(found) {
				out = m[t];
			}
		}
	} else {
		error("PhasePolynomial key should have length n!", "operator[]", "PhasePolynomial");
	}

    return out;
}

int PhasePolynomial::operator[](const int in_I) const {
    int out = 0;

    /*bool found = 0;
    for(int t = 0; (!found)&&(t<T()); t++) {
        if(a.at(t)==in_I) {
            found = 1;
            out = m.at(t);
        }
    }*/

	bool x[n];
	LCL_Bool::IntToBoolVec(x,in_I,n);
	vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i] = x[i];
	out = operator[](v);

    return out;
}



int PhasePolynomial::operator[](const bool* in_x) const {
    int out = 0;

    /*int this_I = LCL_Bool::BoolVecToInt(in_x,n);

    out = operator[](this_I);*/

	vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i] = in_x[i];
	out = operator[](v);

    return out;
}

int PhasePolynomial::operator[](const string in_str) const {
    int out = 0;

    error("Use of unfinished method.", "operator[]", "PhasePolynomial");

    return out;
}

int& PhasePolynomial::operator[](const vector<bool>& in_v) {
    if(in_v.size()==n) {
		bool found = 0;
		for(int t = 0; (!found)&&(t<T()); t++) {
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (a[t][i]==in_v[i]);
			}
			if(found) {
				return m[t];
			}
		}
		m.push_back(0);
		a.push_back(in_v);
		return m.back();
	} else {
		error("PhasePolynomial key should have length n!", "operator[]", "PhasePolynomial");
	}
}

int& PhasePolynomial::operator[](const int in_I) {

    /*for(int t = 0; t<T(); t++) {
        if(a.at(t)==in_I) {
            return m.at(t);
        }
    }
    m.push_back(0);
    a.push_back(in_I);
    return m.back();*/
	bool x[n];
	LCL_Bool::IntToBoolVec(x,in_I,n);
	vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i] = x[i];
	return operator[](v);
}

int& PhasePolynomial::operator[](const bool* in_x) {
    vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i] = in_x[i];
	return operator[](v);
}

int& PhasePolynomial::operator[](const string in_str) {
    int out = 0;

    error("Use of unfinished method.", "operator[]", "PhasePolynomial");

    return out;
}

void PhasePolynomial::clean() {
    for(int t = 0; t < T(); t++) {
        if(m.at(t)==0) {
            m.erase(m.begin()+t);
            a.erase(a.begin()+t);
            t--;
        }
    }
}

int PhasePolynomial::T_count() const {
	int out = 0;
	for(int t = 0; t < T(); t++) {
		int sum = 0;
		for(int i = 0; i < n; i++) sum += a[t][i];
		if(sum) {
			out += (m.at(t)%2);
		}
	}
	return out;
}
