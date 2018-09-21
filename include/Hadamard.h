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

#ifndef HADAMARD
#define HADAMARD

#include "Operator.h"
#include "State.h"


class Hadamard : public Operator {
	private:
		char mode;
		int actOn;
		State actOnOne(const State&, int) ;
		State actOnAll(const State&) ;
		State actOnOneOptimized(const State&, int);
		State actOnAllOptimized(const State&);
	public:
		Hadamard();
		Hadamard(int);
		State act(const State&);
		void toString(char* dest);
};


#endif
