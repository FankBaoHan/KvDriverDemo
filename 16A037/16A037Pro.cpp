/************************************************
Copyright(c) 2006, AsiaControl Company
All rights reserved.
File Name:   16A037Pro.cpp
Class Name:  C16A037Pro
Brief:       Driver project class
History: 
Date	         Author  	       Remarks
Aug. 2006	     Develop Dep.1     
************************************************/

#include "stdafx.h"
#include "16A037.h"
#include "16A037Pro.h"
#include "reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C16A037Pro
IMPLEMENT_DYNCREATE(C16A037Pro, CCmdTarget)

C16A037Pro::C16A037Pro() 
{
	EnableAutomation();	
	CDevBase *pMod = NULL ;
	m_nLastErrorCode = 0;
	
	pMod = new CDev16A037 ;
	pMod->m_pSerialCom = &m_ComObj;
	pMod->SetProPtr(this);
	m_DevList.AddTail(pMod);
	CDebug::CDebug(); 
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF| _CRTDBG_LEAK_CHECK_DF); //Test memory leak, EVC not support.
	AfxOleLockApp();
	
}

C16A037Pro::~C16A037Pro()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	AfxOleUnlockApp();
	while(!m_DevList.IsEmpty())
	{
		CDevBase * pMod = ( CDevBase* )m_DevList.RemoveTail();  //detele all device objects for varlist.
		delete pMod;
		
	}
}

void C16A037Pro::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.	
	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(C16A037Pro, CCmdTarget)
//{{AFX_MSG_MAP(CTestPro)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(C16A037Pro, CCmdTarget)
//{{AFX_DISPATCH_MAP(CTestPro)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ITestPro to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3C8EB874-0F54-4C68-A7AF-0B773AF6A65E}
static const IID IID_ITestPro = { 0x3C8EB874, 0xF54, 0x4C68, { 0xA7, 0xAF, 0xB, 0x77, 0x3A, 0xF6, 0xA6, 0x5E } } ;

BEGIN_INTERFACE_MAP(C16A037Pro, CCmdTarget)
INTERFACE_PART(C16A037Pro, IID_ITestPro, Dispatch)
INTERFACE_PART(C16A037Pro, IID_ProtocolImp, ProtocolImp)
INTERFACE_PART(C16A037Pro, IID_ProtocolImp2, ProtocolImp2)
END_INTERFACE_MAP()

//The following is CLSID
#if defined(_UNICODE) || defined(UNICODE)  || defined(_WIN32_WCE_CEPC) || defined(_ARM_) // for UNICODE and EVC compile
// {F7B8CFAC-977C-41D9-B22C-6029DEFF66C8}
IMPLEMENT_OLECREATE(C16A037Pro, "16A037.16A037Pro", 0xf7b8cfac, 0x977c, 0x41d9, 0xb2, 0x2c, 0x60, 0x29, 0xde, 0xff, 0x66, 0xc8)
#else
// {2D86669F-EEDC-4D41-B6B2-58ED5E3553D0} //for MBCS compile
IMPLEMENT_OLECREATE(C16A037Pro, "16A037.16A037Pro", 0x2d86669f, 0xeedc, 0x4d41, 0xb6, 0xb2, 0x58, 0xed, 0x5e, 0x35, 0x53, 0xd0)
#endif 


////////////////////////////////////////////////////////////////////////////
//				ProtocolImp and  ProtocolImp2 IMPLEMENT code
////////////////////////////////////////////////////////////////////////////

//
STDMETHODIMP_(ULONG) C16A037Pro::XProtocolImp::AddRef(void)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
    return pThis->ExternalAddRef();
}
//
STDMETHODIMP_(ULONG) C16A037Pro::XProtocolImp::Release(void)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
    return pThis->ExternalRelease();
}
//Query the com interface, not to deal 
STDMETHODIMP C16A037Pro::XProtocolImp::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
    return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(ULONG) C16A037Pro::XProtocolImp2::AddRef(void)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) C16A037Pro::XProtocolImp2::Release(void)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	return pThis->ExternalRelease();
}

STDMETHODIMP C16A037Pro::XProtocolImp2::QueryInterface( REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

////////////////////////////////////////////////////////////////////////////
//	kernel	function below should be called in touchexplorer environment
////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*   Name
		StrToDevAddr
*	Type
		public
*	Function
		Check the input device address string and transforme it into lpDevAddr.
*	Return Value
		return true and transform str into lpDevAddr if successful; otherwise false.
*	Parameters
		str
			[in] Pointer a null-terminated string that identifies address string.
		lpDevAddr
			[in,out] Pointer a DEVADDR variant, see "DATATYPE.H" in detail.
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::StrToDevAddr(const TCHAR* str, LPVOID lpDevAddr)
{
	METHOD_PROLOGUE(C16A037Pro, ProtocolImp); 
	CDebug::ShowImpMessage(_T("C16A037Pro::StrToDevAddr"));
	
	//Check the parameter pointer
	ASSERT(str != NULL);
	ASSERT(lpDevAddr != NULL);
	DEVADDR *pDevAddr = (DEVADDR*)lpDevAddr;
	CDevBase* pDev = pThis->GetFirstDevObj() ;	
	if (pDev)
	{
		return pDev->StrToDevAddr(str, pDevAddr) ;
	}
	else
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}
	
	return TRUE;
	
}

/****************************************************************************
*   Name
		GetRegisters
*	Type
		public
*	Function
		The GetRegister retrieves a register list for the specified szDeviceName
*	Return Value
		return true if successful; otherwise false.
*	Parameters
		szDeviceName
			[in] Pointer to a null-terminated string that identifies the register
			list to retrieve
		ppReg
			[out] Pointer to a variable that receives the address of register list
		pRegNum
			[out] Pointer to a variable that receives the size of register list
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::GetRegisters(const TCHAR* szDeviceName, 
																		   LPVOID * ppRegs, int *pRegNum)
{	
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::GetRegisters"));
    ASSERT(szDeviceName != NULL);
	ASSERT(ppRegs != NULL);
	ASSERT(pRegNum != NULL);
	CDevBase* pDev = pThis->GetDevObj(szDeviceName) ;	
	if (pDev)
	{
		return pDev->GetRegisters(ppRegs,pRegNum) ;
	}
	else
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}
	
	return TRUE;
}

/****************************************************************************
*   Name
		ConvertUserConfigToVar
*	Type
		public
*	Function
		Convert the variant string to KINGVIEW's structural variables (PLCVAR, see "DATATYPE.h").
*	Return Value
		return s_ok if successful; otherwise s_false.
*	Parameters
		lpDbItem
			[in]  Pointer to a MiniDbItem variant
		lpVar
			[in,out] Pointer to a PLCVAR variant
*****************************************************************************/
STDMETHODIMP_(WORD) C16A037Pro::XProtocolImp:: ConvertUserConfigToVar( LPVOID lpDbItemItem, LPVOID lpVar)
{
	METHOD_PROLOGUE(C16A037Pro, ProtocolImp); 
	CDebug::ShowImpMessage(_T("C16A037Pro::ConvertUserConfigToVar"));
	ASSERT(lpDbItemItem != NULL);
	ASSERT(lpVar != NULL);
	
	MiniDbItem * pDbItem = (MiniDbItem*)lpDbItemItem;
	PPLCVAR pPlcVar = (PPLCVAR) lpVar;
	CDevBase* pDev = pThis->GetDevObj(pDbItem->szDevName);	//
//	CDevBase* pDev = pThis->GetFirstDevObj() ;	
	if (pDev)
	{
		return pDev->ConvertUserConfigToVar(pDbItem, pPlcVar);
	}
	else
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}
	
	return TRUE;
}

/****************************************************************************
*	Brief
		Not used. return true directly.
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp2::SetInitialString(TCHAR* pDeviceName, LPVOID lpDevAddr,LPVOID InitialString)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	//always return true
	return  TRUE;
}

/****************************************************************************
*   Name
		GetLastError
*	Type
		public
*	Function
		Return to the latest information when  an error operation occurs.
*	Return Value
		Returned a specific null-terminated string;
*	Parameters
		null
*	Remarks
		It Always can be called in TouchExplorer Entironment when an erro occur.
*****************************************************************************/
STDMETHODIMP_(TCHAR *) C16A037Pro::XProtocolImp::GetLastError()
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);	
	CDebug::ShowImpMessage(_T("C16A037Pro::GetLastError"));
    const int nErrorStringLength = 256;
	static TCHAR error_info[nErrorStringLength] = {0};
	static int nPreviousError = NO_ERROR;  

	//If there are no error
	if(NO_ERROR == pThis->m_nLastErrorCode ) 
	{
		ZeroMemory(error_info, sizeof(error_info));
	}	

	//need to reload resource
	else if( nPreviousError != pThis->m_nLastErrorCode ) 
	{	
		//load the Dynamics  DLL 
		HMODULE  hResource = ::LoadLibrary( _T( "StringResource.dll" ));
		if(hResource)
		{  
			int	nLoaded = LoadString(hResource, pThis->m_nLastErrorCode,error_info, nErrorStringLength);
			FreeLibrary(hResource);

			//not found the error string message.
			if( 0 == nLoaded )
			{
				//_tcscpy( error_info, _T( "Undefined Error;\nPlease reference the handbook!" ) );
				nLoaded = ::LoadString(hResource, ERR_ERRORINFO_NOTFOUND, error_info, nErrorStringLength);
				ASSERT(nLoaded != 0);
				if(nLoaded == 0)
				{	
					//Please down load the new DLL from company's website. 
					_tcscpy(error_info, _T("Resource file (StringResource.dll) is too old,Please update it."));
				}
			}
			else
			{
				nPreviousError = pThis->m_nLastErrorCode;
			}
		}  
		//not found "StringResource.dll"
		else
		{   
			_tcscpy( error_info, _T( "File StringResource.dll missed" )); 
		}
	}

	return error_info;	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//			Kernel Function below should be called in touchview environment
//////////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*   Name
		CloseComDevice
*	Type
		public
*	Function
		Open a communication port. Always open a serial com or socket .
*	Return Value
		return true if successful; otherwise false.
*	Parameters
		nDeviceType
			[in] Specifies DeviceType. (No use)
		lpInitData
			[in] Point to an ComDevice variant to initial com
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::OpenComDevice( int nDeviceType, LPVOID lpInitData)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp); 
	CDebug::ShowImpMessage(_T("C16A037Pro::OpenComDevice"));

	ComDevice* pcomdev = static_cast<ComDevice*>(lpInitData);	
	
	return pThis->m_ComObj.OpenCom(*pcomdev);
}


/****************************************************************************
*   Name
		CloseComDevice
*	Type
		public
*	Function
		Close communication Device
*	Return Value
		always true.
*	Parameters
		null 
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::CloseComDevice()
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::CloseComDevice"));
	return pThis->m_ComObj.CloseCom();
	
}

/****************************************************************************
*   Name
		InitialDevice
*	Type
		public
*	Function
		Initial device for the specified pDeviceName
*	Return Value
		It cannot be called in Virtual COM port project.
*	Parameters
		pDeviceName
			[in] Pointer to a null-terminated string that identifies the 
			specified pDeviceName 
		nUnitAddr
			[in]  Specifies the Unit Address 
		lpDevAddr
			[in]  Point to a DEVADDR variant 
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::InitialDevice(const TCHAR*  pDeviceName, int nUnitAddr, LPVOID lpDevAddr)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::InitialDevice"));

	return TRUE;	
}


/****************************************************************************
*	Brief
		Not used. return true directly.
*****************************************************************************/
STDMETHODIMP_(int) C16A037Pro::XProtocolImp::LoadDeviceInfo( const TCHAR* sProd, const TCHAR * sDevName, int nType )
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::LoadDeviceInfo"));
    return  TRUE;
}

/****************************************************************************
*   Name
		AddVarToPacket
*	Type
		public
*	Function
		Whether or not the var can add to the lpPacket
*	Return Value
		return true if the lpVar can be added into the lpPacket;otherwise false. 
*	Parameters
		lpVar 
			[in]  Pointer to a PLCVAR variant whether or not add into the packet
		nVarAccessType
			[in]  Specifies the lpvar Access Type.(NO USED)
		lpPacket
			[int] Pointer to a PACKET variant whether or not contain the lpVar
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp:: AddVarToPacket( LPVOID lpVar, int nVarAccessType, LPVOID lpPacket)
{    
	METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::AddVarToPacket"));
	ASSERT(lpVar != NULL);
	ASSERT(lpPacket != NULL);
	ASSERT(nVarAccessType == PT_READ || nVarAccessType == PT_WRITE);
	PPACKET pPac = (PACKET *)lpPacket;
	PPLCVAR pVar = (PLCVAR *)lpVar;	
	CDevBase* pDev = pThis->GetDevObj(pPac->pszDevName);	
	if ( pDev )
	{
		return pDev->AddVarToPacket(lpVar,nVarAccessType,lpPacket) ;
	}
	else
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}	
	return TRUE;
}

/****************************************************************************
*   Name
		ProcessPacket
*	Type
		public
*	Function
		Processing of data packets in 5.0 interface (old interface in brief)
*	Return Value
		return true directly. 
*	Parameters
		lpPacket
			[in,out] Pointer a PACKET variant
*****************************************************************************/
STDMETHODIMP_(int) C16A037Pro::XProtocolImp:: ProcessPacket(LPVOID lpPacket)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::ProcessPacket"));
	return TRUE;
}

/****************************************************************************
*   Name
		ProcessPacket2
*	Type
		public
*	Function
		Processing of data packets in 5.0 interface (new interface in brief), must be implemented.
*	Return Value
		return true if successful; otherwise false. 
*	Parameters
		lpPacket
			[in,out] Pointer a PACKET variant
*****************************************************************************/
STDMETHODIMP_(int) C16A037Pro::XProtocolImp2:: ProcessPacket2(LPVOID lpPacket)
{
	METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	CDebug::ShowImpMessage(_T("C16A037Pro::ProcessPacket2"));
	//check the pointer
	ASSERT(lpPacket != NULL);
	PPACKET pPack = (PPACKET) lpPacket;
	
	CDevBase* pDev = pThis->GetDevObj(pPack->pszDevName);	
	if (pDev)
	{
		return pDev->ProcessPacket2(lpPacket);
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
}

/****************************************************************************
*   Name
		TryConnect
*	Type
		public
*	Function
		Try to connect with the Device when the communication failed.
*	Return Value
		return true if successful; otherwise false. 
*	Parameters
		pDeviceName
			[in] Pointer to a null-terminated string that identifies the 
			specified pDeviceName.
		nUnitAddr
			[in]  Specifies the Unit Address 
		lpDevAddr
			[in]  Point to a DEVADDR variant 
*	Remarks
		When "processpacket" failed, tryconnect twice,
		if failed again, the touchvew will connect with setted time. 
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp::TryConnect(const TCHAR*  pDeviceName, 
																		 int nUnitAddr, LPVOID lpDevAddr)
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp);
	CDebug::ShowImpMessage(_T("C16A037Pro::TryConnect"));
	ASSERT(pDeviceName != NULL);
	ASSERT(lpDevAddr != NULL);
    CDevBase* pDev = pThis->GetDevObj(pDeviceName);	
	if ( pDev )
	{
		return pDev->TryConnect(pDeviceName,nUnitAddr,lpDevAddr);
	}
	else
	{
		ASSERT(FALSE) ;
		return FALSE ;
	}	
	return FALSE ;
}

/****************************************************************************
*   Name
		SetTrans
*	Type
		public
*	Function
		Set the inner-communication handle.
*	Return Value
		Always return true. 
*	Parameters
		lpHcomm
			[in] Pointer to a handle that Specifies an valid-communication handle.
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp2::SetTrans( LPVOID* pHcomm )
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);
	CDebug::ShowImpMessage(_T("C16A037Pro::SetTrans"));
	//µÃµ½¾ä±ú
	//get the handle
	pThis->m_ComObj.m_hComm = *pHcomm;
	pThis->m_ComObj.m_bUseModem = true;
	return  TRUE;
}


/****************************************************************************
*   Name
		GetTrans
*	Type
		public
*	Function
		Retrieves an inner-communication handle in serial communication project.
*	Return Value
		Always return true. 
*	Parameters
		lpHcomm
			[out] Pointer to a handle that retrieve inner-communication handle.
*****************************************************************************/
STDMETHODIMP_(BOOL) C16A037Pro::XProtocolImp2::GetTrans( LPVOID* pHcomm )
{
    METHOD_PROLOGUE(C16A037Pro, ProtocolImp2);	
	CDebug::ShowImpMessage(_T("C16A037Pro::GetTrans"));
	ASSERT(pHcomm);	
	//·µ»Ø¾ä±ú
	//return the handle
	*pHcomm  = pThis->m_ComObj.m_hComm;
	return TRUE;
}

/****************************************************************************
*   Name
		GetDevObj
*	Type
		public
*	Function
		Get the device class object pointer from varlist.
*	Return Value
		return the pointer's address if successful; otherwise return NULL. 
*	Parameters
		szKind
			[in] Device type.
*****************************************************************************/
CDevBase* C16A037Pro::GetDevObj(const CString& szKind)
{
	DEVICE_INFO * pDeviceInfo = NULL;
	int m_iDeviceNum = 0;
	POSITION pos = m_DevList.GetHeadPosition();	
	
	while (pos)
	{
		CDevBase* pDev = ( CDevBase* )m_DevList.GetNext(pos);
		pDev->GetDevices((LPVOID *)&pDeviceInfo,  &m_iDeviceNum);
		for(int iDeviceIndex = 0; iDeviceIndex < m_iDeviceNum; iDeviceIndex++)		
		{
			if(pDeviceInfo[iDeviceIndex].sDeviceName == szKind)
			{
				return pDev;				
			}			
		}		
	}	
	return NULL;
}

/****************************************************************************
*   Name
		GetFirstDevObj
*	Type
		public
*	Function
		Get the device class object pointer .
*	Return Value
		return the pointer's address if successful; otherwise return NULL. 
*	Parameters
		null
*****************************************************************************/
CDevBase* C16A037Pro::GetFirstDevObj()
{
	DEVICE_INFO * pDeviceInfo = NULL;
	int m_iDeviceNum = 0;
	POSITION pos = m_DevList.GetHeadPosition();		
	if (pos)
	{
		CDevBase* pDev = ( CDevBase* )m_DevList.GetNext(pos);
		pDev->GetDevices((LPVOID *)&pDeviceInfo,  &m_iDeviceNum);	
		if(pDeviceInfo[0].sDeviceName != _T(""))
		{
			return pDev;				
		}			
	}	
	return NULL;
}