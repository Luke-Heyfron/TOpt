#include "Utils.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;


	double Utils::rand_d() {
		return (double)rand()/(double)RAND_MAX;
	}
	
	int Utils::rand_i(int min, int max) {
		int gap = abs(max-min) + 1;
		int out = (rand() % gap) + min;
		return out;
	}
	
	int Utils::rand_i(int num) {
		int out = rand()%num;
		return out;
	}
	
	int Utils::flipBitN(int index, int Nbit) {
		unsigned int flipNum = 1;
		flipNum = flipNum << Nbit;
		unsigned int indexU = (unsigned int)index - (unsigned int)0;
		unsigned int outU = indexU^flipNum;
		int out = (int)(outU + (unsigned int)0);
		return out;
	}
	
	int Utils::kDelta(int i, int j) {
		return (i==j)?1:0;
	}

bool Utils::Bn(int index, int bit) {
	int bitI = 1 << bit;
	int result = index & bitI;
	return bitI == result;
}

void Utils::getComment(istream& inStr) {
	char c = inStr.peek();
	if(c == '#') {		
		char s[1000];
		inStr.get();
		inStr.getline(s, 1000);
		cout << s << endl;
	}
}

int Utils::GCD(int inA, int inB) {
	int r_present, r_past, r_future;
	
	if(inA>inB) {
		r_present = inA;
		r_future = inB;
	} else {
		r_present = inB;
		r_future = inA;
	}
	r_past = 0;	
	
	do{
		r_past = r_present;
		r_present = r_future;
		r_future = r_past % r_present;
	} while(r_future);
	return r_present;
}

int Utils::factorize(int inC, int* inFactors) {
	cout << "CLASSICAL FACTORING BEGIN: C = " << inC << endl;
	if(inC/2 == (double)inC/2.0) {
		inFactors[0] = 2;
		inFactors[1] = inC/2;
		cout << "CLASSICAL FACTORING COMPLETE (Even): Factors = {" << inFactors[0] << ", " << inFactors[1] << "}" << endl;
		return 0;
	}
	if((int)sqrt((double)inC) == sqrt((double)inC)) {
		inFactors[0] = (int)sqrt((double)inC);
		inFactors[1] = (int)sqrt((double)inC);
		cout << "CLASSICAL FACTORING COMPLETE (Power): Factors = {" << inFactors[0] << ", " << inFactors[1] << "}" << endl;
		return 0;
	}
	bool found = false;
	int out = 0;
	int N = (int)ceil(sqrt(inC));
	bool guessTracker[N];
	for(int i = 0; i < N; i++) guessTracker[i] = false;
	while(!found&&(out<(N-3))) {
		out++;
		int guess;
		do {
			guess = Utils::rand_i(3,N-1);
		} while(guessTracker[guess]);
		guessTracker[guess]=true;
		cout << "CLASSICAL FACTORING GUESS: Guess = " << guess << endl;
		int intQ = inC/guess;
		double doubleQ = (double)inC/guess;
		if(intQ == doubleQ) {
			inFactors[0] = guess;
			inFactors[1] = intQ;
			found = true;
			cout << "CLASSICAL FACTORING COMPLETE (Guess): Factors = {" << inFactors[0] << ", " << inFactors[1] << "}" << endl;
		}
	}
	return out;
}
