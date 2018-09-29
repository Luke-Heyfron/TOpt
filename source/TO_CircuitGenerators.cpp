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

#include "TO_CircuitGenerators.h"

#include <iostream>
using namespace std;
#include <sstream>

#include "Signature.h"
#include "TO_Decoder.h"


Signature CircuitGenerator_Toffhash(int N_hash) {
    int n = 2*N_hash+1;
    Signature out(n);
    for(int t = 0; t < N_hash; t++){
        out.set(1,2*t+2,2*t+3);
    }
    ostringstream temp_ss;
    temp_ss << N_hash;
    g_indvar_out = temp_ss.str();
    return out;
}

Signature CircuitGenerator_Toffoli(int N_toff) {
    int n = 3*N_toff;
    Signature out(n);
    for(int t = 0; t < N_toff; t++){
        out.set(3*t+1,3*t+2,3*t+3);
    }
    ostringstream temp_ss;
    temp_ss << N_toff;
    g_indvar_out = temp_ss.str();
    return out;
}

Signature CircuitGenerator(const string& inS) {
    Signature out;

    char split_char = '-';
    int split_pos = inS.find(split_char);
    string circuit_name = inS.substr(0,split_pos);
    string circuit_args = inS.substr(split_pos+1);

    if(!circuit_name.compare("Toffhash")) {
        int N_hash = atoi(circuit_args.c_str());
        cout << "Toffhash circuit; N_hash = " << N_hash << endl;
        out = CircuitGenerator_Toffhash(N_hash);
    } else if(!circuit_name.compare("Toffoli")) {
        int N_toff = atoi(circuit_args.c_str());
        cout << "Toffoli circuit; N_toff = " << N_toff << endl;
        out = CircuitGenerator_Toffoli(N_toff);
    } /*else if(!circuit_name.compare("RandomComplex")) {
        int n;
        int this_seed = 0;
        int arg_split_pos = circuit_args.find(split_char);
        if(arg_split_pos!=circuit_args.npos) {
            string seed_string = circuit_args.substr(arg_split_pos+1);
            this_seed = atoi(seed_string.c_str());
        }
        n = atoi(circuit_args.substr(0,arg_split_pos).c_str());

        cout << "Random complex circuit; n = " << n;
        if(this_seed>0) {
            cout << "; seed = " << this_seed;
            g_random_circuit_seed = this_seed;
        }
        cout << endl;
        out = CircuitGenerator_RandomComplex(n,this_seed);
    }*/

    return out;
}

/*Signature CircuitGenerator_RandomComplex(int n, int in_seed) {
    Signature out;

    GateStringSparse complex_GSS = GateStringSparse::randomUpTo3Qu(n, in_seed);
    out = GateSigInterface::expandGSSTerm(complex_GSS);

    ostringstream temp_ss;
    temp_ss << n;
    g_indvar_out = temp_ss.str();

    return out;
}*/
