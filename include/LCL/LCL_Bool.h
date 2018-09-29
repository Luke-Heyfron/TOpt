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

#ifndef LCL_BOOL_HEADER
#define LCL_BOOL_HEADER

#include <iostream>
using namespace std;

namespace LCL_Bool {
    int IntToBoolVec(bool* out, int I, int m = -1);
    int BoolVecToInt(const bool* x, int len);
    int BitSize(int I);
    int Inner(bool* x1, bool* x2, int len);
    int Weight(bool* x, int len);
    void BitwiseAnd(bool* x1, bool* x2, bool* out, int len);
    void BitwiseXor(bool* x1, bool* x2, bool* out, int len);
    void copy(bool* src, bool* dst, int len);
    void print(bool* x, int len, const char* pre = NULL);
    bool increment(bool* x, int len);
    void zeros(bool* x, int len);
    bool nextUniquePerm(bool* out, const bool* in, int len);
    void fromString(bool* out, const char* in_s);
    int ReedDecoder(bool* x, int R, int M, bool* c = NULL, bool* e = NULL);
}

#endif // LCL_BOOL_HEADER
