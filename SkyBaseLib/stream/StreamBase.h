#pragma once

namespace sky { namespace lib {

template< typename T , typename KEY , class ThreadingModel >class CHash;

} }

namespace sky { namespace lib { namespace stream {

/// <summary>
/// マーク位置情報
/// </summary>
struct MarkInfo
{
	u32 seekPos;
	u32 size;

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	MarkInfo& operator =( const MarkInfo& src )
	{
		seekPos    = src.seekPos;
		size       = src.size;
		return *this;
	};
};
typedef CHash< MarkInfo , s32 , thread::ThreadingModels::CSingleThread > MarkInfoHash;

static const u32 MARK_INFO_HASH_SIZE = 31;

//-------------------------------< インターフェース >----------------------------------------//

static const u32 WRITE_STREAM_BUF_SIZE = 1024;

/// <summary>
/// ストリームインタフェース
/// このクラスを継承するのは下記の機能かと
/// メモリ読込／書込機能
/// ファイル読込／書込機能
/// ネットワーク読込／書込機能
/// コンソール読込／書込機能
/// </summary>
class IStream : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 種類を取得
	/// </summary>
	virtual eStreamKind vGetKind() = 0;

	/// <summary>
	/// エンディアンのスワップフラグの設定
	/// </summary>
	virtual void vSetEndianSwap( skyBool isSwap ) = 0;

	/// <summary>
	/// データを読み取り専用かどうかチェックする
	/// </summary>
	virtual skyBool vIsReadOnly() = 0;

	/// <summary>
	/// データがサイズ変更可能（書き込めば書きこむほどサイズが広がる）かどうか判定する
	/// </summary>
	virtual skyBool vIsEnableChengeSize() = 0;

	/// <summary>
	/// データを取得する（ストリームの実装によっては取れない場合もある）
	/// </summary>
	virtual const u32 vGetSeek() = 0;

	/// <summary>
	/// データを取得する（ストリームの実装によっては取れない場合もある）
	/// </summary>
	virtual const u8* vGetSeekData() = 0;

	/// <summary>
	/// データを取得する（ストリームの実装によっては取れない場合もある）
	/// </summary>
	virtual const u8* vGetData() = 0;

	/// <summary>
	/// データのポインタを取得する（ストリームの実装によっては取れない場合もある）
	/// </summary>
	virtual const u8** vGetDataPtr() = 0;

	/// <summary>
	/// データをクローンする
	/// </summary>
	virtual u8*       vCloneData() = 0;

	/// <summary>
	/// データサイズを取得する（ストリームの実装によっては取れない場合もある）
	/// </summary>
	virtual u32 vGetSize() = 0;

	/// <summary>
	/// エンディアンの設定(skyTrueを設定した場合、書き込み、読み込みの瞬間に値をスワップする）
	/// </summary>
	virtual skyBool vIsSwap() = 0;

	/// <summary>
	/// 開いているか？
	/// </summary>
	virtual skyBool vIsOpen() = 0;

	/// <summary>
	/// ストリームを開く
	/// </summary>
	virtual skyBool vOpen() = 0;

	/// <summary>
	/// ストリームを閉じる
	/// </summary>
	virtual skyBool vClose() = 0;

	/// <summary>
	/// メモリ上にデータを確保する
	/// </summary>
	virtual void vCreate( u32 size ) = 0;

	/// <summary>
	/// メモリ上のデータを破棄する
	/// </summary>
	virtual void vDestroy() = 0;

	/// <summary>
	/// ストリームに書込み
	/// </summary>
	virtual u32 vWriteStream( const skyString *s , ... ) = 0;
	virtual u32 vWriteStream( u8* pData, u32 size ) = 0;
	virtual u32 vWriteStream( u16 data ) = 0;
	virtual u32 vWriteStream( s16 data ) = 0;
	virtual u32 vWriteStream( u32 data ) = 0;
	virtual u32 vWriteStream( s32 data ) = 0;
	virtual u32 vWriteStream( f32 data ) = 0;
	virtual u32 vWriteStream( d64 data ) = 0;

	/// <summary>
	/// ストリームから読込み（戻り値は実際に読み込んだサイズ）
	/// </summary>
	virtual u32 vReadStream( skyString  *data , u32 len ) = 0;
	virtual u32 vReadStream( u8  *pData, u32 size ) = 0;
	virtual u32 vReadStream( u16 &data ) = 0;
	virtual u32 vReadStream( s16 &data ) = 0;
	virtual u32 vReadStream( u32 &data ) = 0;
	virtual u32 vReadStream( s32 &data ) = 0;
	virtual u32 vReadStream( f32 &data ) = 0;
	virtual u32 vReadStream( d64 &data ) = 0;

	/// <summary>
	/// 書込み／読込み
	/// </summary>
	virtual u32 vWriteReadStream( u8* pData , u32 size , skyBool isWrite ) = 0;

	/// <summary>
	/// マーク書き込み
	/// </summary>
	virtual u32 vMarkWrite( const skyString *mark , u32 size ) = 0;
	virtual u32 vMarkReplace( const skyString *mark , u8* data , u32 size ) = 0;

	/// <summary>
	/// Seek位置を設定する
	/// </summary>
	virtual void vSetSeek( u32 seek ) = 0;
	virtual void vAddSeek( u32 seek ) = 0;

	/// <summary>
	/// 更新時間を取得する
	/// </summary>
	virtual updateTime vGetUpdateTime() = 0;
	virtual void vSetUpdateTime( updateTime time ) = 0;

	/// <summary>
	/// 名前を取得する
	/// </summary>
	virtual const skyString *vGetName() = 0;
	virtual void vSetName( const skyString *name ) = 0;

	/// <summary>
	/// １行読込
	/// <summary>
	virtual skyBool vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 ) = 0;

	/// <summary>
	/// １行書き込み
	/// <summary>
	virtual void vWriteLine( const skyString *pBuf , u32 len = 0 ) = 0;
};

//-----------------------------------< 抽象実装 >--------------------------------------------//

class AbsStream : public IStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 実装（抽象実装で実装する場合は必ずvirtualで宣言する。実装でオーバーライドされるかもしれないから）
	/// </summary>
	virtual eStreamKind vGetKind(){ return m_eKind; }
	virtual void vSetEndianSwap( skyBool isSwap ){ m_bIsSwap = isSwap; };
	virtual skyBool vIsSwap(){ return m_bIsSwap; };
	virtual skyBool vIsReadOnly() { return m_IsReadOnly; }
	virtual skyBool vIsEnableChengeSize() { return m_IsEnableChengeSize; }
	virtual void vSetSeek( u32 seek ){ m_Seek = seek; }
	virtual void vAddSeek( u32 seek ){ m_Seek += seek; }
	virtual updateTime vGetUpdateTime(){ return m_nUpdateTime; }
	virtual void vSetUpdateTime( updateTime time ){ m_nUpdateTime = time; }
	virtual skyBool vIsOpen(){ return m_IsOpen; };
	virtual const skyString *vGetName(){ return m_Name.Get(); };
	virtual void vSetName( const skyString *name ){ m_Name = name; };

//-------実装できるものは実装する▽
	virtual const u8*    vGetSeekData();
	virtual const u32    vGetSeek();
	virtual const u8*    vGetData();
	virtual const u8**   vGetDataPtr();
    virtual u8*          vCloneData();
	virtual u32		     vGetSize();
	virtual u32			 vWriteStream( u8* pData , u32 size );
	virtual u32			 vWriteStream( u16 data );
	virtual u32			 vWriteStream( s16 data );
	virtual u32			 vWriteStream( u32 data );
	virtual u32			 vWriteStream( s32 data );
	virtual u32			 vWriteStream( f32 data );
	virtual u32			 vWriteStream( d64 data );
	virtual u32		     vReadStream( u8* pData , u32 size );
	virtual u32			 vReadStream( u16 &data );
	virtual u32			 vReadStream( s16 &data );
	virtual u32			 vReadStream( u32 &data );
	virtual u32			 vReadStream( s32 &data );
	virtual u32			 vReadStream( f32 &data );
	virtual u32			 vReadStream( d64 &data );
	virtual u32          vWriteReadStream( u8* pData , u32 size , skyBool isWrite );
	virtual void	     vCreate( u32 size );
	virtual void 	     vDestroy();
	virtual u32			 vMarkWrite( const skyString *mark , u32 size );
	virtual u32			 vMarkReplace( const skyString *mark , u8* data , u32 size );
	virtual skyBool      vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 );
	virtual void         vWriteLine( const skyString *pBuf , u32 len = 0 );

//-------実装できるものは実装する△

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AbsStream( eStreamKind eKind ) : 
		 m_IsEnableChengeSize( skyFalse ) , 
		 m_eKind( eKind ) , 
		 m_bIsSwap( skyFalse ) , 
		 m_Seek( 0 ) , 
		 m_IsReadOnly( skyFalse ) , 
		 m_nUpdateTime( 0 ) , 
		 m_IsOpen( skyFalse ){}

	/// <summary>
	/// スワップをするフラグ
	/// </summary>
	skyBool m_bIsSwap;

	/// <summary>
	/// シーク位置
	/// </summary>
	u32 m_Seek;

	/// <summary>
	/// 更新時間
	/// </summary>
	updateTime m_nUpdateTime;

	/// <summary>
	/// 名前
	/// </summary>
	string::CString m_Name;
	
	/// <summary>
	/// 読込専用フラグ
	/// </summary>
	skyBool m_IsReadOnly;

	/// <summary>
	/// サイズ変更有効フラグ
	/// </summary>
	skyBool m_IsEnableChengeSize;

	/// <summary>
	/// ストリーム種別
	/// </summary>
	eStreamKind m_eKind;

	/// <summary>
	///  開いているフラグ
	/// </summary>  
	skyBool m_IsOpen;
};

} } }