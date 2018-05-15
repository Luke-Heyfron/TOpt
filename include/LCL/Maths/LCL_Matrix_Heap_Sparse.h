#ifndef LCL_MATRIX_HEAP_SPARSE_HEADER
#define LCL_MATRIX_HEAP_SPARSE_HEADER

#include "LCL/Maths/LCL_Matrix.h"
#include <iostream>
using namespace std;

#include <utility>
#include "LCL/LCL_BoundedInt.h"

#ifndef LCL_DEFAULT_BLOCK_SIZE
#define LCL_DEFAULT_BLOCK_SIZE 10;
#endif // LCL_DEFAULT_BLOCK_SIZE

#ifndef LCL_DEFAULT_CLEANUP_COUNT
#define LCL_DEFAULT_CLEANUP_COUNT 10; // Number of accesses without increasing size of container before attempting to reduce size of container.
#endif // LCL_DEFAULT_CLEANUP_COUNT

/**

*/
template <class T>
class LCL_Matrix_Heap_Sparse : public LCL_Matrix<T> {
    public:
        // Fun statistics
        unsigned int resize_count = 0;
    protected:
        pair<T,LCL_Mat_Flat_Index>** _e = NULL;
        //LCL_Mat_Size _n = 0; /// Current number of non-zero elements in matrix.
        bool _auto_resize = true;
        LCL_Mat_Size _block_size = LCL_DEFAULT_BLOCK_SIZE;
        LCL_Mat_Size _N=0; /// Current size of storage container.
        LCL_Mat_Flat_Index _last_accessed = -1;
        int _cleanup = 0;
        int _cleanup_count = LCL_DEFAULT_CLEANUP_COUNT;
    public:
        // Constructors
        LCL_Matrix_Heap_Sparse();
        LCL_Matrix_Heap_Sparse(const LCL_Matrix<T>& in);
        LCL_Matrix_Heap_Sparse(const LCL_Matrix_Heap_Sparse<T>& in);
        LCL_Matrix_Heap_Sparse(LCL_Mat_Size in_r, LCL_Mat_Size in_c);
        ~LCL_Matrix_Heap_Sparse();
        void copy(const LCL_Matrix_Heap_Sparse<T>& in); /// Copies _e directly. More efficient than default copy between two objects of type LCL_Matrix_Heap_Sparse.

        // Storage
        void set_storage_N(LCL_Mat_Size in_N); /// Destroys information in this matrix
        LCL_Mat_Size get_storage_N() const;
        void resize_storage(LCL_Mat_Size in_N); /// Copies information in this matrix to newly allocated storage
        bool auto_resize() const {return _auto_resize;}
        bool& auto_resize() {return _auto_resize;}
        LCL_Mat_Size block_size() const {return _block_size;}
        LCL_Mat_Size& block_size() {return _block_size;}
        int cleanup_count() const {return _cleanup_count;}
        int& cleanup_count() {return _cleanup_count;}
        void reset_cleanup() {_cleanup = 0;}
        LCL_Mat_Size non_zero() const; /// Returns number of non-zero elements
        void cleanup(); /// Shifts all non-zero elements to front and sets storage size to number of non-zero elements.
        void print_storage() const;

        // Accessors
        /**
            Tries last accessed element first otherwise...
            Search to see if element address (i,j) is in _e, return that element.
            Otherwise, return 0;
        */
        const T& operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) const override;
        /**
            If element address (i,j) is in _e, return that element.
            Otherwise, if last accessed element was set to zero, set that element address to (i,j) and return it.
            Otherwise, search for first zero-valued element, set it's address to (i,j) and return it.
            If none found, _e is full so resize _e to _N -> _N + _block_size. Set a new element to [0, (i,j)] and return it.
            Set _last_accessed to (i,j).
        */
        T& operator()(LCL_Mat_2D_Index i, LCL_Mat_2D_Index j) override;
        const T& operator()(LCL_Mat_Flat_Index this_I) const;
        T& operator()(LCL_Mat_Flat_Index this_I);

        // Assignment
        LCL_Matrix_Heap_Sparse<T>& clear();
        LCL_Matrix_Heap_Sparse<T>& operator=(const LCL_Matrix_Heap_Sparse<T>& in);

        // Arithmetic
            // With generic other matrices
        LCL_Matrix_Heap_Sparse<T> operator+(const LCL_Matrix<T>& B) const;
        LCL_Matrix_Heap_Sparse<T> operator*(const LCL_Matrix<T>& B) const;
        LCL_Matrix_Heap_Sparse<T> operator*(const T& b) const;
        LCL_Matrix_Heap_Sparse<T> operator-(const LCL_Matrix<T>& B) const;
        LCL_Matrix_Heap_Sparse<T> operator-() const;
        LCL_Matrix_Heap_Sparse<T> operator/(const T& b) const;

            // With other Heap Flat matrices
        LCL_Matrix_Heap_Sparse<T> operator+(const LCL_Matrix_Heap_Sparse<T>& B) const;
        LCL_Matrix_Heap_Sparse<T> operator*(const LCL_Matrix_Heap_Sparse<T>& B) const;
        LCL_Matrix_Heap_Sparse<T> operator-(const LCL_Matrix_Heap_Sparse<T>& B) const;

        // Matrix operations
        LCL_Matrix_Heap_Sparse<T> transpose() const; /// Matrix transpose

        // Static element-setting procedures
        static LCL_Matrix_Heap_Sparse<T>& identity(LCL_Matrix_Heap_Sparse<T>& in, LCL_Mat_Size in_n = -1);

};

template <class T>
using Mat = LCL_Matrix_Heap_Sparse<T>;

#include "LCL/Maths/Implementations/LCL_Matrix_Heap_Sparse_imp1.h"

#endif // LCL_MATRIX_HEAP_SPARSE_HEADER
