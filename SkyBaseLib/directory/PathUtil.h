#pragma once



namespace sky { namespace lib { namespace directory {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// パス文字列操作用関数群
/// <summary>
class CPathUtil : public base::SkyObject
{

public:

	/// <summary>
	/// ディレクトリ作成
	/// </summary>
	static skyBool CreateDir( const skyString *srcPath );

	/// <summary>
	/// ディレクトリ削除
	/// </summary>
	static skyBool DeleteDir( const skyString *srcPath );

	/// <summary>
	/// カレントディレクトリパス取得
	/// </summary>
	static skyBool GetCurrentDir( skyString *destPath , u32 destLen );

	/// <summary>
	/// カレントドライブ取得
	/// </summary>
	static s32 GetCurrentDrive();

	/// <summary>
	/// カレントディレクトリ変更
	/// </summary>
	static skyBool ChangeCurrentDirectory( const skyString *srcPath );

	/// <summary>
	/// カレントドライブ変更
	/// </summary>
	static skyBool ChangeCurrentDrive( s32 drive );

	/// <summary>
	/// ファイルパスから拡張子を取る（この関数は入力データを書き換えます）
	/// </summary>
	void CutFileExt( const skyString *srcPath , skyString *destPath , u32 destLen );

	/// <summary>
	/// ファイルパスからドライブ名を取得する
	/// </summary>
	static void		GetDrive		( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ファイルパスから拡張子を取得する
	/// </summary>
	static void		GetFileExt		( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ファイルパスの拡張子を付け替える
	/// </summary>
	static void		ReplaceFileExt	( const skyString *srcPath, skyString *destBuf , u32 destBufSize, const skyString *pExt );

	/// <summary>
	/// ファイルパスの拡張子が指定の文字の場合真を返す
	/// </summary>
	static skyBool  IsExt( const skyString *srcPath , const skyString *srcExt );

	/// <summary>
	/// ファイルパスのファイル名を取り替える
	/// </summary>
    static void     ReplaceFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pName );

	/// <summary>
	/// ファイルパスのファイル名の後ろにサインを付ける（hoge/fileNaem.hg → hoge/fileName_H.hg（_Hを付けた））
	/// </summary>
    static void     AddTailFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pNameSignature );

	/// <summary>
	/// ファイルパスから拡張子付きファイル名を抽出する
	/// </summary>
	static void		GetFileName	( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ファイルパス拡張子をのぞいたファイル名のみを取得
	/// </summary>
	static void		GetFileNameNoExt( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ファイルパスからディレクトリパスを抽出する
	/// </summary>
	static void	    GetDirPath ( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// 相対パスから絶対パスを取得する
	/// </summary>
	static void		GetFullPath ( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// 指定のパスからファイル名だけとって、指定のディレクトリパスと結合したパスを返す
	/// </summary>
	static void     GetPathFileNameAddPath( const skyString *srcPath , const skyString *srcDirPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ファイル存在チェック
	/// </summary>
	static skyBool		IsFileOn		( const skyString *pPath );

	/// <summary>
	/// ディレクトリ存在チェック
	/// </summary>
	static skyBool		IsDirectoryOn	( const skyString *pPath );

	/// <summary>
	/// 末尾スラッシュチェック
	/// </summary>
	static skyBool		IsSlashEnd		( const skyString *pPath );

	/// <summary>
	/// 指定のワイルドカードに指定のパスが一致するかどうか返す
	/// </summary>
	static skyBool		IsWildCardMatch ( const skyString *wildCard , const skyString *filePath );

	/// <summary>
	/// 環境変数の値を取得
	/// </summary>
	static void	GetEnv( memSize &destlen , const skyString *name , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// 環境変数の値を追加
	/// </summary>
	static void PutEnv( const skyString *name , const skyString *val );

	/// <summary>
	/// 環境変数を含む文字列を渡し、変換後の値を返す
	/// </summary>
	static skyBool ReplaceEnv( const skyString *src , skyString *destBuf , u32 destBufSize );

private:

	//各機能封印
	CPathUtil(){};											//コンストラクタ
	CPathUtil( const CPathUtil& ){};							//コピーコンストラクタ
	virtual ~CPathUtil(){};									//デストラクタ
	CPathUtil &operator=(const CPathUtil& ){ return *this; };	//代入演算子オーバーロード

};

//インスタンスアクセス文字列
#define CPathUtil_			sky::lib::directory::CPathUtil

} } }