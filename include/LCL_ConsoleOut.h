#ifndef LCL_CONSOLE_OUT_HEADER
#define LCL_CONSOLE_OUT_HEADER

#include <iostream>
using namespace std;

#include <ostream>

#include <ctime>

#define FOut() if(g_output_file)(*g_output_file)

extern int dout_n;

namespace LCL_ConsoleOut {
    extern int LOut_Pad;

    ostream& LOut();
    void dout();

    void warning(const char* message, const char* function_name = NULL, const char* class_name = NULL);
    void error(const char* message, const char* function_name = NULL, const char* class_name = NULL);

    double secs(clock_t tic, clock_t toc);
}


#endif
