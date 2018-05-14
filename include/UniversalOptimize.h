#ifndef UNIVERSAL_OPTIMIZE_HEADER

#include "SQC_Circuit.h"
//#include "TO_Decoder.h"

// 0. Full UniversalOptimize algorithm
static SQC_Circuit UniversalOptimize(const SQC_Circuit& in, TO_Decoder decoder);

// 1. Convert Y to XZ
static SQC_Circuit convert_Y_to_XZ(const SQC_Circuit& in);

// 2. Expand Toff_n to Toff_3
//int N_Toffn_ancillas() const;
static SQC_Circuit expand_Toffn_to_Toff3(const SQC_Circuit& in); // Assumes out already has enough ancillas

    // 2a) (Optional) Remove all Pauli X gates (resulting circuit will be Clifford equivalent to input circuit)
//static SQC_Circuit remove_Pauli_Xs(const SQC_Circuit& in);
    // 2b) (Optional) Search for H CZ H patterns to rewrite as CNOT's.

// 3. Convert X and Toff_3 to H, Z and CCZ
static SQC_Circuit convert_Xs(const SQC_Circuit& in);
//static SQC_Circuit convert_Toff3_to_HCCZH(const SQC_Circuit& in);

// 4. Convert CZ, CS and CCZ to CNOT and T
static SQC_Circuit convert_Cliff3s(const SQC_Circuit& in);
//static SQC_Circuit convert_CS_to_CNOT_T(const SQC_Circuit& in);
//static SQC_Circuit convert_CCZ_to_CNOT_T(const SQC_Circuit& in);

// 5. Replace T' = ZST and S' = ZS
static SQC_Circuit convert_Daggers(const SQC_Circuit& in);

// 6. Cancel (effectively) adjacent gate-inverse(gate) pairs.
void simplify(); // Assumes gate set is {Z,S,T,H,CNOT} only

// 7. Strip external Hadamards / 6. Partition internal circuit such that each partition contains no more than n_H Hadamards
static void decompose_into_Hadamard_partitions(const SQC_Circuit& in, SQC_Circuit**& inHs, int& N_Hs, SQC_Circuit**& inPs, int& N_Ps); // U = H_NP P_NP ... H_1 P_1 H_0. Assumes gate set {Z,S,T,H,CNOT}
//int N_max_Hadamards() const;

// 8. Convert internal Hadamards of each partition to external Hadamards and postselection
static void Hadamards_to_Gadgets(const SQC_Circuit& in, SQC_Circuit*& outL, SQC_Circuit*& outPp, SQC_Circuit*& outR); // P = R Pp L, where: L contains |0> state preparation and Hadamards on Hadamard ancilals; R contains Hadamards and |0> post-selection on Hadamard ancillas.

// 9. Split each partition into CNOT circuit * Diagonal C_3 circuit
static void decompose_C3_to_CNOT_D3(const SQC_Circuit& in, SQC_Circuit*& CNOT, SQC_Circuit*& D3);

// 10. Optimize each partition's diagonal circuit w.r.t. T-count and map back to circuit decomposition.
static SQC_Circuit optimize_D3(const SQC_Circuit& in, TO_Decoder decoder);
static SQC_Circuit TODD_optimize_D3(const SQC_Circuit& in);


// Debugging
static void SaveRepresentationsToFile(const SQC_Circuit& in_C, const char* in_filename);

#endif // UNIVERSAL_OPTIMIZE_HEADER
