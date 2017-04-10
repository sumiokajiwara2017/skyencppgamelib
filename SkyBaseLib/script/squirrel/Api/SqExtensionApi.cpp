#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

namespace sky { namespace lib { namespace squirrel {

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//includeを実現する
SQAPI( includenut )
{
	SKY_ASSERT_MSG( sq_gettype( v , 2 ) == OT_STRING , _T( "The argument of includenut is set and things except the character are set. " ) ); //includenutの引数が文字以外が設定されています。

	//引数sの受け取り
	const SQChar* url; 
	sq_getstring( v , 2 , &url );

	//スクリプトファイルを登録
	CSqUtil_::RegisterScriptFile( v , CHS( url ) );

	return 0;
}

//Ａｐｉ情報テーブル
static ApiInfo s_ApiTbl[] =
{
	SQAPITBL( includenut )
};
static u32 s_ApiTblSize = array_sizeof( s_ApiTbl );

void SqExtensionApi::PrintApiName( stream::IStreamPtr &spStream )
{
	SKY_ASSERT_MSG( spStream->vIsOpen() , _T( "This stream has not been opened. " ) );

	for ( u32 i = 0 ; i < s_ApiTblSize ; i++ )
	{
		spStream->vWriteStream( ( u8 * )s_ApiTbl[ i ].pName , CStringUtil_::LenByte( s_ApiTbl[ i ].pName ) );
		spStream->vWriteStream( ( u8 * )_T( "\n" ) , CStringUtil_::LenByte( _T( "\n" ) ) ); //改行コードを入れる
	}
}

void SqExtensionApi::RegisterApi( HSQUIRRELVM v )
{
	for ( u32 i = 0 ; i < s_ApiTblSize ; i++ )
	{
		CSqUtil_::RegisterNativeFunc( v , s_ApiTbl[ i ].funcAdr , s_ApiTbl[ i ].pName );
	}
}

#endif

} } }