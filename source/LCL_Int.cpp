#include "LCL_Int.h"

#include <iostream>
using namespace std;

#include <utility>
#include <cstdlib>

void LCL_Int::sort(int* x, int n, bool desc, int* a, int method) {
    if(a) {
        for(int i = 0; i < n; i++) a[i]=i;
    }
    if(desc) {
        for(int i = 0; i < (n-1); i++) {
            for(int j = 0; j < (n-1-i); j++) {
                if(x[j]<x[j+1]) {
                    swap(x[j],x[j+1]);
                    if(a) {
                        swap(a[j],a[j+1]);
                    }
                }
            }
        }
    } else {
        for(int i = 0; i < (n-1); i++) {
            for(int j = 0; j < (n-1-i); j++) {
                if(x[j]>x[j+1]) {
                    swap(x[j],x[j+1]);
                    if(a) {
                        swap(a[j],a[j+1]);
                    }
                }
            }
        }
    }

}

int LCL_Int::randi(int in_min, int in_max) {
    int d = in_max - in_min + 1;
    int out = rand()%d + in_min;
    return out;
}

void LCL_Int::randi(int* x, int n, int in_min, int in_max) {
    for(int i = 0; i < n; i++) {
        x[i] = randi(in_min,in_max);
    }
}

void LCL_Int::print(int* x, int n, const char* pre) {
    if(pre) cout << pre;
    for(int i = 0; i < n; i++) {
        cout << x[i];
        if(i!=(n-1)) {
            cout << ", ";
        }
    }
    cout << endl;
}

void LCL_Int::copy(int* dst, const int* src, int n) {
    for(int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

void LCL_Int::sub(int* dst, const int* src, int n, int m, int i0) {
    for(int i = 0; i < m; i++) {
        dst[i] = src[i+i0];
    }
}

void LCL_Int::concat(int* top, const int* bottom, int n, int m) {
    for(int i = 0; i < m; i++) {
        top[i+n] = bottom[i];
    }
}

void LCL_Int::randperm(int* x, int n, int x0) {
    bool* p = new bool[n];
    for(int i = 0; i < n; i++) p[i] = false;
    for(int i = 0; i < n; i++) {
        int r = LCL_Int::randi(0,n-1-i);
        int shift = 0;
        for(int j = 0; j <= (r+shift); j++) {
            if(p[j]) {
                shift++;
            }
        }
        x[i] = r+shift;
        p[r+shift] = true;
        x[i] += x0;
    }
    delete [] p;
}
