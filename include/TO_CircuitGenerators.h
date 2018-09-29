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

#ifndef TO_CIRCUIT_GENERATORS_HEADER
#define TO_CIRCUIT_GENERATORS_HEADER

#include "Signature.h"

Signature CircuitGenerator(const string& inS);
Signature CircuitGenerator_Toffhash(int N_hash);
Signature CircuitGenerator_Toffoli(int N_toff);
//Signature CircuitGenerator_RandomComplex(int n, int in_seed = 0);

#endif // TO_COMMANDS_HEADER
