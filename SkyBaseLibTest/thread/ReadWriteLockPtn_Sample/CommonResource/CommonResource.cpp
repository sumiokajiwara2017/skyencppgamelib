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
	::Sleep( 500 ); //�ǂݍ��݂Ɏ��Ԃ������邱�Ƃ�\��
	return m_Data.Get();
}

void CommonResource::Write( skyWString *pData )
{
	::Sleep( 3000 );	//�������݂Ɏ��Ԃ������邱�Ƃ�\��
	m_Data = pData;
}
