#ifndef INTERFACE_BMS_GSS_HEADER
#define INTERFACE_BMS_GSS_HEADER

#include <iostream>
using namespace std;

#include "BMSparse.h"
#include "GateStringSparse.h"

namespace Interface_BMSGSS {
    GateStringSparse BMSToGSS(const BMSparse& inBMS);
    BMSparse GSSToBMS(const GateStringSparse& inGSS);
}


#endif // INTERFACE_BMS_GSS_HEADER
