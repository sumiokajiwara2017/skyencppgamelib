#include "StdAfx.h"
#include "SkyBaseLib/File.h"

#include "SkyBaseLib/Directory.h"

SkyImplementRTTI( sky::lib::file::CDiscFileController , sky::lib::base::SkyObject );

namespace sky { namespace lib { namespace file { 

//-------------------------------------< 実装 >---------------------------------------------//

void CDiscFileController::Initialize()
{
}

void CDiscFileController::Dispose()
{
}

const u8 *CDiscFileController::Load( const hash::CHashStringPtr &filePath )
{
	const u8 *result = skyNull;

	//結合ファイルに問い合わせにいく
	result = const_cast< u8 * >( CBindFileManager_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//プールファイルに問い合わせにいく
	result = const_cast< u8 * >( CFilePoolContena_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//ＤＩＳＣファイルを読む（同時にファイルをプールする。プールしないと戻り値が無効になるため。）
	CFilePoolContena_::Load( filePath );
	result = const_cast< u8 * >( CFilePoolContena_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	//デフォルトファイルを読む
	result = const_cast< u8 * >( CDefaultFileManager_::LoadFile( filePath ) );
	if ( result != skyNull ) return result;

	return skyNull; //見つからなかった
}

skyBool CDiscFileController::Delete( const hash::CHashStringPtr &filePath )
{
	skyBool result = skyFalse;

	//結合ファイルを消しにいく
	result = CBindFileManager_::DeleteFile( filePath );
	if ( result ) return result;

	//プールファイルを消しにいく
	result = CFilePoolContena_::DeleteFile( filePath );
	if ( result ) return result;

	return skyFalse;
}

} } }