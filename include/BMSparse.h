/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke Heyfron

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BM_SPARSE_HEADER
#define BM_SPARSE_HEADER

#include <iostream>
using namespace std;

#include <ostream>
#include <unordered_set>

class BMSparse {
    private:
        typedef unordered_set<int> BM_Data;
        BM_Data* data;
        int n;
        int m;

        int ijToI(const int i, const int j) const;
        int IToij(const int I, int& i, int& j) const;
        int maxI() const;

        static bool compare_weight(const char* in_WC, int weight_1, int weight_2);
    public:
        BMSparse();
        BMSparse(const int inN, const int inM);
        ~BMSparse();

        bool E(const int i, const int j) const;
        void E(const int i, const int j, const bool val);
        int get_n() const;
        int get_m() const;

        BMSparse swapRow(const int i1, const int i2) const;
        BMSparse swapCol(const int j1, const int j2) const;
        BMSparse addRows(const int it, const int is) const; //Adds the is^th row to the it^th row

        BMSparse row(const int i) const;
        BMSparse col(const int j) const;
        BMSparse T() const;
        bool isEmpty() const;
        bool isAllZeros() const;

        void resize(const int inN, const int inM); // Destroys contents of data
        BMSparse& assign(const BMSparse& inBM);
        BMSparse increment(int axis = 0) const;
        BMSparse multiply(const BMSparse& inBM) const;
        BMSparse multiply(bool in_x) const;
        BMSparse add(const BMSparse& inBM) const;
        BMSparse bitwise_OR(const BMSparse& inBM) const;
        BMSparse bitwise_AND(const BMSparse& inBM) const;
        bool equals(const BMSparse& inBM) const; // Returns true if dims and all elements are equal

        BMSparse composeLR(const BMSparse& inBMS) const;
        BMSparse composeUD(const BMSparse& inBMS) const;

        BMSparse& operator=(const BMSparse& inBM);
        BMSparse operator*(const BMSparse& inBM) const;
        BMSparse operator*(bool in_x) const;
        BMSparse operator+(const BMSparse& inBM) const;
        BMSparse operator&&(const BMSparse& inBMS) const; // Same as composeLR
        BMSparse operator||(const BMSparse& inBMS) const; // Same as composeUD
        bool operator==(const BMSparse& inBM) const;

        BMSparse sub(const int i0, const int j0, const int np, const int mp) const;
        BMSparse& sub(const int i0, const int j0, const BMSparse& inBMS);

        void print(ostream& out = cout) const;
        void printFull(ostream& out = cout) const;

        void to_bool_array(bool* out) const;
        void toBool(bool** out) const;
        void fromBool(const bool** in, int in_N, int in_M);

        // Begin TOptimizeRM specific methods
        void LQUPDecomposition(BMSparse& outL, BMSparse& outQ, BMSparse& outU, BMSparse& outP, const char* calledby = NULL, bool verbose = false) const;
        BMSparse nullspace(bool verbose=false) const;
        BMSparse forwardSubstitute(const BMSparse& inB) const;
        BMSparse backwardSubstitute(const BMSparse& inB) const;
        BMSparse inverse(bool verbose=false) const;
        BMSparse elementaryFactor() const;
        BMSparse elementaryFactorFast() const;
        BMSparse minimalFactor() const;
        BMSparse RowEchelon() const;
        BMSparse ColEchelon() const;
        void LPUTrapezoidal(BMSparse& outL, BMSparse& outP, BMSparse& outU, int height) const;
        void LEUKTrapezoidal(BMSparse& outL, BMSparse& outE, BMSparse& outU, BMSparse& outK, int height, int order = 0) const;
        int EUKPentagonal(BMSparse& outE, BMSparse& outU, BMSparse& outK, int height) const;
        int Lempel_delta() const;
        BMSparse sub_weight(const char* in_WC, int weight, int in_axis=0) const; // Returns a submatrix based on the weight of rows/cols
        bool S_abc(int a, int b, int c) const; // Returns the signature for term c*x_a*x_b*x_c in weighted polynomial where c in {1,2,4}
        bool LempelCubicCondition(const BMSparse& in_x) const;
        BMSparse GSMatrixExtension2(const BMSparse& in_x) const;
        int compare_signature(const BMSparse& in_BM) const; // Returns 0 if signatures are the same, 1 if they are different and -1 if they aren't comparable i.e. different number of qubits.
        // End TOptimizeRM specific methods

        int rank() const;
        int col_rank() const;
        int sum() const;
        int weight_ij(int i, int j) const; // Returns the weight of row i + weight of col j - weight of element (i,j)
        BMSparse OR_rows() const; // Returns 1 X m matrix that is the result of performing logical OR between all rows of <this>.
        BMSparse OR_cols() const; // Returns n X 1 matrix that is the result of performing logical OR between all columns of <this>.

        // File in out methods
        static BMSparse load(const char* in_filename);

        static BMSparse eye(const int inN, const int inM);
        static BMSparse unit(const int inN, const int in_i);
        static BMSparse perm(const int inP, const int x1, const int x2);
        static BMSparse random(const int inN, const int inM, const double in_P);
        static BMSparse random_perm(const int inN);
        static BMSparse addRows(const int inN, const int it, const int is);
        static BMSparse integer(const int val, const int len=-1); // Constructs col vec where the i^th element is equal to the i^th bit in the binary representation of val
};

#endif // BM_SPARSE_HEADER
