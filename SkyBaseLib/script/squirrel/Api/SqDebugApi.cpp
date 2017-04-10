#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

namespace sky { namespace lib { namespace squirrel {

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//スタックトレースを出す
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


//ローカル変数をプリントする
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
			if ( pName == NULL ) //このレベルに変数はなくなった。
			{
				isIndexLoop = skyFalse;
				continue;
			}
			CSqUtil_::DefaultPrint( v , _T( "Level=%d Index=%d Name=%s \n" ) , level , index , pName );
			index++;
		}
		level++;
		if ( sq_getlocal( v , level , 0 ) == NULL ) //終了判定
		{
			isLevelLoop = skyFalse;
		}
	}
	return 0;
}

//Ａｐｉ情報テーブル
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
		spStream->vWriteStream( ( u8 * )_T( "\n" ) , CStringUtil_::LenByte( _T( "\n" ) ) ); //改行コードを入れる
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