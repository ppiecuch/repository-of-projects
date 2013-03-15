#ifndef _YON_CORE_DELEGATE_H_
#define _YON_CORE_DELEGATE_H_

#include "yonList.h"

namespace yon{
namespace core{

// 0 parameters...
#define _YON_DELEGATE_SUFFIX             0
#define _YON_DELEGATE_TEMPLATE_PARAMS
#define _YON_DELEGATE_TEMPLATE_ARGS
#define _YON_DELEGATE_PARAMS
#define _YON_DELEGATE_ARGS

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS
    
// 1 parameter...
#define _YON_DELEGATE_SUFFIX             1
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1
#define _YON_DELEGATE_PARAMS             TP1 p1
#define _YON_DELEGATE_ARGS               p1

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 2 parameters...
#define _YON_DELEGATE_SUFFIX             2
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2
#define _YON_DELEGATE_ARGS               p1, p2

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 3 parameters...
#define _YON_DELEGATE_SUFFIX             3
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3
#define _YON_DELEGATE_ARGS               p1, p2, p3

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 4 parameters...
#define _YON_DELEGATE_SUFFIX             4
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 5 parameters...
#define _YON_DELEGATE_SUFFIX             5
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 6 parameters...
#define _YON_DELEGATE_SUFFIX             6
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5, class TP6
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5, TP6
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5, p6

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 7 parameters...
#define _YON_DELEGATE_SUFFIX             7
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5, TP6, TP7
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5, p6, p7

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 8 parameters...
#define _YON_DELEGATE_SUFFIX             8
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5, TP6, TP7, TP8
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7, TP8 p8
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5, p6, p7, p8

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 9 parameters...
#define _YON_DELEGATE_SUFFIX             9
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5, TP6, TP7, TP8, TP9
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7, TP8 p8, TP9 p9
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5, p6, p7, p8, p9

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

// 10 parameters...
#define _YON_DELEGATE_SUFFIX             10
#define _YON_DELEGATE_TEMPLATE_PARAMS    , class TP1, class TP2, class TP3, class TP4, class TP5, class TP6, class TP7, class TP8, class TP9, class TP10
#define _YON_DELEGATE_TEMPLATE_ARGS      , TP1, TP2, TP3, TP4, TP5, TP6, TP7, TP8, TP9, TP10
#define _YON_DELEGATE_PARAMS             TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7, TP8 p8, TP9 p9, TP10 p10
#define _YON_DELEGATE_ARGS               p1, p2, p3, p4, p5, p6, p7, p8, p9, p10

#include "delegate_impl.h"

#undef _YON_DELEGATE_SUFFIX
#undef _YON_DELEGATE_TEMPLATE_PARAMS
#undef _YON_DELEGATE_TEMPLATE_ARGS
#undef _YON_DELEGATE_PARAMS
#undef _YON_DELEGATE_ARGS

}
}



#endif