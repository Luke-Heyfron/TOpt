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

#include "Interface_BMSGSS.h"

#include <iostream>
using namespace std;

#include "BMSparse.h"
#include "GateStringSparse.h"

#include <vector>

GateStringSparse Interface_BMSGSS::BMSToGSS(const BMSparse& inBMS) {
    int n = inBMS.get_n();
    GateStringSparse out(n);

    bool* this_x = new bool[n];

    for(int j = 0; j < inBMS.get_m(); j++) {
        for(int i = 0; i < n; i++) {
            this_x[i] = inBMS.E(i,j);
        }
        out.add(this_x);
    }

    delete [] this_x;

    return out;
}

BMSparse Interface_BMSGSS::GSSToBMS(const GateStringSparse& inGSS) {
    BMSparse out(inGSS.get_n(),inGSS.weight(true));

    /*unordered_set<int> this_data = inGSS.get_data();

    bool* this_x = new bool[inGSS.get_n()];
    int col = 0;
    for(unordered_set<int>::iterator it = this_data.begin(); it != this_data.end(); it++) {
        int I = *it;
        if(I) {
            inGSS.I_to_x(I, this_x);
            for(int i = 0; i < inGSS.get_n(); i++) {
                if(this_x[i]) {
                    out.E(i,col,1);
                }
            }
            col++;
        }
    }

    delete [] this_x;*/

    int col = 0;
    vector<vector<bool>> data = inGSS.get_data();
    for(int j = 0; j < (int)data.size(); j++) {
        int sum = 0;
        for(int i = 0; i < inGSS.get_n(); i++) sum += data[j][i];
        if(sum) {
            for(int i = 0; i < inGSS.get_n(); i++) out.E(i,col,data[j][i]);
            col++;
        }
    }

    return out;
}
