#include "Interface_SigBMS.h"

#include <iostream>
using namespace std;

#include "Signature.h"
#include "BMSparse.h"

BMSparse Interface_SigBMS::SigToBMS(const Signature& inS) {
    int n = inS.get_n();

    BMSparse out(n,n);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i==j) {
                out.E(i,j,inS.E(i+1,0,0));
            } else if(i<j) {
                out.E(i,j,inS.E(i+1,j+1,0));
            } else if(j<i) {
                out.E(i,j,inS.E(j+1,i+1,0));
            }
        }
    }

    return out;
}

Signature Interface_SigBMS::BMSToSig(const BMSparse& in_BM) {
    int n = in_BM.get_n();
    Signature out(n);

    for(int i = 0; i < n; i++) {
        if(in_BM.S_abc(i,i,i)) {
            out.set(i+1);
        }
    }

    for(int i = 0; i < (n-1); i++) {
        for(int j = (i+1); j < n; j++) {
            if(in_BM.S_abc(i,j,j)) {
                out.set(i+1,j+1);
            }
        }
    }

    for(int i = 0; i < (n-2); i++) {
        for(int j = (i+1); j < (n-1); j++) {
            for(int k = (j+1); k < n; k++) {
                if(in_BM.S_abc(i,j,k)) {
                    out.set(i+1,j+1,k+1);
                }
            }
        }
    }

    return out;
}
