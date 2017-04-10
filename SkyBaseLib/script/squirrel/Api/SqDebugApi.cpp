#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

namespace sky { namespace lib { namespace squirrel {

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//�X�^�b�N�g���[�X���o��
SQAPI( printStackTrace )
{

	CSqUtil_::DefaultPrint( v , _T( "========< Stack Trace >======== \n" ) );

	u32 level = 0;
	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		SQStackInfos info;
		if( SQ_FAILED( sq_stackinfos( v , level , &info ) ) )
		{
			isLoop = skyFalse;
			continue;
		}
		
		CSqUtil_::DefaultPrint( v , _T( "Level=%d %s %s(%d) \n" ) , level , info.funcname , info.source , info.line );
		level++;
	}


	return 0;
}


//���[�J���ϐ����v�����g����
SQAPI( printLocalVal )
{
	CSqUtil_::DefaultPrint( v , _T( "========< Loacal Val >======== \n" ) );
	u32 level = 0;
	skyBool isLevelLoop = skyTrue;
	while ( isLevelLoop )
	{
		u32 index = 0;
		skyBool isIndexLoop = skyTrue;
		while ( isIndexLoop )
		{
			const SQChar *pName = sq_getlocal( v , level , index );
			if ( pName == NULL ) //���̃��x���ɕϐ��͂Ȃ��Ȃ����B
			{
				isIndexLoop = skyFalse;
				continue;
			}
			CSqUtil_::DefaultPrint( v , _T( "Level=%d Index=%d Name=%s \n" ) , level , index , pName );
			index++;
		}
		level++;
		if ( sq_getlocal( v , level , 0 ) == NULL ) //�I������
		{
			isLevelLoop = skyFalse;
		}
	}
	return 0;
}

//�`�������e�[�u��
static ApiInfo s_ApiTbl[] =
{
	SQAPITBL( printStackTrace )
	SQAPITBL( printLocalVal )
};
static u32 s_ApiTblSize = array_sizeof( s_ApiTbl );

void SqDebugApi::PrintApiName( stream::IStreamPtr &spStream )
{
	SKY_ASSERT_MSG( spStream->vIsOpen() , _T( "This stream has not been opened. " ) );

	for ( u32 i = 0 ; i < s_ApiTblSize ; i++ )
	{
		spStream->vWriteStream( ( u8 * )s_ApiTbl[ i ].pName , CStringUtil_::LenByte( s_ApiTbl[ i ].pName ) );
		spStream->vWriteStream( ( u8 * )_T( "\n" ) , CStringUtil_::LenByte( _T( "\n" ) ) ); //���s�R�[�h������
	}
}

void SqDebugApi::RegisterApi( HSQUIRRELVM v )
{
	for ( u32 i = 0 ; i < s_ApiTblSize ; i++ )
	{
		CSqUtil_::RegisterNativeFunc( v , s_ApiTbl[ i ].funcAdr , s_ApiTbl[ i ].pName );
	}
}

#endif

} } }