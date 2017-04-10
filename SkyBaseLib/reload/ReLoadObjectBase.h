#pragma once

namespace sky { namespace lib { namespace reload {

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// ファイル更新情報
/// </summary>
struct FileUpdateData : public base::SkyRefObject
{
	u32					        updateTime;
	hash::CHashStringPtr		filePath;
};
typedef SmartPointer< FileUpdateData >			FileUpdateDataPtr;
typedef CList< s32 , FileUpdateDataPtr >		FileUpdateDataPtrList;	

/// <summary>
/// リロード情報クラス
/// リロードとは開発効率を高めるために、実行中の再ロードを目的としている。
/// 再ロードが必要な箇所はアプリケーションによって異なるので共通化機能としてできることは下記
/// ①リロード要求が着たらファイルの更新時間を全部見て更新があったリロード情報にマークをする
/// ②PreReLoadFuncがあったら呼ぶ
/// ③リロード情報のUnLoadFuncを呼び、LoadFuncを呼ぶ
/// ④PostReLoadFuncがあったら呼ぶ
/// アプリケーションはこの機能を『うまく』使って設定ファイル、画像ファイルが更新されたら自動でリロードされるようにゲームを作ること。
/// </summary>
class IReLoadObject : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ファイルパスHashを登録する
	/// リロードに関係するすべてのファイルパスを渡す
	/// </summary>
	virtual void			AddFilePath( const hash::CHashStringPtr &ptr ) = 0;

	/// <summary>
	/// ファイルパスリストの参照を取得する
	virtual FileUpdateDataPtrList	&GetFilePathList() = 0;

	/// <summary>
	/// ロード関数（各オブジェクトで実装してください）
	/// </summary>
	virtual void			Load() = 0;

	/// <summary>
	/// アンロード関数（必要ならば各オブジェクトでオーバーライドしてください）
	/// </summary>
	virtual void			UnLoad(){};
};

class IReLoadObject;
typedef SmartPointer< IReLoadObject >	IReLoadObjectPtr;
typedef CList< u32 , IReLoadObjectPtr >	IReLoadObjectPtrList;

//-----------------------------------< 抽象実装 >-------------------------------------------//

class AbsReLoadObject : public IReLoadObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 実装（抽象実装で実装する場合は必ずvirtualで宣言する。実装でオーバーライドされるかもしれないから）
	/// </summary>
	virtual void					AddFilePath( const hash::CHashStringPtr &ptr );
	virtual FileUpdateDataPtrList	&GetFilePathList(){ return m_FilePathInfoList; }

protected:

	/// <summary>
	/// ファイル情報リスト
	/// </summary>
	FileUpdateDataPtrList	m_FilePathInfoList;
};

} } }