#pragma once

namespace sky { namespace lib { namespace memory {

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// メモリ上に存在する領域に関して読み書きするクラス。
/// 全部読み込んだファイル、通信で取得したデータ、コンソールから入力したデータ等をここに格納する予定
/// データを書き込む人と読み込む人でこのクラスのインスタンスを共有したらいいのかも。でもその２人は同じスレッド上で動くか、このクラスにクリティカルセクションを張る必要がある
/// </summary>
class CMemoryStream : public stream::AbsStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
	/// </summary>  
	const u8*    vGetSeekData() { return vGetData() + m_Seek; };
	const u32    vGetSeek()     { return m_Seek; };
	const u8*    vGetData()     { return m_pData; };
	const u8**   vGetDataPtr()  { return ( const u8 ** )&m_pData; };

    u8*          vCloneData();
	u32		     vGetSize(){ return m_Size; };
	skyBool      vOpen();
	skyBool      vClose();
	u32			 vWriteStream( const skyString *format , ... );
	u32			 vWriteStream( u8* pData , u32 size );
	u32			 vWriteStream( u16 data );
	u32			 vWriteStream( s16 data );
	u32			 vWriteStream( u32 data );
	u32			 vWriteStream( s32 data );
	u32			 vWriteStream( f32 data );
	u32			 vWriteStream( d64 data );
	u32		     vReadStream( skyString  *data , u32 len );
	u32		     vReadStream( u8  *data , u32 size );
	u32		     vReadStream( u16 &data );
	u32			 vReadStream( s16 &data );
	u32			 vReadStream( u32 &data );
	u32			 vReadStream( s32 &data );
	u32			 vReadStream( f32 &data );
	u32			 vReadStream( d64 &data );
	void	     vCreate( u32 size );
	void 	     vDestroy();
	u32			 vMarkWrite( const skyString *mark , u32 size );
	u32			 vMarkReplace( const skyString *mark , u8* data , u32 size );

	/// <summary>
	///  コンストラクタ
	/// </summary>
	CMemoryStream( const skyString *name , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize  );
	CMemoryStream( const skyString *name , void* data , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

	/// <summary>
	///  デストラクタ
	/// </summary>
	virtual ~CMemoryStream();

protected:

	/// <summary>
	/// データ
	/// </summary>    
	u8 *m_pData;

	/// <summary>
	/// データサイズ
	/// </summary>    
	u32 m_Size;

	/// <summary>
	/// データを内部関数で破棄するか（外部から渡したデータの場合、破棄を行わないほうがいい場合がある）
	/// </summary>
	skyBool m_IsEnableDelete;

	/// <summary>
	/// マーク情報Ｈａｓｈ
	/// </summary>    
	stream::MarkInfoHash *m_pMarkInfoHash;

	/// <summary>
	/// コンストラクタ共通初期化
	/// </summary>
	void ComInit( const skyString *name , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

};

} } }