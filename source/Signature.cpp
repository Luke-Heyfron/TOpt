#include "Signature.h"

#include <iostream>
using namespace std;

#include <unordered_set>
#include <utility>
#include <iterator>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "LCL/LCL_Bool.h"
#include "LCL/LCL_Int.h"
#include "Bool_Signature.h"
#include "LCL/Core/LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

Signature::Signature(int in_n) {
    n = in_n;
    N = Na() + Nb() + Nc();
    data_us = new unordered_set<int>;
    bad_signature = false;
}

Signature::Signature() {
    n = 0;
    N = Na() + Nb() + Nc();
    data_us = new unordered_set<int>;
    bad_signature = false;
}

Signature::~Signature() {
    delete data_us;
}

bool Signature::E(int I) const {
    int temp[3];
    int thisType = indTo_ijklmp(I,temp);
    switch(thisType) {
    case 0:
        I = iToInd(temp[0]);
        break;
    case 1:
        I = jkToInd(temp[0],temp[1]);
        break;
    case 2:
        I = lmpToInd(temp[0],temp[1],temp[2]);
        break;
    }
    unordered_set<int>::iterator ePos = data_us->find(I);
    bool out = true;
    if(ePos==data_us->end()) out = false;
    return out;
}

bool Signature::E(int i1, int i2, int i3) const {
    if((i2>0)>(i1>0)) swap(i1,i2);
    if((i3>0)>(i2>0)) swap(i2,i3);
    if((i2>0)>(i1>0)) swap(i1,i2);
    int I;
    int thisType = (i1>0)+(i2>0)+(i3>0)-1;
    switch(thisType) {
    case 0:
        I = iToInd(i1);
        break;
    case 1:
        I = jkToInd(i1,i2);
        break;
    case 2:
        I = lmpToInd(i1,i2,i3);
        break;
    }
    unordered_set<int>::iterator ePos = data_us->find(I);
    bool out = true;
    if(ePos==data_us->end()) out = false;
    return out;
}

int Signature::setByInd(int I) {
    int temp[3];
    int thisType = indTo_ijklmp(I,temp);
    switch(thisType) {
    case 0:
        I = iToInd(temp[0]);
        break;
    case 1:
        I = jkToInd(temp[0],temp[1]);
        break;
    case 2:
        I = lmpToInd(temp[0],temp[1],temp[2]);
        break;
    }
    pair<unordered_set<int>::iterator,bool> tempP = data_us->insert(I);
    return tempP.second;
}

int Signature::set(int i) {
    int I = iToInd(i);
    pair<unordered_set<int>::iterator,bool> temp = data_us->insert(I);
    return temp.second;
}

int Signature::set(int j, int k) {
    int I = jkToInd(j,k);
    pair<unordered_set<int>::iterator,bool> temp = data_us->insert(I);
    return temp.second;
}

int Signature::set(int l, int m, int p) {
    int I = lmpToInd(l,m,p);
    pair<unordered_set<int>::iterator,bool> temp = data_us->insert(I);
    return temp.second;
}

int Signature::clearByInd(int I) {
    int temp[3];
    int thisType = indTo_ijklmp(I,temp);
    switch(thisType) {
    case 0:
        I = iToInd(temp[0]);
        break;
    case 1:
        I = jkToInd(temp[0],temp[1]);
        break;
    case 2:
        I = lmpToInd(temp[0],temp[1],temp[2]);
        break;
    }
    unordered_set<int>::size_type tempP = data_us->erase(I);
    return tempP;
}

int Signature::clear(int i) {
    int I = iToInd(i);
    unordered_set<int>::size_type temp = data_us->erase(I);
    return temp;
}

int Signature::clear(int j, int k) {
    int I = jkToInd(j,k);
    unordered_set<int>::size_type temp = data_us->erase(I);
    return temp;
}

int Signature::clear(int l, int m, int p) {
    int I = lmpToInd(l,m,p);
    unordered_set<int>::size_type temp = data_us->erase(I);
    return temp;
}

int Signature::Na() const {
    int out = n;
    return out;
}

int Signature::Nb() const {
    int out = 0.5*n*(n-1);
    return out;
}

int Signature::Nc() const {
    int out = (1.0/6.0)*n*(n-1)*(n-2);
    return out;
}

int Signature::iToInd(int i) const {
    int out = i-1;
    return out;
}

int Signature::jkToInd(int j, int k) const {
    j--;
    k--;
    if(j>k) {
        swap(j,k);
    } else if(j==k) {
        return iToInd(j+1);
    }
    int out = Na() + n*j + k;
    return out;
}

int Signature::lmpToInd(int l, int m, int p) const {
    l--;
    m--;
    p--;
    if(l>m){
        swap(l,m);
    }
    if(m>p){
        swap(m,p);
    }
    if(l>m){
        swap(l,m);
    }
    if((l==m)||(m==p)) {
        return jkToInd(l+1,p+1);
    }
    int out = Na() + n*n + n*n*l + n*m + p; //Not right. Mustn't allow for values where l==m or m==p or l=p.
    return out;
}

int Signature::indTo_ijklmp(int I, int* ijklmp) const {
    /*
    Converts index into either a_i, b_jk, or c_lmp.

    Arguments:
        I - The index value to be converted
        ijklmp - an integer vector of size at least 3 to be declared outside of scope where the a_i, b_jk, c_lmp. Index tuples of size < 3 are padded with -1's.

    Return value:
        -1 if error occurs
        0 if I corresponds to an a_i
        1 if I corresponds to an b_jk
        2 if I corresponds to an c_lmp
    */

    int out = -1;
    if(I<Na()) {
        ijklmp[0] = I+1;
        ijklmp[1] = -1;
        ijklmp[2] = -1;
        out = 0;
    } else if((I>=Na())&&(I<(Na()+n*n))) {
        int tempI = I - Na();
        int k = (tempI%n)+1;
        tempI /= n;
        int j = (tempI%n)+1;
        if(j>k) swap(j,k);
        if(j!=k) {
            ijklmp[0] = j;
            ijklmp[1] = k;
            ijklmp[2] = -1;
            out = 1;
        } else {
            ijklmp[0] = j;
            ijklmp[1] = -1;
            ijklmp[2] = -1;
            out = 0;
        }
    } else if((I>=(Na()+n*n)&&(I<(Na()+n*n+n*n*n)))) {
        int tempI = I - Na() - n*n;
        int p = (tempI%n)+1;
        tempI /= n;
        int m = (tempI%n)+1;
        tempI /= n;
        int l = (tempI%n)+1;
        if(l>m) swap(l,m);
        if(m>p) swap(m,p);
        if(l>m) swap(l,m);

        if((l!=m)&&(m!=p)) {
            ijklmp[0] = l;
            ijklmp[1] = m;
            ijklmp[2] = p;
            out = 2;
        } else {
            if((l!=m)||(m!=p)) {
                ijklmp[0] = l;
                ijklmp[1] = p;
                ijklmp[2] = -1;
                out = 1;
            } else {
                ijklmp[0] = l;
                ijklmp[1] = -1;
                ijklmp[2] = -1;
                out = 0;
            }
        }
    }
    return out;
}

int Signature::getN() const {
    return N;
}

void Signature::print(ostream& inOS) const {
    int this_ijklmp[3];
    int this_type;
    int* this_a = new int[Na()];
    int* this_b_j = new int[Nb()];
    int* this_b_k = new int[Nb()];
    int* this_c_l = new int[Nc()];
    int* this_c_m = new int[Nc()];
    int* this_c_p = new int[Nc()];
    int a_count = 0;
    int b_count = 0;
    int c_count = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        this_type = indTo_ijklmp(thisI, this_ijklmp);
        switch(this_type) {
        case 0:
            this_a[a_count] = this_ijklmp[0];
            a_count++;
            break;
        case 1:
            this_b_j[b_count] = this_ijklmp[0];
            this_b_k[b_count] = this_ijklmp[1];
            b_count++;
            break;
        case 2:
            this_c_l[c_count] = this_ijklmp[0];
            this_c_m[c_count] = this_ijklmp[1];
            this_c_p[c_count] = this_ijklmp[2];
            c_count++;
            break;
        default:
            LOut(); inOS << "Error! Bad index." << endl;
            break;
        }
    }
    LOut(); inOS << "Signature: ";
    //cout << "a_i:" << endl;
    for(int i = 0; i < a_count; i++) {
        inOS << "x" << this_a[i];
        if(i<(a_count+b_count+c_count-1)) {
            inOS << " + ";
        }
    }
    //cout << "b_jk:" << endl;
    for(int i = 0; i < b_count; i++) {
        inOS << "2.x" << this_b_j[i] << ".x" << this_b_k[i];
        if(i<(b_count+c_count-1)) {
            inOS << " + ";
        }
    }
    //cout << "c_lmp:" << endl;
    for(int i = 0; i < c_count; i++) {
        inOS << "4.x" << this_c_l[i] << ".x" << this_c_m[i] << ".x" << this_c_p[i];
        if(i<(c_count-1)) {
            inOS << " + ";
        }
    }
    inOS << endl;
    delete [] this_a;
    delete [] this_b_j;
    delete [] this_b_k;
    delete [] this_c_l;
    delete [] this_c_m;
    delete [] this_c_p;
}

int Signature::getA(int* a_i) const {
    int this_ijklmp[3];
    int this_type;
    int a_count = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        this_type = indTo_ijklmp(thisI, this_ijklmp);
        if(this_type==0) {
            a_i[a_count] = this_ijklmp[0];
            a_count++;
        }
    }
    return a_count;
}

int Signature::getB(int* b_j, int* b_k) const {
    int this_ijklmp[3];
    int this_type;
    int b_count = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        this_type = indTo_ijklmp(thisI, this_ijklmp);
        if(this_type==1) {
            b_j[b_count] = this_ijklmp[0];
            b_k[b_count] = this_ijklmp[1];
            b_count++;
        }
    }
    return b_count;
}
int Signature::getC(int* c_l, int* c_m, int* c_p) const {
    int this_ijklmp[3];
    int this_type;
    int c_count = 0;
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        this_type = indTo_ijklmp(thisI, this_ijklmp);
        if(this_type==2) {
            c_l[c_count] = this_ijklmp[0];
            c_m[c_count] = this_ijklmp[1];
            c_p[c_count] = this_ijklmp[2];
            c_count++;
        }
    }
    return c_count;
}

Signature& Signature::assign(const Signature& inS) {
    *data_us = *inS.data_us;
    n = inS.n;
    N = Na() + Nb() + Nc();
    return *this;
}

Signature& Signature::operator=(const Signature& inS) {
    return assign(inS);
}

Signature Signature::add(const Signature& inS) const {
    Signature out(n);

    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int thisI = *it;
        unordered_set<int>::iterator ePos = inS.data_us->find(thisI);
        if(ePos==inS.data_us->end()) {
            out.setByInd(thisI);
        }
    }

    for(unordered_set<int>::iterator it = inS.data_us->begin(); it != inS.data_us->end(); it++) {
        int thisI = *it;
        unordered_set<int>::iterator ePos = data_us->find(thisI);
        if(ePos==data_us->end()) {
            out.setByInd(thisI);
        }
    }

    return out;
}

Signature Signature::operator+(const Signature& inS) const {
    return add(inS);
}

int Signature::get_n() const {
    return n;
}

void Signature::set_n(int in_n) {
    n = in_n;
    N = Na() + Nb() + Nc();
}

bool Signature::bad() const {
    return bad_signature;
}

Signature Signature::SigFromFile(const char* inFilename) {

    Signature out;
    int linewidth = 100;
    int max_lines = 1000;
    int this_n = -1;
    int linecount = 0;

    ifstream myfile(inFilename);
    if(myfile.bad()||myfile.fail()||(!myfile.good())||myfile.eof()) {
        cout << "That file is no good!" << endl;
        out.bad_signature = true;
    } else {
        char** filetext = new char*[max_lines];

        while(!myfile.eof()) {
            filetext[linecount] = new char[linewidth];

            myfile.getline(filetext[linecount],linewidth);
            if((!myfile.eof())&&(strlen(filetext[linecount]))) {
                char* thistok = NULL;
                char temp[linewidth];
                strcpy(temp,filetext[linecount]);
                thistok = strtok(temp," ");
                if(!strcmp(thistok,"n")) {
                    thistok = strtok(NULL," ");
                    this_n = atoi(thistok);
                }
            }
            linecount++;
        }

        if(this_n<=0) {
            cout << "Error! Circuit size not found." << endl;
            out.bad_signature = true;
        } else {
            out.set_n(this_n);

            for(int i = 0; i < linecount; i++) {
                if(strlen(filetext[i])>0) {
                    char* thistok = NULL;
                    char temp[linewidth];
                    strcpy(temp,filetext[i]);
                    thistok = strtok(temp," ");
                    if(!strcmp(thistok,"a")) {
                        thistok = strtok(NULL," ");
                        int this_i = atoi(thistok);
                        out.set(this_i);
                    } else if(!strcmp(thistok,"b")) {
                        thistok = strtok(NULL," ");
                        int this_j = atoi(thistok);
                        thistok = strtok(NULL," ");
                        int this_k = atoi(thistok);
                        out.set(this_j,this_k);
                    } else if(!strcmp(thistok,"c")) {
                        thistok = strtok(NULL," ");
                        int this_l= atoi(thistok);
                        thistok = strtok(NULL," ");
                        int this_m = atoi(thistok);
                        thistok = strtok(NULL," ");
                        int this_p = atoi(thistok);
                        out.set(this_l,this_m,this_p);
                    }
                }
            }
        }

        for(int i = 0; i < linecount; i++) {
            delete [] filetext[i];
        }
        delete [] filetext;
    }



    myfile.close();
    return out;
}

void Signature::h_decomposition(const int h, Signature& out_alpha, Signature& outAp, Signature& outB) const {
    Signature this_alpha(n), thisAp(n), thisB(n);

    // Set alpha
    if(E(h,0,0)) {
        this_alpha.set(h,0,0);
    }
    int* theseAi = new int[Na()];
    int* theseBj = new int[Nb()];
    int* theseBk = new int[Nb()];
    int* theseCl = new int[Nc()];
    int* theseCm = new int[Nc()];
    int* theseCp = new int[Nc()];

    int n_a = getA(theseAi);
    int n_b = getB(theseBj,theseBk);
    int n_c = getC(theseCl,theseCm,theseCp);

    // Set Ap, set B
    for(int a = 0; a < n_a; a++) {
        int this_ai = theseAi[a];
        if(this_ai!=h) {
            thisB.set(this_ai,0,0);
        }
    }
    for(int b = 0; b < n_b; b++) {
        int this_bj = theseBj[b];
        int this_bk = theseBk[b];
        if(this_bj==h) {
            thisAp.set(this_bk);
        } else if(this_bk==h) {
            thisAp.set(this_bj);
        }
        if((this_bj!=h)&&(this_bk!=h)) {
            thisB.set(this_bj,this_bk,0);
        }
    }
    for(int c = 0; c < n_c; c++) {
        int this_cl = theseCl[c];
        int this_cm = theseCm[c];
        int this_cp = theseCp[c];
        if(this_cl==h) {
            thisAp.set(this_cm,this_cp,0);
        } else if(this_cm==h) {
            thisAp.set(this_cl,this_cp,0);
        } else if(this_cp==h) {
            thisAp.set(this_cl,this_cm,0);
        }
        if((this_cl!=h)&&(this_cm!=h)&&(this_cp!=h)) {
            thisB.set(this_cl,this_cm,this_cp);
        }
    }

    delete [] theseAi;
    delete [] theseBj;
    delete [] theseBk;
    delete [] theseCl;
    delete [] theseCm;
    delete [] theseCp;

    out_alpha.assign(this_alpha);
    outAp.assign(thisAp);
    outB.assign(thisB);
}

Signature Signature::diminish(const int h) const {
    Signature out(n-1);

    int this_labels[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        int this_type = indTo_ijklmp(I,this_labels);
        int n_h = 0;
        for(int i = 0; i < 3; i++) {
            if(this_labels[i]==h) {
                n_h++;
            }
            if(this_labels[i]>h) {
                this_labels[i]--;
            }
        }
        if(!n_h) {
            if(this_type==0) {
                out.set(this_labels[0]);
            } else if(this_type==1) {
                out.set(this_labels[0],this_labels[1]);
            } else if(this_type==2) {
                out.set(this_labels[0],this_labels[1],this_labels[2]);
            }
        }
    }

    return out;
}

Signature Signature::augment(const int h) const {
    Signature out(n+1);

    int this_labels[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        int this_type = indTo_ijklmp(I,this_labels);
        for(int i = 0; i < 3; i++) {
            if(this_labels[i]>=h) {
                this_labels[i]++;
            }
        }
        if(this_type==0) {
            out.set(this_labels[0]);
        } else if(this_type==1) {
            out.set(this_labels[0],this_labels[1]);
        } else if(this_type==2) {
            out.set(this_labels[0],this_labels[1],this_labels[2]);
        }
    }

    return out;
}

bool Signature::isEmpty() const {
    return data_us->empty();
}

Signature Signature::mult2xh(const int h) const {
    Signature out(n);

    int this_inds[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        int this_type = indTo_ijklmp(I, this_inds);
        switch(this_type) {
            case 0:
                if(h==this_inds[0]) {
                    cout << "Signature error! mult2xh: xh should not be an element of FoT(S)." << endl;
                } else {
                    out.set(this_inds[0],h);
                }
                break;
            case 1:
                if((h==this_inds[0])||(h==this_inds[1])) {
                    cout << "Signature error! mult2xh: xh should not be an element of FoT(S)." << endl;
                } else {
                    out.set(this_inds[0],this_inds[1],h);
                }
                break;
        }
    }

    return out;
}

int Signature::h_weight(const int h) const {
    int out = 0;

    int this_inds[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        indTo_ijklmp(I,this_inds);
        bool exit = false;
        for(int i = 0; (!exit)&&(i < this_inds[i]); i++) {
            if(this_inds[i]==h) {
                out++;
                exit = true;
            }
        }
    }

    return out;
}

void Signature::save(const char* inFilename) const {
    ofstream my_file(inFilename);
    my_file << "n " << n << endl;
    int this_inds[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        int this_type = indTo_ijklmp(I, this_inds);
        switch(this_type){
            case 0:
                my_file << "a " << this_inds[0] << endl;
                break;
            case 1:
                my_file << "b " << this_inds[0] << " " << this_inds[1] << endl;
                break;
            case 2:
                my_file << "c " << this_inds[0] << " " << this_inds[1] << " " << this_inds[2] << endl;
                break;
        }
    }
    my_file.close();
}

void Signature::save(ostream& inOS) const {
    inOS << "n " << n << endl;
    int this_inds[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int I = *it;
        int this_type = indTo_ijklmp(I, this_inds);
        switch(this_type){
            case 0:
                inOS << "a " << this_inds[0] << endl;
                break;
            case 1:
                inOS << "b " << this_inds[0] << " " << this_inds[1] << endl;
                break;
            case 2:
                inOS << "c " << this_inds[0] << " " << this_inds[1] << " " << this_inds[2] << endl;
                break;
        }
    }
}

int Signature::get_N_eff() const {
    int out = 0;

    for(int h = 1; h <= n; h++) {
        out += (bool)h_weight(h);
    }

    return out;
}

Signature Signature::subsig(bool* inc_vec) const {
    Signature out;

    int n_sub = LCL_Bool::Weight(inc_vec,n);
    out.set_n(n_sub);

    int* this_sub_mapping = new int[n];

    int h_sub = 1;
    for(int i = 0; i < n; i++) {
        if(inc_vec[i]) {
            this_sub_mapping[i] = h_sub;
            h_sub++;
        } else {
            this_sub_mapping[i] = 0;
        }
    }

    //LCL_Int::print(this_sub_mapping,n);

    int this_inds[3];
    int this_sub_inds[3];
    for(unordered_set<int>::iterator it = data_us->begin(); it != data_us->end(); it++) {
        int this_I = *it;
        int this_type = indTo_ijklmp(this_I, this_inds);
        bool exit = false;
        //LCL_Int::print(this_inds,3,"this_inds: ");
        for(int i = 0; (!exit)&&(i < 3); i++) {
            if(this_inds[i]>0) {
                int this_h = this_sub_mapping[this_inds[i]-1];
                if(this_h>0) {
                    this_sub_inds[i] = this_h;
                } else {
                    exit = true;
                }
            } else {
                this_sub_inds[i] = -1;
            }
        }
        if(!exit) {
            //LCL_Int::print(this_sub_inds,3,"this_sub_inds: ");
            switch(this_type) {
                case 0:
                    out.set(this_sub_inds[0]);
                    break;
                case 1:
                    out.set(this_sub_inds[0],this_sub_inds[1]);
                    break;
                case 2:
                    out.set(this_sub_inds[0],this_sub_inds[1],this_sub_inds[2]);
                    break;
            }
        }
    }

    delete [] this_sub_mapping;

    return out;
}

void Signature::toBool(bool*** out) const {
    for(int i = 0; i < n; i++) {
        Bool_Signature::set(out,i,i,i,E(i+1,0,0));
        for(int j = (i+1); (i<(n-1))&&(j < n); j++) {
            Bool_Signature::set(out,i,j,j,E(i+1,j+1,0));
            for(int k = (j+1); (j<(n-1))&&(k < n); k++) {
                Bool_Signature::set(out,i,j,k,E(i+1,j+1,k+1));
            }
        }
    }
}
