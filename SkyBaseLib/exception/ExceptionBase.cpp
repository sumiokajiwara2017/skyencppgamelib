#include "StdAfx.h"
#include "SkyBaseLib/Exception.h"

SkyImplementRTTI( sky::lib::exception::CExceptionBase , sky::lib::base::SkyObject );

namespace sky { namespace lib { namespace exception {

CExceptionBase::CExceptionBase( const skyString *pMessage )
{
	m_Message = pMessage;
}

CExceptionBase::CExceptionBase( const skyString *pMessage , const skyString *pFileName , const skyString *pFunctionName , u32 line )
{
	m_Message = pMessage;
	m_FileName = pFileName;
	m_FunctionName = pFunctionName;
	m_Line = line;
}

CExceptionBase::~CExceptionBase() throw()
{
}

const skyString *CExceptionBase::GetMessage()
{
	return m_Message.Get();
}

const skyString *CExceptionBase::GetFileName()
{
	return m_FileName.Get();
}

const skyString *CExceptionBase::GetFunction()
{
	return m_FunctionName.Get();
}

u32 CExceptionBase::GetLine()
{
	return m_Line;
}

} } }