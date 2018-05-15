#include "BMSparse.h"

#include <iostream>
using namespace std;

#include <cmath>
#include <ostream>
#include <unordered_set>
#include <cstring>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>

#include "LCL/LCL_Int.h"

BMSparse::BMSparse() {
    data = new BM_Data;
    n = 0;
    m = 0;
}

BMSparse::BMSparse(const int inN, const int inM) {
    data = new BM_Data;
    n = inN;
    m = inM;
}

BMSparse::~BMSparse() {
    delete data;
    data = NULL;
}

bool BMSparse::E(const int i, const int j) const {
    bool out = false;
    int I = ijToI(i,j);
    BM_Data::iterator pos = data->find(I);
    if(pos!=data->end()) {
        out = true;
    }
    return out;
}

void BMSparse::E(const int i, const int j, const bool val) {
    int I = ijToI(i,j);

    if(val) {
        data->insert(I);
    } else {
        data->erase(I);
    }
}

int BMSparse::get_n() const {
    return n;
}

int BMSparse::get_m() const {
    return m;
}

int BMSparse::ijToI(const int i, const int j) const {
    int out;
    if((i<0)||(i>=n)||(j<0)||(j>=m)) {
        cout << "BMSparse error! Index out of bounds. ijToI(" << i << ", " << j << ")" << endl;
        out = -1;
    } else {
        out = m*i + j;
    }
    return out;
}

int BMSparse::IToij(const int I, int& i, int& j) const {
    int out;
    if((I<0)||(I>maxI())) {
        cout << "BMSparse error! Index out of bounds. I = " << I << "maxI = " << maxI() << ", n = " << n << "m = " << m << endl;
        out = -1;
    } else {
        j = I % m;
        i = I/m;
        out = 1;
    }
    return out;
}

int BMSparse::maxI() const {
    int out = m*n - 1;
    return out;
}

void BMSparse::print(ostream& out) const {
    out << "BMSparse object. Dims: " << n << "x" << m << endl;
    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int thisI = *it;
        out << thisI << " ";
    }
    out << endl;
}

void BMSparse::printFull(ostream& out) const {
    out << "BMSparse object. Dims: " << n << "x" << m << endl;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            out << E(i,j);
        }
        out << endl;
    }
    out << endl;
}

void BMSparse::resize(const int inN, const int inM) {
    data->clear();
    n = inN;
    m = inM;
}

BMSparse& BMSparse::assign(const BMSparse& inBM) {
    if((n!=inBM.n)||(m!=inBM.m)) {
        resize(inBM.n, inBM.m);
    }
    (*data) = *(inBM.data);
    return (*this);
}

BMSparse& BMSparse::operator=(const BMSparse& inBM) {
    return assign(inBM);
}

BMSparse BMSparse::multiply(const BMSparse& inBM) const {
    BMSparse out;

    if(m==inBM.n) {
        int outN = n;
        int outM = inBM.m;
        out.resize(outN,outM);
        BMSparse thisA_rows(n,1);
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI_A = *it;
            int this_i_A, this_j_A;
            IToij(thisI_A, this_i_A, this_j_A);
            if(!thisA_rows.E(this_i_A,0)) {
                BMSparse thisA_i = row(this_i_A);
                BMSparse thisB_cols(1,inBM.m);
                for(BM_Data::iterator jt = inBM.data->begin(); jt != inBM.data->end(); jt++) {
                    int thisI_B = *jt;
                    int this_i_B, this_j_B;
                    inBM.IToij(thisI_B, this_i_B, this_j_B);
                    if(!thisB_cols.E(0,this_j_B)) {
                        BMSparse thisB_j = (inBM.col(this_j_B)).T();
                        int share_count = 0;
                        for(BM_Data::iterator kt = thisA_i.data->begin(); kt != thisA_i.data->end(); kt++) {
                            int thisI = *kt;
                            BM_Data::iterator pos = thisB_j.data->find(thisI);
                            if(pos!=thisB_j.data->end()) {
                                share_count++;
                            }
                            out.E(this_i_A,this_j_B,share_count%2);
                        }
                        thisB_cols.E(0,this_j_B,1);
                    }
                }
                thisA_rows.E(this_i_A,0,1);
            }
        }
    } else {
        cout << "BMSparse error! For *, inner dimensions must agree." << endl;
    }

    return out;
}

BMSparse BMSparse::multiply(bool in_x) const {
    BMSparse out(n,m);

    if(in_x) {
        out.assign(*this);
    }

    return out;
}

BMSparse BMSparse::operator*(const BMSparse& inBM) const {
    return multiply(inBM);
}

BMSparse BMSparse::operator*(bool in_x) const {
    return multiply(in_x);
}

BMSparse BMSparse::add(const BMSparse& inBM) const {
    BMSparse out;
    if((n==inBM.n)&&(m==inBM.m)) {
        out.resize(n,m);
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            BM_Data::iterator pos = inBM.data->find(thisI);
            if(pos == inBM.data->end()) {
                out.E(this_i,this_j,1);
            }
        }
        for(BM_Data::iterator it = inBM.data->begin(); it != inBM.data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            BM_Data::iterator pos = data->find(thisI);
            if(pos == data->end()) {
                out.E(this_i,this_j,1);
            }
        }
    } else {
        cout << "BMSparse error! For +, dimensions must match." << endl;
    }
    return out;
}

BMSparse BMSparse::operator+(const BMSparse& inBM)  const {
    return add(inBM);
}

BMSparse BMSparse::row(const int i) const {
    BMSparse out;
    if(i<n) {
        out.resize(1,m);
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            if(this_i==i) {
                out.E(0,this_j,1);
            }
        }
    } else {
        cout << "BMSparse error! Index out of bounds." << endl;
    }
    return out;
}

BMSparse BMSparse::col(const int j) const {
    BMSparse out;
    if(j<m) {
        out.resize(n,1);
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            if(this_j==j) {
                out.E(this_i,0,1);
            }
        }
    } else {
        cout << "BMSparse error! Index out of bounds." << endl;
    }
    return out;
}

BMSparse BMSparse::T() const {
    BMSparse out(m,n);
    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int thisI = *it;
        int this_i, this_j;
        IToij(thisI, this_i, this_j);
        out.E(this_j,this_i,1);
    }
    return out;
}

BMSparse BMSparse::RowEchelon() const {
    BMSparse out(n,m);
    out.assign(*this);
    // Find first row for which the first column is non-zero
    int e = 0;
    for(int k = 0; k < (int)fmin(n,m); k++) {
        bool exit = false;
        while((!exit)&&((k+e)<m)) {
            int pivot_i = n;
            for(BM_Data::iterator it = out.data->begin(); (pivot_i==n)&&(it != out.data->end()); it++) {
                int thisI = *it;
                int this_i, this_j;
                out.IToij(thisI, this_i, this_j);
                if((this_i>=k)&&(this_j==(k+e))) {
                    pivot_i = this_i;
                }
            }
            if(pivot_i < n) {
                {
                    BMSparse tempout = out.swapRow(k,pivot_i);
                    out.assign(tempout);
                }
                BMSparse this_out;
                this_out.assign(out);
                for(BM_Data::iterator it = out.data->begin(); it != out.data->end(); it++) {
                    int thisI = *it;
                    int this_i, this_j;
                    out.IToij(thisI, this_i, this_j);
                    if((this_i>k)&&(this_j==(k+e))) {
                        {
                            BMSparse tempout = this_out.addRows(this_i,k);
                            this_out.assign(tempout);
                        }
                    }
                }
                out.assign(this_out);
                exit = true;
            } else {
                e++;
            }
        }
    }

    return out;
}

BMSparse BMSparse::ColEchelon() const {
    BMSparse out = (T().RowEchelon()).T();
    return out;
}

BMSparse BMSparse::swapRow(const int i1, const int i2) const {
    BMSparse out(n, m);
    if((i1>=0)&&(i1<n)&&(i2>=0)&&(i2<n)) {
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            if(this_i==i1) {
                out.E(i2,this_j,1);
            } else if(this_i==i2) {
                out.E(i1,this_j,1);
            } else {
                out.E(this_i, this_j,1);
            }
        }
    } else {
        cout << "BMSparse error! Row index out of bounds." << endl;
    }

    return out;
}

BMSparse BMSparse::swapCol(const int j1, const int j2) const {
    BMSparse out(n, m);
    if((j1>=0)&&(j1<m)&&(j2>=0)&&(j2<m)) {
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int thisI = *it;
            int this_i, this_j;
            IToij(thisI, this_i, this_j);
            if(this_j==j1) {
                out.E(this_i,j2,1);
            } else if(this_j==j2) {
                out.E(this_i,j1,1);
            } else {
                out.E(this_i, this_j,1);
            }
        }
    } else {
        cout << "BMSparse error! Col index out of bounds." << endl;
    }

    return out;
}

BMSparse BMSparse::addRows(const int it, const int is) const {
    BMSparse out(n,m);
    out.assign(*this);
    BMSparse S_ab(n,n);
    S_ab.assign(BMSparse::eye(n,n) + BMSparse::unit(n,it)*(BMSparse::unit(n,is).T()));
    out.assign(S_ab*out);
    return out;
}

BMSparse BMSparse::eye(const int inN, const int inM) {
    BMSparse out(inN, inM);
    int len = (int)fmin(inN,inM);
    for(int i = 0; i < len; i++) {
        out.E(i,i,1);
    }
    return out;
}

BMSparse BMSparse::unit(const int inN, const int in_i) {
    BMSparse out(inN,1);
    out.E(in_i,0,1);
    return out;
}

void BMSparse::LQUPDecomposition(BMSparse& outL, BMSparse& outQ, BMSparse& outU, BMSparse& outP, const char* calledby, bool verbose) const {
    if(verbose) cout << "LQUPDecomposition BEGIN";
    if(verbose) if(calledby) cout << ": called by " << calledby;
    if(verbose) cout << endl;
    if(verbose) cout << "Input matrix:" << endl;
    if(verbose) printFull();

    outL.resize(n,n);
    outL = BMSparse::eye(n,n);
    outQ.resize(n,n);
    outQ = BMSparse::eye(n,n);
    outU.resize(n,m);
    outU = (*this);
    outP.resize(m,m);
    outP = BMSparse::eye(m,m);
    BMSparse thisL, thisQ, thisU, thisP;
    thisL = BMSparse::eye(n,n);
    thisQ = BMSparse::eye(n,n);
    thisU.assign(*this);
    thisP = BMSparse::eye(m,m);

    int e = 0;
    int r = 0;
    for(int j = 0; j < fmin((n-r),m); j++) {
        if(verbose) cout << "j = " << j << endl;
        if(verbose) cout << "(n-r) = " << (n-r) << endl;
        bool exit;
        do {
            int this_j1, this_j2;
            if(!thisU.E(j+e,j)) {
                this_j1 = j;
                exit = false;
                for(int k = j+1; (!exit)&&(k < m); k++) {
                    if(verbose) cout << "k = " << k << endl;
                    if(thisU.E(j+e,k)) {
                        this_j2 = k;
                        BMSparse tempP = (perm(m,this_j1,this_j2) * thisP);
                        thisP.assign(tempP);
                        BMSparse tempU = (thisU * perm(m,this_j1,this_j2));
                        thisU.assign(tempU);
                        exit = true;
                        if(verbose) cout << "thisU" << endl;
                        if(verbose) thisU.printFull();
                        if(verbose) cout << "thisP" << endl;
                        if(verbose) thisP.printFull();
                    }
                }
                if(!exit) {
                    if(verbose) cout << "LUP Error!" << endl;
                    e++;
                    r++;
                }
            } else {
                exit = true;
            }
        } while((!exit)&&((j+e)<n));
        for(int i = j+e+1; i < n; i++) {
        if(verbose) cout << "i = " << i << endl;
            if(thisU.E(i,j)) {
                BMSparse tempU = thisU.addRows(i,j+e);
                thisU.assign(tempU);
                thisL.E(i,j+e,1);
                if(verbose) cout << "thisU" << endl;
                if(verbose) thisU.printFull();
                if(verbose) cout << "thisL" << endl;
                if(verbose) thisL.printFull();
            }
        }
    }

    for(int i = 0; i < n; i++) {
        int sum1 = 0;
        if(verbose) cout << "BEFORE SUM, U, i = " << i << endl;
        if(verbose) thisU.printFull();
        for(int j = 0; j < m; j++) {
            sum1 += thisU.E(i,j);
        }
        if(!sum1) {
            int this_i1 = i;
            bool exit = false;
            for(int k = i+1; (!exit)&&(k < n); k++) {
                int sum2 = 0;
                for(int j = 0; j < m; j++) {
                    sum2 += thisU.E(k,j);
                }
                if(sum2) {
                    int this_i2 = k;
                    BMSparse tempQ = thisQ*BMSparse::perm(n,this_i1,this_i2);
                    thisQ.assign(tempQ);
                    BMSparse tempU = BMSparse::perm(n,this_i1,this_i2)*thisU;
                    thisU.assign(tempU);
                    if(verbose) cout << "(i1,i2) = (" << this_i1 << "," << this_i2 << ")"<<endl;
                    if(verbose) cout << "thisQ" << endl;
                    if(verbose) thisQ.printFull();
                    if(verbose) cout << "thisU" << endl;
                    if(verbose) thisU.printFull();
                    exit = true;
                }
            }
        }
    }

    outL.assign(thisL);
    outQ.assign(thisQ);
    outU.assign(thisU);
    outP.assign(thisP);
    if(verbose) cout << "LQUPDecomposition END" << endl;
}

BMSparse BMSparse::perm(const int inP, const int x1, const int x2) {
    BMSparse out(inP,inP);
    for(int i = 0; i < inP; i++) {
        if(i==x1) {
            out.E(i,x2,1);
        } else if(i==x2) {
            out.E(i,x1,1);
        } else {
            out.E(i,i,1);
        }
    }
    return out;
}

BMSparse BMSparse::forwardSubstitute(const BMSparse& inB) const {
    BMSparse out(m,inB.m);

    for(int k = 0; k < out.m; k++) {
        for(int i = 0; i < n; i++) {
            int thisX = inB.E(i,k);
            for(int j = 0; j < i; j++) {
                thisX += E(i,j)*out.E(j,k);
            }
            out.E(i,k,thisX%2);
        }
    }

    return out;
}

BMSparse BMSparse::backwardSubstitute(const BMSparse& inB) const {
    BMSparse out(m,inB.m);

    for(int k = 0; k < out.m; k++) {
        for(int i = (n-1); i >= 0; i--) {
            int thisX = inB.E(i,k);
            for(int j = i+1; j < out.m; j++) {
                thisX += E(i,j)*out.E(j,k);
            }
            out.E(i,k,thisX%2);
        }
    }

    return out;
}

BMSparse BMSparse::inverse(bool verbose) const {
    BMSparse out;

    if(n==m) {
        BMSparse thisL, thisQ, thisU, thisP;
        BMSparse thisB = BMSparse::eye(n,n);

        if(verbose) cout << "Before LQUP" << endl;
        LQUPDecomposition(thisL, thisQ, thisU, thisP,"inverse");
        if(verbose) cout << "After LQUP" << endl;

        BMSparse thisY = thisL.forwardSubstitute(thisB);
        BMSparse thisXp = thisU.backwardSubstitute(thisY);
        out = (thisP.T())*thisXp;
    } else {
        cout << "BMSparse error! For inverse, Matrix must be square and non-singular." << endl;
    }


    return out;
}

BMSparse BMSparse::sub(const int i0, const int j0, const int np, const int mp) const {
    int this_np = (np>=0)?np:(n-i0);
    int this_mp = (mp>=0)?mp:(m-j0);
    if(this_np<0) this_np = 0;
    if(this_mp<0) this_mp = 0;

    BMSparse out(this_np,this_mp);

    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int I = *it;
        int i, j;
        IToij(I,i,j);
        if((i0<=i)&&(i<(i0+this_np))&&(j0<=j)&&(j<(j0+this_mp))) {
            int ip = i - i0;
            int jp = j - j0;
            out.E(ip,jp,1);
        }
    }

    return out;
}

BMSparse BMSparse::nullspace(bool verbose) const {
    if(verbose) cout << "nullspace BEGIN" << endl;
    BMSparse out;

    BMSparse thisL, thisQ, thisU, thisP;

    LQUPDecomposition(thisL, thisQ, thisU, thisP, "nullspace");


    int e = 0;
    bool exit = false;
    for(int i = (thisU.n-1); (!exit)&&(i >= 0); i--) {
        int thisSum = 0;
        for(int j = 0; j < thisU.m; j++) {
            thisSum += thisU.E(i,j);
        }
        if(thisSum) {
            exit = true;
        } else {
            e++;
        }
    }

    int r = thisU.n - e;

    BMSparse thisU1 = thisU.sub(0,0,r,r);

    BMSparse thisF = thisU.sub(0,r,r,-1);


    if(verbose) cout << "thisU1" << endl;
    if(verbose) thisU1.printFull();
    if(verbose) cout << "thisF" << endl;
    if(verbose) thisF.printFull();

    out.resize(thisU.m,thisF.m);
    if(!thisF.isEmpty()) {
        BMSparse thisTop = thisU1.inverse()*thisF;

        if(verbose) cout << "thisTop" << endl;
        if(verbose) thisTop.printFull();
        for(BM_Data::iterator it = thisTop.data->begin(); it != thisTop.data->end(); it++) {
            int I = *it;
            int i,j;
            thisTop.IToij(I,i,j);
            out.E(i,j,1);
        }

        for(int j = 0; j < (thisU.m-r); j++) {
            out.E(r+j,j,1);
        }

        BMSparse temp = thisP.T()*out;
        out.assign(temp);
    }

    if(verbose) cout << "nullspace END" << endl;
    return out;
}

bool BMSparse::isEmpty() const {
    return !(n*m);
}

bool BMSparse::isAllZeros() const {
    return data->empty();
}

BMSparse BMSparse::elementaryFactor() const {
    BMSparse out;

    int cols = 0;
    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int I = *it;
        int i,j;
        IToij(I,i,j);
        if(i<=j) cols++;
    }
    for(int k = 0; k < n; k++) {
        int thisP = 0;
        for(int j = (k + 1); j < n; j++) {
            thisP += E(k,j);
        }
        for(int i = 0; i < k; i++) {
            thisP += E(i,k);
        }
        if(thisP%2) cols++;
    }

    out.resize(n,cols);

    int col = 0;
    for(int i = 0; i < n; i++) {
        if(E(i,i)) {
            out.E(i,col,1);
            col++;
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = (i+1); j < n; j++) {
            if(E(i,j)) {
                out.E(i,col,1);
                out.E(j,col,1);
                col++;
            }
        }
    }
    for(int k = 0; k < n; k++) {
        int thisP = 0;
        for(int j = (k + 1); j < n; j++) {
            thisP += E(k,j);
        }
        for(int i = 0; i < k; i++) {
            thisP += E(i,k);
        }
        if(thisP%2) {
            out.E(k,col,1);
            col++;
        }
    }
    return out;
}

BMSparse BMSparse::elementaryFactorFast() const {
    BMSparse out;

    BMSparse leftside = BMSparse::eye(n,n-1);

    for(int col = 0; col < leftside.m; col++) {
        BMSparse toprow = leftside.sub(col,0,1,fmax(0,col));
        for(int k = col + 1; k < n; k++) {
            BMSparse bottomrow = leftside.sub(k,0,1,fmax(0,col));
            BMSparse bothrows = toprow||bottomrow;
            BMSparse temp = bothrows*(bothrows.T());
            bool currentparity = temp.E(0,1);
            bool Aparity = E(col,k);
            leftside.E(k,col,(currentparity+Aparity)%2);
        }
    }

    int rhs_cols = 0;
    for(int i = 0; i < leftside.n; i++) {
        BMSparse thisrow = leftside.sub(i,0,1,-1);
        BMSparse temp = thisrow*thisrow.T();
        bool currentparity = temp.E(0,0);
        bool Aparity = E(i,i);
        if((currentparity+Aparity)%2) {
            rhs_cols++;
        }
    }
    BMSparse rightside(n,rhs_cols);
    int rhs_col=0;
    for(int i = 0; i < leftside.n; i++) {
        BMSparse thisrow = leftside.sub(i,0,1,-1);
        BMSparse temp = thisrow*thisrow.T();
        bool currentparity = temp.E(0,0);
        bool Aparity = E(i,i);
        if((currentparity+Aparity)%2) {
            rightside.E(i,rhs_col,1);
            rhs_col++;
        }
    }
    out = leftside&&rightside;

    return out;
}

BMSparse BMSparse::composeLR(const BMSparse& inBMS) const {
    int out_n = n;
    int out_m = m + inBMS.m;
    BMSparse out(out_n,out_m);

    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int I = *it;
        int i,j;
        IToij(I,i,j);
        out.E(i,j,1);
    }

    for(BM_Data::iterator it = inBMS.data->begin(); it != inBMS.data->end(); it++) {
        int I = *it;
        int i,j;
        inBMS.IToij(I,i,j);
        out.E(i,j+m,1);
    }

    return out;
}

BMSparse BMSparse::operator&&(const BMSparse& inBMS) const {
    return composeLR(inBMS);
}

BMSparse BMSparse::composeUD(const BMSparse& inBMS) const {
    int out_n = n + inBMS.n;
    int out_m = m;
    BMSparse out(out_n, out_m);

    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int I = *it;
        int i,j;
        IToij(I,i,j);
        out.E(i,j,1);
    }

    for(BM_Data::iterator it = inBMS.data->begin(); it != inBMS.data->end(); it++) {
        int I = *it;
        int i,j;
        inBMS.IToij(I,i,j);
        out.E(i+n,j,1);
    }

    return out;
}

BMSparse BMSparse::operator||(const BMSparse& inBMS) const {
    return composeUD(inBMS);
}

bool BMSparse::operator==(const BMSparse& inBM) const {
    return equals(inBM);
}

BMSparse& BMSparse::sub(const int i0, const int j0, const BMSparse& inBMS) {
    if(((i0+inBMS.n)>n)||((j0+inBMS.m)>m)) {
        cout << "BMS Error! Input submatrix does not fit." << endl;
    } else {
        for(int i = 0; i < inBMS.n; i++) {
            for(int j = 0; j < inBMS.m; j++) {
                E(i0+i,j0+j,inBMS.E(i,j));
            }
        }
    }

    return *this;
}

BMSparse BMSparse::minimalFactor() const {
    BMSparse out;

    BMSparse thisB = BMSparse::elementaryFactorFast();

    BMSparse thisNullspace = thisB.nullspace();

    int this_rank = rank();
    int this_delta = Lempel_delta();

    bool exit = (thisB.m==(this_rank+this_delta));

    int iteration = 0;
    while((!exit)/*&&(!thisNullspace.isEmpty())*/) {
        BMSparse thisIV = thisNullspace.sub(0,0,-1,1);
        BMSparse thisF(n,0), thisG(n,0);
        for(int j = 0; j < thisB.m; j++) {
            if(thisIV.E(j,0)) {
                BMSparse tempG = thisG && thisB.sub(0,j,-1,1);
                thisG.assign(tempG);
            } else {
                BMSparse tempF = thisF && thisB.sub(0,j,-1,1);
                thisF.assign(tempF);
            }
        }
        if(thisG.m%2) {
            BMSparse tempG = thisG && BMSparse(n,1);
            thisG.assign(tempG);
        }
        BMSparse this_x = thisF.sub(0,0,-1,1) + thisG.sub(0,0,-1,1);
        BMSparse this_u(1,thisG.m);
        for(int i = 0; i < thisG.m; i++) {
            this_u.E(0,i,1);
        }
        {
            BMSparse tempG = thisG + this_x*this_u;
            thisG.assign(tempG);
        }
        if(false&&thisF.isEmpty()&&(!thisG.isEmpty())){
            BMSparse tempB = thisG.sub(0,1,-1,-1);
            thisB.assign(tempB);
        } else {
            BMSparse tempB = thisF.sub(0,1,-1,-1) && thisG.sub(0,1,-1,-1);
            thisB.assign(tempB);
        }
        if(false&&thisF.isEmpty()&&(!thisG.isEmpty())) {
            BMSparse tempNullspace(0,0);
            thisNullspace.assign(tempNullspace);
        } else {
            BMSparse tempNullspace = thisB.nullspace();
            thisNullspace.assign(tempNullspace);
        }
        exit = (thisB.m==(this_rank+this_delta));
        iteration++;
    }

    out.assign(thisB);

    return out;
}

int BMSparse::rank() const {
    int out = 0;

    BMSparse this_RE = RowEchelon();
    bool* this_non_zero = new bool[n];
    for(int i = 0; i < n; i++) this_non_zero[i] = 0;

    for(BM_Data::iterator it = this_RE.data->begin(); it != this_RE.data->end(); it++) {
        int I = *it;
        int i,j;
        this_RE.IToij(I,i,j);
        this_non_zero[i] = 1;
    }

    for(int i = 0; i < n; i++) out += this_non_zero[i];

    delete [] this_non_zero;

    return out;
}

int BMSparse::col_rank() const {
    int out = 0;

    BMSparse temp = T();

    out = temp.rank();

    return out;
}

int BMSparse::Lempel_delta() const {
    int out = 0;

    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        int I = *it;
        int i,j;
        IToij(I,i,j);
        if(i==j) out = 1;
    }

    out = !out;

    return out;
}

bool BMSparse::compare_weight(const char* in_WC, int weight_1, int weight_2) {
    bool out;

    if(!strcmp(in_WC,"<")) {
        out = (weight_1<weight_2);
    } else if(!strcmp(in_WC,"<=")) {
        out = (weight_1<=weight_2);
    } else if(!strcmp(in_WC,"==")) {
        out = (weight_1==weight_2);
    } else if(!strcmp(in_WC,">=")) {
        out = (weight_1>=weight_2);
    } else if(!strcmp(in_WC,">")) {
        out = (weight_1>weight_2);
    } else {
        cout << "Error! Unrecognised weight comparator." << endl;
    }

    return out;
}

BMSparse BMSparse::sub_weight(const char* in_mode, int weight, int in_axis) const {
    /*
    Arguments
        in_mode = {'<','<=','==','=>','>'}
            Comparison operator used to determine if sub-unit of <this> are kept.
        weight
            Second operand used in comparison of sub-unit weight
        in_axis = {0,1,2}
            First operand is the 'sub-unit' and is given by
            0: Weight of whole matrix
            1: Weight of each row
            2: Weight of each column

    Return value
        A submatrix that only includes sub-units with weight that obeys: weight(sub-unit) <in_mode> <weight>
    */

    BMSparse out;

    switch(in_axis) {
        case 0: // Weight of whole matrix
            {
                int this_weight = 0;
                for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
                    this_weight++;
                }
                if(compare_weight(in_mode,this_weight,weight)) {
                    out = (*this);
                }
            }
            break;
        case 1: // Weight of each row
            {
                out.resize(0,m);
                for(int i = 0; i < n; i++) {
                    int this_weight = 0;
                    for(int j = 0; j < m; j++) {
                        this_weight += E(i,j);
                    }
                    if(compare_weight(in_mode,this_weight,weight)) {
                        out = (out||row(i));
                    }
                }
            }
            break;
        case 2: // Weight of each column
            {
                out.resize(n,0);
                for(int j = 0; j < m; j++) {
                    //cout << "j = " << j << endl;
                    int this_weight = 0;
                    for(int i = 0; i < n; i++) {
                        //cout << "i = " << i << endl;
                        this_weight += E(i,j);
                    }
                    if(compare_weight(in_mode,this_weight,weight)) {
                        //cout << "TYUI" << endl;
                        out = (out&&col(j));
                        //cout << "ghjk" << endl;
                    }
                }
            }
            break;
        default:
            cout << "Error! Unrecognised axis for weight comparison." << endl;
            break;
    }

    return out;
}

BMSparse BMSparse::random(const int inN, const int inM, const double in_P) {
    BMSparse out(inN, inM);

    for(int k = 0; k < (in_P*inN*inM); k++) {
        int this_I = LCL_Int::randi(0,inN*inM-1);
        int i,j;
        out.IToij(this_I,i,j);
        out.E(i,j,1);
    }

    return out;
}

BMSparse BMSparse::bitwise_OR(const BMSparse& inBMS) const {
    BMSparse out;

    if((n==inBMS.n)&&(m==inBMS.m)) {
        out = (*this);
        for(BM_Data::iterator it = inBMS.data->begin(); it != inBMS.data->end(); it++) {
            int this_I = *it;
            int i,j;
            inBMS.IToij(this_I,i,j);
            out.E(i,j,1);
        }
    }

    return out;
}

BMSparse BMSparse::bitwise_AND(const BMSparse& inBMS) const {
    BMSparse out;

    if((n==inBMS.n)&&(m==inBMS.m)) {
        out.resize(n,m);
        for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
            int this_I = *it;
            int i,j;
            IToij(this_I,i,j);
            if(inBMS.E(i,j)) {
                out.E(i,j,1);
            }
        }
    }

    return out;
}

bool BMSparse::equals(const BMSparse& inBM) const {
    bool out = true;

    if((n==inBM.get_n())&&(m==inBM.get_m())) {
        for(int i = 0; out&&(i<n); i++) {
            for(int j = 0; out&&(j<m); j++) {
                out = (E(i,j)==inBM.E(i,j));
            }
        }
    } else {
        out = false;
    }

    return out;
}

int BMSparse::sum() const {
    int out = 0;

    for(BM_Data::iterator it = data->begin(); it != data->end(); it++) {
        out++;
    }

    return out;
}

BMSparse BMSparse::OR_rows() const {
    BMSparse out(1,m);

    for(int i = 0; i < n; i++) {
        out = (out.bitwise_OR(row(i)));
    }

    return out;
}

BMSparse BMSparse::OR_cols() const {
    BMSparse out(n,1);

    for(int j = 0; j < m; j++) {
        out = (out.bitwise_OR(col(j)));
    }

    return out;
}

void BMSparse::LPUTrapezoidal(BMSparse& outL, BMSparse& outP, BMSparse& outU, int height) const {
    // Assumes non-singular
    outL = BMSparse::eye(n,n);
    outP = BMSparse::eye(n,n);
    outU.assign(*this);

    int e = 0;
    for(int k = 0; k < (int)fmin(fmin(n,m),height); k++) {
        int pivot_i = k;
        int pivot_j = -1;
        bool pivot = false;
        do {
            pivot_j = k+e;
            bool exit = false;
            for(int i = k; (!exit)&&(i < n); i++) {
                if(outU.E(i,pivot_j)) {
                    exit = true;
                    pivot = true;
                    BMSparse thisP = BMSparse::perm(n,k,i);
                    BMSparse tempU = (thisP*outU);
                    outU.assign(tempU);
                    BMSparse tempP = (outP*thisP);
                    outP.assign(tempP);
                }
            }
            if(!exit) {
                e++;
            }
        } while((!pivot)&&((k+e)<m));

        BMSparse outU_temp;
        outU_temp.assign(outU);
        for(BM_Data::iterator it = outU.data->begin(); it != outU.data->end(); it++) {
            int this_I = *it;
            int this_i, this_j;
            outU.IToij(this_I,this_i,this_j);
            if((this_i>pivot_i)&&(this_j==pivot_j)) {
                BMSparse tempU = (outU_temp.addRows(this_i,pivot_i));
                outU_temp.assign(tempU);
                outL.E(this_i,pivot_i,1);
            }
        }
        outU.assign(outU_temp);
    }
}

void BMSparse::LEUKTrapezoidal(BMSparse& outL, BMSparse& outE, BMSparse& outU, BMSparse& outK, int height, int order) const {
    // Assumes non-singular
    outL = BMSparse::eye(n,n);
    outE = BMSparse::eye(n,n);
    outU.assign(*this);
    outK = BMSparse::eye(m,m);

    int e = 0;
    for(int k = 0;(true)&&(k < (int)fmin(fmin(n,m),height)); k++) {
        int pivot_i = -1;
        int pivot_j = -1;


        if(order==0) { // Find pivot based on large amount of activity
            BMSparse this_U_sub = outU.sub(k,k+e,-1,-1);
            int w_current = 0;
            int ip_current = -1;
            int jp_current = -1;
            for(int ip = 0; ip < (n-k); ip++) {
                for(int jp = 0; jp < (m-k-e); jp++) {
                    if(this_U_sub.E(ip,jp)) {
                        int this_w = this_U_sub.weight_ij(ip,jp);
                        if(this_w>w_current) {
                            w_current = this_w;
                            ip_current = ip;
                            jp_current = jp;
                        }
                    }
                }
            }
            pivot_i = ip_current+k;
            pivot_j = jp_current+k+e;
        } else if(order == 1) { // Random
            pivot_j = LCL_Int::randi(k+e,m-1);
            vector<int> temp_is;
            for(int i = k; i < n; i++) {
                if(outU.E(i,pivot_j)) temp_is.push_back(i);
            }
            pivot_i = temp_is.at(LCL_Int::randi(0,temp_is.size()-1));
        }


        // Update permutation matrices E and K
        {
            BMSparse this_E = BMSparse::perm(n,k,pivot_i);
            BMSparse this_K = BMSparse::perm(m,k+e,pivot_j);

            BMSparse temp_U = this_E*outU*this_K;
            BMSparse temp_E = outE*this_E;
            BMSparse temp_K = this_K*outK;

            outU.assign(temp_U);
            outE.assign(temp_E);
            outK.assign(temp_K);
        }

        // Perform Gaussian elimination
        BMSparse outU_temp;
        outU_temp.assign(outU);
        for(BM_Data::iterator it = outU.data->begin(); it != outU.data->end(); it++) {
            int this_I = *it;
            int this_i, this_j;
            outU.IToij(this_I,this_i,this_j);
            if((this_i>k)&&(this_j==(k+e))) {
                BMSparse tempU = (outU_temp.addRows(this_i,k));
                outU_temp.assign(tempU);
                BMSparse this_E = BMSparse::addRows(n,this_i,k);
                BMSparse temp_E = (outE*this_E);
                outE.assign(temp_E);
                //outL.E(this_i,k,1);
            }
        }
        outU.assign(outU_temp);

        // Search for columns that preserve column rank
        for(int j = (k+e+1); j < m; j++) {
            int this_weight = 0;
            for(int i = k; i < n; i++) {
                this_weight += outU.E(i,j);
            }
            if(!this_weight) {
                BMSparse this_K = BMSparse::perm(m,j,k+e+1);
                BMSparse temp_U = outU*this_K;
                BMSparse temp_K = this_K*outK;
                outU.assign(temp_U);
                outK.assign(temp_K);
                e++;
            }
        }

    }

}

int BMSparse::weight_ij(int i, int j) const {
    int out = 0;

    for(int i = 0; i < n; i++) out += E(i,j);
    for(int j = 0; j < m; j++) out += E(i,j);
    out -= E(i,j);

    return out;
}

BMSparse BMSparse::addRows(const int inN, const int it, const int is) {
    BMSparse out = BMSparse::eye(inN, inN);

    out.E(it,is,1);

    return out;
}

int BMSparse::EUKPentagonal(BMSparse& outE, BMSparse& outU, BMSparse& outK, int height) const {
    int out=0;

    outE = BMSparse::eye(n,n);
    outU.assign(*this);
    outK = BMSparse::eye(m,m);

    int u_rank = 0;
    int b_start_col = 0;
    int k_GE = 0;
    int e_GE = 0;

    int construction_round = 0;
    int search_weight = 3;
    do {
        cout << "EUK Construction round = " << construction_round << endl;
    // Find a vector in the nullspace of submatrix B with weight of 3 (or nearest value >= 3)
        BMSparse this_N = outU.sub(0,b_start_col,-1,-1).nullspace();

        bool found_v = false;
        BMSparse this_v;
        BMSparse this_V(this_N.get_n()+1,height-search_weight+1); // Nullspace vectors that define set of columns in next Gaussian elimination round. ith col has weight of (i+3). Bottom row stores presence of each kind of vec based on weight.
        BMSparse this_x(this_N.get_m(),1);
        this_x = this_x.increment();
        clock_t null_start_time = clock();
        int c = 0;
        while(!found_v&&(this_x.sum())) {
            if(!(c%1024)) {
                double this_time = (clock()-null_start_time)/CLOCKS_PER_SEC;
                double this_rate = c/this_time;
                double time_left = (pow(2,this_N.get_m())-c)/this_rate;
                cout << "Percentage done = " << (double)c/pow(2,this_N.get_m()) << "%; Time elapsed = " << this_time << " s; " << "Time left = " << time_left << " s" << endl;
            }
            c++;
            BMSparse this_nv = this_N*this_x;
            int this_w = this_nv.sum();

            if(this_w==search_weight) {
                found_v = true;
                this_V.sub(0,0,this_nv);
                this_V.E(this_N.get_n(),0,1);
                this_v = this_nv;
            } else if((this_w>search_weight)&&(this_w<=height)) {
                this_V.sub(0,this_w-search_weight,this_nv);
                this_V.E(this_N.get_n(),this_w-search_weight,1);
            }
            this_x = this_x.increment();
        }

        {
            int j = 1;
            while((!found_v)&&(j<this_V.get_m())) {
                if(this_V.E(this_N.get_n(),j)) {
                    found_v = true;
                    this_v = this_V.sub(0,j,this_N.get_n(),1);
                }
                j++;
            }
        }

        // Shift columns to start of B matrix
        if(found_v) {
            int c0 = 0;
            //int c0 = b_start_col;
            for(int i = c0; i < this_N.get_n(); i++) {
                if(this_v.E(i,0)) {
                    BMSparse thisK = BMSparse::perm(m,c0+b_start_col,i+b_start_col);
                    BMSparse tempK = thisK*outK;
                    BMSparse tempU = outU*thisK;
                    outK.assign(tempK);
                    outU.assign(tempU);
                    c0++;
                }
            }
            b_start_col += this_v.sum();
        } else {
            b_start_col = m;
        }

        // Perform Gaussian elimination until col rank=6 or start of B matrix
        {
            bool exit_GE = false;

            while(!exit_GE) {
                // Find pivot row
                bool pivot_found = false;
                for(int i = k_GE; (!pivot_found)&&(i < n); i++) {
                    if(outU.E(i,k_GE+e_GE)) {
                        // Swap rows i <-> k
                        BMSparse thisE = BMSparse::perm(n,i,k_GE);
                        BMSparse tempE = outE*thisE;
                        BMSparse tempU = thisE*outU;
                        outE.assign(tempE);
                        outU.assign(tempU);
                        pivot_found = true;
                    }
                }

                // Eliminate other 1's
                if(pivot_found) {
                    for(int i = (k_GE+1); i < n; i++) {
                        if(outU.E(i,k_GE+e_GE)) {
                            BMSparse thisE = BMSparse::addRows(n,i,k_GE);
                            BMSparse tempE = outE*thisE;
                            BMSparse tempU = thisE*outU;
                            outE.assign(tempE);
                            outU.assign(tempU);
                        }
                    }
                } else {
                    e_GE++;
                }

                if(pivot_found) k_GE++;
                if(((k_GE+e_GE)>=b_start_col)||(k_GE>=height)) exit_GE = true;
            }
        }

        // Determine col rank of U
        u_rank = k_GE;
        construction_round++;
    } while(true&&(u_rank<height));


    bool count_exit = false;
    while(!count_exit) {
        bool zero_col = true;
        for(int i = height; zero_col&&(i < n); i++) {
            zero_col = !outU.E(i,out);
        }
        if(zero_col) out++;
        else count_exit = true;
    }

    //out = (k_GE+e_GE);

    return out;
}

BMSparse BMSparse::increment(int axis) const {
    BMSparse out(n,m);

    if(!axis) {
        if((n==1)&&(m>1)) {
            axis = 1;
        } else if((m==1)&&(n>1)) {
            axis = 2;
        } else {
            axis = 1;
        }
    }

    if(axis==1) {
        for(int i = 0; i < n; i++) {
            bool carry = 0;
            BMSparse this_v = row(i);
            for(int j = 0; j < m; j++) {
                out.E(i,j,(E(i,j)+(j==0)+carry)%2);
                carry = ((E(i,j)+(j==0)+carry)>1);
            }
        }
    } else if(axis==2) {
        for(int j = 0; j < m; j++) {
            bool carry = 0;
            BMSparse this_v = col(j);
            for(int i = 0; i < n; i++) {
                out.E(i,j,(E(i,j)+(i==0)+carry)%2);
                carry = ((E(i,j)+(i==0)+carry)>1);
            }
        }
    }

    return out;
}

bool BMSparse::S_abc(int a, int b, int c) const {
    bool out;

    int out_int = 0;
    for(int k = 0; k < m; k++) {
        out_int += (E(a,k)*E(b,k)*E(c,k));
    }
    out = (out_int%2);

    return out;
}

bool BMSparse::LempelCubicCondition(const BMSparse& in_x) const {
    bool out = 0;

    for(int i = 0; i < (n-2); i++) {
        for(int j = (i+1); j < (n-1); j++) {
            for(int k = (j+1); k < n; k++) {
                out += (in_x.E(i,0)*S_abc(j,k,k)+in_x.E(j,0)*S_abc(k,i,i)+in_x.E(k,0)*S_abc(i,j,j))%2;
            }
        }
    }

    return out;
}

BMSparse BMSparse::GSMatrixExtension2(const BMSparse& in_x) const {
    BMSparse out(n,m);
    out.assign(*this);

    out.sub(0,0,*this);

    int c = 0;
    for(int i = 0; i < (n-2); i++) {
        for(int j = i + 1; j < (n-1); j++) {
            for(int k = j + 1; k < n; k++) {
                if(in_x.E(i,0)||in_x.E(j,0)||in_x.E(k,0)) {
                    BMSparse this_row = ((row(j).bitwise_AND(row(k))*in_x.E(i,0)) + (row(i).bitwise_AND(row(k))*in_x.E(j,0)) + (row(j).bitwise_AND(row(i))*in_x.E(k,0)));
                    bool found = false;
                    for(int l = 0; (!found)&&(l < out.get_n()); l++) {
                        BMSparse this_search_row = out.row(l);
                        if(!((this_row+this_search_row).sum())) found = true;
                    }
                    if(!found) {
                        BMSparse temp_out = (out||this_row);
                        out.assign(temp_out);
                    }
                }

                c++;
            }
        }
    }

    return out;
}

int BMSparse::compare_signature(const BMSparse& in_BM) const {
    int out;

    bool same = true;
    if(n==in_BM.get_n()) {
        for(int i = 0; same&&(i < n); i++) {
            if(S_abc(i,i,i)!=in_BM.S_abc(i,i,i)) same = false;
        }
        for(int i = 0; same&&(i < (n-1)); i++) {
            for(int j = (i+1); same&&(j < n); j++) {
                if(S_abc(i,j,j)!=in_BM.S_abc(i,j,j)) same = false;
            }
        }
        for(int i = 0; same&&(i < (n-2)); i++) {
            for(int j = (i+1); same&&(j<(n-1)); j++) {
                for(int k = (j+1); same&&(k<n); k++) {
                    if(S_abc(i,j,k)!=in_BM.S_abc(i,j,k)) same = false;
                }
            }
        }
        out = (same?0:1);
    } else {
        out = -1;
    }

    return out;
}

BMSparse BMSparse::load(const char* in_filename) {
    BMSparse out;
    int max_line_length = 1000;

    vector<string> file_contents;
    ifstream my_file(in_filename);

    char this_line[max_line_length];
    while(!my_file.eof()) {
        my_file.getline(this_line,max_line_length);
        if(strlen(this_line)) {
            file_contents.push_back(string(this_line));
        }
    }

    int n = file_contents.size();
    int m = 0;

    for(int i = 0; i < n; i++) {
        stringstream this_line_ss;
        int this_bit_count = 0;
        this_line_ss.str(file_contents[i]);
        while(!this_line_ss.eof()) {
            char this_char;
            this_line_ss >> this_char;
            if(!this_line_ss.eof()) {
                if((this_char=='0')||(this_char=='1')) this_bit_count++;
            }
        }
        if(this_bit_count>m) m = this_bit_count;
    }

    out.resize(n,m);
    for(int i = 0; i < n; i++) {
        stringstream this_line_ss;
        this_line_ss.str(file_contents[i]);
        int j = 0;
        while(!this_line_ss.eof()) {
            char this_char;
            this_line_ss >> this_char;
            if(!this_line_ss.eof()) {
                if(this_char=='0') {
                    out.E(i,j,0);
                    j++;
                } else if(this_char=='1') {
                    out.E(i,j,1);
                    j++;
                }
            }
        }
    }

    my_file.close();

    return out;
}

BMSparse BMSparse::integer(const int val, const int len) {
    BMSparse out(len,1);

    {
        int len_req = val?(int)(log2(val))+1:0;
        if(len==-1) out.resize(len_req,1);
    }

    int this_val = val;
    int i = 0;
    do {
        out.E(i,0,this_val%2);
        this_val /= 2;
        i++;
    } while(this_val);

    return out;
}

BMSparse BMSparse::random_perm(const int inN) {
    BMSparse out(inN, inN);

    int* ind_map = new int[inN];
    LCL_Int::randperm(ind_map,inN);
    for(int i = 0; i < inN; i++) {
        out.E(i,ind_map[i],1);
    }


    delete [] ind_map;

    return out;
}

void BMSparse::to_bool_array(bool* out) const {
    bool row_vec = true;
    if((get_n()>1)&&(get_m()==1)) row_vec = false;
    if(row_vec) {
        for(int i = 0; i < get_m(); i++) out[i] = E(0,i);
    } else {
        for(int i = 0; i < get_n(); i++) out[i] = E(i,0);
    }
}

void BMSparse::toBool(bool** out) const {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            out[i][j] = E(i,j);
        }
    }
}

void BMSparse::fromBool(bool** in, int in_N, int in_M) {
    resize(in_N, in_M);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            E(i,j,in[i][j]);
        }
    }
}
