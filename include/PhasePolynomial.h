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

#ifndef PHASE_POLYNOMIAL_HEADER
#define PHASE_POLYNOMIAL_HEADER

#include <iostream>
using namespace std;

#include <string>
#include <unordered_set>
#include <vector>

class PhasePolynomial {
    private:
        //int* a = NULL;
        int n = 0;
        int N = 0;
        vector<int> m; // Stores coefficients of the non-zero elements of this phase polynomial
        vector<vector<bool>> a; // Stores keys/element labels associated with each element of m
    public:
        PhasePolynomial(int in_n);
        PhasePolynomial(const PhasePolynomial& in);
        ~PhasePolynomial();

        void print() const;

        int get_n() const;
        int get_N() const;
        int T() const;
        int get_m_at(const int in_t) const;
        vector<bool> get_a_at(const int in_t) const;
        void get_a_at(bool* out_x, const int in_t) const;

        void operator+=(const PhasePolynomial& inPP);
        void operator*=(const int in_I);
        void operator-=(const PhasePolynomial& inPP);
        void operator=(const PhasePolynomial& in);
        void operator%=(const int in_m);

        // Getters
		int operator[](const vector<bool>& in_v) const;
        int operator[](const int in_I) const;
        int operator[](const bool* in_x) const;
        int operator[](const string in_str) const;

        // Setters
		int& operator[](const vector<bool>& in_v);
        int& operator[](const int in_I);
        int& operator[](const bool* in_x); // Culprit
        int& operator[](const string in_str);

        // Methods
        void mod8(); // Maps elements from Z to Z_8
        void mod2(); // Maps elements from Z or Z_8 to Z_2
        void clean();
		int T_count() const;

};

#endif // PHASE_POLYNOMIAL_HEADER
