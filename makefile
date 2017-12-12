all: main.o ./obj/BMSparse.o ./obj/Bool_Signature.o ./obj/BoolMat.o ./obj/Complex.o ./obj/GateSigInterface.o ./obj/GateStringSparse.o ./obj/GateSynthesisMatrix.o ./obj/Interface_BMSGSS.o ./obj/Interface_SigBMS.o ./obj/LCL_Bool.o ./obj/LCL_ConsoleOut.o ./obj/LCL_Int.o ./obj/LCL_Mat_GF2.o ./obj/LCL_Maths.o ./obj/Matrix.o ./obj/Menu.o ./obj/MenuUtils.o ./obj/PhasePolynomial.o ./obj/Signature.o ./obj/SQC_Circuit.o ./obj/TO_CircuitGenerators.o ./obj/TO_Decoder.o ./obj/TO_Maps.o ./obj/UniversalOptimize.o ./obj/Utils.o ./obj/WeightedPolynomial.o
	g++ main.o ./obj/BMSparse.o ./obj/Bool_Signature.o ./obj/BoolMat.o ./obj/Complex.o ./obj/GateSigInterface.o ./obj/GateStringSparse.o ./obj/GateSynthesisMatrix.o ./obj/Interface_BMSGSS.o ./obj/Interface_SigBMS.o ./obj/LCL_Bool.o ./obj/LCL_ConsoleOut.o ./obj/LCL_Int.o ./obj/LCL_Mat_GF2.o ./obj/LCL_Maths.o ./obj/Matrix.o ./obj/Menu.o ./obj/MenuUtils.o ./obj/PhasePolynomial.o ./obj/Signature.o ./obj/SQC_Circuit.o ./obj/TO_CircuitGenerators.o ./obj/TO_Decoder.o ./obj/TO_Maps.o ./obj/UniversalOptimize.o ./obj/Utils.o ./obj/WeightedPolynomial.o -o TOpt
main.o: main.cpp
	g++ -I ./include -c -std=c++11 main.cpp
./obj/BMSparse.o: ./source/BMSparse.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/BMSparse.o ./source/BMSparse.cpp
./obj/Bool_Signature.o: ./source/Bool_Signature.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Bool_Signature.o ./source/Bool_Signature.cpp
./obj/BoolMat.o: ./source/BoolMat.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/BoolMat.o ./source/BoolMat.cpp
./obj/Complex.o: ./source/Complex.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Complex.o ./source/Complex.cpp
./obj/GateSigInterface.o: ./source/GateSigInterface.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/GateSigInterface.o ./source/GateSigInterface.cpp
./obj/GateStringSparse.o: ./source/GateStringSparse.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/GateStringSparse.o ./source/GateStringSparse.cpp
./obj/GateSynthesisMatrix.o: ./source/GateSynthesisMatrix.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/GateSynthesisMatrix.o ./source/GateSynthesisMatrix.cpp
./obj/Interface_BMSGSS.o: ./source/Interface_BMSGSS.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Interface_BMSGSS.o ./source/Interface_BMSGSS.cpp
./obj/Interface_SigBMS.o: ./source/Interface_SigBMS.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Interface_SigBMS.o ./source/Interface_SigBMS.cpp
./obj/LCL_Bool.o: ./source/LCL_Bool.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/LCL_Bool.o ./source/LCL_Bool.cpp
./obj/LCL_ConsoleOut.o: ./source/LCL_ConsoleOut.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/LCL_ConsoleOut.o ./source/LCL_ConsoleOut.cpp
./obj/LCL_Int.o: ./source/LCL_Int.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/LCL_Int.o ./source/LCL_Int.cpp
./obj/LCL_Mat_GF2.o: ./source/LCL_Mat_GF2.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/LCL_Mat_GF2.o ./source/LCL_Mat_GF2.cpp
./obj/LCL_Maths.o: ./source/LCL_Maths.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/LCL_Maths.o ./source/LCL_Maths.cpp
./obj/Matrix.o: ./source/Matrix.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Matrix.o ./source/Matrix.cpp
./obj/Menu.o: ./source/Menu.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Menu.o ./source/Menu.cpp
./obj/MenuUtils.o: ./source/MenuUtils.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/MenuUtils.o ./source/MenuUtils.cpp
./obj/PhasePolynomial.o: ./source/PhasePolynomial.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/PhasePolynomial.o ./source/PhasePolynomial.cpp
./obj/Signature.o: ./source/Signature.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Signature.o ./source/Signature.cpp
./obj/SQC_Circuit.o: ./source/SQC_Circuit.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/SQC_Circuit.o ./source/SQC_Circuit.cpp
./obj/TO_CircuitGenerators.o: ./source/TO_CircuitGenerators.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/TO_CircuitGenerators.o ./source/TO_CircuitGenerators.cpp
./obj/TO_Decoder.o: ./source/TO_Decoder.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/TO_Decoder.o ./source/TO_Decoder.cpp
./obj/TO_Maps.o: ./source/TO_Maps.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/TO_Maps.o ./source/TO_Maps.cpp
./obj/UniversalOptimize.o: ./source/UniversalOptimize.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/UniversalOptimize.o ./source/UniversalOptimize.cpp
./obj/Utils.o: ./source/Utils.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/Utils.o ./source/Utils.cpp
./obj/WeightedPolynomial.o: ./source/WeightedPolynomial.cpp
	g++ -I ./include -c -std=c++11 -o ./obj/WeightedPolynomial.o ./source/WeightedPolynomial.cpp
