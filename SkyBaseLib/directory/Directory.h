#pragma once

namespace sky { namespace lib { namespace directory {

class CDirectory;
typedef SmartPointer< CDirectory >			CDirectoryPtr;
typedef CList< s32 , CDirectory * >			DirectoryList;
typedef CComposite< CDirectory * , s32 >	DirectoryBase;

//---------------------------------< ファクトリー >-----------------------------------------//

class CDirectoryFactory : public base::SkyObject
{

public:

	/// <summary>
	/// 作成（スマートポインタで保護してるため削除の必要なし）
	/// </summary>
	static CDirectoryPtr	Create		( const skyString *pPath , const skyString *pWildCard );
	static CDirectoryPtr	Create		( const skyString *pPath );
};

#define CDirectoryFactory_ sky::lib::directory::CDirectoryFactory

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ディレクトリクラス
/// </summary>
class CDirectory : public DirectoryBase
{
    SkyTypeDefRTTI;

friend class CDirectoryFactory;
friend class SmartPointer< CDirectory >;

public:

	/// <summary>
	/// 自分自身がファイルかどうか判定する
	/// </summary>
	skyBool				IsFile			();

	/// <summary>
	/// パスの取得
	/// </summary>
	const skyString*	GetPath		();

	/// <summary>
	/// ファイルリストの出力(isRがOnの場合子のフォルダ以下まで探しに行く）
	/// </summary>
	void				WriteFileList	( file::CFileStreamPtr &textFilePtr , skyBool isR = skyTrue , const skyString *wildCard = skyNull );

	/// <summary>
	/// ディレクトリリストの出力
	/// </summary>
	void				WriteDirList	( file::CFileStreamPtr &textFilePtr , skyBool isR = skyTrue );

	/// <summary>
	/// ファイルパスリストの取得
	/// </summary>
	void				GetFilePathList( hash::CHashStringPtrList &filePathList , skyBool isR = skyTrue , const skyString *wildCard = skyNull );

	/// <summary>
	/// ディレクトリパスリストの取得
	/// </summary>
	void				GetDirPathList	( hash::CHashStringPtrList &dirPathList , skyBool isR = skyTrue );

	/// <summary>
	/// 初期化
	/// コンストラクタと初期化処理は分けて考える。初期化に必要なパラメーターはコンストラクタの引数か、別途設定関数でオブジェクトに取り入れる。
	/// </summary>
	void				Initialize		();

	/// <summary>
	/// 解放
	/// デストラクタと開放処理は分けて考える。
	/// </summary>
	void				Dispose		();

private:

	/// <summary>
	/// 初期化フラグ
	/// </summary>
	skyBool m_bIsInitialized;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDirectory					( const skyString *pPath , CDirectory *pParent , const skyString *pWildCard );	//ディレクトリ用コンストラクタ
	CDirectory					( const skyString *pPath , CDirectory *pParent );								//ファイル用コンストラクタ

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual		~CDirectory	();

	/// <summary>
	/// 子の要素をすべて消す（ツリーをたどって下から全部消す）
	/// </summary>
	void		_DeleteChild		();

	/// <summary>
	/// データの種類
	/// </summary>
	typedef enum
	{
		Type_Directory = 0,	//ファイルです。
		Type_File ,			//ディレクトリです。
	} Type;

	/// <summary>
	/// パス
	/// </summary>
	string::CString		m_Path;

	/// <summary>
	/// ワイルドカード
	/// </summary>
	string::CString		m_WildCard;

	/// <summary>
	/// 属性
	/// </summary>
	Type		m_eType;

	/// <summary>
	/// 再帰検索処理
	/// </summary>
	void SearchDir( const skyString *pRootDir, const skyString *pWildCard );
};

} } }
