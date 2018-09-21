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
