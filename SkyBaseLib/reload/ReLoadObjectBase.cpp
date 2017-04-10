#include "StdAfx.h"
#include "SkyBaseLib/ReLoad.h"

SkyImplementRTTI( sky::lib::reload::IReLoadObject   , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::reload::AbsReLoadObject , sky::lib::reload::IReLoadObject );

namespace sky { namespace lib { namespace reload {

//------------------------------------< 抽象実装 >--------------------------------------------//

void AbsReLoadObject::AddFilePath( const hash::CHashStringPtr &ptr )
{
	FileUpdateDataPtr spFile( NEW__( FileUpdateData , FileUpdateData() ) );

	//ファイルパスを設定する
	spFile->filePath = ptr;

	//ファイルの更新日付を取得する
	spFile->updateTime = CFileUtil_::GetFileUpdateTime( ptr->GetString() );

	//リストに登録
	m_FilePathInfoList.AddTail( spFile );
}

} } }