CC      = g++
LFLAGS  = -L. 
LIBS    = -lm
CFLAGS  = -O3 -std=c++11
INCS    = -I ./include
EXEC    = bin/TOpt

SRCS =  BMSparse.cpp BoolMat.cpp Bool_Signature.cpp Complex.cpp GateSigInterface.cpp GateStringSparse.cpp \
	   	GateSynthesisMatrix.cpp Interface_BMSGSS.cpp Interface_SigBMS.cpp LCL_Bool.cpp LCL_ConsoleOut.cpp \
		LCL_Int.cpp LCL_Mat_GF2.cpp LCL_Maths.cpp Matrix.cpp Menu.cpp MenuUtils.cpp PhasePolynomial.cpp \
		Signature.cpp SQC_Circuit.cpp TO_CircuitGenerators.cpp TO_Decoder.cpp TO_Maps.cpp \
		UniversalOptimize.cpp Utils.cpp WeightedPolynomial.cpp main.cpp

OBJS := $(SRCS:%.cpp=bin/%.o)

all: $(EXEC) test

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LFLAGS) $(LIBS)

bin/%.o : source/%.cpp
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

test: $(EXEC)
	./$(EXEC) circuit benchmarks/simple/test.tfc -a todd -o benchmarks/simple/test_output.sqc

clean:
	rm -rf $(EXEC) $(OBJS) *~

.PHONY: clean all test

