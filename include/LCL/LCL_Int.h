#ifndef LCL_INT_HEADER
#define LCL_INT_HEADER

#include <iostream>
using namespace std;

/// A collection of useful functions for integer arrays.
namespace LCL_Int {
    /// Sorts int vector x of length n in descending (ascending) order
    /**
              \param x - Vector to be sorted.
              \param n - length of x
              \param desc - sorts in descending order if true, ascending otherwise
              \param a - stores the original index of x[i] at position a[i]
              \param method - The sorting algorithm to be used. 0 - Bubble sort.
    */
    void sort(int* x, int n, bool desc = true , int* a = NULL, int method=0);

    int randi(int in_min, int in_max);
    void randi(int* x, int n, int in_min, int in_max); //Populates x with n random integers between in_min and in_max
    void print(int* x, int n, const char* pre=NULL);
    void copy(int* dst, const int* src, int n);

    /// Copies a sub-array of length m < n to a new array.
    /**
        \param dst - Destination array.
        \param src - Source array.
        \param
    */
    void sub(int* dst, const int* src, int n, int m, int i0=0);

    /// Concatenates vector top of length n with bottom of length bottom, the result of which is stored in top.
    void concat(int* top, const int* bottom, int n, int m);

    /// Randomly permutes the input array x. Optionally adds a constant x0 to each element.
    void randperm(int* x, int n, int x0 = 0);
}

#endif
