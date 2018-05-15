#ifndef LCL_FINITE_FIELD_HEADER
#define LCL_FINITE_FIELD_HEADER

#include "LCL/Maths/LCL_Field.h"

#include <iostream>
using namespace std;

template<int P>
class LCL_FiniteField : public LCL_Field {
    private:
        int _val; /// Is always in range [0,P-1]
        int _p;
    public:
        LCL_FiniteField();
        LCL_FiniteField(const LCL_Field& in);
        LCL_FiniteField(int in);

        // Assignment
        LCL_Field& operator=(const LCL_Field& b);
        LCL_Field& operator=(int b);

        // Arithmetic
        const LCL_Field& operator+(const LCL_Field& b) const;
        const LCL_Field& operator-() const;
        const LCL_Field& operator*(const LCL_Field& b) const;
        const LCL_Field& operator~() const;
        const LCL_Field& operator/(const LCL_Field& b) const;

        // Comparison
        bool operator==(const LCL_Field& b) const;

        // Conversion
        operator int () const;

        // stdout
        template <int Q>
        friend ostream& operator<<(ostream& os, const LCL_FiniteField<Q>&  f);

        // Linear algebraic
        //template <int N, int M>
        //LCL_Matrix<LCL_FiniteField<P>,N,M>& operator*(const LCL_Matrix<LCL_FiniteField<P>,N,M>& in) const {return in*(*this);}
};

// Some convenient typedefs for common finite fields
typedef LCL_FiniteField<2> F2;
typedef LCL_FiniteField<3> F3;
typedef LCL_FiniteField<5> F5;
typedef LCL_FiniteField<7> F7;

/*template <int N, int M>
using F2Mat = LCL_Matrix<F2,N,M>;
template <int N, int M>
using F3Mat = LCL_Matrix<F3,N,M>;
template <int N, int M>
using F5Mat = LCL_Matrix<F5,N,M>;
template <int N, int M>
using F7Mat = LCL_Matrix<F7,N,M>;*/

#include "LCL/Maths/Implementations/LCL_FiniteField_imp1.h"

#endif // LCL_FINITE_FIELD_HEADER
