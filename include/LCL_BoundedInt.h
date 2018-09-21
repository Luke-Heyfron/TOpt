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

#ifndef LCL_BOUNDED_INT_HEADER
#define LCL_BOUNDED_INT_HEADER

class LCL_BoundedInt {
    private:
        int _v = 0;
        int _min = 0;
        int _max = 0;
    public:
        LCL_BoundedInt();
        LCL_BoundedInt(int in_v, int in_min, int in_max);

        // Conversion
        operator int() const {return _v;}
        operator int&() {return _v;}

        // Assignment
        LCL_BoundedInt& operator=(const int in_v);
        LCL_BoundedInt& operator()(const int in_min, const int in_max);
        LCL_BoundedInt& operator++(int);
};

#endif // LCL_BOUNDED_INT_HEADER
