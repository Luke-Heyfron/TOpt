#ifndef LCL_HEADER
#define LCL_HEADER

#define LCL_USE_LCL_NAMESPACES

#include "LCL/Core/LCL_ConsoleIn.h"
#include "LCL/Core/LCL_ConsoleOut.h"
#include "LCL/Core/LCL_Utils.h"
#include "LCL/Core/LCL_Array.h"
#include "LCL_Array_imp1.h"
#include "LCL/Maths/LCL_MathsUtils.h"
#include "LCL/Maths/LCL_Field.h"
#include "LCL/Maths/LCL_Matrix.h"
#include "LCL_Matrix_imp1.h"
#include "LCL/Maths/LCL_OrderedField.h"
#include "LCL/Maths/LCL_FiniteField.h"
#include "LCL_FiniteField_imp1.h"
#include "LCL/Menu/LCL_Menu.h"
#include "LCL/Menu/LCL_MenuUtils.h"
#include "LCL/LCL_Bool.h"
#include "LCL/LCL_BooleanMatrix.h"
#include "LCL_BooleanMatrix_imp1.h"
#include "LCL/LCL_BoundedInt.h"
#include "LCL/LCL_Int.h"
#include "LCL/LCL_Mat_GF2.h"
#include "LCL/LCL_RealMatrix.h"
#include "LCL_RealMatrix_imp1.h"


#ifdef LCL_USE_LCL_NAMESPACES
    using namespace LCL_ConsoleIn;
    using namespace LCL_ConsoleOut;
    using namespace LCL_Utils;
    using namespace LCL_Int;
    using namespace LCL_Bool;
    using namespace LCL_Mat_GF2;
    using namespace LCL_MathsUtils;

#endif // LCL_USE_LCL_NAMESPACES

#endif // LCL_HEADER
