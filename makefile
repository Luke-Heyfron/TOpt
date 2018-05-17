CC      = g++
LFLAGS  = -L. 
LIBS    = -lm
CFLAGS  = -O3 -std=c++11
INCS    = -I ./include
EXEC    = bin/TOpt

SRCS =  BMSparse.cpp BoolMat.cpp Bool_Signature.cpp Complex.cpp CTX_Circuit.cpp GateSigInterface.cpp GateStringSparse.cpp \
	   	GateSynthesisMatrix.cpp Interface_BMSGSS.cpp Interface_SigBMS.cpp LCL_Bool.cpp LCL_BoundedInt.cpp LCL_ConsoleIn.cpp LCL_ConsoleOut.cpp \
		LCL_Int.cpp LCL_Mat_GF2.cpp LCL_MathsUtils.cpp LCL_Menu.cpp LCL_MenuUtils.cpp LCL_Utils.cpp Matrix.cpp Menu.cpp MenuUtils.cpp PhasePolynomial.cpp \
		Signature.cpp SQC_Circuit.cpp tests.cpp TO_CircuitGenerators.cpp TO_Decoder.cpp TO_Maps.cpp TO_Matrix.cpp \
		UniversalOptimize.cpp Utils.cpp WeightedPolynomial.cpp main.cpp

OBJS := $(SRCS:%.cpp=bin/%.o)

all: $(EXEC) test

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LFLAGS) $(LIBS)

bin/%.o : source/%.cpp
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

test: $(EXEC)
	./$(EXEC) circuit test.tfc

clean:
	rm -rf $(EXEC) $(OBJS) *~

.PHONY: clean all test

