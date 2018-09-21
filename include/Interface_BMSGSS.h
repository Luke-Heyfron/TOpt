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

#ifndef INTERFACE_BMS_GSS_HEADER
#define INTERFACE_BMS_GSS_HEADER

#include <iostream>
using namespace std;

#include "BMSparse.h"
#include "GateStringSparse.h"

namespace Interface_BMSGSS {
    GateStringSparse BMSToGSS(const BMSparse& inBMS);
    BMSparse GSSToBMS(const GateStringSparse& inGSS);
}


#endif // INTERFACE_BMS_GSS_HEADER
