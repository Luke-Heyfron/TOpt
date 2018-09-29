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

#ifndef LCL_MENU_UTILS_HEADER
#define LCL_MENU_UTILS_HEADER

#include <string>

#include <iostream>
using namespace std;

namespace LCL_MenuUtils {
    void getOneInt(int& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneDouble(double& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneChar(char& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneCString(char* inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneString(string& inOut, istream& inIS, ostream& inOS, const char* inMessage);
}

#endif // MENU_UTILS_HEADER
