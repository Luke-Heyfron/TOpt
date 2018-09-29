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

#include "LCL_Maths.h"

#include <iostream>
using namespace std;

#include <cmath>

unsigned long long int LCL_Maths::fact(int n) {
    unsigned long long int out = 1;

    for(int i = 2; i <= n; i++) out *= i;

    return out;
}

unsigned long long int LCL_Maths::nCr(int n, int r) {
    unsigned long long int out = fact(n)/(fact(r)*fact(n-r));
    return out;
}
