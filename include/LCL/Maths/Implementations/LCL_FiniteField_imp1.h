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
LCL_FiniteField<P>::LCL_FiniteField(const LCL_Field& b) {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    _p = P;
    _val = bp->_val;
}

template <int P>
LCL_FiniteField<P>::LCL_FiniteField(int b) {
    _p = P;
    (b>=0)? _val = (b % _p) : _val = (b + _p*ceil((-b)/(double)_p));
}

// Assignment
template <int P>
LCL_Field& LCL_FiniteField<P>::operator=(const LCL_Field& b) {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    _val = bp->_val;
    return (*this);
}

template <int P>
LCL_Field& LCL_FiniteField<P>::operator=(int b) {
    (b>=0)? _val = (b % _p) : _val = (b + _p*ceil((-b)/(double)_p));
    return (*this);
}

// Arithmetic
template <int P>
const LCL_Field& LCL_FiniteField<P>::operator+(const LCL_Field& b) const {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out;
    out = (_val + bp->_val)%_p;
    return out;
}

template <int P>
const LCL_Field& LCL_FiniteField<P>::operator-() const {
    LCL_FiniteField<P> out = (_p - _val);
    return out;
}
template <int P>
const LCL_Field& LCL_FiniteField<P>::operator*(const LCL_Field& b) const {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out = (_val * bp->_val)%_p;
    return out;
}

template <int P>
const LCL_Field& LCL_FiniteField<P>::operator~() const {
    LCL_FiniteField<P> out = (int)pow(_val,_p-2);
    return out;
}


template <int P>
const LCL_Field& LCL_FiniteField<P>::operator/(const LCL_Field& b) const {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    LCL_FiniteField<P> out = (*this) * (~b);
    return out;
}

// Comparison
template <int P>
bool LCL_FiniteField<P>::operator==(const LCL_Field& b) const {
    const LCL_FiniteField<P>* bp = dynamic_cast<const LCL_FiniteField<P>*>(&b);
    return (_val==bp->_val);
}

// Conversion
template <int P>
LCL_FiniteField<P>::operator int() const {
    return _val;
}

// stdout
template <int P>
ostream& operator<<(ostream& os, const LCL_FiniteField<P>&  f) {
    //return (os << f._val << " mod " << f._p);
    return os << f._val;
}

#endif // LCL_FINITE_FIELD_IMP1_HEADER
