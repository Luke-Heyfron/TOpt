#ifndef HEADER_BOOL_SIGNATURE
#define HEADER_BOOL_SIGNATURE

#include <iostream>
using namespace std;
#include <ostream>

namespace Bool_Signature {
    // Basics
    bool*** construct(int n);
    void destruct(bool*** S, int n);
    void copy(bool*** S, int n, bool*** O);
    void print(bool*** S, int n, char* pre = NULL, ostream& inOS = cout);

    // Accessors
    void set(bool*** S, int i, int j, int k, bool val);
    bool get(bool*** S, int i, int j, int k);
    void from_A_matrix(bool*** S, bool** A, int n, int m);

    bool equals(bool*** S1, bool*** S2, int n);

    // Populators
    void random(bool*** S, int n);
}

#endif // HEADER_SIGNATURE
