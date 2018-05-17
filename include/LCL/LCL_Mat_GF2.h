#ifndef HEADER_LCL_Mat_GF2
#define HEADER_LCL_Mat_GF2

#include <iostream>
#include <ostream>

using namespace std;

namespace LCL_Mat_GF2 {
    // Basics
    bool** construct(int n, int m);
    void destruct(bool** A, int n, int m);
    void copy(bool const** A, int n, int m, bool** O);
    void print(bool const** A, int n, int m, char* pre = NULL, bool header = true, ostream& inOS = cout);

    // Arithmetic
    void add(bool const** A, bool const** B, int n, int m, bool** O);
    void times(bool const** A, bool const** B, int n, int m, int p, bool** O);

    // Transpose
    void transpose(bool const** A, int n, int m, bool** O);

    // Row operations
    void addrow(bool** A, int n, int m, int i_t, int i_s);
    void swaprow(bool** A, int n, int m, int i_1, int i_2);

    // Col operations
    void addcol(bool** A, int n, int m, int j_t, int j_s);
    void swapcol(bool** A, int n, int m, int j_1, int j_2);

    // Advanced operations
    void rowechelon(bool** A, int n, int m);
    bool** nullspace(bool const** A, int n, int m, int& d);

    // Populate elements procedurally
    void eye(bool** A, int n, int m);
    void zeros(bool** A, int n, int m);
    void random(bool** A, int n, int m);
}

#endif // HEADER_LCL_Mat_GF2
