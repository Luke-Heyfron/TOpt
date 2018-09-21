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

#ifndef LCL_CONSOLE_IN_HEADER
#define LCL_CONSOLE_IN_HEADER

namespace LCL_ConsoleIn {

    /// Retrieves the standard-out of a system command and puts it in a character array.
    /**
        \param dest - Destination c-string.
        \param n - Size of c-string.
        \param comm - The command to be passed to the system terminal.
        \return 0 if successful, non-zero otherwise.

        Remarks: This function makes use of popen, pclose etc. This requires including <stdio.h> and compiler options -std=c++11 -U__STRICT_ANSI__
    */
    int GetCommandOutput(char* dest, int n, const char* comm);
}

#endif // LCL_CONSOLE_IN_HEADER
