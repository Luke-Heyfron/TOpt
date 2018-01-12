#ifndef LCL_BOUNDED_INT_HEADER
#define LCL_BOUNDED_INT_HEADER

class LCL_BoundedInt {
    private:
        int _v = 0;
        int _min = 0;
        int _max = 0;
    public:
        LCL_BoundedInt();
        LCL_BoundedInt(int in_v, int in_min, int in_max);

        // Conversion
        operator int() const {return _v;}

        // Assignment
        LCL_BoundedInt& operator=(const int in_v);
        LCL_BoundedInt& operator()(const int in_min, const int in_max);
};

#endif // LCL_BOUNDED_INT_HEADER
