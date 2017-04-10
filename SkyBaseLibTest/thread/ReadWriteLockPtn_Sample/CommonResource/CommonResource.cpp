#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "CommonResource.h"

CommonResource::CommonResource( void )
{
}

CommonResource::~CommonResource( void )
{
}

const skyWString *CommonResource::Read( void )
{
	::Sleep( 500 ); //“Ç‚İ‚İ‚ÉŠÔ‚ª‚©‚©‚é‚±‚Æ‚ğ•\Œ»
	return m_Data.Get();
}

void CommonResource::Write( skyWString *pData )
{
	::Sleep( 3000 );	//‘‚«‚İ‚ÉŠÔ‚ª‚©‚©‚é‚±‚Æ‚ğ•\Œ»
	m_Data = pData;
}
