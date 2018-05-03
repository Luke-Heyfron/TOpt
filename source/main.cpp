#include <iostream>
using namespace std;

#include "TO_Decoder.h"
#include "Signature.h"
#include "GateStringSparse.h"
#include "GateSigInterface.h"
#include "LCL_Maths.h"
#include "LCL_Bool.h"
#include "BoolMat.h"
#include "BMSparse.h"
#include "Interface_SigBMS.h"
#include "Interface_BMSGSS.h"
#include "LCL_Int.h"
#include "Bool_Signature.h"
#include "LCL_Mat_GF2.h"
#include "GateSynthesisMatrix.h"
#include "SQC_Circuit.h"
#include "LCL_ConsoleOut.h"
using namespace LCL_ConsoleOut;
#include "PhasePolynomial.h"
#include "TO_Maps.h"
#include "WeightedPolynomial.h"
#include "Utils.h"
#include "TO_CircuitGenerators.h"

#include <climits>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <utility>

int main(int argc, char* argv[]) {
    g_output_filename.clear();
    g_csv_filename.clear();

    srand(time(NULL));

    if(argc>1) {
        string this_command = argv[1];
        if(!this_command.compare("circuit")&&(argc>=3)) {
            cout << "Optimize a Clifford + T circuit." << endl;
            SQC_Circuit* this_circuit = NULL;
            TO_Decoder this_decoder = TODD;
            string circuit_filename = argv[2];
            LOut() << "Input filename = " << circuit_filename << endl;
            string this_input_filetype = "tfc";
            bool this_verify = 0;
            string this_tool_selector="r";
            bool this_tool_feedback=1;

            for(int i = 3; i < argc; i++) {
                string this_option = argv[i];
                if((this_option[0]=='-')&&((i+1)<argc)) {
                    string this_value = argv[i+1];
                    char this_option_char = this_option[1];
                    switch(this_option_char) {
                        case 'f':
                            // filetype
                            this_input_filetype = this_value;
                            break;
                        case 'o':
                            // output filename
                            g_output_filename = this_value;
                            break;
                        case 'a':
                            // algorithm
                            g_algorithm = this_value;
                            break;
                        case 'v':
                            // verify?
                            this_verify = atoi(this_value.c_str());
                            break;
                        case 'h':
                            //no. had. ancs
                            g_Hadamard_ancillas = atoi(this_value.c_str());
                            break;
                        case 's':
                            //TOOL selector
                            this_tool_selector = this_value;
                            break;
                        case 'b':
                            //TOOL feedback param
                            this_tool_feedback = atoi(this_value.c_str());
                            break;
                        case 'r':
                            //n_RM
                            g_Reed_Muller_max = atoi(this_value.c_str());
                            break;
						case 'c':
							// csv filename
							g_csv_filename = this_value;
							break;
                    }
                    i++;
                }
            }

            // Load circuit depending on filetype
            if(!this_input_filetype.compare("sqc")) {
                this_circuit = new SQC_Circuit();
                this_circuit->Load(circuit_filename.c_str());
            } else if(!this_input_filetype.compare("tfc")) {
                this_circuit = SQC_Circuit::LoadTFCFile(circuit_filename.c_str());
            }

            // Determine algorithm to be used
            if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TODD)) {
                this_decoder = TODD;
            } else if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TOOL)) {
                if(!this_tool_selector.compare("g")) {
                    this_tool_feedback?this_decoder = TOOL_F_G:this_decoder = TOOL_WF_G;
                } else if(!this_tool_selector.compare("lg")) {
                    this_tool_feedback?this_decoder = TOOL_F_LG:this_decoder = TOOL_WF_LG;
                } else if(!this_tool_selector.compare("r")) {
                    this_tool_feedback?this_decoder = TOOL_F_R:this_decoder = TOOL_WF_R;
                }
            } else if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::DAFT_GUESS)) {
                this_decoder = RE;
            } else if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::REED_MULLER)) {
                this_decoder = ReedMullerSynthesis2;
            }

            if(this_circuit) {
		LOut() << "Circuit filename: " << circuit_filename << endl << endl;
                this_circuit->Print();
		LOut() << endl;
		//this_circuit->name = "Input";

		LOut() << "Algorithm used: " << g_algorithm << endl;
		if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TOOL)) LOut() << "TOOL Version: " << (this_tool_feedback?"f":"nf") << " " << this_tool_selector  << " rm = " << g_Reed_Muller_max << endl;
		LOut() << endl;

		int input_tcount = this_circuit->TCount();

		clock_t tic = clock();
                SQC_Circuit result = SQC_Circuit::UniversalOptimize(*this_circuit,this_decoder);
		clock_t toc = clock();

                LOut() << "Output circuit:" << endl;
                result.Print();
		//result.name = "Output";

                LOut() << "Gate distributions:" << endl;
                LOut() << "Input:" << endl;
                this_circuit->PrintOperatorDistribution();

                LOut() << "Output:" << endl;
                result.PrintOperatorDistribution();
				
				LOut() << "Output T Count (SQC_Circuit) = " << (result.TCount()) << endl;
				LOut() << "Output T Count (PhasePolynomial) = " << g_out_T_count << endl;
				LOut() << "Fail count = " << g_fail_count << endl;

		LOut() << endl;

		LOut() << "Execution time: " << LCL_ConsoleOut::secs(tic,toc) << "s" << endl;

                if(this_verify) {
                    VerifyOptimization2(*this_circuit,result);
                }
                if(!g_output_filename.empty()) {
					ofstream my_file(g_output_filename.c_str(), iostream::out);
					my_file << "Circuit filename: " << circuit_filename << endl << endl;

					my_file << "Input circuit:" << endl;
					my_file.close();
					this_circuit->Save(g_output_filename.c_str(), ios_base::app);
					my_file.open(g_output_filename.c_str(), iostream::app);					
					my_file << endl;

					my_file << endl;

					my_file << "Output circuit:" << endl;
					my_file.close();
							result.Save(g_output_filename.c_str(),ios_base::app);
					my_file.open(g_output_filename.c_str(), iostream::app);
					

					my_file << endl;

					my_file << "Input operator distribution:" << endl;
					this_circuit->PrintOperatorDistribution(&my_file);
					if(g_print_load_tfc_debug&&g_gate_hist) my_file << "Number of unknown gates = " << g_gate_hist[SQC_OPERATOR_N] << endl;
					if(g_print_load_tfc_debug&&g_qubit_hist) my_file << "Number of unknown qubit labels = " << g_qubit_hist[0] << endl;

					my_file << endl;

					my_file << "Algorithm used: " << g_algorithm << endl;
					if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TOOL)) my_file << "TOOL Version: " << (this_tool_feedback?"f":"nf") << " " << this_tool_selector  << " rm = " << g_Reed_Muller_max << endl << endl;

					my_file << "Output operator distribution:" << endl;
					result.PrintOperatorDistribution(&my_file);

					my_file << endl;
					
					my_file << "Output T Count (SQC_Circuit) = " << (result.TCount()) << endl;
					my_file << "Output T Count (PhasePolynomial) = " << g_out_T_count << endl;
					my_file << "Fail count = " << g_fail_count << endl;

					my_file << "Execution time: " << LCL_ConsoleOut::secs(tic,toc) << "s" << endl;
					
					my_file.close();
                }
				if(!g_csv_filename.empty()) {
					// Check if csv output file exists. If not, create it an add table header line.
					{
						ifstream my_infile(g_csv_filename.c_str(), iostream::in);
						if(!my_infile.good()) {
							ofstream my_outfile(g_csv_filename.c_str(), iostream::out);
							my_outfile << "InputQCFilename,n_data_in,n_toff_in,n_had_in,T_in,AlgorithmUsed,Hcap,n_data_out,n_toff_out,n_had_out,T_out,no_Hparts,exec_time,OutputTCountCheck,FailCount" << endl;
							my_outfile.close();
						}
						my_infile.close();
					}
					// Open csv output file and append a row for this run of TOpt
					ofstream my_file(g_csv_filename.c_str(), iostream::app);
					if(my_file.good()) {
						my_file << circuit_filename << ",";
						my_file << (this_circuit->d) << ",";
						my_file << (this_circuit->n-this_circuit->d-this_circuit->p_hads) << ",";
						my_file << (this_circuit->p_hads) << ",";
						my_file << input_tcount << ",";
						my_file << g_algorithm;
						if(!g_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TOOL)) my_file << ":" << (this_tool_feedback?"f":"nf") << "_" << this_tool_selector  << "_rm=" << g_Reed_Muller_max;
						my_file << ",";
						my_file << g_Hadamard_ancillas << ",";
						my_file << result.d << ",";
						my_file << (result.n - result.d - result.p_hads) << ",";
						my_file << result.p_hads << ",";
						my_file << result.TCount() << ",";
						my_file << g_out_no_partitions << ",";
						my_file << LCL_ConsoleOut::secs(tic,toc) << ",";
						my_file << g_fail_count << ",";
						my_file << g_out_T_count << endl;
						my_file.close();
					}
				}
            }



            // Delete circuits
            if(this_circuit) {
                delete this_circuit;
                this_circuit = NULL;
            }
        } else if(!this_command.compare("signature")&&(argc>2)) {
            string circuit_filename = argv[2];
            int option_maxRM = 5;

            string option_filename;
            string option_algorithm;
            bool option_feedback = true;
            for(int i = 3; i < argc; i++) {
                string this_option = argv[i];
                if((this_option[0]=='-')&&((i+1)<argc)) {
                    string this_value = argv[i+1];
                    char this_option_char = this_option[1];
                    switch(this_option_char) {
                        case 'r':
                            option_maxRM = atoi(this_value.c_str());
                            break;
                        case 'a':
                            option_algorithm = this_value;
                            g_algorithm = option_algorithm;
                            break;
                        case 'f':
                            if((!this_value.compare("true"))||(!this_value.compare("1"))) {
                                option_feedback = true;
                            } else if((!this_value.compare("false"))||(!this_value.compare("0"))) {
                                option_feedback = false;
                            }
                            break;
                        case 'c':
                            g_csv_filename = this_value;
                            break;
                    }
                    i++;
                }
            }
            g_lempel_feedback = option_feedback;
            g_Reed_Muller_max = option_maxRM;
            g_output_filename = option_filename;
            Signature this_sig;
            char first_char = circuit_filename.c_str()[0];
            if(first_char==':') {
                cout << "Structured signature." << endl;
                this_sig = CircuitGenerator(circuit_filename.substr(1));
            } else {
                cout << "Signature from file: " << circuit_filename << endl;
                this_sig = Signature::SigFromFile(circuit_filename.c_str());
            }
            GateStringSparse out(this_sig.get_n());
            cout << "Input signature polynomial:" << endl;
            this_sig.print();
            cout << endl << "Synthesis algorithm: ";
            if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_LEAST_GREEDY)) {
                cout << "TOOL least-greedy" << endl;
                GateStringSparse tempout = TOOL(this_sig,option_maxRM,LempelSelector_LeastGreedy,option_feedback);
                out.assign(tempout);
            } else if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::REED_MULLER)) {
                cout << "RM" << endl;
                GateStringSparse tempout = ReedMullerSynthesis2(this_sig);
                out.assign(tempout);
            } else if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_GREEDY)) {
                cout << "TOOL greedy" << endl;
                GateStringSparse tempout = TOOL(this_sig,option_maxRM,LempelSelector_Greedy,option_feedback);
                out.assign(tempout);
            } else if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::DAFT_GUESS)) {
                cout << "RE" << endl;
                GateStringSparse tempout = GateSigInterface::SigToGSS(this_sig);
                if(!g_csv_filename.empty()) {
                    ofstream my_csv(g_csv_filename.c_str(),iostream::app);
                    int n = this_sig.get_n();
                    my_csv << SYNTHESIS_ALGORITHM_ID::DAFT_GUESS << "," << n << "," << g_random_circuit_seed << "," << tempout.weight(true) << endl;
                    my_csv.close();
                }
                out.assign(tempout);
            } else if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::LEMPEL_RANDOM)) {
                cout << "TOOL random" << endl;
                GateStringSparse tempout = TOOL(this_sig,option_maxRM,LempelSelector_Random,option_feedback);
                out.assign(tempout);
            } else if(!option_algorithm.compare(SYNTHESIS_ALGORITHM_TAG::TODD)) {
                cout << "TODD" << endl;
                GateStringSparse tempout = TODD(this_sig);
                out.assign(tempout);
            }
            result_analysis(this_sig,out);
            cout << "Output phase polynomial:" << endl;
            out.print();
        } else if(!this_command.compare("help")) {
            ifstream myfile("README.md");
            while(!myfile.eof()) {
                char this_line[1000];
                myfile.getline(this_line,1000);
                cout << this_line << endl;
            }
            myfile.close();
        }
    }
    return 0;
}
