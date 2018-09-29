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

#include "LCL/LCL_BoundedInt.h"

#include "LCL/Core/LCL_ConsoleOut.h"

LCL_BoundedInt::LCL_BoundedInt() {
    ;
}

LCL_BoundedInt::LCL_BoundedInt(int in_v, int in_min, int in_max) {
    _v = in_v;
    _min = in_min;
    _max = in_max;
}

LCL_BoundedInt& LCL_BoundedInt::operator=(const int in_v) {
    if((in_v>=_min)&&(in_v<=_max)) {
        _v = in_v;
    } else {
        LCL_ConsoleOut::error("Input value out of range.", "operator=", "LCL_BoundedInt");
    }
    return (*this);
}

LCL_BoundedInt& LCL_BoundedInt::operator()(const int in_min, const int in_max) {
    _min = in_min;
    _max = in_max;
    return (*this);
}

LCL_BoundedInt& LCL_BoundedInt::operator++(int) {
    operator=(_v+1);
    return (*this);
}
