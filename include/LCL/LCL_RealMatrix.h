#ifndef LCL_REALMATRIX
#define LCL_REALMATRIX

/*
    LCL_RealMatrix is a class for fast matrix calculations on the real set. It uses double as it's base data type and strictly static allocation for maximum speed.
    Implementations of all functions are optimized for performance. Use a different class if dynamic allocation is necessary.
*/

#include <iostream>
using namespace std;

#include <ostream>

template <int N, int M>
class LCL_RealMatrix {
    private:
        double data[N][M];
        int n = N;
        int m = M;
    public:
    // Constructor
    LCL_RealMatrix();

    // Output
    void print(ostream& in_OS = cout) const;

    // Accessors
    double operator()(int i, int j) const;
    double& operator()(int i, int j);

    // Binary Operators

    // Unary Operators
};

#endif // LCL_REALMATRIX
