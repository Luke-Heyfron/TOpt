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

#ifndef LCL_CONSOLE_OUT_HEADER
#define LCL_CONSOLE_OUT_HEADER

#include <iostream>
using namespace std;

#include <ostream>

#include <ctime>

#define FOut() if(g_output_file)(*g_output_file)

extern int dout_n;

namespace LCL_ConsoleOut {
    extern int LOut_Pad;
	extern int NErrors;
	extern int NWarnings;

    ostream& LOut();
    void dout();

    void warning(const char* message, const char* function_name = NULL, const char* class_name = NULL);
    void error(const char* message, const char* function_name = NULL, const char* class_name = NULL);

    double secs(clock_t tic, clock_t toc);
}


#endif
