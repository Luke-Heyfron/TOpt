#ifndef LCL_MATRIX_HEAP_SPARSE_IMP1_HEADER
#define LCL_MATRIX_HEAP_SPARSE_IMP1_HEADER

#include "LCL/Maths/LCL_Matrix_Heap_Sparse.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include <cmath>

// Constructors

template <class T>
LCL_Matrix_Heap_Sparse<T>::LCL_Matrix_Heap_Sparse() {
    this->resize(0,0);
}

template <class T>
LCL_Matrix_Heap_Sparse<T>::LCL_Matrix_Heap_Sparse(const LCL_Matrix<T>& in) {
    this->copy(in);
}

template <class T>
LCL_Matrix_Heap_Sparse<T>::LCL_Matrix_Heap_Sparse(LCL_Mat_Size in_r, LCL_Mat_Size in_c) {
    this->resize(in_r, in_c);
}

template <class T>
LCL_Matrix_Heap_Sparse<T>::~LCL_Matrix_Heap_Sparse() {
    this->resize(0,0);
    set_storage_N(0);
}

template <class T>
LCL_Matrix_Heap_Sparse<T>::LCL_Matrix_Heap_Sparse(const LCL_Matrix_Heap_Sparse<T>& in) {
    LCL_Matrix_Heap_Sparse<T>::copy(in);
}

template <class T>
void LCL_Matrix_Heap_Sparse<T>::copy(const LCL_Matrix_Heap_Sparse<T>& in) {
    this->resize(in.r(), in.c());
    set_storage_N(in.get_storage_N());
    _auto_resize = in.auto_resize();
    _cleanup = in._cleanup;
    _cleanup_count = in._cleanup_count;
    _last_accessed = in._last_accessed;
    _block_size - in._block_size;
    for(int i = 0; i < _N; i++) {
        get<0>(*_e[i]) = get<0>(*in._e[i]);
        get<1>(*_e[i]) = get<1>(*in._e[i]);
    }
}

// Storage

template <class T>
void LCL_Matrix_Heap_Sparse<T>::set_storage_N(LCL_Mat_Size in_N) {
    if(_e) {
        for(int i = 0; i < _N; i++) {
            if(_e[i]) {
                delete _e[i];
                _e[i] = NULL;
            }
        }
        delete [] _e;
        _e = NULL;
    }
    _N = in_N;
    if(_N) {
        _e = new pair<T,LCL_Mat_Flat_Index>*[_N];
        for(int i = 0; i < _N; i++) {
            _e[i] = new pair<T,LCL_Mat_Flat_Index>;
            get<0>(*_e[i]) = 0;
            get<1>(*_e[i]) = -1;
        }
    }
}

template <class T>
LCL_Mat_Size LCL_Matrix_Heap_Sparse<T>::get_storage_N() const {
    return _N;
}

template <class T>
void LCL_Matrix_Heap_Sparse<T>::resize_storage(LCL_Mat_Size in_N) {
    LCL_Matrix_Heap_Sparse<T> temp(*this);
    set_storage_N(in_N);
    int N_copied = 0;
    for(int i = 0; (i < temp._N)&&(N_copied<_N); i++) {
        // Only copy if value is non-zero
        if(get<0>(*(temp._e[i]))!=0) {
            get<0>(*_e[N_copied]) = get<0>(*temp._e[i]);
            get<1>(*_e[N_copied]) = get<1>(*temp._e[i]);
            N_copied++;
        }
    }
    if(_N) _last_accessed = N_copied;
    else _last_accessed = -1;
    _cleanup = 0;
    resize_count++;
}

template <class T>
void LCL_Matrix_Heap_Sparse<T>::print_storage() const {
    cout << "LCL_Matrix_Heap_Sparse: " << this->r() << " x " << this->c() << endl;
    cout << "    clean_up = " << _cleanup << endl;
    for(int i = 0; i < _N; i++) {
        cout << "\t[" << i << "] = (" << get<0>(*_e[i]) << ", " << get<1>(*_e[i]) << ")" << endl;
    }
}

template <class T>
LCL_Mat_Size LCL_Matrix_Heap_Sparse<T>::non_zero() const {
    LCL_Mat_Size out = 0;
    for(int i = 0; i < _N; i++) {
        if(get<0>(*_e[i])!=0) out++;
    }
    return out;
}

template <class T>
void LCL_Matrix_Heap_Sparse<T>::cleanup() {
    resize_storage(non_zero());
    _cleanup = 0;
    _last_accessed = -1;
}


// Accessors

template <class T>
const T& LCL_Matrix_Heap_Sparse<T>::operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const {
    LCL_Mat_Flat_Index this_I = this->Index_2D_to_Flat(i,j);

    return operator()(this_I);
}

template <class T>
T& LCL_Matrix_Heap_Sparse<T>::operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) {
    LCL_Mat_Flat_Index this_I = this->Index_2D_to_Flat(i,j);

    return operator()(this_I);
}

template <class T>
const T& LCL_Matrix_Heap_Sparse<T>::operator()(LCL_Mat_Flat_Index this_I) const {
    if((_last_accessed>=0)&&(_last_accessed<_N)) {
        if(get<1>(*_e[_last_accessed])==this_I) {
            return get<0>(*_e[_last_accessed]);
        }
    }

    for(int k = 0; k < _N; k++) {
        int this_index = (k+_last_accessed+1)%_N;
        if(get<1>(*_e[this_index])==this_I) {
            return get<0>(*_e[this_index]);
        }
    }
    return 0;
}

template <class T>
T& LCL_Matrix_Heap_Sparse<T>::operator()(LCL_Mat_Flat_Index this_I) {
    _cleanup++;
    if(_auto_resize&&(_cleanup==_cleanup_count)) resize_storage(non_zero()+1);

    for(int k = 0; k < _N; k++) {
        if(get<1>(*_e[k])==this_I) {
            _last_accessed = k;
            return get<0>(*_e[k]);
        }
    }
    if((_last_accessed>=0)&&(_last_accessed<_N)) {
        if(get<0>(*_e[_last_accessed])==0) {
            get<1>(*_e[_last_accessed]) = this_I;
            return get<0>(*_e[_last_accessed]);
        }
    } else {
        _last_accessed = -1;
    }
    for(int k = 0; k < _N; k++) {
        int this_index = (k+_last_accessed+1)%_N;
        if(get<0>(*_e[this_index])==0) {
            get<1>(*_e[this_index]) = this_I;
            _last_accessed = this_I;
            return get<0>(*_e[this_index]);
        }
    }
    if(_auto_resize) {
        //cout << "Resizing..." << endl;
        int new_index = _N;
        _block_size = 2*(int)log(this->r()*this->c()+1) + 1;
        _cleanup_count = 2*(int)log(this->r()*this->c()+1) + 1;
        resize_storage(_N+_block_size);
        get<1>(*_e[new_index]) = this_I;
        _last_accessed = new_index;
        return get<0>(*_e[new_index]);
    } else {
        // Throw an exception
        error("Can't access element as matrix is full. Please allocate more storage.", "operator()", "LCL_Matrix_Heap_Sparse");
        return LCL_Matrix<T>::LCL_Matrix_Bad_Val;
    }
}

// Arithmetic
template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator+(const LCL_Matrix<T>& B) const {
    LCL_Matrix_Heap_Sparse<T> out;
    LCL_Matrix<T>::addition(out, *this, B);
    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator*(const LCL_Matrix<T>& B) const {
    LCL_Matrix_Heap_Sparse<T> out;
    LCL_Matrix<T>::multiplication(out, *this, B);
    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator-(const LCL_Matrix<T>& B) const {
    LCL_Matrix_Heap_Sparse<T> out;
    LCL_Matrix<T>::subtraction(out, *this, B);
    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator*(const T& b) const {
    LCL_Matrix_Heap_Sparse<T> out(this->r(), this->c());
    for(int i = 0; i < _N; i++) {
        T this_val = get<0>(*_e[i])*b;
        if(this_val!=0) {
            out(get<1>(*_e[i])) = this_val;
        }
    }
    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator/(const T& b) const {
    LCL_Matrix_Heap_Sparse<T> out(this->r(), this->c());
    for(int i = 0; i < _N; i++) {
        T this_val = get<0>(*_e[i])/b;
        if(this_val!=0) {
            out(get<1>(*_e[i])) = this_val;
        }
    }
    return out;
}


template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator-() const {
    LCL_Matrix_Heap_Sparse<T> out(this->r(), this->c());
    for(int i = 0; i < _N; i++) {
        T this_val = -get<0>(*_e[i]);
        if(this_val!=0) {
            out(get<1>(*_e[i])) = this_val;
        }
    }
    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator+(const LCL_Matrix_Heap_Sparse<T>& B) const {
    LCL_Matrix_Heap_Sparse<T> out;
    // If dimensions match
    if((this->r()==B.r())&&(this->c()==B.c())) {
        out.resize(this->r(),this->c());
        // Search through this
        for(int i = 0; i < _N; i++) {
            T this_val = get<0>(*_e[i]);
            LCL_Mat_Flat_Index this_address = get<1>(*_e[i]);

            // If we find a non-zero at this storage position
            if(this_val!=0) {
                // Then we see if B is non-zero at that address
                T this_B_val = B(this_address);
                // If so, we set out at this address to this + B
                if(this_B_val!=0) {
                    out(this_address) = this_val + this_B_val;
                }
                // Otherwise set to just this val
                else {
                    out(this_address) = this_val;
                }
            }
        }
        for(int i = 0; i < B._N; i++) {
            T this_B_val = get<0>(*B._e[i]);
            LCL_Mat_Flat_Index this_B_address = get<1>(*B._e[i]);

            // If we find a non-zero at this storage position
            if(this_B_val!=0) {
                // Then we see if this is zero at that address
                T this_val = operator()(this_B_address);
                // If so, we set out at this address to B
                if(this_val==0) {
                    out(this_B_address) = this_B_val;
                }
                // Otherwise we have already accounted for it in previous sum
            }
        }
    }

    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::operator*(const LCL_Matrix_Heap_Sparse<T>& B) const {
    LCL_Matrix_Heap_Sparse<T> out;

    // If dimensions match
    if(this->c()==B.r()) {
        out.resize(this->r(), B.c());
        for(int i = 0; i < _N; i++) {
            T this_val = get<0>(*_e[i]);

            // If we find a non-zero at this storage position
            if(this_val!=0) {
                LCL_Mat_Flat_Index this_address = get<1>(*_e[i]);
                LCL_Mat_2D_Index this_i = this->Index_Flat_to_i(this_address);
                LCL_Mat_2D_Index this_j = this->Index_Flat_to_j(this_address);

                // Then we see if any B has address with this_j == this_B_i
                for(int j = 0; j < B._N; j++) {
                    T this_B_val = get<0>(*B._e[j]);
                    LCL_Mat_Flat_Index this_B_address = get<1>(*B._e[j]);
                    LCL_Mat_2D_Index this_B_i = B.Index_Flat_to_i(this_B_address);
                    LCL_Mat_2D_Index this_B_j = B.Index_Flat_to_j(this_B_address);
                    if(this_j==this_B_i) {
                        out(this_i,this_B_j) += this_val*this_B_val;
                    }
                }
            }
        }
    }

    return out;
}

template <class T>
LCL_Matrix_Heap_Sparse<T>& LCL_Matrix_Heap_Sparse<T>::identity(LCL_Matrix_Heap_Sparse<T>& in, LCL_Mat_Size in_n) {
    int n = in_n;
    if(n<0) {
        n = min(in.r(),in.c());
    }
    in.clear();
    for(int i = 0; i < n; i++) {
        in(i,i) = 1;
    }
    return in;
}

template <class T>
LCL_Matrix_Heap_Sparse<T>& LCL_Matrix_Heap_Sparse<T>::clear() {
    for(int i = 0; i < _N; i++) {
        get<0>(*_e[i]) = 0;
    }
    return (*this);
}

template <class T>
LCL_Matrix_Heap_Sparse<T>& LCL_Matrix_Heap_Sparse<T>::operator=(const LCL_Matrix_Heap_Sparse<T>& in) {
    set_storage_N(in.get_storage_N());
    this->resize(in.r(), in.c());
    for(int i = 0; i < in.get_storage_N(); i++) {
        get<0>(*_e[i]) = get<0>(*in._e[i]);
        get<1>(*_e[i]) = get<1>(*in._e[i]);
    }
    return (*this);
}

template <class T>
LCL_Matrix_Heap_Sparse<T> LCL_Matrix_Heap_Sparse<T>::transpose() const {
    LCL_Matrix_Heap_Sparse<T> out(this->c(),this->r());
    LCL_Matrix<T>::transpose(out,*this);
    return out;
}

#endif // LCL_MATRIX_HEAP_SPARSE_IMP1_HEADER
