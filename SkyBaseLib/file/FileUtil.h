#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< 実装 >---------------------------------------------//

//非同期ファイル読み込み処理の戻り値型
typedef SmartPointer< FunctionCallBackAsync_r< stream::IStreamPtr > > FileLoadAsyncRs;

//非同期ファイル書き込みの戻り値型
typedef SmartPointer< FunctionCallBackAsync_v > FileSaveAsyncRs;

/// <summary>
/// ファイル関連処理共通処理
/// テキストファイルを読む場合、読み込むテキストファイルの文字コードが重要になる。
/// もしメモリ上がワイド文字で、マルチバイトのテキストファイルを読んだ場合、読み込んだ直後は文字化けしている。
/// </summary>
class CFileUtil : base::SkyObject
{
public :

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放
	/// </summary>
	static void Dispose();

	/// <summary>
	/// ファイルの更新時間を取得する
	/// </summary>
	static u32		GetFileUpdateTime		( const skyString *pPath );

	/// <summary>
	/// ファイルの更新時間が渡した値と違うことを判定する
	/// </summary>
	static skyBool IstFileUpdateTimeChange	( u32 time , const skyString *pPath );

//---------------メモリにファイルが乗り切らない場合これらを使う---------------------▽

	/// <summary>
	/// ファイルサイズの取得
	/// </summary>
	static skyBool GetSize					( IFileInfoPtr &pFileInfo );

	/// <summary>
	/// ファイルをオープンする（使い終わったら閉じてください。pFileInfoは内部で実体が生成されます）
	/// </summary>
	static skyBool	Open					( const skyString *pPath , IFileInfoPtr &pFileInfo , eOpenMode eMode ); //読込専用でオープン（ファイルが無ければエラー）

	/// <summary>
	/// ファイルの任意の位置から任意のサイズのデータを取り出す（pBufは使い終わったらDELしてください）
	/// </summary>
	static u32 Read							( IFileInfoPtr &pFileInfo , u8 *pBuf , u32 bufSize , u32 readSize , u32 pos );

	/// <summary>
	/// ファイルの任意の位置に任意のサイズのデータを書き込む
	/// </summary>
	static u32 Write						( IFileInfoPtr &pFileInfo , u8* pBuf , u32 bufSize , u32 writeSize , u32 pos );

	/// <summary>
	/// ファイルを閉じる（pFileInfoは内部で実体が削除されます）
	/// </summary>
	static skyBool Close					( IFileInfoPtr &pFileInfo );

//---------------メモリにファイルが乗り切らない場合これらを使う---------------------△

//---------------メモリにファイルが乗りる場合これらを使う--------------------------▽

	/// <summary>
	/// ファイルの同期読み込み(※非同期にも使うためにあえて参照渡しにしていない）
	/// </summary>
	static stream::IStreamPtr LoadFile      ( const hash::CHashStringPtr pPath );

	/// <summary>
	/// ファイルの非同期読み込み（終了判定は戻り値のオブジェクトのメソッドIsEnd()を使用してください。）
	/// ★★内部でワーカースレッドに処理を委譲しています。CCThreadPoolManagerを初期化し、ワーカースレッドを作成してから使用してください。★★
	/// </summary>
	static FileLoadAsyncRs LoadFileAsync    ( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// ファイルの非同期読み込みキャンセル
	/// </summary>
	static void LoadFileAsyncCancel         ( FileLoadAsyncRs &retObject );

	/// <summary>
	/// ファイルの同期書き込み(※非同期にも使うためにあえて参照渡しにしていない）
	/// </summary>
	static void			SaveFile            ( stream::IStreamPtr spFile , skyBool isResume = skyFalse  );

	/// <summary>
	/// ファイルの同期書き込み（終了判定は戻り値のオブジェクトのメソッドIsEnd()を使用してください。）
	/// ★★内部でワーカースレッドに処理を委譲しています。CCThreadPoolManagerを初期化し、ワーカースレッドを作成してから使用してください。★★
	/// </summary>
	static FileSaveAsyncRs SaveFileAsync    ( stream::IStreamPtr &spFile , skyBool isResume = skyFalse  );

	/// <summary>
	/// ファイルの非同期書き込みキャンセル
	/// </summary>
	static void FileSaveAsyncCancel         ( FileLoadAsyncRs &retObject );

	/// <summary>
	/// テキストファイルの読み込み（バッファへの読みきり）（pBufは使い終わったらDELしてください）
	/// </summary>
	static skyBool LoadTextFileFromDisc		( const skyString *pPath , u8 **ppBuf , u32 *pReadSize );

	/// <summary>
	/// テキストファイルの書き込み（バッファからの書き込み）
	/// </summary>
	static skyBool SaveLineTextFileForDisc	( const skyString *pPath , u8 *pBuf , skyBool isResume = skyFalse  );

	/// <summary>
	/// ファイルの読み込み（バッファへの読みきり）（pBufは使い終わったらDELしてください）
	/// </summary>
	static skyBool LoadFileFromDisc			( const skyString *pPath , u8 **ppBuf , u32 *pReadSize );

	/// <summary>
	/// ファイルの書き込み（バッファからの書き込み）
	/// </summary>
	static skyBool SaveBinFileForDisc			( const skyString *pPath , const u8 *pBuf , u32 bufSize , skyBool isResume = skyFalse  );

//---------------メモリにファイルが乗りる場合これらを使う--------------------------△

private:

	//各機能封印
	CFileUtil(){};											//コンストラクタ
	CFileUtil( const CFileUtil& ){};							//コピーコンストラクタ
	virtual ~CFileUtil(){};									//デストラクタ
	CFileUtil &operator=(const CFileUtil& ){ return *this; };	//代入演算子オーバーロード

};


//インスタンスアクセス文字列
#define CFileUtilInitialize_()		sky::lib::file::CFileUtil::Initialize()
#define CFileUtil_					sky::lib::file::CFileUtil
#define CFileUtilDispose_()			sky::lib::file::CFileUtil::Dispose()

#ifdef SKYLIB_PLATFORM_WIN32

struct FileInfoWin32 : public IFileInfo
{
    SkyTypeDefRTTI;

	FILE*	pFp;
};

/// ＭＳＤＮによると
/// テキストモードで開かれた場合はマルチバイト文字列として、
/// バイナリモードで開かれた場合はワイド文字列として読み出します。
/// すなわちUnicode文字を読む場合はrbにしなければならない。
namespace OpenMode
{
	//テキストモード（bがないので）
	static const skyString*	rp		= _T( "r+" );	//読み込み書き込み可能（ファイルが無ければエラー）
	static const skyString*	wp		= _T( "w+" ); 	//書き込み読込可能（上書き）（ファイルが無ければ作成）
	static const skyString*	ap		= _T( "a+" );	//書き込み読込可能（追記）（ファイルが無ければ作成）

	//バイナリモード
	static const skyString*	rpb		= _T( "r+b" );	//読み込み書き込み可能（ファイルが無ければエラー）
	static const skyString*	wpb		= _T( "w+b" ); 	//書き込み読込可能（上書き）（ファイルが無ければ作成）
	static const skyString*	apb		= _T( "a+b" );	//書き込み読込可能（追記）（ファイルが無ければ作成）
}

#endif

/// <summary>
/// そのプラットフォームで最もネイティブな関数を使用してファイルを元の状態のまま全部メモリ上に読みきる。
/// file機能が初期化されていなくても、ファイルを読むことができます。
/// </summary>
inline skyBool Native_FileRead( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{

#ifdef SKYLIB_PLATFORM_WIN32 //Win32上の実装
	
	FILE*	pFp;

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , _T( "rb" ) ) ) != 0 ) //バイナリモードで開く
#else
	if ( ( pFp = _tfopen( pPath , _T( "rb" ) ) ) == 0 ) //バイナリモードで開く
#endif
	{
		return skyFalse;
	}

//-------ファイルサイズを取得

	u32 saveSeek = ftell( pFp );			//現在位置を保存
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//ファイルサイズの計測
	fseek( pFp, saveSeek, SEEK_SET );		//現在位置の復元

//-------ファイルバッファの確保

//	*ppBuf = ( u8 * )MALLOC__( sizeof( u8 ) * fileSize , sky::lib::memory::DEFAULT_ALIGN , __FILE__ , __LINE__ );
	*ppBuf = ( u8 * )MALLOC__( sizeof( u8 ) * fileSize , __FILE__ , __LINE__ );

//-------ファイルを読む

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
		return skyFalse;
	}

//-------ファイルを閉じる

	fclose( pFp );
//		_fcloseall(); //なんとこれを使うと全てのファイルが閉じるようだ

	return skyTrue;
#endif

}

} } }