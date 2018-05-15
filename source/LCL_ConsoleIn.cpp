#include "LCL/Core/LCL_ConsoleIn.h"

#include <iostream>
using namespace std;

#include <stdio.h>

int LCL_ConsoleIn::GetCommandOutput(char* dest, int n, const char* comm) {
    int out = 0;
    /*FILE* pipe = NULL;
    if((pipe = popen(comm,"r"))!=NULL) {
        fgets(dest,n,pipe);
        pclose(pipe);
        pipe = NULL;
    } else out = 1;*/
    return out;
}


