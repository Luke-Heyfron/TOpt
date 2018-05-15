#include "LCL/Core/LCL_Array.h"

#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include <iostream>
using namespace std;

template <class T, int N>
T LCL_Array<T, N>::LCL_ArrayBadVal;

template <class T, int N>
LCL_Array<T,N>::LCL_Array() {
    _N = N;
    _size(0,_N);
    _size = 0;
}

template <class T, int N>
LCL_Array<T,N>::LCL_Array(int in_size) {
    _N = N;
    _size(0,_N);
    _size = in_size;
}

template <class T, int N>
LCL_Array<T,N>::LCL_Array(int in_size, const T& in_cv) {
    _N = N;
    _size(0,_N);
    clear_value = in_cv;
    for(int i = 0; i < _N; i++) data[i] = clear_value;
    _size = in_size;
}

template <class T, int N>
T LCL_Array<T,N>::operator[](int i) const {
    if((i>=0)&&(i<_size)) {
        return data[i];
    } else {
        error("Index out of bounds.", "operator[]", "LCL_Array");
        return LCL_ArrayBadVal;
    }
}

template <class T, int N>
T& LCL_Array<T,N>::operator[](int i) {
    if((i>=0)&&(i<_size)) {
        return data[i];
    } else {
        error("Index out of bounds.", "operator[]", "LCL_Array");
        return LCL_ArrayBadVal;
    }
}

template <class T, int N>
LCL_Array<T,N>& LCL_Array<T,N>::operator+=(const T& in) {
    if(_size<_N) {
        data[_size] = in;
        _size++;
    } else {
        warning("LCL_Array is full.", "operator+=", "LCL_Array");
    }
    return (*this);
}

template <class T, int N>
LCL_Array<T,N>& LCL_Array<T,N>::operator+=(const LCL_Array& in) {
    if((_size+in.size())<=_N) {
        for(int i = 0; i < in.size(); i++) {
            data[_size] = in[i];
        }
        _size += in.size();
    } else {
        warning("LCL_Array is too small.", "operator+=", "LCL_Array");
    }
    return (*this);
}

template <class T, int N>
int LCL_Array<T,N>::size() const {
    return _size;
}

template <class T, int N>
int LCL_Array<T,N>::max_size() const {
    return _N;
}

template <class T, int N>
LCL_BoundedInt& LCL_Array<T,N>::size() {
    return _size;
}

template <class T, int N>
void LCL_Array<T,N>::print() const {
    for(int i = 0; i < size(); i++) {
        LOut() << operator[](i) << endl;
    }
}

template <class T>
T LCL_ArrayBadVal() {
    T out;
    error("BADVAL", "LCL_ArrayBadVal");
    return out;
}

template<class T, int N>
ostream& operator<<(ostream& os, const LCL_Array<T,N>& in) {
    os << "(";
    for(int i = 0; i < in._size; i++) {
        if(i) os << ", ";
        os << in[i];
    }
    os << ")";
    return os;
}
