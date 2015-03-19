#include "stdafx.h"
#include "config.h"
#include "AddInIE_i.h"

const IID LIBID_AddInWebLib = LIBID_AddInIELib;

LPCOLESTR sAppId = L"AddInIE";
//---------------------------------------------------------------------------//
LPCOLESTR sAddInServiceEx = L"AddInIE.AddInServiceEx";
//---------------------------------------------------------------------------//
LPCOLESTR sAddInSite = L"AddInIE.AddInSite";
//---------------------------------------------------------------------------//
LPCOLESTR sAdapterCOM = L"AddInIE.AdapterCOM";
//---------------------------------------------------------------------------//
LPCOLESTR sAdapterNative = L"AddInIE.AdapterNative";
//---------------------------------------------------------------------------//

const wchar_t *nameFilePrj = L"AddInIE.dll";
const wchar_t *nameFileComponent = L"AddInNative.dll";
const AddInComponentType typeComponent = eAddInNative;
