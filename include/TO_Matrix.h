#ifndef TO_MATRIX_HEADER
#define TO_MATRIX_HEADER

#include "LCL/Maths/LCL_Matrix_Heap_Sparse.h"
#include "LCL/Maths/LCL_FiniteField.h"

using TO_Matrix = LCL_Matrix_Heap_Sparse<F2>;

TO_Matrix& R(TO_Matrix& mat, int c, int t, int n=-1);

#endif // TO_MATRIX_HEADER
