#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "nsIGenericFactory.h"

enum AddInComponentType
{
    eAddInCom = 1,
    eAddInNative,
    eAddInvalid = -1
};

extern const char ADDINSERVICEEX_CONTRACTID[];
extern const char ADDINSERVICEEX_CLASSNAME[];
extern const nsIID ADDINSERVICEEX_CID;

extern const char ADDINSITE_CONTRACTID[];
extern const char ADDINSITE_CLASSNAME[];
extern const nsIID ADDINSITE_CID;

extern const char ADAPTERNATIVE_CONTRACTID[];
extern const char ADAPTERNATIVE_CLASSNAME[];
extern const nsIID ADAPTERNATIVE_CID;

extern const char ADAPTERCOM_CONTRACTID[];
extern const char ADAPTERCOM_CLASSNAME[];
extern const nsIID ADAPTERCOM_CID;

extern const char module[];
extern const char nameFilePrj[];
extern const char nameFileComponent[];
extern const AddInComponentType typeComponent;

#ifndef __linux__
extern nsModuleComponentInfo components[4];
#else
extern nsModuleComponentInfo components[3];
#endif //__linux__

#endif //__CONFIG_H__
