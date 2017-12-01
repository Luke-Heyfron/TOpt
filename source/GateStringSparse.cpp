#include "GateStringSparse.h"

#include <iostream>
using namespace std;

#include <unordered_set>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "LCL_Maths.h"
#include "LCL_Bool.h"
#include "LCL_Int.h"
#include "BoolMat.h"

GateStringSparse::GateStringSparse(int in_n) {
    data_us = new unordered_set<int>;
    n = in_n;
}

GateStringSparse::~GateStringSparse() {
    delete data_us;
}

int GateStringSparse::set(int I) {
    pair<unordered_set<int>::iterator,bool> temp = data_us->insert(I);
    return temp.second;
}

int GateStringSparse::clear(int I) {
    unordered_set<int>::size_type temp = data_us->erase(I);
    return temp;
}

int GateStringSparse::set(const bool* x) {
    return set(x_to_I(x));
}

int GateStringSparse::clear(const bool* x) {
    return clear(x_to_I(x));
}

void GateStringSparse::clear() {
    data_us->clear();
}

int GateStringSparse::get_n() const {
    return n;
}

void GateStringSparse::print(ostream& inOS) const {
    inOS << "Gate string: ";

    bool this_x[n];
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
    }
    inOS << endl;

}

bool GateStringSparse::E(int I) const {
    unordered_set<int>::iterator ePos = data_us->find(I);
    if(ePos==data_us->end()) {
        return 0;
    } else {
        return 1;
    }
}

void GateStringSparse::printString(const char* pre) const {
    if(pre) cout << pre;
    for(int i = 0; i < pow(2,n); i++) {
        cout << E(i);
    }
    cout << endl;
}

int GateStringSparse::x_to_I(const bool* x) const {
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
}

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

GateStringSparse GateStringSparse::add(const GateStringSparse& inGSS) const {
    GateStringSparse out(n);

    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        unordered_set<int>::iterator ePos = inGSS.data_us->find(thisI);
        if(ePos==inGSS.data_us->end()) {
            out.set(thisI);
        }
    }

    for(unordered_set<int>::iterator it = inGSS.data_us->begin(); it != inGSS.data_us->end(); it++) {
        int thisI = *it;
        unordered_set<int>::iterator ePos = data_us->find(thisI);
        if(ePos==data_us->end()) {
            out.set(thisI);
        }
    }

    return out;
}

GateStringSparse GateStringSparse::operator+(const GateStringSparse& inGSS) const {
    return add(inGSS);
}

GateStringSparse& GateStringSparse::assign(const GateStringSparse& inGSS) {
    *data_us = *(inGSS.data_us);
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
            out.set(i);
        }
    }
    return out;
}

int GateStringSparse::weight(bool punc) const {
    return data_us->size()-punc*E(0);
}

GateStringSparse GateStringSparse::mult2xh(const int h) const {
    GateStringSparse out(n);

    out.assign(*this);

    bool* this_x = new bool[n];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        I_to_x(I,this_x);
        this_x[h-1]=1;
        out.set(this_x);
    }

    if(weight(true)%2) {
        for(int i = 0; i < n; i++) {
            if(i==(h-1)) {
                this_x[i]=1;
            } else {
                this_x[i]=0;
            }
        }
        out.set(this_x);
    }
    delete [] this_x;

    return out;
}

unordered_set<int> GateStringSparse::get_data() const {
    unordered_set<int> out;

    out = *data_us;

    return out;
}

GateStringSparse GateStringSparse::augment(const int h) const {
    GateStringSparse out(n+1);

    bool* this_x = new bool[n];
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
    delete [] this_xout;

    return out;
}

GateStringSparse GateStringSparse::addxh(const int h) const {
    GateStringSparse out(n);

    bool* this_x = new bool[n];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        I_to_x(I,this_x);
        this_x[h-1] = !this_x[h-1];
        out.set(this_x);
    }
    delete [] this_x;

    return out;
}

GateStringSparse GateStringSparse::randomWeightN(int in_n, int N) {
    GateStringSparse out(in_n);

    for(int i = 0; i < N; i++) {
        bool exit = false;
        while(!exit) {
            int this_I = LCL_Int::randi(1,pow(2,in_n)-1);
            if(!out.E(this_I)) {
                out.set(this_I);
                exit = true;
            }
        }
    }

    return out;
}

GateStringSparse GateStringSparse::randomUpTo3Qu(int n, int in_seed) {
    GateStringSparse out(n);
    if(in_seed>0) {
        srand(in_seed);
    }
    int Ni = n;
    int Njk = 0.5*n*(n-1);
    int Nlmp = (1.0/6.0)*n*(n-1)*(n-2);
    int NI = Ni + Njk + Nlmp;
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
        bool* this_x = new bool[n];
        bool* super_x = new bool[len];
        for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
            int this_I = *it;
            I_to_x(this_I, this_x);
            for(int i = 0; i < len; i++) super_x[i] = 0;
            for(int i = 0; i < n; i++) {
                if(this_x[i]) {
                    super_x[to_super_mapping[i]-1] = 1;
                }
            }
            out.set(super_x);
        }

        delete [] this_x;
        delete [] super_x;
    } else {
        cout << "GateStringSparse error! inc_vec must have weight of exactly n." << endl;
    }

    delete [] to_super_mapping;

    return out;
}

void GateStringSparse::printMatrix() const {
    BoolMat thisBM(n, weight(true));

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

    thisBM.print();

    delete [] this_x;
}

GateStringSparse GateStringSparse::subGSS_xh(int h) const {
    GateStringSparse out(n);

    bool* this_x = new bool[n];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        I_to_x(this_I,this_x);
        if(this_x[h-1]) {
            out.set(this_x);
        }
    }
    delete [] this_x;

    return out;
}

GateStringSparse GateStringSparse::subGSS_s(bool* in_s) const {
    GateStringSparse out(n);

    int weight = 0;
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
    delete [] this_x;

    return out;
}

GateStringSparse GateStringSparse::multiply(const GateStringSparse& inGSS) const {
    GateStringSparse out(n);

    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        for(unordered_set<int>::iterator jt = inGSS.data_us->begin(); jt != inGSS.data_us->end(); jt++) {
            int this_J = *jt;
            if(this_I==this_J) {
                out.set(this_I);
            }
        }
    }

    return out;
}

GateStringSparse GateStringSparse::operator*(const GateStringSparse& inGSS) const {
    return multiply(inGSS);
}
