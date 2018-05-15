#ifndef LCL_BOOL_HEADER
#define LCL_BOOL_HEADER

#include <iostream>
using namespace std;

namespace LCL_Bool {
    int IntToBoolVec(bool* out, int I, int m = -1);
    int BoolVecToInt(const bool* x, int len);
    int BitSize(int I);
    int Inner(bool* x1, bool* x2, int len);
    int Weight(bool* x, int len);
    void BitwiseAnd(bool* x1, bool* x2, bool* out, int len);
    void BitwiseXor(bool* x1, bool* x2, bool* out, int len);
    void copy(bool* src, bool* dst, int len);
    void print(bool* x, int len, const char* pre = NULL);
    bool increment(bool* x, int len);
    void zeros(bool* x, int len);
    bool nextUniquePerm(bool* out, const bool* in, int len);
    void fromString(bool* out, const char* in_s);
    int ReedDecoder(bool* x, int R, int M, bool* c = NULL, bool* e = NULL);
}

#endif // LCL_BOOL_HEADER
