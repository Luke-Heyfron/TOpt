#ifndef MENU_UTILS_HEADER
#define MENU_UTILS_HEADER

#include <string>

#include <iostream>
using namespace std;

namespace MenuUtils {
    void getOneInt(int& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneDouble(double& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneChar(char& inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneCString(char* inOut, istream& inIS, ostream& inOS, const char* inMessage);
    void getOneString(string& inOut, istream& inIS, ostream& inOS, const char* inMessage);
}

#endif // MENU_UTILS_HEADER
