#pragma once

namespace sky { namespace lib { namespace process {

class IProcess;

#ifdef SW_SKYLIB_NO_USE_SMART_POINTER
typedef IProcess*	IProcessPtr;
#else
typedef SmartPointer< IProcess >	IProcessPtr;
#endif

typedef CList< u32 , IProcessPtr >	IProcessPtrList;

class CProcessManager;
typedef SmartPointer< CProcessManager >	    CProcessManagerPtr;
typedef CList< u32 , CProcessManagerPtr >	CProcessManagerPtrList;

} } }