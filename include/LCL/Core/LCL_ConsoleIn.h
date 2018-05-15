#ifndef LCL_CONSOLE_IN_HEADER
#define LCL_CONSOLE_IN_HEADER

namespace LCL_ConsoleIn {

    /// Retrieves the standard-out of a system command and puts it in a character array.
    /**
        \param dest - Destination c-string.
        \param n - Size of c-string.
        \param comm - The command to be passed to the system terminal.
        \return 0 if successful, non-zero otherwise.

        Remarks: This function makes use of popen, pclose etc. This requires including <stdio.h> and compiler options -std=c++11 -U__STRICT_ANSI__
    */
    int GetCommandOutput(char* dest, int n, const char* comm);
}

#endif // LCL_CONSOLE_IN_HEADER
