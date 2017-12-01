#ifndef WEIGHTED_POLYNOMIAL_HEADER
#define WEIGHTED_POLYNOMIAL_HEADER

#include <istream>
using namespace std;

class WeightedPolynomial {
    private:
        int n;
        int N_l;
        int N_q;
        int N_c;
        int* _l = NULL;
        int** _q = NULL;
        int*** _c = NULL;
    public:
        WeightedPolynomial(int in_n);
        WeightedPolynomial(const WeightedPolynomial& in);
        ~WeightedPolynomial();

        void print() const;

        int get_n() const;
        int get_N_l() const;
        int get_N_q() const;
        int get_N_c() const;

        int& operator()(int a);
        const int& operator()(int a) const;
        int& operator()(int a, int b);
        const int& operator()(int a, int b) const;
        int& operator()(int a, int b, int c);
        const int& operator()(int a, int b, int c) const;

        // Assignment Operators
        void operator+=(const WeightedPolynomial& in);
        void operator*=(int in);
        void operator-=(const WeightedPolynomial& in);
        void operator=(const WeightedPolynomial& in);
        void operator%=(int in);

        // Index maps
        void ijk_to_abc(int i, int j, int k, int* a = NULL, int* b = NULL,int* c = NULL) const;
        void abc_to_ijk(int a, int b, int c, int* i = NULL, int* j = NULL,int* k = NULL) const;
        void dims_l(int* N_i = NULL) const;
        void dims_q(int i, int* N_i = NULL, int* N_j = NULL) const;
        void dims_c(int i, int j, int* N_i = NULL, int* N_j = NULL, int* N_k = NULL) const;
};

#endif // WEIGHTED_POLYNOMIAL_HEADER
