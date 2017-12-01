#ifndef HADAMARD
#define HADAMARD

#include "Operator.h"
#include "State.h"


class Hadamard : public Operator {
	private:
		char mode;
		int actOn;
		State actOnOne(const State&, int) ;
		State actOnAll(const State&) ;
		State actOnOneOptimized(const State&, int);
		State actOnAllOptimized(const State&);
	public:
		Hadamard();
		Hadamard(int);
		State act(const State&);
		void toString(char* dest);
};


#endif
