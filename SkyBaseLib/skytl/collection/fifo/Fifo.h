#pragma once

namespace sky { namespace lib {

namespace fifo
{
	static const u32 DEFAULT_FIFO_SIZE	= 128;
}
using namespace fifo;

/// <summary>
/// 汎用型のＦＩＦＯクラス（このクラスは代入には対応していません。代入するような使い方をしないでください）
/// </summary>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CFifo : public IteratorIf< T > , public ThreadingModel
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFifo( s32 size = DEFAULT_FIFO_SIZE );

	/// <summary>
	/// コピーコンストラクタ（未対応）
	/// </summary>
	inline CFifo( const CFifo& src ){ SKY_PANIC(); }

	/// <summary>
	/// 代入演算子（未対応）
	/// </summary>
	inline CFifo& operator =( const CFifo& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFifo();

	/// <summary>
	/// ＦＩＦＯ追加
	/// </summary>
	inline skyBool Push( const T &data );
	inline skyBool Push();
	inline skyBool IsFull();

	/// <summary>
	/// 指定の値にキャンセルフラグを建てる。IsFrontCancelやIsCancelなどで値のキャンセル判定ができる。
	/// キャンセル指定したとしてもキューからはなくならない、使用者が判定し、値をスキップすること。
	/// </summary>
	inline void SetCancelFlag( const T &data , skyBool isCancel );

	/// <summary>
	/// 先頭の値がキャンセルされているかどうか判定する
	/// </summary>
	inline skyBool IsFrontCancel();

	/// <summary>
	/// Fifoの末尾の値の参照を取得
	/// </summary>
	inline T &GetBack();

	/// <summary>
	/// Fifoの先頭の値の参照を取得
	/// </summary>
	inline T &GetFront();

	/// <summary>
	/// ＦＩＦＯから値を取り出す
	/// </summary>
	inline skyBool Pop( T &dest );
	inline skyBool Pop();

	/// <summary>
	/// ＦＩＦＯから値を全部取り出す。isExecDataFreeがskyTrueだと値がすべてフリーされる。（値がポインタじゃないとハング）
	/// </summary>
	inline void AllPop();

	/// <summary>
	/// ＦＩＦＯから指定の位置のキャンセル判定をする
	/// </summary>
	skyBool IsCancel( u32 idx ) const;

	/// <summary>
	/// ＦＩＦＯから指定の位置の値を取り出す。
	/// </summary>
	inline skyBool	GetItem( u32 idx , T* &data ) const;
	inline T	&operator[]( s32 idx ) const;

	/// <summary>
	/// 値が存在しないかどうか判定
	/// </summary>
	inline skyBool IsNoData() const;

	/// <summary>
	/// 現在の格納数を取得する
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// ＦＩＦＯのサイズを取得する
	/// </summary>
	inline u32 GetSize() const;


	/// <summary>
	/// 空判定
	/// </summary>
	inline skyBool IsEmpty() const { return m_PushNum == 0; };

//---------------------イテレーター関連-----------------------▽

	/// <summary>
	/// イテレータークラス
	/// </summary>
	class iterator : public Iterator< CFifo< T , ThreadingModel > , T >
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline iterator( CFifo< T , ThreadingModel > *pFifo , s32 cursor );
	};

	/// <summary>
	/// カーソルがBegin()より手前にある時True（FOREACH_REVERSのため）
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// 初期位置カーソルを指すイテレーター取得
	/// </summary>
	inline iterator Begin();

	/// <summary>
	/// イテレーター取得
	/// </summary>
	inline iterator End();

	/// <summary>
	/// イテレーターの指し示す位置を削除
	/// </summary>
	inline iterator Erases( iterator &it );

	/// <summary>
	/// 指定のインデックスにカーソルを当てる
	/// </summary>
	inline skyBool SetItCursor( u32 idx , T* &data ) const;

	/// <summary>
	/// インデックスでリスト削除
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

//---------------------イテレーター関連-----------------------△

protected:

	/// <summary>
	/// メンバー
	/// </summary>

	struct Item
	{
		T data;
		skyBool isCancel;
	};

	Item	*m_pDataBuffer;		//データ配列
	u32		m_BufferSize;		//データサイズ
	u32 	m_Rp;         	 	//読み込みポインタ
	u32 	m_Wp;         	 	//書き込みポインタ
	u32 	m_PushNum;      	//格納数
};

template< typename T >
class CFifoMT : public CFifo< T , thread::ThreadingModels::CMultiThread >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFifoMT( s32 size = DEFAULT_FIFO_SIZE );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFifoMT();
};

} }

#include "Fifo.inl"
