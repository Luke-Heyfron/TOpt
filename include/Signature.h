#ifndef SIGNATURE_HEADER
#define SIGNATURE_HEADER

#include <iostream>
using namespace std;

#include <unordered_set>

class Signature {
    /*
        Notes:
        - Indices I take values from 0 -> (n+n^2+n^3-1) where only indices that correspond to j<k, l<m<p are valid.
        - Indices (i,j,k,l,m,p) each take values from 1 -> n.
    */
private:
    unordered_set<int>* data_us;
    int N; // Length of Signature vector
    int n; // Number of qubits.
    bool bad_signature;
public:
    Signature(int in_n);
    Signature();
    ~Signature();

    bool E(int I) const;
    bool E(int i1, int i2, int i3) const;

    int setByInd(int I);
    int set(int i); // Sets coefficient a_i to 1. Returns 1 if element added, 0 otherwise.
    int set(int j, int k); // Sets coefficient b_jk to 1. Returns 1 if element added, 0 otherwise.
    int set(int l, int m, int p); // Sets coefficient c_lmp to 1. Returns 1 if element added, 0 otherwise.
    int clearByInd(int I);
    int clear(int i);
    int clear(int j, int k);
    int clear(int l, int m, int p);

    void print(ostream& inOS = cout) const;
    void save(const char* inFilename) const;
    void save(ostream& inOS = cout) const;

    int iToInd(int i) const;
    int jkToInd(int j, int k) const;
    int lmpToInd(int l, int m, int p) const;
    int indTo_ijklmp(int I, int* ijklmp) const;
    int Na() const; //Number of a coefficients
    int Nb() const; //Number of b coefficients
    int Nc() const; //Number of c coefficients
    int getN() const;
    int get_n() const;
    void set_n(int in_n);
    bool bad() const;
    bool isEmpty() const;

    int getA(int* a_i) const; //Puts indices for a into a_i. Returns number of a elements.
    int getB(int* b_j, int* b_k) const; // '' for b_jk
    int getC(int* c_l, int* c_m, int* c_p) const;

    Signature& assign(const Signature& inS);
    Signature& operator=(const Signature& inS);
    Signature add(const Signature& inS) const;
    Signature operator+(const Signature& inS) const;

    Signature diminish(const int h) const; // Returns a signature where the h^th qubit has been removed and all qubits > h are relabelled
    Signature augment(const int h) const; // Returns a signature where a qubit is inserted at position h and all qubit >= h are relabelled
    Signature mult2xh(const int h) const;
    Signature subsig(bool* inc_vec) const; // Returns a sub-signature where inc_vec is the incidence vector of length n

    void h_decomposition(const int h, Signature& out_alpha, Signature& outAp, Signature& outB) const;
    int h_weight(const int h) const;
    int get_N_eff() const;

    void toBool(bool*** outBS) const;

    unordered_set<int> get_a() const;
    static Signature SigFromFile(const char* inFilename);
};


#endif // SIGNATURE_HEADER
