#ifndef INTERFACE_SIG_BMS
#define INTERFACE_SIG_BMS

#include "Signature.h"
#include "BMSparse.h"

namespace Interface_SigBMS {
    BMSparse SigToBMS(const Signature& inS);
    Signature BMSToSig(const BMSparse& in_BM);
}

#endif // INTERFACE_SIG_BMS
