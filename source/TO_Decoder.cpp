#include "TO_Decoder.h"

#include <iostream>
using namespace std;

#include "GateStringSparse.h"
#include "Signature.h"
#include "GateSigInterface.h"
#include "LCL/LCL_Bool.h"
#include "BMSparse.h"
#include "Interface_BMSGSS.h"
#include "Interface_SigBMS.h"
#include "PhasePolynomial.h"
#include "LCL/Core/LCL_ConsoleOut.h"
#include "LCL/LCL_Mat_GF2.h"
#include "GateSynthesisMatrix.h"
#include "TO_Maps.h"
#include "WeightedPolynomial.h"
#include "Matrix.h"
#include "LCL/Maths/Implementations/LCL_Matrix_Heap_Sparse_imp1.h"
using namespace LCL_ConsoleOut;
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>

string g_output_filename;
string g_csv_filename;
string g_indvar_out;
ostringstream g_h_order_stream;
string g_best_random_h_order_f, g_best_random_h_order_nf;
bool g_error_report = false;
string g_algorithm = SYNTHESIS_ALGORITHM_TAG::TODD;
int g_random_circuit_seed = 0;
bool g_lempel_feedback = 1;
int g_Reed_Muller_max = 6;
int g_Hadamard_ancillas = -1;
double g_matrix_precision = 1.0e-16;
ofstream* g_output_file = NULL;
LempelSelector g_lempel_selector = LempelSelector_Random;
string g_lempel_selector_name = LEMPEL_SELECTOR_NAME::RANDOM;
int g_out_no_partitions = -1;
bool g_print_load_tfc_debug = true;
int* g_gate_hist = NULL;
int* g_qubit_hist = NULL;
int g_out_T_count = 0;
int g_fail_count = 0;
string g_algebra_prefix;
int g_current_partition = 0;
bool g_remove_pauli_xs = false;

GateStringSparse ReedMullerSynthesis2(const Signature& inS) {
    int n = inS.get_n();
    int N = (int)pow(2,n);
    GateStringSparse out(n);

    /*GateStringSparse daft = GateSigInterface::SigToGSS(inS);
    bool* gss_vec = new bool[N];
    for(int i = 0; i < N; i++) gss_vec[i] = daft.E(i);
    bool* result = new bool[N];
    cout << "Reed decoder begin." << endl;
    clock_t tic = clock();
    LCL_Bool::ReedDecoder(gss_vec,n-4,n,NULL,result);
    clock_t toc = clock();
    cout << "Reed decoder end." << endl;
    double exec_time = ((double)toc-tic)/CLOCKS_PER_SEC;
    cout << "Execution time = " << exec_time << "s" << endl;
    for(int i = 0; i < N; i++) {
        if(result[i]) {
            out.set(i);
        }
    }

    delete [] gss_vec;
    delete [] result;*/

	error("Use of unimplemented function.", "ReedMullerSynthesis2", "TO_Decoder");

    return out;
}

GateStringSparse ReedMullerSynthesis(const Signature& inS) {
    int n = inS.get_n();
    GateStringSparse out(n);

    cout << "Circuit to optimize:" << endl;
    inS.print();
    cout << endl;

    GateStringSparse daft = GateSigInterface::SigToGSS(inS);
    daft.print();
    int weight = daft.weight(true);
    cout << "Daft weight = " << weight << endl;
    out = daft;
    cout << endl;

    vector<GateStringSparse*> myRM = GateStringSparse::ReedMullerGenerators(n-4,n);
    vector<GateStringSparse*> optimalStrings;
    int N = myRM.size();
    double N_total = pow(2,myRM.size());
    int* weightdist = new int[(int)pow(2,n)+1];
    for(int i = 0; i < (pow(2,n)+1); i++) weightdist[i] = 0;
    cout << "Total codewords to search = " << N_total << endl;
    cout << "Search begin..." << endl;
    bool thisBS[N],lastBS[N],addBS[N];
    GateStringSparse thisCodeWord(n);
    clock_t tic = clock();
    clock_t start = clock();
    LCL_Bool::zeros(lastBS,N);
    LCL_Bool::zeros(thisBS,N);
    bool exit = false;
    double i = 0;
    while(!exit) {
        if(((clock()-tic)/(double)CLOCKS_PER_SEC)>=update_time) {
            double perc_done = i/N_total;
            double done_rate = CLOCKS_PER_SEC*perc_done/(double)(clock()-start);
            double time_left = (1-perc_done)/done_rate;
            cout << "Percentage done = " << perc_done*100 << ";\tcompletion rate = " << 100*done_rate << "%ps;\tTime elapsed = " << (clock()-start)/(double)CLOCKS_PER_SEC << "s;\tTime left = " << time_left << "s;\t";
            cout << "Completion rate (iterations) = " << i*(double)CLOCKS_PER_SEC/(double)(clock()-start) << "ps" << endl;
            tic = clock();
        }
        LCL_Bool::BitwiseXor(thisBS,lastBS,addBS,N);

        for(int j = 0; j < N; j++) {
            if(addBS[j]) {
                thisCodeWord = (thisCodeWord + (*myRM[j]));

            }
        }
        GateStringSparse newFunc = (daft+thisCodeWord);
        int newWeight = newFunc.weight(true);
        weightdist[newWeight]++;
        if(newWeight<weight) {
            weight = newWeight;
            out = newFunc;
            for(int i = 0; i < (int)optimalStrings.size(); i++) {
                delete optimalStrings[i];
                optimalStrings[i] = NULL;
            }
            optimalStrings.clear();
        }
        if(newWeight==weight) {
            GateStringSparse* thisOptimal = new GateStringSparse(N);
            thisOptimal->assign(newFunc);
            optimalStrings.push_back(thisOptimal);
        }
        LCL_Bool::copy(thisBS,lastBS,N);
        exit = !LCL_Bool::increment(thisBS,N);
        i++;
    }
    double total_time = (clock()-start)/(double)CLOCKS_PER_SEC;
    cout << "Search done! Total time = " << total_time << "s" << endl << endl;

    cout << "Optimal weight = " << out.weight(true) << endl;
    cout << "Weight reduced by " << (daft.weight(true)-out.weight(true)) << endl;
    cout << endl;
    cout << "Optimal gate strings:" << endl;
    for(int i = 0; i < (int)optimalStrings.size(); i++) {
        cout << "Optimal " << i << ":" << endl;
        GateStringSparse* thisGSS = optimalStrings[i];
        //thisGSS->printString();
        Signature difference(n);
        Signature this_GSS_Sig = GateSigInterface::expandGSSTerm(*thisGSS);
        difference.assign(inS + this_GSS_Sig);
        bool success = difference.isEmpty();
        cout << "Success? " << (success?"Yes":"No") << endl;
        if(!success) difference.print();
    }
    cout << endl;
    cout << "Optimal decompositions:" << endl;
    for(int i = 0; i < (int)optimalStrings.size(); i++) {
        cout << "Optimal " << i << ":" << endl;
        GateStringSparse* thisGSS = optimalStrings[i];
        thisGSS->print();
    }



    cout << endl;

    //cout << "Weight distribution:" << endl;
    //for(int i = 0; i < (pow(2,n)+1); i++) cout << "N(w = " << i << ") = " << weightdist[i] << endl;

    for(int i = 0; i < (int)myRM.size(); i++) {
        delete myRM[i];
        myRM[i] = NULL;
    }
    for(int i = 0; i < (int)optimalStrings.size(); i++) {
        delete optimalStrings[i];
        optimalStrings[i] = NULL;
    }
    delete [] weightdist;
    weightdist = NULL;

    if(false&&(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::REED_MULLER))&&(!g_output_filename.empty())) {
        ofstream my_file(g_output_filename.c_str(),iostream::app);
        my_file << "Input signature" << endl;
        inS.print(my_file);
        my_file << "Output gate string" << endl;
        out.print(my_file);
        //result_analysis(inS,out,my_file);
        my_file << "Total execution time = " << total_time << "s" << endl;
        my_file << endl;
        my_file << "Input signature as file" << endl;
        inS.save(my_file);
        my_file << endl;
        my_file << "Output gate synthesis matrix" << endl;
        BMSparse out_BMS = Interface_BMSGSS::GSSToBMS(out);
        out_BMS.printFull(my_file);
        my_file.close();
    }
    if(false&&(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::REED_MULLER))&&(!g_csv_filename.empty())&&(g_random_circuit_seed!=0)) {
        ofstream csv_out_file(g_csv_filename.c_str(), iostream::app);
        //CSV format: "2, <#qubits>, <seed#>, <T-count>\n"

        csv_out_file << SYNTHESIS_ALGORITHM_ID::REED_MULLER << "," << n << "," << g_random_circuit_seed << "," << out.weight(true) << endl;

        csv_out_file.close();
    }

    return out;
}

/*void result_analysis(const Signature& inS, const GateStringSparse& inResult, ostream& inOS) {
    GateStringSparse daft = GateSigInterface::SigToGSS(inS);
    int daft_weight = daft.weight(true);
    int result_weight = inResult.weight(true);
    //Signature restored_sig = GateSigInterface::expandGSSTerm(inResult);
    Signature restored_sig = Interface_SigBMS::BMSToSig(Interface_BMSGSS::GSSToBMS(inResult));
    bool success = (inS + restored_sig).isEmpty();
    inOS << "Successful? " << (success?"Yes":"No") << endl;
    if(!success) {
        Signature difference = inS + restored_sig;
        cout << "Difference:" << endl;
        difference.print();
    }
    inOS << "T-fast? " << (T_fast(inResult)?"Yes":"No") << endl;
    inOS << "Daft T-count = " << daft_weight << endl;
    inOS << "Output T-count = " << result_weight << endl;
    inOS << "Reduced T-count by " << (daft_weight-result_weight) << endl;
    inOS << endl;
}*/

bool synthesis_success(const WeightedPolynomial& in_pre, const WeightedPolynomial& in_post) {
    Signature pre_sig = TO_Maps::WeightedPolynomial_to_Signature(in_pre);
    Signature post_sig = TO_Maps::WeightedPolynomial_to_Signature(in_post);
    Signature difference = pre_sig + post_sig;
    return difference.isEmpty();
}

bool synthesis_success(const Signature& inS, const GateStringSparse& inResult) {
    bool out;

    Signature restored_sig = GateSigInterface::expandGSSTerm(inResult);
    //Signature restored_sig = Interface_SigBMS::BMSToSig(Interface_BMSGSS::GSSToBMS(inResult));
    Signature difference = inS + restored_sig;
    out = difference.isEmpty();

    return out;
}

bool synthesis_success(const GateStringSparse& pre, const GateStringSparse& post) {
    bool out;

    Signature pre_sig = GateSigInterface::expandGSSTerm(pre);
    Signature post_sig = GateSigInterface::expandGSSTerm(post);

    out = (pre_sig+post_sig).isEmpty();

    return out;
}

bool T_fast(const GateStringSparse& inGSS) {
    bool out;

    int k = inGSS.get_n();
    int T = inGSS.weight(true);
    out = (T<=((double)(k*k + 3*k - 14)/2.0));

    return out;
}

GateStringSparse TODD(const Signature& inS) {
    LOut(); cout << "Gate synthesis begin." << endl;
    clock_t start = clock();
    GateStringSparse A_GSS = GateSigInterface::SigToGSS(inS);
    BMSparse A_BMS = Interface_BMSGSS::GSSToBMS(A_GSS);
    int n = A_BMS.get_n();
    int m = A_BMS.get_m();
    bool** A_bool = LCL_Mat_GF2::construct(n,m);
    A_BMS.toBool(A_bool);
    int t;
    clock_t tic = clock();
    LOut_Pad++;
    if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_X))
        GateSynthesisMatrix::LempelX(A_bool,n,m,t);
    else if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_X_2))
        GateSynthesisMatrix::LempelX2(A_bool,n,m,t);
    else
        GateSynthesisMatrix::LempelX(A_bool,n,m,t);
    clock_t toc = clock();
    double exec_time = ((double)toc-(double)tic)/CLOCKS_PER_SEC;
    BMSparse out_BMS(n,t);
    out_BMS.fromBool(A_bool,n,t);
    GateStringSparse out = Interface_BMSGSS::BMSToGSS(out_BMS);
    LCL_Mat_GF2::destruct(A_bool,n,m);
    LOut() << "TODD executed in " << exec_time << " seconds." << endl;
    clock_t finish = clock();
    LOut() << "Total time: " << secs(start,finish) << "s" << endl;
    LOut_Pad--;
    LOut() << "Gate synthesis end." << endl;
    if(false&&!g_csv_filename.empty()) {
        ofstream my_csv(g_csv_filename.c_str(),iostream::app);
        int n = inS.get_n();
        my_csv << SYNTHESIS_ALGORITHM_ID::DAFT_GUESS << "," << n << "," << g_random_circuit_seed << "," << A_GSS.weight(true) << endl;
        my_csv << SYNTHESIS_ALGORITHM_ID::LEMPEL_X << "," << n << "," << g_random_circuit_seed << "," << out.weight(true) << endl;
        my_csv.close();
    }
	//result_analysis(inS, out);
    return out;
}

GateStringSparse TODD(const GateStringSparse& inGSM) {
    LOut(); cout << "Gate synthesis begin." << endl;

    clock_t start = clock();

    BMSparse A_BMS = Interface_BMSGSS::GSSToBMS(inGSM);

    int n = A_BMS.get_n();
    int m = A_BMS.get_m();

    bool** A_bool = LCL_Mat_GF2::construct(n,m);
    A_BMS.toBool(A_bool);

    int t;
    clock_t tic = clock();

    LOut_Pad++;

    if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_X))
        GateSynthesisMatrix::LempelX(A_bool,n,m,t);
    else if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_X_2))
        GateSynthesisMatrix::LempelX2(A_bool,n,m,t);
    else
        GateSynthesisMatrix::LempelX(A_bool,n,m,t);

    clock_t toc = clock();
    double exec_time = ((double)toc-(double)tic)/CLOCKS_PER_SEC;

    BMSparse out_BMS(n,t);
    out_BMS.fromBool(A_bool,n,t);

    GateStringSparse out = Interface_BMSGSS::BMSToGSS(out_BMS);

    LCL_Mat_GF2::destruct(A_bool,n,m);

    LOut() << "TODD executed in " << exec_time << " seconds." << endl;
    clock_t finish = clock();
    LOut() << "Total time: " << secs(start,finish) << "s" << endl;
    LOut_Pad--;
    LOut() << "Gate synthesis end." << endl;
    return out;
}

PhasePolynomial FullDecoderWrapper(const PhasePolynomial& in, TO_Decoder decoder) {
    int n = in.get_n();
    PhasePolynomial out(n);

    WeightedPolynomial f = TO_Maps::PhasePolynomial_to_WeightedPolynomial(in);

    WeightedPolynomial f_prime(f);

    WeightedPolynomial two_f_tilde(f);
    f_prime %= 2;
    two_f_tilde -= f_prime;

    Signature f_prime_sig = TO_Maps::WeightedPolynomial_to_Signature(f);

    GateStringSparse f_prime_GSS_optimized = decoder(f_prime_sig);

    PhasePolynomial g = TO_Maps::GateStringSparse_to_PhasePolynomial(f_prime_GSS_optimized);

    WeightedPolynomial f_post = TO_Maps::PhasePolynomial_to_WeightedPolynomial(g);

    g_fail_count += (!synthesis_success(f,f_post));

    out = g;

    out += TO_Maps::WeightedPolynomial_to_PhasePolynomial(two_f_tilde);

    WeightedPolynomial two_g_tilde = TO_Maps::PhasePolynomial_to_WeightedPolynomial(g);

    two_g_tilde -= f_prime;

    out -= TO_Maps::WeightedPolynomial_to_PhasePolynomial(two_g_tilde);

    out %= 8;

    return out;
}

PhasePolynomial TODDWrapper(const PhasePolynomial& in) {
	// Can output PP

    int n = in.get_n();
    PhasePolynomial out(n);

    WeightedPolynomial f = TO_Maps::PhasePolynomial_to_WeightedPolynomial(in);
	// Can output WP

    WeightedPolynomial f_prime(f);

    WeightedPolynomial two_f_tilde(f);
    f_prime %= 2;
    two_f_tilde -= f_prime;

    Signature f_prime_sig = TO_Maps::WeightedPolynomial_to_Signature(f);
	// Can output ST

	GateStringSparse in_gsm = TO_Maps::PhasePolynomial_to_GateStringSparse(in);
	// Can output GSM

    GateStringSparse f_prime_GSS_optimized = TODD(in_gsm);

    PhasePolynomial g = TO_Maps::GateStringSparse_to_PhasePolynomial(f_prime_GSS_optimized);

    WeightedPolynomial f_post = TO_Maps::PhasePolynomial_to_WeightedPolynomial(g);

    g_fail_count += (!synthesis_success(f,f_post));

    out = g;

    out += TO_Maps::WeightedPolynomial_to_PhasePolynomial(two_f_tilde);

    WeightedPolynomial two_g_tilde = TO_Maps::PhasePolynomial_to_WeightedPolynomial(g);

    two_g_tilde -= f_prime;

    out -= TO_Maps::WeightedPolynomial_to_PhasePolynomial(two_g_tilde);

    out %= 8;

    return out;
}

Matrix RemoveRoundingError(const Matrix& in) {
    Matrix out(in);

    for(int i = 0; i < out.getRows(); i++) {
        for(int j = 0; j < out.getCols(); j++) {
            double this_r = out.E(i,j).getReal();
            double this_c = out.E(i,j).getImaginary();
            if(fabs(this_r)<g_matrix_precision) this_r = 0.0;
            else if(fabs(this_r-1)<g_matrix_precision) this_r = 1.0;
            if(this_c<g_matrix_precision) this_c = 0.0;
            else if(fabs(this_c-1)<g_matrix_precision) this_c = 1.0;
            out.E(i,j,this_r,this_c);
        }
    }

    return out;
}

GateStringSparse TOOL(const Signature& inS, int maxRM, LempelSelector lempelSelector, bool feedback) {
    cout << "Gate synthesis begin." << endl;
    int n = inS.get_n();
    GateStringSparse out(inS.get_n());

    Signature thisSig;
    thisSig.assign(inS);

    int n_eff = thisSig.get_N_eff();

    cout << "TOOL Optimization begin." << endl;
    clock_t start_tic = clock();
    g_h_order_stream.str("");
    while(n_eff>maxRM) {
        int h = lempelSelector(thisSig);
        string stream_contents = g_h_order_stream.str();
        if(!stream_contents.empty()) g_h_order_stream << ", ";
        g_h_order_stream << h;
        clock_t toc = clock();
        double perc_done = 100.0*(double)(n-n_eff)/(n-maxRM);
        double time_remaining = (100.0-perc_done)*perc_done/((double)(toc-start_tic)/CLOCKS_PER_SEC);
        Signature this_alpha, this_Ap, this_B;
        thisSig.h_decomposition(h,this_alpha,this_Ap,this_B);
        BMSparse this_Ap_BMS = Interface_SigBMS::SigToBMS(this_Ap);
        BMSparse minfac = this_Ap_BMS.minimalFactor();
        GateStringSparse this_Ap_GSS = Interface_BMSGSS::BMSToGSS(minfac);
        GateStringSparse this_out(n);
        if(feedback) {
            if(!this_Ap.isEmpty()) {
                if(this_Ap_GSS.weight(true)%2) {
                    bool* this_xh = new bool[n];
                    for(int i = 0; i < n; i++) this_xh[i] = (i==(h-1));
                    this_out.set(this_xh);
                    delete [] this_xh;
                }
                this_out = (this_out + this_Ap_GSS.addxh(h) + GateSigInterface::SigToGSS(this_alpha));
            } else {
                this_out = GateSigInterface::SigToGSS(this_alpha);
            }
            thisSig = (GateSigInterface::expandGSSTerm(this_Ap_GSS) + this_B);
        } else {
            this_out = (this_Ap_GSS.mult2xh(h) + GateSigInterface::SigToGSS(this_alpha));
            thisSig = this_B;
        }
        out = (out + this_out);
        n_eff = thisSig.get_N_eff();
    }
    cout << "TOOL Optimization end." << endl << endl;

    bool* rm_inc_vec = new bool[n];
    for(int i = 0; i < n; i++) rm_inc_vec[i] = (bool)thisSig.h_weight(i+1);

    Signature rm_sig = thisSig.subsig(rm_inc_vec);

    cout << "Reed-Muller synthesis begin." << endl << endl;
    GateStringSparse rm_GSS = ReedMullerSynthesis(rm_sig);
    cout << "Reed-Muller synthesis end." << endl << endl;

    GateStringSparse restored_rm_GSS = rm_GSS.superGSS(rm_inc_vec,n);

    out = (out + restored_rm_GSS);

    cout << "Gate synthesis end." << endl << endl;

    delete [] rm_inc_vec;
    return out;
}

int LempelSelector_LeastGreedy(const Signature& inS) {
    // Not yet tested.
    int out = 0;

    int n = inS.get_n();
    int* h_weights = new int[n];
    int* h_order = new int[n];

    for(int i = 0; i < n; i++) {
        h_weights[i] = inS.h_weight(i+1);
    }

    LCL_Int::sort(h_weights,n,false,h_order);

    //LCL_Int::print(h_weights,n,"h_weights: ");
    //LCL_Int::print(h_order,n,"h_order: ");

    bool exit = false;
    for(int i = 0; (!exit)&&(i < n); i++) {
        if(h_weights[i]) {
            out = h_order[i];
            exit = true;
        }
    }

    delete [] h_order;
    delete [] h_weights;

    return (out+1);// Returns qubit number - not array index.
}

int LempelSelector_Greedy(const Signature& inS) {
    // Not yet tested.
    int out = 0;

    int n = inS.get_n();
    int* h_weights = new int[n];
    int* h_order = new int[n];

    for(int i = 0; i < n; i++) {
        h_weights[i] = inS.h_weight(i+1);
    }

    LCL_Int::sort(h_weights,n,true,h_order);

    out = h_order[0];

    delete [] h_order;
    delete [] h_weights;

    return (out+1);// Returns qubit number - not array index.
}

int LempelSelector_Random(const Signature& inS) {
    // Not yet tested.
    int out = 0;

    int n = inS.get_n();
    int* h_weights = new int[n];
    int* h_order = new int[n];

    for(int i = 0; i < n; i++) {
        h_weights[i] = inS.h_weight(i+1);
    }

    LCL_Int::sort(h_weights,n,true,h_order);

    int n_eff = inS.get_N_eff();

    out = h_order[LCL_Int::randi(0,n_eff-1)];

    delete [] h_order;
    delete [] h_weights;

    return (out+1);// Returns qubit number - not array index.
}

int trapezoid_width(const BMSparse& inBMS, int height) {
    int out = 0;

    int end_zeros;
    bool exit = false;
    do {
        end_zeros = 0;
        for(int i = height; i < inBMS.get_n(); i++) {
            if(!inBMS.E(i,out)) end_zeros++;
        }
        if(end_zeros>=(inBMS.get_n()-height)) {
            out++;
        } else {
            exit = true;
        }
    } while(!exit);


    return out;
}

GateStringSparse RE(const Signature& inS) {
    return GateSigInterface::SigToGSS(inS);
}

GateStringSparse TOOL_F_G(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_Greedy,true);
}

GateStringSparse TOOL_WF_G(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_Greedy,false);
}
GateStringSparse TOOL_F_LG(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_LeastGreedy,true);
}

GateStringSparse TOOL_WF_LG(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_LeastGreedy,false);
}
GateStringSparse TOOL_F_R(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_Random,true);
}

GateStringSparse TOOL_WF_R(const Signature &inS) {
    return TOOL(inS,g_Reed_Muller_max,LempelSelector_Random,false);
}
