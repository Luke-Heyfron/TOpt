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

#ifndef LCL_ARRAY_HEADER
#define LCL_ARRAY_HEADER

#include "LCL_BoundedInt.h"

#include <iostream>
using namespace std;

///
/**
    T must should defined operator= override
*/
template <class T, int N>
class LCL_Array {
    private:
        LCL_BoundedInt _size; /// Actual array length.
        int _N; /// Maximum size for container.
        T data[N];
        T clear_value;
    public:
        // Constructor
        LCL_Array();
        LCL_Array(int in_size);
        LCL_Array(int in_size, const T& in_cv);

        // Accessors
        T operator[](int i) const;
        T& operator[](int i);

        // Append
        LCL_Array& operator+=(const T& in);
        LCL_Array& operator+=(const LCL_Array& in);

        // Clear
        LCL_Array& clear();

        // Properties
        int size() const;
        int max_size() const;
        LCL_BoundedInt& size();

        // Print
        void print() const;

        // Static
        static T LCL_ArrayBadVal;

        //stdout
        template<class Y, int M>
        friend ostream& operator<<(ostream& os, const LCL_Array<Y,M>& in);
};



#endif // LCL_ARRAY_HEADER
