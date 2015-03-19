#include "nsIID.h"
#include "config.h"


//---------------------------------------------------------------------------//
const char ADDINSERVICEEX_CONTRACTID[] = "@vendor.ru/ClassService;1";
const char ADDINSERVICEEX_CLASSNAME[] = "My AddInFF";
// {20C7B0F7-EBE0-4802-98E1-ED78E2583C1C}
const nsIID ADDINSERVICEEX_CID = { 0x20c7b0f7, 0xebe0, 0x4802, { 0x98, 0xe1, 
        0xed, 0x78, 0xe2, 0x58, 0x3c, 0x1c } };
//---------------------------------------------------------------------------//
const char ADDINSITE_CONTRACTID[] = "@vendor.ru/ClassSite;1";
const char ADDINSITE_CLASSNAME[] = "My AddInFF";
// {A1525B05-BEA6-4a11-9142-C058C0C60D09}
const nsIID ADDINSITE_CID = { 0xa1525b05, 0xbea6, 0x4a11, { 0x91, 0x42, 0xc0, 
        0x58, 0xc0, 0xc6, 0xd, 0x9 } };
//---------------------------------------------------------------------------//
const char ADAPTERNATIVE_CONTRACTID[] = "@vendor.ru/ClassNative;1";
const char ADAPTERNATIVE_CLASSNAME[] = "My AddInFF";
// {553A941B-3177-463b-8566-1231F12B1680}
const nsIID ADAPTERNATIVE_CID = { 0x553a941b, 0x3177, 0x463b, { 0x85, 0x66, 
        0x12, 0x31, 0xf1, 0x2b, 0x16, 0x80 } };
//---------------------------------------------------------------------------//
const char ADAPTERCOM_CONTRACTID[] = "@vendor.ru/ClassCOM;1";
const char ADAPTERCOM_CLASSNAME[] = "My AddInFF";
// {FC15CD6C-CCD8-4163-B280-DC199F4659DE}
const nsIID ADAPTERCOM_CID = { 0xfc15cd6c, 0xccd8, 0x4163, { 0xb2, 0x80, 0xdc, 
        0x19, 0x9f, 0x46, 0x59, 0xde } };
//---------------------------------------------------------------------------//
#ifndef __linux__
const char nameFilePrj[] = "AddInFF.dll";
const char nameFileComponent[] = "AddInNative.dll";
#else
const char nameFilePrj[] = "libAddInFF.so";
const char nameFileComponent[] = "libAddInNative.so";
#endif //__linux__
const AddInComponentType typeComponent = eAddInNative;
//---------------------------------------------------------------------------//
const char module[] = "AddInFFModule";
//---------------------------------------------------------------------------//