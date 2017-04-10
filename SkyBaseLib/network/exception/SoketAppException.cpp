#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

CSoketAppException::CSoketAppException( eSoketAppErrorId errorId , const skyString *pMessage ) :
CExceptionBase( pMessage )
{
	m_eErrorId = errorId;
}

eSoketAppErrorId CSoketAppException::GetErrorId()
{
	return m_eErrorId;
}

void CSoketAppException::Print()
{
	SKY_PRINTF( _T( "%s err[ %d ] \n" ) , ( const skyString * )m_Message , m_eErrorId );
}

} } }