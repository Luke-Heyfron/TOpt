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

#ifndef LCL_ORDERED_FIELD_HEADER
#define LCL_ORDERED_FIELD_HEADER

#include "LCL/Maths/LCL_Field.h"

class LCL_OrderedField : public LCL_Field {
    public:
        // Order operators
        virtual bool operator<(const LCL_Field& b) const = 0;
        virtual bool operator>(const LCL_Field& b) const {return !((*this)<b);}
        virtual bool operator<=(const LCL_Field& b) const {return ((*this)<b)||((*this)==b);}
        virtual bool operator>=(const LCL_Field& b) const {return ((*this)>b)||((*this)==b);}
};

#endif // LCL_ORDERED_FIELD_HEADER
