#ifndef TO_DECODER_HEADER
#define TO_DECODER_HEADER

#include <iostream>
using namespace std;

#include "GateStringSparse.h"
#include "Signature.h"
#include "GateSigInterface.h"
#include "LCL_Bool.h"
#include "PhasePolynomial.h"
#include "Matrix.h"
#include "BMSparse.h"
#include "LCL_Int.h"
#include "LCL_Array.h"
#include "LCL_Array_imp1.h"
#include "LCL_Matrix_Heap_Sparse.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>

typedef GateStringSparse (*TO_Decoder)(const Signature& in_S);
typedef int (*LempelSelector) (const Signature& inS);

const double update_time = 5.0;

// Global variables
extern string g_output_filename;
extern string g_csv_filename;
extern string g_indvar_out;
extern ostringstream g_h_order_stream;
extern string g_best_random_h_order_f, g_best_random_h_order_nf;
extern bool g_error_report;
extern string g_algorithm;
extern int g_random_circuit_seed;
extern bool g_lempel_feedback;
extern int g_Reed_Muller_max;
extern int g_Hadamard_ancillas;
extern double g_matrix_precision;
extern ofstream* g_output_file;
extern LempelSelector g_lempel_selector;
extern int g_out_no_partitions;
extern bool g_print_load_tfc_debug;
extern int* g_gate_hist;
extern int* g_qubit_hist;
extern int g_out_T_count;
extern int g_fail_count;

namespace SYNTHESIS_ALGORITHM_TAG {
    const string DAFT_GUESS = "re";
    const string LEMPEL_LEAST_GREEDY = "tool_l";
    const string LEMPEL_GREEDY = "tool_lg";
    const string LEMPEL_RANDOM = "tool_r";
    const string TODD = "todd";
    const string TOOL = "tool";
    const string LEMPEL_X = "lx";
    const string LEMPEL_X_2 = "lx2";
    const string REED_MULLER = "rm";
    const string ALL_LEMPEL_SELECTORS = "als";
    const string ALL_LEMPEL = "all";
}
enum SYNTHESIS_ALGORITHM_ID {
    DAFT_GUESS,
    REED_MULLER,
    LEMPEL_LEAST_GREEDY_F,
    LEMPEL_GREEDY_F,
    LEMPEL_RANDOM_F,
    LEMPEL_LEAST_GREEDY_NF,
    LEMPEL_GREEDY_NF,
    LEMPEL_RANDOM_NF,
    LEMPEL_X,
    LEMPEL_X_2
};



GateStringSparse ReedMullerSynthesis(const Signature& inS);
GateStringSparse ReedMullerSynthesis2(const Signature& inS);

//TODD synthesis functions
GateStringSparse TODD(const Signature& inS);
GateStringSparse TODD(const GateStringSparse& inGSM);
PhasePolynomial TODDWrapper(const PhasePolynomial& in);

//Main Lempel synthesis functions
GateStringSparse TOOL(const Signature& inS, int maxRM, LempelSelector lempelSelector, bool feedback);
GateStringSparse TOOL_F_G(const Signature& inS);
GateStringSparse TOOL_F_LG(const Signature& inS);
GateStringSparse TOOL_F_R(const Signature& inS);
GateStringSparse TOOL_WF_G(const Signature& inS);
GateStringSparse TOOL_WF_LG(const Signature& inS);
GateStringSparse TOOL_WF_R(const Signature& inS);

//Lempel selectors
int LempelSelector_LeastGreedy(const Signature& inS);
int LempelSelector_Greedy(const Signature& inS);
int LempelSelector_Random(const Signature& inS);


GateStringSparse RE(const Signature& inS);

void result_analysis(const Signature& inS, const GateStringSparse& inResult, ostream& inOS = cout);
bool synthesis_success(const Signature& inS, const GateStringSparse& inResult);
bool T_fast(const GateStringSparse& inGSS);

PhasePolynomial FullDecoderWrapper(const PhasePolynomial& in, TO_Decoder decoder = TODD);

Matrix RemoveRoundingError(const Matrix& in);

//Post-synthesis Reed-Muller corrections
GateStringSparse PSRMC_Lightweight_Search(const GateStringSparse& inGSS);
GateStringSparse PSRMC_Complex_Submat(const GateStringSparse& inGSS, int n_RM=6, int col_order = 0, int n_orders = 1);
int trapezoid_width(const BMSparse& inBMS, int height);

#endif // TO_DECODER_HEADER
