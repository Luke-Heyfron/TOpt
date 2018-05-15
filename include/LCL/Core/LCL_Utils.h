#ifndef LCL_UTILS
#define LCL_UTILS

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

/// Contains handy functions.
namespace LCL_Utils {
    /// Returns a random double between 0 and 1.
	double rand_d();

	/// Returns a random int between min and max.
	int rand_i(int min, int max);

	/// Returns a random int between 0 and num.
	int rand_i(int num);

	/// Flips the Nth bit of binary expansion of index.
	int flipBitN(int index, int Nbit);

    /// Returns 1 if and only if i==j.
	int kDelta(int i, int j);

	/// Returns the Nth bit of binary expansion of index.
	bool Bn(int index, int bit); // bit starts from 0 (least significant bit)

	/// Displays a commented out line from an input stream if it begins with one.
	void getComment(istream& inStr);

	/// (I think) GCD calculates the Greatest Common Divisor between inA and inB.
	int GCD(int inA, int inB);

	/// Factorizes an integer inC and places the factors in an array inFactors.
	int factorize(int inC, int* inFactors);
}

#endif
