#ifndef GATE_SIG_INTERFACE_HEADER
#define GATE_SIG_INTERFACE_HEADER

#include "GateStringSparse.h"
#include "Signature.h"

namespace GateSigInterface {
    GateStringSparse SigToGSS(const Signature& inSig);
    Signature expandGSSTerm(const GateStringSparse& inGSS);
}

#endif // GATE_SIG_INTERFACE_HEADER
