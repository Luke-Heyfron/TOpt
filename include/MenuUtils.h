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

#ifndef MENU_UTILS_HEADER
#define MENU_UTILS_HEADER

#include <string>

#include <iostream>
using namespace std;

namespace MenuUtils {
    void getOneInt(int& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneDouble(double& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneChar(char& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneCString(char* inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneString(string& inOut, istream& inIS, ostream& inOS, const char* inMessage);
}

#endif // MENU_UTILS_HEADER
