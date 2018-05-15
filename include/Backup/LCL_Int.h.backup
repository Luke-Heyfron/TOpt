#ifndef LCL_INT_HEADER
#define LCL_INT_HEADER

#include <iostream>
using namespace std;

namespace LCL_Int {
    void sort(int* x, int n, bool desc = true , int* a = NULL, int method=0);
    // Sorts int vector x of length n in descending (ascending) order
    //      Args:
    //          x - vector to be sorted
    //          n - length of x
    //          desc - sorts in descending order if true, ascending otherwise
    //          a - stores the original index of x[i] at position a[i]
    //          method - The sorting algorithm to be used. 0 - Bubble sort.
    int randi(int in_min, int in_max);
    void randi(int* x, int n, int in_min, int in_max); //Populates x with n random integers between in_min and in_max
    void print(int* x, int n, const char* pre=NULL);
    void copy(int* dst, const int* src, int n);
    void sub(int* dst, const int* src, int n, int m, int i0=0);
    void concat(int* top, const int* bottom, int n, int m);
    void randperm(int* x, int n, int x0 = 0);
}

#endif
