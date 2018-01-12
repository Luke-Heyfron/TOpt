#include <LCL_BoundedInt.h>

#include "LCL_ConsoleOut.h"

LCL_BoundedInt::LCL_BoundedInt() {
    ;
}

LCL_BoundedInt::LCL_BoundedInt(int in_v, int in_min, int in_max) {
    _v = in_v;
    _min = in_min;
    _max = in_max;
}

LCL_BoundedInt& LCL_BoundedInt::operator=(const int in_v) {
    if((in_v>=_min)&&(in_v<=_max)) {
        _v = in_v;
    } else {
        LCL_ConsoleOut::error("Input value out of range.", "operator=", "LCL_BoundedInt");
    }
}

LCL_BoundedInt& LCL_BoundedInt::operator()(const int in_min, const int in_max) {
    _min = in_min;
    _max = in_max;
}
