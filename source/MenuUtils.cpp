#include "MenuUtils.h"

#include <cstring>
#include <string>

#include <iostream>
using namespace std;

void MenuUtils::getOneInt(int& inOut, istream& inIS, ostream& inOS, const char* inMessage) {
    int temp;

    inOS << inMessage << endl;
    inIS >> temp;

    inOut = temp;
}

void MenuUtils::getOneDouble(double& inOut, istream& inIS, ostream& inOS, const char* inMessage) {
    double temp;

    inOS << inMessage << endl;
    inIS >> temp;

    inOut = temp;
}

void MenuUtils::getOneChar(char& inOut, istream& inIS, ostream& inOS, const char* inMessage) {
    char temp;

    inOS << inMessage << endl;
    inIS >> temp;

    inOut = temp;
}

void MenuUtils::getOneCString(char* inOut, istream& inIS, ostream& inOS, const char* inMessage) {
    string temp;

    inOS << inMessage << endl;
    inIS >> temp;

    strcpy(inOut, temp.c_str());
}

void MenuUtils::getOneString(string& inOut, istream& inIS, ostream& inOS, const char* inMessage) {
    string temp;

    inOS << inMessage << endl;
    inIS >> temp;

    inOut = temp;
}
