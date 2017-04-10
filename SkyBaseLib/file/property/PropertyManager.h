#pragma once

namespace sky { namespace lib { namespace file {

/// <summary>
/// プロパティ管理クラス
/// </summary>
class CPropertyManager : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化（data/propertyディレクトリ以下のファイルをすべて読む）
	/// </summary>
	static void Initialize	();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose		();

	/// <summary>
	/// ファイル名とキーとインデックスで値を文字で取得
	/// </summary>
	static const hash::CHashStringPtr &GetString( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// ファイル名とキーとインデックスで値を整数で取得
	/// </summary>
	static s32 Gets32( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// ファイル名とキーとインデックスで値を少数で取得
	/// </summary>
	static d64 Getd64( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index = 0 );

	/// <summary>
	/// プロパティファイルの内容をダンプする
	/// </summary>
	static void Dump( stream::IStreamPtr &dest );

private:

	/// <summary>
	/// ファイル毎のプロパティ格納ハッシュ
	/// </summary>
	static hash::CHashStringPtrListPtrHashPtrHash	*m_pFileDataHash;
};

#define CPropertyManagerInitialize_()	sky::lib::file::CPropertyManager::Initialize()
#define CPropertyManager_				sky::lib::file::CPropertyManager
#define CPropertyManagerDispose_()		sky::lib::file::CPropertyManager::Dispose()

} } }