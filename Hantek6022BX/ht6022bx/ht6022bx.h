#ifndef HT6022BX_H
#define HT6022BX_H

#include "ht6022bx_global.h"
#include "firmware6022bx.h"
class HT6022BXSHARED_EXPORT HT6022bx
{

public:
    HT6022bx();
    Firmware6022BX *Fw;
};

#endif // HT6022BX_H
