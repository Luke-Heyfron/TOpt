#ifndef BOOL_MAT_HEADER
#define BOOL_MAT_HEADER

#define BM_ZERO BoolMat(1,1,0)
#define BM_ONE BoolMat(1,1,1)

class BoolMat {
    //Implements an n X m binary matrix.
private:
    bool** data;
    int n;
    int m;
public:
    BoolMat();
    BoolMat(int inN, int inM, bool val = 0);
    BoolMat(const BoolMat& inBM);
    ~BoolMat();
    bool& E(int i, int j, bool periodic=true);
    bool E(int i, int j, bool periodic=true) const;

    int N() const;
    int M() const;

    BoolMat add(const BoolMat& inBM) const;
    BoolMat operator+(const BoolMat& inBM) const;
    BoolMat multiply(const BoolMat& inBM) const; //TODO
    BoolMat operator*(const BoolMat& inBM) const; //TODO
    BoolMat& assign(const BoolMat& inBM);
    BoolMat& operator=(const BoolMat& inBM);
    BoolMat composeLR(const BoolMat& inBM) const;
    BoolMat operator&&(const BoolMat& inBM) const; //Same as composeLR
    BoolMat composeUD(const BoolMat& inBM) const;
    BoolMat operator||(const BoolMat& inBM) const; //Same as composeUD
    BoolMat subMatrix(int start_row, int end_row, int start_col, int end_col) const; // Use index -1 for 'beginning' and 'end' cols e.g. subMatrix(-1,-1,-1,-1) will return the full matrix.
    BoolMat& subMatrix(int start_row, int end_row, int start_col, int end_col, const BoolMat& inBM);
    BoolMat row(int i) const;
    BoolMat& row(int i, const BoolMat& inBM);
    BoolMat col(int j) const;
    BoolMat& col(int j, const BoolMat& inBM);
    void save(const char* inFilename, bool append=false) const;

    int sum() const;

    void print() const;

    static int sum(const BoolMat& inBM);
    void resize(int inN, int inM); //Warning! Destroys all contents of data

    static BoolMat AllUniqueBinaryPerms(int n, int w);
};

#endif // BOOL_MAT_HEADER
