#ifndef LCL_FIELD_HEADER
#define LCL_FIELD_HEADER

#include <iostream>
using namespace std;

/**
    Derived classes should define static consts '<Derived class>_ZERO' and '<Derived class>_UNITY'.
    A derived class must implement:
        - operator=
        - operator+
        - operator-
        - operator*
        - operator~
        - operator==
    A derivate class should implement:
        - constructor: <derived class>(const LCL_Field&)
        - operator*(const LCL_Matrix& in) const {return in*(*this);}
*/

class LCL_Field {
    private:

    public:
        // Access value
        virtual operator int () const = 0;
        virtual operator double () const = 0;
        virtual operator bool () const = 0;

        // Assignment
        virtual LCL_Field& operator=(const LCL_Field& b) {return (*this);}
        virtual LCL_Field& operator=(int b) = 0;
        virtual LCL_Field& operator=(double b) = 0;
        virtual LCL_Field& operator=(bool b) = 0;
        /*virtual LCL_Field& operator+=(const LCL_Field& b) {return ((*this)=(*this)+b);}
        virtual LCL_Field& operator*=(const LCL_Field& b) {return ((*this)=(*this)*b);}
        virtual LCL_Field& operator-=(const LCL_Field& b) {return ((*this)=(*this)-b);}
        virtual LCL_Field& operator/=(const LCL_Field& b) {return ((*this)=(*this)/b);}*/

        // Arithmetic Operators
        /*virtual const LCL_Field& operator+(const LCL_Field& b) const = 0;
        virtual const LCL_Field& operator-() const = 0;
        virtual const LCL_Field& operator-(const LCL_Field& b) const {return (*this) + (-b);}
        virtual const LCL_Field& operator*(const LCL_Field& b) const = 0;
        virtual const LCL_Field& operator~() const = 0;    /// Multiplicative inverse
        virtual const LCL_Field& operator/(const LCL_Field& b) const {return (*this) * (~b);}*/

        // Comparison Operators
        virtual bool operator==(const LCL_Field& b) const = 0;
        virtual bool operator!=(const LCL_Field& b) const {return !operator==(b);}
};

// ostream& operator<<(ostream& os, const LCL_Field& f);

#endif // LCL_FIELD_HEADER
