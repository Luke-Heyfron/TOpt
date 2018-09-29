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

#include "LCL/Core/LCL_ConsoleOut.h"

#include <iostream>
using namespace std;

#include <ostream>
#include <ctime>
#include <TO_Decoder.h>

int LCL_ConsoleOut::LOut_Pad = 0;
int dout_n = 0;
int LCL_ConsoleOut::NErrors = 0;
int LCL_ConsoleOut::NWarnings = 0;

ostream& LCL_ConsoleOut::LOut() {
    for(int i = 0; i < LOut_Pad; i++) {
        cout << "\t";
    }
    return cout;
}

void LCL_ConsoleOut::dout() {
    LOut() << dout_n << endl;
    dout_n++;
}

double LCL_ConsoleOut::secs(clock_t tic, clock_t toc) {
    double out = 0.0;

    out = ((double)toc-(double)tic)/(double)CLOCKS_PER_SEC;

    return out;
}

void LCL_ConsoleOut::warning(const char* message, const char* function_name, const char* class_name) {
    LOut() << "WARNING! ";
    if(function_name||class_name) {
        cout << "In `";
        if(class_name) {
            cout << class_name;
            if(function_name) {
                cout << "::";
            }
        }
        if(function_name) {
            cout << function_name;
        }
        cout << "'. ";
    }
    cout << message << endl;
	NWarnings++;
}

void LCL_ConsoleOut::error(const char* message, const char* function_name, const char* class_name) {
    LOut() << "ERROR! ";
    if(function_name||class_name) {
        cout << "In `";
        if(class_name) {
            cout << class_name;
            if(function_name) {
                cout << "::";
            }
        }
        if(function_name) {
            cout << function_name;
        }
        cout << "'. ";
    }
    cout << message << endl;
	NErrors++;
}
