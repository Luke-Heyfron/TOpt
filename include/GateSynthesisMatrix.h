#ifndef HEADER_GATESYNTHESISMATRIX
#define HEADER_GATESYNTHESISMATRIX

namespace GateSynthesisMatrix {
    // Basics
    bool** from_signature(bool*** S, int n, int& m);
    void cleanup(bool** A, int n, int m, int& mp); // Sets pairwise col duplicates to zero, moves all zero cols to rhs, returns number of non-zero cols

    // Advanced
    void LempelX(bool** A, int n, int m, int& mp); // Form x = c_i + c_j for all col pairs. Extend matrix with rows x_a r_b r_c + x_b r_c r_a + x_c r_a r_b. Find nullspace. If N_i + N_j = 1, add x to all cols in nullspace, and eliminate c_i and c_j. Repeat until all c_i and c_j pairs have been exhausted.
    void Chi(bool** A, bool** x, int n, int m, bool** Aext); // Constructs the chi matrix from GSM, A. Aext must be n^3 x m in dimension.
    void ChiPrime(bool** A, bool** x, int n, int m, bool** Aext); // Constructs the chi matrix from GSM, A. Aext must be n^3 x m in dimension.
    void LempelX2(bool** A, int n, int m, int& mp);
}

#endif // HEADER_GATESYNTHESISMATRIX
