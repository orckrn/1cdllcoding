// AddInCOM.cpp : Implementation of CAddInCOM

#include "stdafx.h"
#include "AddInCOM.h"

static IAsyncEvent *pAsyncEvent = NULL;

VOID CALLBACK MyTimerProc(
    HWND hwnd,    // handle of window for timer messages
    UINT uMsg,    // WM_TIMER message
    UINT idEvent, // timer identifier
    DWORD dwTime  // current system time
);

// CAddInCOM
//---------------------------------------------------------------------------//
BOOL CAddInCOM::LoadProperties()
{  
    if (!m_iProfile)
        return FALSE;

    HRESULT hRes;
    wchar_t* csProperty = L"Enabled:0";; 
    VARIANT varEnabled;

    ::VariantInit(&varEnabled);
    V_VT(&varEnabled) = VT_I4;
    hRes = m_iProfile->Read(::SysAllocString(csProperty),&varEnabled,NULL);

    if (hRes != S_OK) 
        return FALSE;

    m_boolEnabled = V_I4(&varEnabled)?FALSE:TRUE;

    return TRUE;
}
//---------------------------------------------------------------------------//
void CAddInCOM::SaveProperties()
{
    if (!m_iProfile)
        return;

    wchar_t* csProperty = L"Enabled";
    VARIANT varEnabled;

    ::VariantInit(&varEnabled);
    V_VT(&varEnabled) = VT_I4;
    V_I4(&varEnabled) = m_boolEnabled?1:0;
    m_iProfile->Write(::SysAllocString(csProperty),&varEnabled);
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::Init(IDispatch *pConnection)
{
    if (!pConnection)
        return E_FAIL;

    m_iConnect = pConnection;
    m_iConnect->AddRef();

    UINT tiCount = 0;

    m_iConnect->QueryInterface(IID_IErrorLog,(void **)&m_iErrorLog);
    m_iConnect->QueryInterface(IID_IAsyncEvent,(void **)&m_iAsyncEvent);
    m_iConnect->QueryInterface(IID_IStatusLine,(void **)&m_iStatusLine);
    m_iConnect->GetTypeInfoCount(&tiCount);

    BSTR csProfileName = ::SysAllocString(L"TestAddInProfName");
    m_iProfile = NULL;
    pConnection->QueryInterface(IID_IPropertyProfile,(void **)&m_iProfile);

    if (m_iProfile) 
    {
        m_iProfile->RegisterProfileAs(csProfileName);

        if (LoadProperties() == FALSE)
            m_boolEnabled = TRUE;
    }
    ::SysFreeString(csProfileName);

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::Done()
{
    if (m_uiTimer != 0)
        ::KillTimer(NULL,m_uiTimer);

    SaveProperties();

    if (m_iStatusLine) 
        m_iStatusLine->Release();

    if (m_iProfile) 
        m_iProfile->Release();

    if (m_iAsyncEvent) 
        m_iAsyncEvent->Release();

    if (m_iErrorLog) 
        m_iErrorLog->Release();

    if (m_iConnect) 
        m_iConnect->Release();

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetInfo(SAFEARRAY **pInfo)
{
    // Component should put supported component technology version 
    // in VARIANT at index 0     
    long lInd = 0;
    VARIANT varVersion;
    V_VT(&varVersion) = VT_I4;
    // This component supports 2.0 version
    V_I4(&varVersion) = 2000;
    SafeArrayPutElement(*pInfo,&lInd,&varVersion);
    
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// ILanguageExtender
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::RegisterExtensionAs(BSTR *bstrExtensionName)
{
    wchar_t* csExtenderName = L"AddInExtension";
    *bstrExtensionName = ::SysAllocString(csExtenderName);

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetNProps(long *plProps)
{
    //// You may delete next lines and add your own implementation code here

    *plProps = ePropLast;

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::FindProp(BSTR bstrPropName,long *plPropNum)
{
    *plPropNum = -1;
    CAtlString csPropName = OLE2T(bstrPropName);

    if (TermString(IDS_TERM_ENABLED,0) == csPropName) 
    {
        *plPropNum = 0;
    }
    else if (TermString(IDS_TERM_ENABLED,1) == csPropName) 
    {
        *plPropNum = 0;
    }
    else if (TermString(IDS_TERM_TIMERPRESENT,0) == csPropName) 
    {
        *plPropNum = 1;
    }
    else if (TermString(IDS_TERM_TIMERPRESENT,1) == csPropName) 
    {
        *plPropNum = 1;
    }

    return (*plPropNum == -1) ? S_FALSE : S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetPropName(long lPropNum,long lPropAlias,
                                    BSTR *pbstrPropName)
{
    CAtlString csPropName = L""; 

    switch(lPropNum)
    {
    case ePropIsEnabled:
        csPropName = TermString(IDS_TERM_ENABLED,lPropAlias);
        *pbstrPropName = csPropName.AllocSysString();
        break;
    case ePropIsTimerPresent:
        csPropName = TermString(IDS_TERM_TIMERPRESENT,lPropAlias);
        *pbstrPropName = csPropName.AllocSysString();
        break;
    default:
        *pbstrPropName = csPropName.AllocSysString();
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetPropVal(long lPropNum,VARIANT *pvarPropVal)
{
    ::VariantInit(pvarPropVal);

    switch(lPropNum)
    {
    case ePropIsEnabled:
        V_VT(pvarPropVal) = VT_I4;
        V_I4(pvarPropVal) = m_boolEnabled?1:0;
        break;
    case ePropIsTimerPresent:
        V_VT(pvarPropVal) = VT_I4;
        V_I4(pvarPropVal) = 1;
        break;
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::SetPropVal(long lPropNum,VARIANT *pvarPropVal)
{
    switch(lPropNum)
    { 
    case ePropIsEnabled:
        if (V_VT(pvarPropVal) != VT_I4)
            return S_FALSE;
        m_boolEnabled = V_I4(pvarPropVal)?1:0;
        break;
    case ePropIsTimerPresent:
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::IsPropReadable(long lPropNum,BOOL *pboolPropRead)
{
    switch(lPropNum)
    { 
    case ePropIsEnabled:
        *pboolPropRead = TRUE;
        break;
    case ePropIsTimerPresent:
        *pboolPropRead = TRUE;
        break;
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::IsPropWritable(long lPropNum,BOOL *pboolPropWrite)
{
    switch(lPropNum)
    { 
    case ePropIsEnabled:
        *pboolPropWrite = TRUE;
        break;
    case ePropIsTimerPresent:
        *pboolPropWrite = FALSE;
        break;
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetNMethods(long *plMethods)
{
    *plMethods = eMethLast;

    if (this->m_iStatusLine)
        m_iStatusLine->SetStatusLine(L"Query NMethods");

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::FindMethod(BSTR bstrMethodName, long *plMethodNum)
{
    USES_CONVERSION;

    *plMethodNum = -1;
    CAtlString csPropName = OLE2T(bstrMethodName);

    if (TermString(IDS_TERM_ENABLE,0) == csPropName) 
    {
        *plMethodNum = 0;
    }
    else if (TermString(IDS_TERM_ENABLE,1) == csPropName) 
    {
        *plMethodNum = 0;
    }
    else if (TermString(IDS_TERM_DISABLE,0) == csPropName) 
    {
        *plMethodNum = 1;
    }
    else if (TermString(IDS_TERM_DISABLE,1) == csPropName) 
    {
        *plMethodNum = 1;
    }
    else if (TermString(IDS_TERM_SHOWSTATUS,0) == csPropName) 
    {
        *plMethodNum = 2;
    }
    else if (TermString(IDS_TERM_SHOWSTATUS,1) == csPropName) 
    {
        *plMethodNum = 2;
    }
    else if (TermString(IDS_TERM_STARTTIMER,0) == csPropName) 
    {
        *plMethodNum = 3;
    }
    else if (TermString(IDS_TERM_STARTTIMER,1) == csPropName) 
    {
        *plMethodNum = 3;
    }
    else if (TermString(IDS_TERM_STOPTIMER,0) == csPropName) 
    {
        *plMethodNum = 4;
    }
    else if (TermString(IDS_TERM_STOPTIMER,1) == csPropName) 
    {
        *plMethodNum = 4;
    }
    else if (TermString(IDS_TERM_OPENFORM,0) == csPropName) 
    {
        *plMethodNum = 5;
    }
    else if (TermString(IDS_TERM_OPENFORM,1) == csPropName) 
    {
        *plMethodNum = 5;
    }

    return (*plMethodNum == -1) ? S_FALSE : S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetMethodName(long lMethodNum,long lMethodAlias,
                                      BSTR *pbstrMethodName)
{

    CAtlString csMethodName;

    switch(lMethodNum)
    { 
    case eMethEnable:
        csMethodName = TermString(IDS_TERM_ENABLE,lMethodAlias);
        break;
    case eMethDisable:
        csMethodName = TermString(IDS_TERM_DISABLE,lMethodAlias);
        break;
    case eMethShowInStatusLine:
        csMethodName = TermString(IDS_TERM_SHOWSTATUS,lMethodAlias);
        break;
    case eMethStartTimer:
        csMethodName = TermString(IDS_TERM_STARTTIMER,lMethodAlias);
        break;
    case eMethStopTimer:
        csMethodName = TermString(IDS_TERM_STOPTIMER,lMethodAlias);
        break;
    case eMethOpenForm:
        csMethodName = TermString(IDS_TERM_OPENFORM,lMethodAlias);
        break;
    default:
        *pbstrMethodName = SysAllocString(csMethodName);
        return S_FALSE;
    }
    
    *pbstrMethodName = ::SysAllocString(csMethodName);

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetNParams(long lMethodNum,long *plParams)
{
    *plParams = 0;

    switch(lMethodNum)
    { 
    case eMethEnable:
        *plParams = 0;
        break;
    case eMethDisable:
        *plParams = 0;
        break;
    case eMethShowInStatusLine:
        *plParams = 1;
        break;
    case eMethStartTimer:
        *plParams = 0;
        break;
    case eMethStopTimer:
        *plParams = 0;
        break;
    case eMethOpenForm:
        *plParams = 0;
        break;
    default:
        return S_FALSE;
    }
    
    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::GetParamDefValue(long lMethodNum,long lParamNum,
                                         VARIANT *pvarParamDefValue)
{
    ::VariantInit(pvarParamDefValue);

    switch(lMethodNum)
    { 
    case eMethEnable:
    case eMethDisable:
    case eMethShowInStatusLine:
    case eMethStartTimer:
    case eMethStopTimer:
    case eMethOpenForm:
        /* There are no parameter values by default */
        break;
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::HasRetVal(long lMethodNum,BOOL *pboolRetValue)
{
    switch(lMethodNum)
    { 
    case eMethEnable:
    case eMethDisable:
    case eMethShowInStatusLine:
    case eMethStartTimer:
    case eMethStopTimer:
    case eMethOpenForm:
        *pboolRetValue = FALSE;
        break;
    default:
        return S_FALSE;
    }

    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::CallAsProc(long lMethodNum, SAFEARRAY **paParams)
{
    USES_CONVERSION;

    switch(lMethodNum)
    { 
    case eMethEnable:
        m_boolEnabled = TRUE;
        break;
    case eMethDisable:
        m_boolEnabled = FALSE;
        break;
    case eMethShowInStatusLine:
        if (m_iStatusLine)
        {
            VARIANT var = GetNParam(*paParams,0);
            m_iStatusLine->SetStatusLine(V_BSTR(&var));
            Sleep(5000);
            ::VariantClear(&var);
        }
        else if (m_iErrorLog)
        {
            CAtlString csErrorSource,csErrorDescription,csNull = L"";
            EXCEPINFO eiInfo;
            eiInfo.wCode = ADDIN_E_VERY_IMPORTANT;
            eiInfo.scode = S_OK;
            csErrorSource.LoadString(IDS_ERROR_SOURCE);
            eiInfo.bstrSource = csErrorSource.AllocSysString();
            csErrorDescription.LoadString(IDS_ERROR_DESCRIPTION);
            eiInfo.bstrDescription = csErrorDescription.AllocSysString();
            m_iErrorLog->AddError(csNull.AllocSysString(),&eiInfo);
        }

        break;
    case eMethStartTimer:
        pAsyncEvent = m_iAsyncEvent;
        m_uiTimer = ::SetTimer(NULL,0,100,MyTimerProc);
        break;
    case eMethStopTimer:
        if (m_uiTimer != 0) 
            ::KillTimer(NULL,m_uiTimer);
        m_uiTimer = 0;
        pAsyncEvent = NULL;
        break;
    case eMethOpenForm:
        break;
    default:
        return S_FALSE;
    }
    
    return S_OK;
}
//---------------------------------------------------------------------------//
STDMETHODIMP CAddInCOM::CallAsFunc(long lMethodNum,VARIANT *pvarRetValue,
                                   SAFEARRAY **paParams)
{
    ::VariantInit(pvarRetValue);
    V_VT(pvarRetValue) = VT_I4;
    V_I4(pvarRetValue) = 0;

    return S_OK;;
}
//---------------------------------------------------------------------------//
CAtlStringW CAddInCOM::TermString(UINT uiResID,long nAlias)
{
    USES_CONVERSION;
    CAtlStringA cs;
    cs.LoadString(uiResID);

    int iInd = cs.Find(',');
    if (iInd == -1)
        return CAtlString(cs);
    switch(nAlias)
    {
    case 0: // First language
        return CAtlString(cs.Left(iInd));
    case 1: // Second language
        return CAtlString(cs.Mid(iInd+1));
    default:
        return CAtlString(L"");
    };
}
//---------------------------------------------------------------------------//
VARIANT CAddInCOM::GetNParam(SAFEARRAY *pArray,long lIndex)
{
    _ASSERT(pArray);
    _ASSERT(pArray->fFeatures | FADF_VARIANT);

    VARIANT vt;
    HRESULT hRes = ::SafeArrayGetElement(pArray,&lIndex,&vt);
    _ASSERT(hRes == S_OK);

    return vt;
}
//---------------------------------------------------------------------------//
HRESULT CAddInCOM::SetLocale(BSTR bstrLocale)
{
    return S_OK;
}
//---------------------------------------------------------------------------//
VOID CALLBACK MyTimerProc(
  HWND hwnd,    // handle of window for timer messages
  UINT uMsg,    // WM_TIMER message
  UINT idEvent, // timer identifier
  DWORD dwTime  // current system time
)
{
    BSTR who, what, data;
    CAtlString csSource(L"ComponentCOM"),csMessage(L"Timer"),csData;
    wchar_t wstime[34];
    ::_ultow(dwTime, wstime, 10);
    csData.Append(wstime);
    
    if (pAsyncEvent)
    {
        who = csSource.AllocSysString();
        what = csMessage.AllocSysString();
        data = csData.AllocSysString();
        pAsyncEvent->ExternalEvent(who, what, data);
    }
}
//---------------------------------------------------------------------------//

