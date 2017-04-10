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
	::Sleep( 500 ); //読み込みに時間がかかることを表現
	return m_Data.Get();
}

void CommonResource::Write( skyWString *pData )
{
	::Sleep( 3000 );	//書き込みに時間がかかることを表現
	m_Data = pData;
}
