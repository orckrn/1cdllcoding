#pragma once
#include <unknwn.h>
#include <atlbase.h>

enum AddInComponentType
{
    eAddInCom = 1,
    eAddInNative,
    eAddInvalid = -1
};

EXTERN_C const IID LIBID_AddInWebLib;
extern const AddInComponentType typeComponent;
