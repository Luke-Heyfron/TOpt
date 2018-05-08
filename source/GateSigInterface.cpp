#include "GateSigInterface.h"

#include <iostream>
using namespace std;

#include "GateStringSparse.h"
#include "Signature.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

#include <vector>

GateStringSparse GateSigInterface::SigToGSS(const Signature& inSig) {
    int n = inSig.get_n();
    GateStringSparse out(n);

    int n_a = inSig.Na();
    int n_b = inSig.Nb();
    int n_c = inSig.Nc();

    int* a_i = new int[n_a];
    int* b_j = new int[n_b];
    int* b_k = new int[n_b];
    int* c_l = new int[n_c];
    int* c_m = new int[n_c];
    int* c_p = new int[n_c];

    int a_count = inSig.getA(a_i);

    int b_count = inSig.getB(b_j,b_k);

    int c_count = inSig.getC(c_l,c_m,c_p);

    for(int i = 0; i < a_count; i++) {
        out = out + GateStringSparse::expandLin(n, a_i[i]);
    }

    for(int i = 0; i < b_count; i++) {
        out = out + GateStringSparse::expandQuad(n, b_j[i], b_k[i]);
        //cout << "b_j[i] = " << b_j[i] << "; b_k[i] = " << b_k[i] << endl;
        //GateStringSparse::expandQuad(n, b_j[i], b_k[i]).print();
        //out.print();
    }

    for(int i = 0; i < c_count; i++) {
        out = out + GateStringSparse::expandCube(n, c_l[i], c_m[i], c_p[i]);
    }

    delete [] a_i;
    delete [] b_j;
    delete [] b_k;
    delete [] c_l;
    delete [] c_m;
    delete [] c_p;
    return out;
}

Signature GateSigInterface::expandGSSTerm(const GateStringSparse& inGSS) {
    Signature out(inGSS.get_n());

    for(int i = 0; (i < inGSS.get_n()); i++) {
        int this_a = 0;
        for(int t = 0; t < inGSS.weight(); t++) {
            this_a += inGSS.E(t,i);
        }
        this_a %= 2;
        if(this_a) out.set(i+1);
        for(int j = (i+1); (i < inGSS.get_n())&&(j < inGSS.get_n()); j++) {
            int this_b = 0;
            for(int t = 0; t < inGSS.weight(); t++) {
                this_b += inGSS.E(t,i)*inGSS.E(t,j);
            }
            this_b %= 2;
            if(this_b) out.set(i+1,j+1);
            for(int k = (j+1); (i < inGSS.get_n())&&(j < inGSS.get_n())&&(k < inGSS.get_n()); k++) {
                int this_c = 0;
                for(int t = 0; t < inGSS.weight(); t++) {
                    this_c += inGSS.E(t,i)*inGSS.E(t,j)*inGSS.E(t,k);
                }
                this_c %= 2;
                if(this_c) out.set(i+1,j+1,k+1);
            }
        }
    }

    /*int n_t = inGSS.weight(true);
    if(n_t>1) {
        unordered_set<int> data = inGSS.get_data();
        bool* this_x = new bool[inGSS.get_n()];
        for(unordered_set<int>::iterator it = data.begin(); it != data.end(); it++) {
            int I = *it;
            if(I!=0) {
                inGSS.I_to_x(I,this_x);
                GateStringSparse tempGSS(inGSS.get_n());
                tempGSS.set(this_x);
                {
                    Signature tempout = out + GateSigInterface::expandGSSTerm(tempGSS);
                    out.assign(tempout);
                }
            }
        }
        delete this_x;
    } else if(n_t==1) {
        unordered_set<int> data = inGSS.get_data();
        for(unordered_set<int>::iterator it = data.begin(); it != data.end(); it++) {
            int I = *it;
            if(I!=0) {
                bool* this_x = new bool[inGSS.get_n()];
                int* these_pos = new int[inGSS.get_n()];
                inGSS.I_to_x(I,this_x);
                int this_t = 0;
                for(int i = 0; i < inGSS.get_n(); i++) {
                    if(this_x[i]) {
                        these_pos[this_t] = i;
                        this_t++;
                    }
                }
                switch(this_t) {
                    case 0:
                        break;
                    case 1:
                        out.set(these_pos[0]+1);
                        break;
                    case 2:
                        out.set(these_pos[0]+1);
                        out.set(these_pos[1]+1);
                        out.set(these_pos[0]+1,these_pos[1]+1);
                        break;
                    default:
                        int this_h = these_pos[0];
                        this_x[this_h]=false;
                        out.set(this_h+1);
                        {
                            GateStringSparse tempGSS(inGSS.get_n());
                            tempGSS.set(this_x);
                            Signature temp_term = GateSigInterface::expandGSSTerm(tempGSS);
                            Signature tempout = out + temp_term + temp_term.mult2xh(this_h+1);
                            out.assign(tempout);
                        }
                        break;
                }

                delete [] these_pos;
                delete [] this_x;
            }
        }
    }*/

    return out;
}
