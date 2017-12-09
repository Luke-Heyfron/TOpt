TOpt
----

Implementation of T gate optimization algorithm in reference https://arxiv.org/abs/1712.01557.

Installation:
Make sure GCC is installed and run "make all" in the TOpt directory. 

Commands:

circuit <filename>
	Optimizes Clifford + T circuit. Takes as input .tfc file with path given by argument <filename>. See http://webhome.cs.uvic.ca/~dmaslov/mach-read.html for syntax description.
	
	Options:
		-a <algorithm>
			Selects 3-STR solver where <algorithm> can take values the following values.
				todd	TODD algorithm.
				tool	TOOL algorithm. Has additional options:
					-b <0|1> 	Without feedback|Feedback
					-s <g|lg|r> 	Greedy|Least greedy|Random qubit order selection.
					-r <integer> 	Sets n_RM to <integer>
				re	RE algorithm.
				rm	RM algorithm.
		-o <filename>
			If set, saves output circuit to file with path given by argument <filename> in deprocated .sqc format. Will implement code to output in .tfc format soon.
		-v <0|1>
			If argument is 1, TOpt will verify the functional equivalence of the input and output circuits using a brute force simulation of the quantum circuit.
		-h <integer>
			Sets cap on number of Hadamard ancillas to <integer>.

signature <filename>
	Optimizes a signature tensor. Takes as input .sig file with path given by argument <filename>. A .sig file is a sparse representation of the signature tensor with syntax as follows.
	First line must be
		n <value of n>
	which sets the number of qubits of signature tensor, S.
	Remaining lines are of format
		a <alpha>			FOR 	S_{alpha,alpha,alpha}=1
			OR
		b <alpha> <beta>		FOR	S_{alpha,beta,beta}=S_{alpha,alpha,beta}=1
			OR
		c <alpha> <beta> <gamma>	FOR	S_{alpha,beta,gamma}
	where each line indicates a value for which signature tensor element equals 1.

help
	Displays this readme file.
