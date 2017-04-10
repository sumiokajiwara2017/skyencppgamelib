#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// ファイル情報クラス（実装はほとんど各環境毎）
/// </summary>
class IFileInfo : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	u32		fileSize;

};
typedef SmartPointer< IFileInfo >	IFileInfoPtr;

} } }