#include "PhasePolynomial.h"

#include <iostream>
using namespace std;

#include <cmath>
#include <unordered_set>

#include "LCL_Bool.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;


PhasePolynomial::PhasePolynomial(int in_n) {
    n = in_n;
    N = (int)pow(2,n);
}


PhasePolynomial::~PhasePolynomial() {
    ;
}

void PhasePolynomial::print() const {
    LOut() << "f(x) =";

    for(int t = 0; t < T(); t++) {
        if((m[t]<0)||(t>0)) {
            if(m[t]<0) LOut() << " -";
            else LOut() << " +";
        }
        LOut() << " ";
        if(((int)fabs(m.at(t))!=1)) LOut() << (int)fabs(m[t]);
        LOut() << "(";
        bool x[n];
        LCL_Bool::IntToBoolVec(x,a[t],n);
        bool first_var = 1;
        for(int i = 0; i < n; i++) {
            if(x[i]) {
                if(first_var) {
                    first_var = 0;
                } else {
                    LOut() << "^";
                }
                LOut() << "x_" << (i+1);
            }
        }
        LOut() << ")";
    }
    LOut() << endl;
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

int PhasePolynomial::get_a_at(const int in_t) const {
    return a.at(in_t);
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

int PhasePolynomial::operator[](const int in_I) const {
    int out = 0;

    bool found = 0;
    for(int t = 0; (!found)&&(t<T()); t++) {
        if(a.at(t)==in_I) {
            found = 1;
            out = m.at(t);
        }
    }

    return out;
}

int PhasePolynomial::operator[](const bool* in_x) const {
    int out = 0;

    int this_I = LCL_Bool::BoolVecToInt(in_x,n);
    out = operator[](this_I);

    return out;
}

int PhasePolynomial::operator[](const string in_str) const {
    int out = 0;

    warning("TODO.", "operator[]", "PhasePolynomial");

    return out;
}

int& PhasePolynomial::operator[](const int in_I) {

    for(int t = 0; t<T(); t++) {
        if(a.at(t)==in_I) {
            return m.at(t);
        }
    }
    m.push_back(0);
    a.push_back(in_I);
    return m.back();
}

int& PhasePolynomial::operator[](const bool* in_x) {
    int this_I = LCL_Bool::BoolVecToInt(in_x,n);
    return operator[](this_I);
}

int& PhasePolynomial::operator[](const string in_str) {
    int out = 0;

    warning("TODO.", "operator[]", "PhasePolynomial");

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
