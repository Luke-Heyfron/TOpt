#ifndef LCL_FINITE_FIELD_IMP1_HEADER
#define LCL_FINITE_FIELD_IMP1_HEADER

#include "LCL/Maths/LCL_FiniteField.h"
#include <cmath>

#include <iostream>
using namespace std;

template <int P>
LCL_FiniteField<P>::LCL_FiniteField() {
    _p = P;
    _val = 0;
}

template <int P>
LCL_FiniteField<P>::LCL_FiniteField(const LCL_FiniteField<P>& b) {
    _p = P;
    _val = b._val;
}

template <int P>
LCL_FiniteField<P>::LCL_FiniteField(int b) {
    _p = P;
    (b>=0)? _val = (b % _p) : _val = (b + _p*ceil((-b)/(double)_p));
}

// Assignment
/*template <int P>
LCL_FiniteField<P>& LCL_FiniteField<P>::operator=(const LCL_FiniteField<P>& b) {
    //const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    _val = (int)b;
    if(!(((int)_val==0)||((int)_val==1))) cout << "Bad operator=(const LCL_Field&) _val = " << (int)_val << endl;
    return (*this);
}*/

template <int P>
LCL_FiniteField<P>& LCL_FiniteField<P>::operator=(int b) {
    (b>=0)? _val = (b % _p) : _val = (b + _p*ceil((-b)/(double)_p));
    if(!(((int)_val==0)||((int)_val==1))) cout << "Bad operator=(int) _val = " << (int)_val << endl;
    return (*this);
}

/*template <int P>
LCL_FiniteField<P>& LCL_FiniteField<P>::operator=(double b) {
    return operator=((int)b);
}*/

/*template <int P>
LCL_FiniteField<P>& LCL_FiniteField<P>::operator=(bool b) {
    return operator=((int)b);
}*/

// Arithmetic
template <int P>
LCL_FiniteField<P> LCL_FiniteField<P>::operator+(const LCL_FiniteField<P>& b) const {
    //const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out;
    out = (_val + (int)b)%_p;
    return out;
}

template <int P>
LCL_FiniteField<P> LCL_FiniteField<P>::operator-() const {
    LCL_FiniteField<P> out = (_p - _val);
    return out;
}
template <int P>
LCL_FiniteField<P> LCL_FiniteField<P>::operator*(const LCL_FiniteField<P>& b) const {
    //const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out = (_val * (int)b)%_p;
    return out;
}

template <int P>
LCL_FiniteField<P> LCL_FiniteField<P>::operator~() const {
    LCL_FiniteField<P> out = (int)pow(_val,_p-2);
    return out;
}


template <int P>
LCL_FiniteField<P> LCL_FiniteField<P>::operator/(const LCL_FiniteField<P>& b) const {
    //const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out = (*this) * (~b);
    return out;
}

// Comparison
template <int P>
bool LCL_FiniteField<P>::operator==(const LCL_FiniteField<P>& b) const {
    //const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    return (((int)_val%_p)==((int)b%_p));
}


// stdout
/*template <int P>
ostream& operator<<(ostream& os, const LCL_FiniteField<P>&  f) {
    //return (os << f._val << " mod " << f._p);
    return os << f._val;
}*/

// Conversion
template <int P>
LCL_FiniteField<P>::operator int() const {
    //if(!(((int)_val==0)||((int)_val==1))) cout << "Bad operator int = " << (int)_val << endl;
    return _val;
}

/*template <int P>
LCL_FiniteField<P>::operator double () const {
    return (double) _val;
}*/

/*template <int P>
LCL_FiniteField<P>::operator bool () const {
    return (bool) _val;
}*/


#endif // LCL_FINITE_FIELD_IMP1_HEADER
