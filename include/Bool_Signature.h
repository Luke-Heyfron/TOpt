/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke E. Heyfron, Earl T. Campbell
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

#ifndef HEADER_BOOL_SIGNATURE
#define HEADER_BOOL_SIGNATURE

#include <iostream>
using namespace std;
#include <ostream>

namespace Bool_Signature {
    // Basics
    bool*** construct(int n);
    void destruct(bool*** S, int n);
    void copy(bool*** S, int n, bool*** O);
    void print(bool*** S, int n, char* pre = NULL, ostream& inOS = cout);

    // Accessors
    void set(bool*** S, int i, int j, int k, bool val);
    bool get(bool*** S, int i, int j, int k);
    void from_A_matrix(bool*** S, bool** A, int n, int m);

    bool equals(bool*** S1, bool*** S2, int n);

    // Populators
    void random(bool*** S, int n);
}

#endif // HEADER_SIGNATURE
