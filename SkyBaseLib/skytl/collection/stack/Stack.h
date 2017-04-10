#pragma once

namespace sky { namespace lib {

/// <summary>
/// 汎用型のスタッククラス（このクラスは代入には対応していません。代入するような使い方をしないでください）
/// </summary>
template< typename T >
class CStack : public IteratorIf< T >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CStack( s32 size );

	/// <summary>
	/// コピーコンストラクタ（未対応）
	/// </summary>
	inline CStack( const CStack& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); }

	/// <summary>
	/// 代入演算子（未対応）
	/// </summary>
	inline CStack& operator =( const CStack& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CStack();

	/// <summary>
	/// スタック追加
	/// </summary>
	inline u32 Push( const T &data );
	inline u32 Push();

	/// <summary>
	/// スタックの一番上に値を設定する
	/// </summary>
	inline void Set( const T &data );

	/// <summary>
	/// スタックから値を取り出す
	/// </summary>
	inline skyBool Pop( T &data );
	inline skyBool Pop();
	inline skyBool PopBottom( T &data ); //重い（普通スタックでこれはやらない。）
	inline skyBool PopBottom();          //重い（普通スタックでこれはやらない。）

	/// <summary>
	/// クリアする（内部的にはカウンタを0に戻すだけ）
	/// </summary>
	inline void Clear();

	/// <summary>
	/// 現在の格納数を取得する
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// スタックのサイズを取得する
	/// </summary>
	inline u32 GetSize() const;

	/// <summary>
	/// スタックの一番上の値の参照を取得します。（この参照はスタックの状態が変化したら値が変化する。すぐに使用すること）
	/// </summary>
	inline T &GetTop();

	/// <summary>
	/// スタックの一番下の値の参照を取得します。（この参照はスタックの状態が変化したら値が変化する。すぐに使用すること）
	/// </summary>
	inline T &GetBottom();

	/// <summary>
	/// スタックの底を０とし、インデックス指定で値を取得します。
	/// </summary>
	inline T &GetItem( s32 idx ) const;
	inline skyBool GetItem( s32 idx , T &data ) const;
	inline T &operator[]( s32 idx ) const;

	/// <summary>
	/// 空判定
	/// </summary>
	inline skyBool IsEmpty() const { return m_PushNum == 0; };

//---------------------イテレーター関連-----------------------▽

	/// <summary>
	/// イテレーター
	/// </summary>
	class iterator : public Iterator<CStack< T >,T>
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline iterator( CStack< T > *pStack, s32 cursor );
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
	T*		m_pData;
	u32		m_Size;
	u32		m_PushNum;	//スタックの登録数

private:

	/// <summary>
	/// 現在のカーソル位置アドレスを取得
	/// </summary>
	inline skyBool	_SetCursor( u32 idx , T* &pCur ) const;

};

} }

#include "Stack.inl"