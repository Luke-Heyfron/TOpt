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

#include "GateStringSparse.h"

#include <iostream>
using namespace std;

#include <unordered_set>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>

#include "LCL/Modules/LCL_Maths.h"
#include "LCL/LCL_Bool.h"
#include "LCL/LCL_Int.h"
#include "BoolMat.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

GateStringSparse::GateStringSparse(int in_n) {
    //data_us = new unordered_set<vector<bool>>;
    n = in_n;
}

GateStringSparse::~GateStringSparse() {
    //delete data_us;
}

int GateStringSparse::set(const vector<bool>& in_v) {
	if((int)in_v.size()==n) {
		bool found = 0;
		for(int t = 0; (!found)&&(t<(int)data_us.size()); t++) {
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (data_us[t][i]==in_v[i]);
			}
			if(found) {
				return 0;
			}
		}
		if(!found) {
			data_us.push_back(in_v);
			return 1;
		}
	} else {
		error("GateStringSparse key should have length n!", "set", "GateStringSparse");
		LOut_Pad++;
            LOut() << "n = " << n << endl;
            LOut() << "Input key:" << endl;
            LOut();
            for(int i = 0; i < (int)in_v.size(); i++) cout << in_v[i];
            cout << endl;
		LOut_Pad--;
		throw "GateStringSparse key should have length n!";

	}
    //pair<unordered_set<vector<bool>>::iterator,bool> temp = data_us->insert(in_v);
    //return temp.second;
    return 0;
}

int GateStringSparse::clear(const vector<bool>& in_v) {
	if((int)in_v.size()==n) {
		bool found = 0;
		for(int t = 0; (!found)&&(t<(int)data_us.size()); t++) {
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (data_us[t][i]==in_v[i]);
			}
			if(found) {
				data_us.erase(data_us.begin() + t);
				return 1;
			}
		}
		if(!found) {
			return 0;
		}
	} else {
		error("GateStringSparse key should have length n!", "clear", "GateStringSparse");
		throw "GateStringSparse key should have length n!";

	}
    //unordered_set<vector<bool>>::size_type temp = data_us->erase(in_v);
    //return temp;
    return 0;
}

/*int GateStringSparse::set(int I) {
    pair<unordered_set<int>::iterator,bool> temp = data_us->insert(I);
	warning("Don't use this", "set", "GateStringSparse");
    return temp.second;
}

int GateStringSparse::clear(int I) {
    unordered_set<int>::size_type temp = data_us->erase(I);
	warning("Don't use this", "clear", "GateStringSparse");
    return temp;
}*/

int GateStringSparse::set(const bool* x) {
	vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i]=x[i];

    return set(v);
}

int GateStringSparse::clear(const bool* x) {
	vector<bool> v;
	v.resize(n);
	for(int i = 0; i < n; i++) v[i]=x[i];

    return clear(v);
}

void GateStringSparse::clear() {
    //data_us->clear();
    data_us.clear();
}

int GateStringSparse::get_n() const {
    return n;
}

void GateStringSparse::print(ostream& inOS) const {
    inOS << "Gate string: " << n << " x " << weight(true) << endl;

    /*bool this_x[n];
    int this_I;
    int c = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it!=data_us->end(); it++) {
        this_I = *it;
        I_to_x(this_I, this_x);
        int m = 0;
        for(int i = 0; i < n; i++) m += this_x[i];
        int l = 0;
        for(int i = 0; i < n; i++) {
            if(this_x[i]) {
                l++;
                inOS << "x" << (i+1);
                if(l<m) {
                    inOS << "^";
                }
            }
        }
        if(c<(int)(data_us->size()-1))
        inOS << " + ";
        c++;
    }*/
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < (int)data_us.size(); j++) {
			inOS << data_us[j][i];
		}
		inOS << endl;
	}
    inOS << endl;

}

bool GateStringSparse::E(const vector<bool>& in_v) const {
    /*unordered_set<int>::iterator ePos = data_us->find(in_v);
    if(ePos==data_us->end()) {
        return 0;
    } else {
        return 1;
    }*/
	if((int)in_v.size()==n) {
		bool found = 0;
		for(int t = 0; (!found)&&(t<(int)data_us.size()); t++) {
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (data_us[t][i]==in_v[i]);
			}
			if(found) {
				return 1;
			}
		}
		if(!found) {
			return 0;
		}
	} else {
		error("GateStringSparse key should have length n!", "E", "GateStringSparse");
		throw "GateStringSparse key should have length n!";

	}
	return 0;
}

/*bool GateStringSparse::E(int I) const {
    unordered_set<int>::iterator ePos = data_us->find(I);
	warning("Don't use this", "E", "GateStringSparse");
    if(ePos==data_us->end()) {
        return 0;
    } else {
        return 1;
    }
}*/

void GateStringSparse::printString(const char* pre) const {
    /*if(pre) cout << pre;
    for(int i = 0; i < pow(2,n); i++) {
        cout << E(i);
    }
    cout << endl;*/
	error("Use of unimplemented function", "printString", "GateStringSparse");
}

/*int GateStringSparse::x_to_I(const bool* x) const {
    int out = 0;
    for(int i = 0; i < n; i++) {
        out += x[i]*pow(2,i);
    }
    return out;
}

void GateStringSparse::I_to_x(int I, bool* out) const {
    int temp = I;
    for(int i = 0; i < n; i++) {
        out[i] = temp%2;
        temp /= 2;
    }
}*/

GateStringSparse GateStringSparse::expandQuad(int in_n, int in_x1, int in_x2) {
    in_x1--;
    in_x2--;
    GateStringSparse out(in_n);
    bool x1x2[in_n], x1[in_n], x2[in_n];
    for(int i = 0; i < in_n; i++) {
        if((i==in_x1)||(i==in_x2)) x1x2[i] = 1;
        else x1x2[i] = 0;

        if(i==in_x1) x1[i] = 1;
        else x1[i] = 0;

        if(i==in_x2) x2[i] = 1;
        else x2[i] = 0;
    }
    out.set(x1x2);
    out.set(x1);
    out.set(x2);
    return out;
}

GateStringSparse GateStringSparse::expandCube(int in_n, int in_x1, int in_x2, int in_x3) {
    in_x1--;
    in_x2--;
    in_x3--;
    GateStringSparse out(in_n);
    bool x1x2x3[in_n], x1x2[in_n], x2x3[in_n], x1x3[in_n], x1[in_n], x2[in_n], x3[in_n];
    for(int i = 0; i < in_n; i++) {
        if((i==in_x1)||(i==in_x2)||(i==in_x3)) x1x2x3[i] = 1;
        else x1x2x3[i] = 0;

        if((i==in_x1)||(i==in_x2)) x1x2[i] = 1;
        else x1x2[i] = 0;

        if((i==in_x2)||(i==in_x3)) x2x3[i] = 1;
        else x2x3[i] = 0;

        if((i==in_x1)||(i==in_x3)) x1x3[i] = 1;
        else x1x3[i] = 0;

        if(i==in_x1) x1[i] = 1;
        else x1[i] = 0;

        if(i==in_x2) x2[i] = 1;
        else x2[i] = 0;

        if(i==in_x3) x3[i] = 1;
        else x3[i] = 0;
    }
    out.set(x1x2x3);
    out.set(x1x2);
    out.set(x2x3);
    out.set(x1x3);
    out.set(x1);
    out.set(x2);
    out.set(x3);
    return out;
}

GateStringSparse GateStringSparse::expandLin(int in_n, int in_x1) {
    in_x1--;
    GateStringSparse out(in_n);
    bool x1[in_n];
    for(int i = 0; i < in_n; i++) {
        if(i==in_x1) x1[i] = 1;
        else x1[i] = 0;
    }
    out.set(x1);
    return out;
}

int GateStringSparse::add(const vector<bool>& in_v) {
    int out = set(in_v);
    if(!out) {
        clear(in_v);
    }
    return out;
}

int GateStringSparse::add(const bool* in_v) {
    int out = set(in_v);
    if(!out) {
        clear(in_v);
    }
    return out;
}


GateStringSparse GateStringSparse::add(const GateStringSparse& inGSS) const {
    GateStringSparse out(n);

	// Search both GSM's. Add columns if and only if they appear in one GSM but not the other.

	for(int t1 = 0; t1 < (int)data_us.size(); t1++) {
		vector<bool> col1 = data_us[t1];
		bool found = 0;
		for(int t2 = 0; (!found)&&(t2 < (int)inGSS.data_us.size()); t2++) {
			vector<bool> col2 = inGSS.data_us[t2];
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (col1[i]==col2[i]);
			}
		}
		if(!found) {
            out.set(col1);
        }
	}

	for(int t1 = 0; t1 < (int)inGSS.data_us.size(); t1++) {
		vector<bool> col1 = inGSS.data_us[t1];
		bool found = 0;
		for(int t2 = 0; (!found)&&(t2 < (int)data_us.size()); t2++) {
			vector<bool> col2 = data_us[t2];
			found = 1;
			for(int i = 0; found&&(i < n); i++) {
				found = (col1[i]==col2[i]);
			}
		}
		if(!found) {
            out.set(col1);
        }
	}

    /*for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        vector<bool> thisI = *it;
        unordered_set<int>::iterator ePos = inGSS.data_us->find(thisI);
        if(ePos==inGSS.data_us->end()) {
            out.set(thisI);
        }
    }

    for(unordered_set<int>::iterator it = inGSS.data_us->begin(); it != inGSS.data_us->end(); it++) {
        vector<bool> thisI = *it;
        unordered_set<int>::iterator ePos = data_us->find(thisI);
        if(ePos==data_us->end()) {
            out.set(thisI);
        }
    }*/

    return out;
}

bool GateStringSparse::E(int i, int j) const {
    if((0<=i)&&(i<(int)data_us.size())&&(0<=j)&&(j<(int)data_us[i].size())) {
        return data_us[i][j];
    } else {
        error("Index out of bounds.", "E", "GateStringSparse");
        LOut_Pad++;
            LOut() << "i = " << i << "; j = " << j << endl;
        LOut_Pad--;
        return 0;
    }
}

bool GateStringSparse::E(int i, int j, bool v) {
    if((0<=i)&&(i<(int)data_us.size())&&(0<=j)&&(j<(int)data_us[i].size())) {
        data_us[i][j] = v;
        return data_us[i][j];
    } else {
        error("Index out of bounds.", "E", "GateStringSparse");
        LOut_Pad++;
            LOut() << "i = " << i << "; j = " << j << endl;
        LOut_Pad--;
        return 0;
    }
}

GateStringSparse GateStringSparse::operator+(const GateStringSparse& inGSS) const {
    return add(inGSS);
}

GateStringSparse& GateStringSparse::assign(const GateStringSparse& inGSS) {
    data_us = (inGSS.data_us);
    n = inGSS.n;
    return *this;
}

GateStringSparse& GateStringSparse::operator=(const GateStringSparse& inGSS) {
    return assign(inGSS);
}

vector<GateStringSparse*> GateStringSparse::ReedMullerGenerators(int in_r, int in_m) {

    vector<GateStringSparse*> out;

    for(int r = 0; r <= in_r; r++) {
        BoolMat monvec_BM = BoolMat::AllUniqueBinaryPerms(in_m, r);
        bool this_monvec[in_m];
        for(int i = 0; i < monvec_BM.N(); i++) {
            for(int j = 0; j < in_m; j++) {
                this_monvec[j] = monvec_BM.E(i,j);
            }
            GateStringSparse* this_monBF = new GateStringSparse(in_m);
            this_monBF->assign(GateStringSparse::MonomialBF(this_monvec,in_m));
            out.push_back(this_monBF);

        }
    }
	//error("Use of unimplemented function", "ReedMullerGenerators", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::MonomialBF(bool* in_monvec, int in_m) {
    int N = pow(2,in_m);
    GateStringSparse out(in_m);
    bool B_i[in_m];
    int r = 0;
    for(int i = 0; i < in_m; i++) r += in_monvec[i];
    for(int i = 0; i < N; i++) {
        LCL_Bool::IntToBoolVec(B_i,i,in_m);
        if(LCL_Bool::Inner(B_i,in_monvec,in_m)==r) {
            bool x[in_m];
            LCL_Bool::IntToBoolVec(x,i,in_m);
            out.set(x);
        }
    }
	//error("Use of unimplemented function", "MonomialBF", "GateStringSparse");
    return out;
}

int GateStringSparse::weight(bool punc) const {
	vector<bool> v;
	v.resize(n);
    return data_us.size()-punc*E(v);
}

GateStringSparse GateStringSparse::mult2xh(const int h) const {
    GateStringSparse out(n);

    // Start by copying this to out
    out.assign(*this);

    /*
    bool* this_x = new bool[n];
    // Iterate over all elements
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        I_to_x(I,this_x);
        // Get the key and set (h-1)^th element to 1 and set this element in output.
        this_x[h-1]=1;
        out.set(this_x);
    }*/

    vector<bool> this_x(n);
    for(int t = 0; t < (int)data_us.size(); t++) {
        this_x = data_us[t];
        int sum = 0;
        for(int i = 0; i < n; i++) sum += this_x[i];
        if(sum>0) {
            this_x[h-1] = !this_x[h-1];
            out.add(this_x);
        }
    }

    // If this has odd no. cols
    if(weight(true)%2) {
        for(int i = 0; i < n; i++) {
            // Create a key with (h-1)^th element equal to 1 and the rest zero. Set that element in output.
            if(i==(h-1)) {
                this_x[i]=1;
            } else {
                this_x[i]=0;
            }
        }
        out.add(this_x);
    }

	//error("Use of unimplemented function", "mult2xh", "GateStringSparse");

    return out;
}

void GateStringSparse::proper() {
    for(int i = 0; i < weight(); i++) {
        vector<bool> c_i = data_us[i];
        int sum = 0;
        for(int k = 0; k < n; k++) {
            sum += c_i[k];
        }
        if(!sum) {
            data_us.erase(data_us.begin()+i);
            i--;
        }
    }

    for(int i = 0; i < (weight()-1); i++) {
        vector<bool> c_i = data_us[i];
        for(int j = (i+1); j < weight(); j++) {
            vector<bool> c_j = data_us[j];
            int sum = 0;
            for(int k = 0; k < n; k++) {
                sum += ((c_i[k]+c_j[k])%2);
            }
            if(!sum) {
                data_us.erase(data_us.begin()+j);
                j--;
                data_us.erase(data_us.begin()+i);
                i--;
            }
        }
    }
}

vector<vector<bool>> GateStringSparse::get_data() const {
    vector<vector<bool>> out;

    out = data_us;

    return out;
}

GateStringSparse GateStringSparse::augment(const int h) const {
    GateStringSparse out(n+1);

    /*bool* this_x = new bool[n];
    bool* this_xout = new bool[n+1];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        I_to_x(I, this_x);
        for(int i = 0; i < n; i++) {
            if(i<(h-1)){
                this_xout[i] = this_x[i];
            }
            if(i==(h-1)) {
                this_xout[i] = 0;
            }
            if(i>=(h-1)) {
                this_xout[i+1] = this_x[i];
            }
        }
        if((h-1)>=n) this_xout[n] = false;
        out.set(this_xout);
    }
    delete [] this_x;
    delete [] this_xout;*/

	error("Use of unimplemented function", "augment", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::addxh(const int h) const {
    GateStringSparse out(n);

    /*bool* this_x = new bool[n];
    // Iterate over each element
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        // Get the key as a column vector
        I_to_x(I,this_x);
        // Flip the (h-1)^th element of the key.
        this_x[h-1] = !this_x[h-1];
        // Set the element of out given by the new key.
        out.set(this_x);
    }
    delete [] this_x;*/

    vector<bool> this_x(n);
    for(int t = 0; t < (int)data_us.size(); t++) {
        int sum = 0;
        for(int i = 0; i < n; i++) sum += data_us[t][i];
        if(sum) {
            this_x = data_us[t];
            this_x[h-1] = !this_x[h-1];
            out.add(this_x);
        }
    }

	//error("Use of unimplemented function", "addxh", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::randomWeightN(int in_n, int N) {
    GateStringSparse out(in_n);

    /*for(int i = 0; i < N; i++) {
        bool exit = false;
        while(!exit) {
            int this_I = LCL_Int::randi(1,pow(2,in_n)-1);
            if(!out.E(this_I)) {
                out.set(this_I);
                exit = true;
            }
        }
    }*/

	error("Use of unimplemented function", "randomWeightN", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::randomUpTo3Qu(int n, int in_seed) {
    GateStringSparse out(n);
    if(in_seed>0) {
        srand(in_seed);
    }
    //int Ni = n;
    //int Njk = 0.5*n*(n-1);
    //int Nlmp = (1.0/6.0)*n*(n-1)*(n-2);
    //int NI = Ni + Njk + Nlmp;
    bool* this_x = new bool[n];
    for(int i = 0; i < n; i++) {
        bool this_coin = rand()%2;
        if(this_coin) {
            for(int a = 0; a < n; a++) {
                this_x[a] = (a==i);
            }
            out.set(this_x);
        }
    }
    for(int j = 0; j < (n-1); j++) {
        for(int k = j+1; k < n; k++) {
            bool this_coin = rand()%2;
            if(this_coin) {
                for(int a = 0; a < n; a++) {
                    this_x[a] = ((a==j)||(a==k));
                }
                out.set(this_x);
            }
        }
    }
    for(int l = 0; l < (n-2); l++) {
        for(int m = l+1; m < (n-1); m++) {
            for(int p = m+1; p < n; p++) {
                bool this_coin = rand()%2;
                if(this_coin) {
                    for(int a = 0; a < n; a++) {
                        this_x[a] = ((a==l)||(a==m)||(a==p));
                    }
                    out.set(this_x);
                }
            }
        }
    }

    delete [] this_x;
    if(in_seed>0) {
        srand(time(NULL));
    }
    return out;
}

GateStringSparse GateStringSparse::superGSS(bool* inc_vec, int len) const {
    GateStringSparse out(len);

    int* to_super_mapping = new int[n];

    int h_count = 0;
    for(int i = 0; i < len; i++) {
        if(inc_vec[i]) {
            to_super_mapping[h_count] = (i+1);
            h_count++;
        }
    }

    if(h_count==n) {
        /*bool* this_x = new bool[n];
        bool* super_x = new bool[len];*/
        vector<bool> this_x(n);
        vector<bool> super_x(len);
        // Iterate over all elements
        for(int t = 0; t < weight(); t++) {
            this_x = data_us[t];
            // Key the key (confirmed it is non-zero
            for(int i = 0; i < len; i++) super_x[i] = 0;
            for(int i = 0; i < n; i++) {
                if(this_x[i]) {
                    super_x[to_super_mapping[i]-1] = 1;
                }
            }
            out.set(super_x);
        }

        /*delete [] this_x;
        delete [] super_x;*/
    } else {
        error("inc_vec must have weight of exactly n.", "supperGSS", "GateStringSparse");
    }

    delete [] to_super_mapping;

	//error("Use of unimplemented function", "superGSS", "GateStringSparse");

    return out;
}

void GateStringSparse::printMatrix(ostream& inOS) const {
    /*BoolMat thisBM(n, weight(true));

    bool* this_x = new bool[n];
    int col = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        I_to_x(this_I, this_x);
        if(this_I) {
            for(int i = 0; i < n; i++) thisBM.E(i,col) = this_x[i];
            col++;
        }
    }

    thisBM.print(inOS);

    delete [] this_x;*/

	print(inOS);
}

GateStringSparse GateStringSparse::subGSS_xh(int h) const {
    GateStringSparse out(n);

    /*bool* this_x = new bool[n];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        I_to_x(this_I,this_x);
        if(this_x[h-1]) {
            out.set(this_x);
        }
    }
    delete [] this_x;*/

	error("Use of unimplemented function", "subGSS_xh", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::subGSS_s(bool* in_s) const {
    GateStringSparse out(n);

    /*int weight = 0;
    for(int i = 0; i < n; i++) weight += in_s[i];
    bool* this_x = new bool[n];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        I_to_x(this_I,this_x);
        int this_weight = 0;
        for(int i = 0; i < n; i++) this_weight += (in_s[i]&&this_x[i]);
        if(this_weight>=weight) {
            out.set(this_x);
        }
    }
    delete [] this_x;*/

	error("Use of unimplemented function", "subGSS_s", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::multiply(const GateStringSparse& inGSS) const {
    GateStringSparse out(n);

    /*for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        for(unordered_set<int>::iterator jt = inGSS.data_us->begin(); jt != inGSS.data_us->end(); jt++) {
            int this_J = *jt;
            if(this_I==this_J) {
                out.set(this_I);
            }
        }
    }*/

	error("Use of unimplemented function", "multiply", "GateStringSparse");

    return out;
}

GateStringSparse GateStringSparse::operator*(const GateStringSparse& inGSS) const {
    return multiply(inGSS);
}

GateStringSparse* GateStringSparse::LoadCSV(const char* in_filename) {
    GateStringSparse* out = NULL;

    ifstream f(in_filename);

    if(f.good()) {
        char* line = new char[CSV_LINE_LENGTH];
        char* token = NULL;
        vector<vector<bool>> data;
        vector<bool> row;
        int n_cols = 0;
        int n_cols_before = 0;
        while(!f.eof()) {
            f.getline(line,CSV_LINE_LENGTH);
            line[strcspn(line,"#")]='\0';
            if(line&&strlen(line)) {
                row.clear();
                token = strtok(line,", \t");
                do {
                    if(token&&strlen(token)) {
                        bool b = (bool)atoi(token);
                        row.push_back(b);

                    }
                } while((token = strtok(NULL,", \t")));
                if((int)row.size()>n_cols) {
                    n_cols_before = n_cols;
                    n_cols = row.size();
                    if((n_cols_before>0)&&(n_cols>n_cols_before)) {
                        warning("Number of columns is inconsistent.", "LoadCSV", "GateStringSparse");
                    }
                }
                data.push_back(row);
            }
        }
        delete [] line;

        int n_rows = data.size();
        for(int i = 0; i < n_rows; i++) {
            data[i].resize(n_cols);
        }

        out = new GateStringSparse(n_rows);
        for(int j = 0; j < n_cols; j++) {
            bool x[n_rows];
            for(int i = 0; i < n_rows; i++) {
                x[i] = data[i][j];
            }
            out->add(x);
        }
    }

    f.close();

    return out;
}
