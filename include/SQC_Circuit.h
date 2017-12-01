#ifndef HEADER_SQC_CIRCUIT
#define HEADER_SQC_CIRCUIT

#include <iostream>
using namespace std;
#include <ostream>
#include "Signature.h"
#include "GateStringSparse.h"
#include "BMSparse.h"
#include "Matrix.h"
#include "TO_Decoder.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;

typedef int* SQC_Operator; // 0^th element = Operator type, rest of elements = qubit labels
typedef SQC_Operator* SQC_Operator_List;

const int SQC_DEFAULT_MAX_M = 1;
const int SQC_CIRCUIT_EXPAND = 100;

enum SQC_AncillaMode {
    SQC_ANCILLA_MODE_MANUAL,
    SQC_ANCILLA_MODE_PER_GATE,
    SQC_ANCILLA_MODE_PER_CIRCUIT
};

enum SQC_ToffoliNMode {
    SQC_TOFFOLI_N_MODE_TOFF3, // Concatenate Toffoli-3 gates
    SQC_TOFFOLI_N_MODE_JONES,   // Add controls using Cody Jones method
    SQC_TOFFOLI_N_MODE_PSCJ     // Peter Selinger - Cody Jones method
};

enum SQC_HadamardMode {
    SQC_HADAMARD_MODE_PARTITION,
    SQC_HADAMARD_MODE_MONTANARO
};

enum SQC_Operator_Label {
        SQC_OPERATOR_IDENTITY,
        SQC_OPERATOR_X,
        SQC_OPERATOR_Y,
        SQC_OPERATOR_HADAMARD,
        SQC_OPERATOR_CNOT,
        SQC_OPERATOR_T,
        SQC_OPERATOR_T_DAG,
        SQC_OPERATOR_CS,
        SQC_OPERATOR_CS_DAG,
        SQC_OPERATOR_CZ,
        SQC_OPERATOR_CCZ,
        SQC_OPERATOR_S,
        SQC_OPERATOR_S_DAG,
        SQC_OPERATOR_Z,
        SQC_OPERATOR_TOFFOLI,
        SQC_OPERATOR_TOFFOLI_4,
        SQC_OPERATOR_TOFFOLI_N,
        SQC_OPERATOR_PARTITION,
        SQC_OPERATOR_POST_0, // Measures a single qubit in Pauli-Z basis and post-selects +1 outcome (projects qubit onto |0>)
        SQC_OPERATOR_N

// To add operator, update:
//  Print, AddOperator, GetPartition
};
constexpr static char SQC_OPSTRING_IDENTITY[] = "I";
constexpr static char SQC_OPSTRING_HADAMARD[] = "H";
constexpr static char SQC_OPSTRING_CNOT[] = "CNOT";
constexpr static char SQC_OPSTRING_T[] = "T";
constexpr static char SQC_OPSTRING_T_DAG[] = "T'";
constexpr static char SQC_OPSTRING_CS[] = "CS";
constexpr static char SQC_OPSTRING_CS_DAG[] = "CS'";
constexpr static char SQC_OPSTRING_CZ[] = "CZ";
constexpr static char SQC_OPSTRING_CCZ[] = "CCZ";
constexpr static char SQC_OPSTRING_S[] = "S";
constexpr static char SQC_OPSTRING_S_DAG[] = "S'";
constexpr static char SQC_OPSTRING_Z[] = "Z";
constexpr static char SQC_OPSTRING_TOFFOLI[] = "Toffoli";
constexpr static char SQC_OPSTRING_TOFFOLI_4[] = "Toffoli-4";
constexpr static char SQC_OPSTRING_TOFFOLI_N[] = "t"; // N is given by number of arguments using formula N = (nargs+3)/2
constexpr static char SQC_OPSTRING_PARTITION[] = "P";
constexpr static char SQC_OPSTRING_X[] = "X";
constexpr static char SQC_OPSTRING_Y[] = "Y";
constexpr static char SQC_OPSTRING_POST_0[] = "M";

struct SQC_Circuit {
    // Properties
    int n = 0; //Number of qubits (total)
    int d = 0; //Number of data qubits
    int m = 0; //Number of operators
    int max_m = SQC_DEFAULT_MAX_M; //Max number of operators
    SQC_Operator_List operator_list = NULL;
    int hadamard_mode_max_ancillas = -1; // n_H in writing
    int p_hads = 0; // Number of Hadamard ancillas indexed from n - p_hads to n-1

    int p = 0; //(deprocated) Number of ancillas, ancilla indices: n-p+1 -> n
               //Number of non-ancilla qubits = n-p
               //If p=-1, then allocate ancillas automatically using SQC_ANCILLA_MODE_PER_GATE
               //If p=-2, then allocate ancillas automatically using SQC_ANCILLA_MODE_PER_CIRCUIT

    SQC_AncillaMode ancilla_mode = SQC_ANCILLA_MODE_MANUAL; // (deprocated)
    SQC_ToffoliNMode toffoli_n_mode = SQC_TOFFOLI_N_MODE_PSCJ; // (deprocated)
    SQC_HadamardMode hadamard_mode = SQC_HADAMARD_MODE_MONTANARO; // (deprocated)


    // Methods

    SQC_Circuit();
    SQC_Circuit(int in_n);
    SQC_Circuit(int in_n, int in_d);
    SQC_Circuit(int in_n, int in_d, int in_p);
    SQC_Circuit(const SQC_Circuit& in_C);
    ~SQC_Circuit();
    void Construct();
    void Destruct();
    void Copy(const SQC_Circuit& in_C);

    void Print(ostream* in_OS = &cout, int start_i = 0, int print_n = -1,bool with_t = true) const;

    void Load(const char* in_filename);
    void Save(const char* in_filename) const;
    void Clear();
    void Resize(int in_max_m);
    void AddAncilla(int anc_inc = 1);

    void AddOperator(const char* in_op_str);
    void AddOperator(const SQC_Operator in_op, int len = 0);
    void DeleteOperator(int t);

    bool GetPartition(SQC_Circuit* out_Hadamards, SQC_Circuit* out_CNOT_T);
    bool GetPartition(SQC_Circuit* out);
    void DecompositionVW(SQC_Circuit* out_V, SQC_Circuit* out_W) const;
    BMSparse toGateSynthesisMatrix() const;
    void fromGateSynthesisMatrix(const BMSparse& inBMS);

    Matrix toMatrix() const;
    bool NextSignature(Signature& outSig);

    void ReplaceOperator(SQC_Circuit* in_new_ops, int t, int n_rep=1);
    void ConvertFromToffoli();
    void ConvertHadamard(SQC_Circuit* out, int max_ancillas = -1); // Implements H -> new bool variable method of Ashley Montanaro
    int CountOperators(SQC_Operator_Label in_op = SQC_OPERATOR_N) const;
    void RemoveExternalHadamards();
    bool CancelAdjacentHadamards();
    bool CancelAdjacentTs();

    void PrintOperatorDistribution(ostream* in_OS = &cout) const;

    void AllocateAncillas(const SQC_Circuit& in_C); // Constructs new circuit with automatically allocated ancillas
    int GetNArgs(int i) const;

    static SQC_Circuit* LoadTFCFile(const char* inFilename);

    #include "UniversalOptimize.h"
};

double VerifyOptimization(const SQC_Circuit& in, const SQC_Circuit& out); //Returns 0.0 only if in and out are identical, otherwise a negative number.
double VerifyOptimization2(const SQC_Circuit& in, const SQC_Circuit& out); //Returns 0.0 only if in and out are identical, otherwise a negative number.

#endif // HEADER_SQC_CIRCUIT
