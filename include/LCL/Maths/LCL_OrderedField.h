#ifndef LCL_ORDERED_FIELD_HEADER
#define LCL_ORDERED_FIELD_HEADER

#include "LCL/Maths/LCL_Field.h"

class LCL_OrderedField : public LCL_Field {
    public:
        // Order operators
        virtual bool operator<(const LCL_Field& b) const = 0;
        virtual bool operator>(const LCL_Field& b) const {return !((*this)<b);}
        virtual bool operator<=(const LCL_Field& b) const {return ((*this)<b)||((*this)==b);}
        virtual bool operator>=(const LCL_Field& b) const {return ((*this)>b)||((*this)==b);}
};

#endif // LCL_ORDERED_FIELD_HEADER
