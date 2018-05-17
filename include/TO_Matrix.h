#ifndef TO_MATRIX_HEADER
#define TO_MATRIX_HEADER

#include "LCL/Maths/LCL_Matrix_Heap_Sparse.h"
#include "LCL/Maths/LCL_FiniteField.h"

using TO_Matrix = LCL_Matrix_Heap_Sparse<F2>;

TO_Matrix& R(TO_Matrix& mat, int c, int t, int n=-1);

TO_Matrix& add_rows(TO_Matrix& A, int t, int s);
TO_Matrix& swap_rows(TO_Matrix& mat, int r1, int r2);
TO_Matrix& rowechelon(TO_Matrix& out, const TO_Matrix& in);
TO_Matrix& nullspace(TO_Matrix& out, const TO_Matrix& in);
//TO_Matrix& chi(TO_Matrix& out, const TO_Matrix& A, const TO_Matrix& z);


TO_Matrix& swap_columns(TO_Matrix& mat, int c1, int c2);
TO_Matrix& proper(TO_Matrix& mat);

#endif // TO_MATRIX_HEADER
