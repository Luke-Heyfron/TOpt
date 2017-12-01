#ifndef TO_CIRCUIT_GENERATORS_HEADER
#define TO_CIRCUIT_GENERATORS_HEADER

#include "Signature.h"

Signature CircuitGenerator(const string& inS);
Signature CircuitGenerator_Toffhash(int N_hash);
Signature CircuitGenerator_Toffoli(int N_toff);
Signature CircuitGenerator_RandomComplex(int n, int in_seed = 0);

#endif // TO_COMMANDS_HEADER
