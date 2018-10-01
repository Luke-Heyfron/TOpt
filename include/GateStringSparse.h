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

#ifndef GATE_STRING_SPARSE
#define GATE_STRING_SPARSE

#include <iostream>
using namespace std;

#include <unordered_set>
#include <vector>

class GateStringSparse {
public:
    // Settings
    const static int CSV_LINE_LENGTH = 10000;
private:
    vector<vector<bool>> data_us;
    int n;
public:
    GateStringSparse(int in_n);
    ~GateStringSparse();

    int set(const vector<bool>& in_v);
    int clear(const vector<bool>& in_v);
	//int set(int I);
    //int clear(int I);
    int set(const bool* x);
    int clear(const bool* x);
    int add(const vector<bool>& in_v);
    int add(const bool* x);
    void clear();

    //bool E(int I) const;
    bool E(const vector<bool>& in_v) const;
    bool E(int i, int j) const;
    bool E(int i, int j, bool v);

    void proper();

    int get_n() const;
    int weight(bool punc = false) const;

    void print(ostream& inOS = cout) const;
    void printString(const char* pre = NULL) const;
    void printMatrix(ostream& inOS = cout) const;
	void save(const char* filename) const;

    //int x_to_I(const bool* x) const;
    //void I_to_x(int I, bool* out) const;

    GateStringSparse& assign(const GateStringSparse& inGSS);
    GateStringSparse& operator=(const GateStringSparse& inGSS);
    GateStringSparse add(const GateStringSparse& inGSS) const;
    GateStringSparse operator+(const GateStringSparse& inGSS) const;
    GateStringSparse multiply(const GateStringSparse& inGSS) const;
    GateStringSparse operator*(const GateStringSparse& inGSS) const;

    GateStringSparse augment(const int h) const;

    vector<vector<bool>> get_data() const;

    GateStringSparse mult2xh(const int h) const;
    GateStringSparse addxh(const int h) const;

    GateStringSparse superGSS(bool* inc_vec, int len) const; // Construct a phase polynomial over a superspace where inc_vec is a weight n incidence vector of this superspace that corresponds to the space of the original phase polynomial
    GateStringSparse subGSS_xh(int h) const; // Finds the subset GSS of <this> for which x_h = 1 for all elements.
    GateStringSparse subGSS_s(bool* in_s) const;

    static GateStringSparse expandLin(int in_n, int in_x1);
    static GateStringSparse expandQuad(int in_n, int in_x1, int in_x2);
    static GateStringSparse expandCube(int in_n, int in_x1, int in_x2, int in_x3);

    static vector<GateStringSparse*> ReedMullerGenerators(int in_r, int in_m);
    static GateStringSparse MonomialBF(bool* in_monvec, int in_m);
    static GateStringSparse randomWeightN(int in_n, int N);
    static GateStringSparse randomUpTo3Qu(int in_n, int in_seed=0); // randomly generates GSS with at most three qubits in each of N terms

    static GateStringSparse* LoadCSV(const char* in_filename); /// Loads a CSV file containing the elements of a gate synthesis matrix into a new GateStringSparse object. Returns NULL if file is not found or there is an error with the file.
};

#endif // GATE_STRING_SPARSE
