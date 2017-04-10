#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

CSoketException::CSoketException( s32 errorId , const skyString *pMessage ) :
CExceptionBase( pMessage )
{
	m_ErrorId = errorId;
}

s32 CSoketException::GetErrorId()
{
	return m_ErrorId;
}

void CSoketException::Print()
{
	SKY_PRINTF( _T( "%s err[ %d ] \n" ) , ( const skyString * )m_Message , m_ErrorId );
}

} } }