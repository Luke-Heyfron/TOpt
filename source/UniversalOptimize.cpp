#include "SQC_Circuit.h"
#include "TO_Maps.h"

#include <iostream>
using namespace std;

#include <ctime>
#include <string>
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;
#include "TO_Decoder.h"

SQC_Circuit SQC_Circuit::UniversalOptimize(const SQC_Circuit& in, TO_Decoder decoder) {
	g_out_T_count = 0;
	g_fail_count = 0;
    LOut() << "UniversalOptimize begin." << endl;
    LOut_Pad++;
    clock_t start = clock();
    clock_t tic, toc;
    double accounted_time = 0.0;

    //LOut() << "Step 1 begin: Convert Y to XZ." << endl;
    tic = clock();
    SQC_Circuit step1 = SQC_Circuit::convert_Y_to_XZ(in);
    toc = clock();
    //LOut() << "Step 1 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    //LOut() << "Step 2 begin: Expand Toffoli-n." << endl;
    tic = clock();
    SQC_Circuit step2 = SQC_Circuit::expand_Toffn_to_Toff3(step1);
    toc = clock();
    //LOut() << "Step 2 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    //LOut() << "Step 3 begin: Convert X to HZH etc." << endl;
    tic = clock();
    SQC_Circuit step3 = SQC_Circuit::convert_Xs(step2);
    toc = clock();
    //LOut() << "Step 3 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    //LOut() << "Step 4 begin: Convert CS to {CNOT, T} etc." << endl;
    tic = clock();
    SQC_Circuit step4 = SQC_Circuit::convert_Cliff3s(step3);
    toc = clock();
    //LOut() << "Step 4 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);


    //LOut() << "Step 5 begin: Convert T* to ZST etc." << endl;
    tic = clock();
    SQC_Circuit step5 = SQC_Circuit::convert_Daggers(step4);
    toc = clock();
    //LOut() << "Step 5 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    //LOut() << "Step 6 begin: Cancel trivial gates." << endl;
    tic = clock();
    step5.simplify();
    toc = clock();
    //LOut() << "Step 6 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    //LOut() << "Step 7 begin: Hadamard partitions." << endl;
    tic = clock();
    SQC_Circuit** step6_Hs = NULL;
    SQC_Circuit** step6_Ps = NULL;
    int step6_N_Hs, step6_N_Ps;
    SQC_Circuit::decompose_into_Hadamard_partitions(step5, step6_Hs, step6_N_Hs, step6_Ps, step6_N_Ps);
    toc = clock();

    /*cout << "Pre partition" << endl;
    step5.Print();
    step5.Save("step5.log");
    cout << endl;*/

    //LOut() << "Number of partitions = " << step6_N_Ps << endl;
    /*for(int i = 0; i < step6_N_Hs; i++) {
        cout << "H-part " << i << endl;
        step6_Hs[i]->Print();
        cout << endl;
    }*/

    //LOut() << "Number of Hadamard partitions = " << step6_N_Hs << endl;
    /*for(int i = 0; i < step6_N_Ps; i++) {
        cout << "D3-part " << i << endl;
        step6_Ps[i]->Print();
        cout << endl;
    }*/

    //LOut() << "Step 7 end. Executed in " << secs(tic,toc) << "s" << endl;
    accounted_time += secs(tic,toc);

    double step8_time = 0.0;
    double step9_time = 0.0;
    double step10_time = 0.0;

    g_out_no_partitions = step6_N_Ps;

    SQC_Circuit** step10s = new SQC_Circuit*[step6_N_Ps];
    int final_n = in.n;
	int final_p_hads = 0;
    for(int p = 0; p < step6_N_Ps; p++) {
		g_current_partition = p;
        SQC_Circuit* this_part = step6_Ps[p];
        SQC_Circuit* this_L = NULL;
        SQC_Circuit* this_Pp = NULL;
        SQC_Circuit* this_R = NULL;

        tic = clock();
        SQC_Circuit::Hadamards_to_Gadgets(*this_part,this_L,this_Pp,this_R);
        toc = clock();
        step8_time += secs(tic,toc);

		if(this_Pp->p_hads>final_p_hads) final_p_hads = this_Pp->p_hads;

        if(this_Pp->n>final_n) final_n = this_Pp->n;
        SQC_Circuit* this_CNOT = NULL;
        SQC_Circuit* this_D3 = NULL;

        tic = clock();
        SQC_Circuit::decompose_C3_to_CNOT_D3(*this_Pp, this_CNOT, this_D3);
        toc = clock();
        step9_time += secs(tic,toc);

        tic = clock();
        SQC_Circuit step10 = g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::NONE)?(g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TODD)?SQC_Circuit::optimize_D3(*this_D3, decoder):SQC_Circuit::TODD_optimize_D3(*this_D3)):SQC_Circuit(*this_D3);
        // After implementing new version of optimize_D3, update final_n according to site of step10.n
        //cout << "HGJAEEFFGFGH" << endl;

		step10.simplify();

		if(!g_algebra_prefix.empty()) {
			char temp_numstr[10];
			sprintf(temp_numstr,"%d",(p+1));
			string temp_filename = g_algebra_prefix + string(temp_numstr);
			SaveRepresentationsToFile(step10, temp_filename.c_str());
		}

        toc = clock();
        step10_time += secs(tic,toc);

        step10s[p] = new SQC_Circuit(this_Pp->n,this_Pp->d,this_Pp->p_hads); // step10->n instead of this_Pp->n
        for(int i = 0; i < this_L->m; i++) step10s[p]->AddOperator(this_L->operator_list[i],this_L->n+1);
        for(int i = 0; i < step10.m; i++) step10s[p]->AddOperator(step10.operator_list[i]);
        for(int i = 0; i < this_CNOT->m; i++) step10s[p]->AddOperator(this_CNOT->operator_list[i]);
        for(int i = 0; i < this_R->m; i++) step10s[p]->AddOperator(this_R->operator_list[i],this_R->n+1);


    }
    SQC_Circuit out(final_n, in.d, final_p_hads);

    for(int p = 0; p < step6_N_Ps; p++) {
        for(int i = 0; i < step6_Hs[p]->m; i++) out.AddOperator(step6_Hs[p]->operator_list[i],step6_Hs[p]->n+1);
        for(int i = 0; i < step10s[p]->m; i++) out.AddOperator(step10s[p]->operator_list[i],step10s[p]->n+1);
    }
    for(int i = 0; i < step6_Hs[step6_N_Hs-1]->m; i++) out.AddOperator(step6_Hs[step6_N_Hs-1]->operator_list[i],step6_Hs[step6_N_Hs-1]->n+1);

    clock_t finish = clock();
    /*
    LOut() << "Step 8: Hadamard gadgets. Executed in " << step8_time << "s" << endl;
    LOut() << "Step 9: Diagonalise {CNOT, T}. Executed in " << step9_time << "s" << endl;
    LOut() << "Step 10: Optimize. Executed in " << step10_time << "s" << endl;
    */
    accounted_time += step8_time;
    accounted_time += step9_time;
    accounted_time += step10_time;

    LOut() << "Total time: " << secs(start,finish) << "s" << endl;

    LOut_Pad--;

    LOut() << "UniversalOptimize end." << endl;
    return out;
}

SQC_Circuit SQC_Circuit::convert_Y_to_XZ(const SQC_Circuit& in) {
    int n = in.n;
    SQC_Circuit out(n,in.d);

    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_Y:
                {
                    int this_X[] = {SQC_OPERATOR_X,this_op[1]};
                    int this_Z[] = {SQC_OPERATOR_Z,this_op[1]};
                    out.AddOperator(this_X,2);
                    out.AddOperator(this_Z,2);
                }
                break;
            default:
                out.AddOperator(this_op);
                break;
        }
    }

    return out;
}

SQC_Circuit SQC_Circuit::expand_Toffn_to_Toff3(const SQC_Circuit& in) {
    int n_in = in.n;

    int n_Toff = 0;
    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        if(this_op[0]==SQC_OPERATOR_TOFFOLI_N) {
            int this_nargs = in.GetNArgs(t);
            if((this_nargs-3)>n_Toff) n_Toff = (this_nargs-3);
        }
    }

    SQC_Circuit out(n_in + n_Toff,in.d);

    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        if(this_op[0]==SQC_OPERATOR_TOFFOLI_N) {
            int this_n_Toff = in.GetNArgs(t);
            if(this_n_Toff>3) {
                {
                    int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, (n_in+1), this_op[2], this_op[3]};
                    out.AddOperator(this_Toff_3,4);
                }
                for(int i = 2; i <= (this_n_Toff-3); i++) {
                    int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, n_in+i, n_in+i-1, this_op[i+2]};
                    out.AddOperator(this_Toff_3,4);
                }
                {
                    int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, this_op[1], n_in+this_n_Toff-3, this_op[this_n_Toff]};
                    out.AddOperator(this_Toff_3,4);
                }
                for(int i = (this_n_Toff-3); i >= 2; i--) {
                    int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, n_in+i, n_in+i-1, this_op[i+2]};
                    out.AddOperator(this_Toff_3,4);
                }
                {
                    int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, (n_in+1), this_op[2], this_op[3]};
                    out.AddOperator(this_Toff_3,4);
                }
            } else if(this_n_Toff==3) {
                int this_Toff_3[] = {SQC_OPERATOR_TOFFOLI, this_op[1], this_op[2], this_op[3]};
                out.AddOperator(this_Toff_3,4);
            } else if(this_n_Toff==2) {
                int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[1], this_op[2]};
                out.AddOperator(this_CNOT,3);
            } else if(this_n_Toff==1) {
                int this_X[] = {SQC_OPERATOR_X, this_op[1]};
                out.AddOperator(this_X,2);
            }
        } else {
            out.AddOperator(this_op,n_in+1);
        }
    }

    return out;
}

SQC_Circuit SQC_Circuit::convert_Xs(const SQC_Circuit& in) {
    int n = in.n;
    SQC_Circuit out(n,in.n);

    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_X:
                {
                    int this_H[] = {SQC_OPERATOR_HADAMARD, this_op[1]};
                    int this_Z[] = {SQC_OPERATOR_Z, this_op[1]};
                    out.AddOperator(this_H,2);
                    out.AddOperator(this_Z,2);
                    out.AddOperator(this_H,2);
                }
                break;
            case SQC_OPERATOR_TOFFOLI:
                {
                    int this_H[] = {SQC_OPERATOR_HADAMARD, this_op[1]};
                    int this_CCZ[] = {SQC_OPERATOR_CCZ, this_op[1], this_op[2], this_op[3]};
                    out.AddOperator(this_H,2);
                    out.AddOperator(this_CCZ,4);
                    out.AddOperator(this_H,2);
                }
                break;
            default:
                {
                    out.AddOperator(this_op);
                }
                break;
        }
    }

    return out;
}

SQC_Circuit SQC_Circuit::convert_Cliff3s(const SQC_Circuit& in) {
    int n = in.n;
    SQC_Circuit out(n,in.d);

    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_CZ:
                {
                    {
                        int this_S[] = {SQC_OPERATOR_S,this_op[1]};
                        out.AddOperator(this_S,2);
                    }
                    {
                        int this_S[] = {SQC_OPERATOR_S,this_op[2]};
                        out.AddOperator(this_S,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_S_dagger[] = {SQC_OPERATOR_S_DAG,this_op[1]};
                        out.AddOperator(this_S_dagger,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                }
                break;
            case SQC_OPERATOR_CS:
                {
                    {
                        int this_T[] = {SQC_OPERATOR_T,this_op[1]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T,this_op[2]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T_dagger[] = {SQC_OPERATOR_T_DAG,this_op[1]};
                        out.AddOperator(this_T_dagger,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                }
                break;
            case SQC_OPERATOR_CS_DAG:
                {
                    {
                        int this_T_dag[] = {SQC_OPERATOR_T_DAG,this_op[1]};
                        out.AddOperator(this_T_dag,2);
                    }
                    {
                        int this_T_dag[] = {SQC_OPERATOR_T_DAG,this_op[2]};
                        out.AddOperator(this_T_dag,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T,this_op[1]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT,this_op[1],this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                }
                break;
            case SQC_OPERATOR_CCZ:
                {
                    {
                        int this_T[] = {SQC_OPERATOR_T, this_op[1]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T, this_op[2]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T, this_op[3]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[2], this_op[1]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T_dag[] = {SQC_OPERATOR_T_DAG, this_op[2]};
                        out.AddOperator(this_T_dag,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[3], this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T, this_op[3]};
                        out.AddOperator(this_T,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[3], this_op[1]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T_dag[] = {SQC_OPERATOR_T_DAG, this_op[3]};
                        out.AddOperator(this_T_dag,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[3], this_op[2]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_T_dag[] = {SQC_OPERATOR_T_DAG, this_op[3]};
                        out.AddOperator(this_T_dag,2);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[3], this_op[1]};
                        out.AddOperator(this_CNOT,3);
                    }
                    {
                        int this_CNOT[] = {SQC_OPERATOR_CNOT, this_op[2], this_op[1]};
                        out.AddOperator(this_CNOT,3);
                    }
                }
                break;
            default:
                {
                    out.AddOperator(this_op);
                }
                break;

        }
    }

    return out;
}

SQC_Circuit SQC_Circuit::convert_Daggers(const SQC_Circuit& in) {
    int n = in.n;
    SQC_Circuit out(n,in.d);

    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_T_DAG:
                {
                    {
                        int this_Z[] = {SQC_OPERATOR_Z, this_op[1]};
                        out.AddOperator(this_Z,2);
                    }
                    {
                        int this_S[] = {SQC_OPERATOR_S, this_op[1]};
                        out.AddOperator(this_S,2);
                    }
                    {
                        int this_T[] = {SQC_OPERATOR_T, this_op[1]};
                        out.AddOperator(this_T,2);
                    }
                }
                break;
            case SQC_OPERATOR_S_DAG:
                {
                    {
                        int this_Z[] = {SQC_OPERATOR_Z, this_op[1]};
                        out.AddOperator(this_Z,2);
                    }
                    {
                        int this_S[] = {SQC_OPERATOR_S, this_op[1]};
                        out.AddOperator(this_S,2);
                    }
                }
                break;
            default:
                {
                    out.AddOperator(this_op);
                }
                break;
        }
    }

    return out;
}

void SQC_Circuit::simplify() {
    // Assumes gate set {Z,S,T,H,CNOT}
    bool changed = 1;
    while(changed) {
        changed = 0;
        for(int t1 = 0; (!changed)&&(t1 < (m-1)); t1++) {
            int* this_op1 = operator_list[t1];
            bool blocked = 0;
            for(int t2 = (t1+1); (!changed)&&(!blocked)&(t2 < m); t2++) {
                int* this_op2 = operator_list[t2];
                // Check cancellations
                if(!changed) {
                    bool cancel_check = 0;
                    bool cancel_check_single = (this_op1[0]==SQC_OPERATOR_Z)&&(this_op2[0]==SQC_OPERATOR_Z);
                    cancel_check_single += (this_op1[0]==SQC_OPERATOR_HADAMARD)&&(this_op2[0]==SQC_OPERATOR_HADAMARD);
                    cancel_check_single *= (this_op1[1]==this_op2[1]);
                    bool cancel_check_CNOT = (this_op1[0]==SQC_OPERATOR_CNOT)&&(this_op2[0]==SQC_OPERATOR_CNOT);
                    cancel_check_CNOT *= (this_op1[1]==this_op2[1])&&(this_op1[2]==this_op2[2]);
                    cancel_check = cancel_check_single||cancel_check_CNOT;
                    if(cancel_check) {
                        DeleteOperator(t2);
                        DeleteOperator(t1);
                        changed = 1;
                    }
                }
                // Check blockages
                {
                    switch(this_op1[0]) {
                        case SQC_OPERATOR_Z:
                        case SQC_OPERATOR_S:
                        case SQC_OPERATOR_T:
                            {
                                switch(this_op2[0]) {
                                    case SQC_OPERATOR_HADAMARD:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        break;
                                    case SQC_OPERATOR_CNOT:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        break;
                                }
                            }
                            break;
                        case SQC_OPERATOR_CNOT:
                            {
                                switch(this_op2[0]) {
                                    case SQC_OPERATOR_Z:
                                    case SQC_OPERATOR_S:
                                    case SQC_OPERATOR_T:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        break;
                                    case SQC_OPERATOR_HADAMARD:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        blocked += (this_op1[2]==this_op2[1]);
                                        break;
                                    case SQC_OPERATOR_CNOT:
                                        blocked += (this_op1[1]==this_op2[2]);
                                        blocked += (this_op1[2]==this_op2[1]);
                                        break;
                                }
                            }
                            break;
                        case SQC_OPERATOR_HADAMARD:
                            {
                                switch(this_op2[0]) {
                                    case SQC_OPERATOR_Z:
                                    case SQC_OPERATOR_S:
                                    case SQC_OPERATOR_T:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        break;
                                    case SQC_OPERATOR_CNOT:
                                        blocked += (this_op1[1]==this_op2[1]);
                                        blocked += (this_op1[1]==this_op2[2]);
                                        break;
                                }
                            }
                        default:
                            {

                            }
                            break;
                    }
                }
            }
        }
    }
}

void SQC_Circuit::decompose_into_Hadamard_partitions(const SQC_Circuit& in, SQC_Circuit**& inHs, int& N_Hs, SQC_Circuit**& inPs, int& N_Ps) {
    int n = in.n;
    int N_H = 0;
    N_Hs = 0;
    N_Ps = 0;
    // Calculate number of Hadamards
    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        if(this_op[0]==SQC_OPERATOR_HADAMARD) N_H++;
    }
    // Calculate maximum number of D3 partitions
    int max_N_P = 0;
    if(g_Hadamard_ancillas>0) max_N_P = ceil((double)N_H/(double)g_Hadamard_ancillas);
    else if(g_Hadamard_ancillas==0) max_N_P = (N_H-1);
    if(!max_N_P) max_N_P = 1;

    // Allocate arrays to store the two types of partitions
    inPs = new SQC_Circuit*[max_N_P];
    for(int i = 0; i < max_N_P; i++) inPs[i] = NULL;
    inHs = new SQC_Circuit*[max_N_P+1];
    for(int i = 0; i < (max_N_P+1); i++) inHs[i] = NULL;

    // Copy the input circuit to this_C, which is the "remaining gates" partition
    SQC_Circuit this_C(in);

    // Each Hadamard partitions is a H-then-D3 pair. We need to know if a final H partition is needed so we track it with Boolean variable final_H.
    bool final_H = 1;

    // Loop until no gates are left in "remaining gates" partition
    while(this_C.m>0) {

        // Strip away the left-most Hadamards from "remaining gates" and put into next H-partition
        if(this_C.m>0) {
            inHs[N_Hs] = new SQC_Circuit(n,in.d);

            bool exit = 0;

            // For each qubit line, we need to track whether the left-to-right search for Hadamards is blocked by a CNOT or T^n gate. To do this we store whether we have found such a gate in q_blocked.
            bool q_blocked[n];
            for(int i = 0; i < n; i++) q_blocked[i] = 0;

            // Search left-to-right through circuit and exit if we reach then end or if all qubit lines are blocked
            for(int t = 0; (!exit)&&(t < this_C.m); t++) {

                int* this_op = this_C.operator_list[t];
                switch(this_op[0]) {
                    // If we find a Hadamard, check whether the qubit line it acts upon has `seen' a CNOT or T^k gate. If not, add the H gate to the next H-part.
                    case SQC_OPERATOR_HADAMARD:
                        {
                            if(!q_blocked[this_op[1]-1]) {
                                inHs[N_Hs]->AddOperator(this_op);
                                this_C.DeleteOperator(t);
                                t--;
                            }
                        }
                        break;
                    // If we find a CNOT or T^k gate, block the lines it acts upon.
                    case SQC_OPERATOR_Z:
                    case SQC_OPERATOR_S:
                    case SQC_OPERATOR_T:
                        {
                            q_blocked[this_op[1]-1] = 1;
                        }
                        break;
                    case SQC_OPERATOR_CNOT:
                        {
                            q_blocked[this_op[1]-1] = 1;
                            q_blocked[this_op[2]-1] = 1;
                        }
                        break;
                }
                exit = 1;
                for(int i = 0; i < n; i++) {
                    exit *= q_blocked[i];
                }
            }
            N_Hs++;
        }
        // Find next D3 partition. Similar process as for H-parts but (roughly) swapped roles for H's and {CNOT,T^k}'s.
        if(this_C.m>0) {
            if(g_Hadamard_ancillas==-1) {
                inPs[N_Ps] = new SQC_Circuit(this_C);

                while(this_C.m>0) {
                    this_C.DeleteOperator(this_C.m-1);
                }
                N_Ps++;
            } else {
                inPs[N_Ps] = new SQC_Circuit(n,in.d);
                int this_N_H = 0;
                while((this_N_H<g_Hadamard_ancillas)&&(0<this_C.m)) {
                    int* this_op = this_C.operator_list[0];
                    if(this_op[0]==SQC_OPERATOR_HADAMARD) {
                        this_N_H++;
                    }
                    inPs[N_Ps]->AddOperator(this_op);
                    this_C.DeleteOperator(0);
                }
                bool exit = 0;
                bool seen_hadamard[n],seen_D3[n],seen_CNOT_target[n], seen_CNOT_control[n];
                for(int i = 0; i < n; i++) seen_hadamard[i] = seen_D3[i] = seen_CNOT_target[i] = seen_CNOT_control[i] = 0;
                for(int t = 0; (!exit)&&(t < this_C.m); t++) {
                    int* this_op = this_C.operator_list[t];
                    switch(this_op[0]) {
                        case SQC_OPERATOR_HADAMARD:
                            {
                                seen_hadamard[this_op[1]-1] = 1;

                            }
                            break;
                        case SQC_OPERATOR_Z:
                        case SQC_OPERATOR_S:
                        case SQC_OPERATOR_T:
                            {
                                if(!(seen_hadamard[this_op[1]-1]||seen_CNOT_target[this_op[1]-1])) {
                                    inPs[N_Ps]->AddOperator(this_op);
                                    this_C.DeleteOperator(t);
                                    t--;
                                } else {
                                    // We only `see' gates if we fail to move them to a partition.
                                    seen_D3[this_op[1]-1] = 1;
                                }
                            }
                            break;
                        case SQC_OPERATOR_CNOT:
                            {
                                if(!(seen_hadamard[this_op[1]-1]||seen_hadamard[this_op[2]-1]||seen_CNOT_control[this_op[1]-1]||seen_CNOT_target[this_op[2]-1]||seen_D3[this_op[1]-1])) {
                                    inPs[N_Ps]->AddOperator(this_op);
                                    this_C.DeleteOperator(t);
                                    t--;
                                } else {
                                    seen_CNOT_target[this_op[1]-1] = 1;
                                    seen_CNOT_control[this_op[2]-1] = 1;
                                }
                            }
                            break;
                    }
                    exit = 1;
                    for(int i = 0; i < n; i++) {
                        exit *= (seen_hadamard[i])||((seen_D3[i]||seen_CNOT_control)*seen_CNOT_target[i]);
                    }
                }
                N_Ps++;
            }
        } else {
            final_H = 0;
        }
    }
    // Find final Hadamard partition
    if(final_H) {
        inHs[N_Hs] = new SQC_Circuit(n,in.d);
        bool exit = 0;
        bool q_blocked[n];
        for(int i = 0; i < n; i++) q_blocked[i] = 0;
        for(int t = (inPs[N_Ps-1]->m-1); (!exit)&&(t >= 0); t--) {
            int* this_op = inPs[N_Ps-1]->operator_list[t];
            switch(this_op[0]) {
                case SQC_OPERATOR_HADAMARD:
                    {
                        if(!q_blocked[this_op[1]-1]) {
                            inHs[N_Hs]->AddOperator(this_op);
                            inPs[N_Ps-1]->DeleteOperator(t);
                        }
                    }
                    break;
                case SQC_OPERATOR_Z:
                case SQC_OPERATOR_S:
                case SQC_OPERATOR_T:
                    {
                        q_blocked[this_op[1]-1] = 1;
                    }
                    break;
                case SQC_OPERATOR_CNOT:
                    {
                        q_blocked[this_op[1]-1] = 1;
                        q_blocked[this_op[2]-1] = 1;
                    }
                    break;
            }
            exit = 1;
            for(int i = 0; i < n; i++) {
                exit *= q_blocked[i];
            }
        }
        N_Hs++;
    }
}

void SQC_Circuit::Hadamards_to_Gadgets(const SQC_Circuit& in, SQC_Circuit*& outL, SQC_Circuit*& outPp, SQC_Circuit*& outR) {
    // Count hadamards
    int n = in.n;
    int N_H = 0;
    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        if(this_op[0] == SQC_OPERATOR_HADAMARD) N_H++;
    }
    outL = new SQC_Circuit(n+N_H,in.d);
    outPp = new SQC_Circuit(n+N_H,in.d);
    outR = new SQC_Circuit(n+N_H,in.d);
    outL->p_hads = N_H;
    outPp->p_hads = N_H;
    outR->p_hads = N_H;
    int this_H = 0;
    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        switch(this_op[0]) {
            case SQC_OPERATOR_HADAMARD:
                {
                    int had_index = n + 1 + this_H;

                    // Add Hadamard to outL
                    {
                        int this_g[] = {SQC_OPERATOR_HADAMARD, had_index};
                        outL->AddOperator(this_g,2);
                    }
                    // Add CZ to Pp
                    {
                        int this_g[] = {SQC_OPERATOR_S, this_op[1]};
                        outPp->AddOperator(this_g,2);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_S, had_index};
                        outPp->AddOperator(this_g,2);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_CNOT, had_index, this_op[1]};
                        outPp->AddOperator(this_g,3);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_S_DAG, had_index};
                        outPp->AddOperator(this_g,2);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_CNOT, this_op[1], had_index};
                        outPp->AddOperator(this_g,3);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_CNOT, had_index, this_op[1]};
                        outPp->AddOperator(this_g,3);
                    }
                    // Add Hadamard and post-selection to outR
                    {
                        int this_g[] = {SQC_OPERATOR_HADAMARD, had_index};
                        outR->AddOperator(this_g,2);
                    }
                    {
                        int this_g[] = {SQC_OPERATOR_POST_0, had_index};
                        outR->AddOperator(this_g,2);
                    }
                    this_H++;
                }
                break;
            case SQC_OPERATOR_Z:
            case SQC_OPERATOR_S:
            case SQC_OPERATOR_S_DAG:
            case SQC_OPERATOR_T:
            case SQC_OPERATOR_T_DAG:
            case SQC_OPERATOR_CNOT:
                {
                    outPp->AddOperator(this_op,n+1);
                }
                break;
            default:
                {
                    outPp->AddOperator(this_op,n+1);
                    cout << "In SQC_Circuit::Hadamards_to_Gadgets. Warning! Gate set should be {Z,S,T,H,CNOT} only." << endl;
                }
                break;
        }
    }
}

void SQC_Circuit::decompose_C3_to_CNOT_D3(const SQC_Circuit& in, SQC_Circuit*& CNOT, SQC_Circuit*& D3) {
    int n = in.n;
    CNOT = new SQC_Circuit(n,in.d,in.p_hads);
    D3 = new SQC_Circuit(n,in.d,in.p_hads);
    // Add each CNOT in circuit 'in' to CNOT
    for(int t = 0; t < in.m; t++) {
        int* this_op = in.operator_list[t];
        D3->AddOperator(this_op);
        if(this_op[0]==SQC_OPERATOR_CNOT) {
            CNOT->AddOperator(this_op);
        }
    }
    // Add each CNOT in circuit 'CNOT' to 'D3' in reverse order
    for(int t = (CNOT->m-1); t >= 0; t--) {
        int* this_op = CNOT->operator_list[t];
        D3->AddOperator(this_op);
    }
}

SQC_Circuit SQC_Circuit::optimize_D3(const SQC_Circuit& in, TO_Decoder decoder) {
    int n = in.n;
    PhasePolynomial in_f = TO_Maps::SQC_Circuit_to_PhasePolynomial(in);
    PhasePolynomial out_f = FullDecoderWrapper(in_f,decoder);
	g_out_T_count += out_f.T_count();
    SQC_Circuit out = TO_Maps::PhasePolynomial_to_SQC_Circuit(out_f);
    out.d = in.d;
    out.p_hads = in.p_hads;
    return out;
}

SQC_Circuit SQC_Circuit::TODD_optimize_D3(const SQC_Circuit& in) {
    int n = in.n;
    PhasePolynomial in_f = TO_Maps::SQC_Circuit_to_PhasePolynomial(in);
    PhasePolynomial out_f = TODDWrapper(in_f);
	g_out_T_count += out_f.T_count();
    SQC_Circuit out = TO_Maps::PhasePolynomial_to_SQC_Circuit(out_f);
    out.d = in.d;
    out.p_hads = in.p_hads;
    return out;
}

void SQC_Circuit::SaveRepresentationsToFile(const SQC_Circuit& in_C, const char* in_filename) {
	// SQC_Circuit
		in_C.Save(in_filename);

	// PP
		PhasePolynomial pp = TO_Maps::SQC_Circuit_to_PhasePolynomial(in_C);
	// WP
		WeightedPolynomial wp = TO_Maps::PhasePolynomial_to_WeightedPolynomial(pp);
	// GSM
		GateStringSparse gsm = TO_Maps::PhasePolynomial_to_GateStringSparse(pp);
	// ST
		Signature st = TO_Maps::WeightedPolynomial_to_Signature(wp);

	// Output
		ofstream f(in_filename, iostream::app);

		f << endl;

		// PP
		f << "Phase polynomial:" << endl;
		for(int t = 0; t < pp.T(); t++) {
			f << "\t";
			bool* x = new bool[pp.get_n()];
			for(int i = 0; i < pp.get_n(); i++) x[i]=0;
			pp.get_a_at(x,t);
			for(int i = 0; i < pp.get_n(); i++) f << x[i];
			f << "\t" << pp.get_m_at(t);
			f << endl;
			delete [] x;
		}

		f << endl;

		// WP
		f << "WeightedPolynomial:" << endl;
		for(int i = 1; i <= wp.get_n(); i++) {
			if(wp(i)) f << "\tl_" << i << " = " << wp(i) << endl;
			for(int j = (i+1); j <= wp.get_n(); j++) {
				if(wp(i,j)) f << "\tq_{" << i << ", " << j << "} = " << wp(i,j) << endl;
				for(int k = (j+1); k <= wp.get_n(); k++) {
					if(wp(i,j,k)) f << "\tc_{" << i << ", " << j << ", " << k << "} = " << wp(i,j,k) << endl;
				}
			}
		}

		f << endl;

		// GSM
		f << "Gate synthesis matrix:" << endl;
		gsm.printMatrix(f);

		f << endl;

		// ST
		f << "Signature tensor:" << endl;
		st.print(f);

		f.close();
}
