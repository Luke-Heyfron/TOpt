#ifndef UTILS
#define UTILS

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

namespace Utils {
	double rand_d();
	int rand_i(int min, int max);
	int rand_i(int num);
	int flipBitN(int index, int Nbit);
	
	int kDelta(int i, int j);
	bool Bn(int index, int bit); //bit starts from 0 (least significant bit)
	void getComment(istream& inStr);
	int GCD(int inA, int inB);
	int factorize(int inC, int* inFactors);
}

#endif
