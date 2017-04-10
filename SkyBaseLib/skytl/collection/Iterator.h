#pragma once

namespace sky { namespace lib {

/// <summary>
/// イテレーターを実装するコンテナのインターフェース
/// </summary>
template<typename ITEM>
class IteratorIf : public base::SkyRefObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline IteratorIf(){};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IteratorIf(){};

	/// <summary>
	/// 指定のカーソルの値を取得する
	/// </summary>
	virtual skyBool SetItCursor( u32 idx , ITEM* &data ) const = 0;

	/// <summary>
	/// 指定のカーソルの値を削除する
	/// </summary>
	virtual skyBool DeleteIdx( u32 idx ) = 0;

	/// <summary>
	/// 要素の数を返す
	/// </summary>
	virtual u32 GetNum() const = 0;

};

/// <summary>
/// イテレーター
/// </summary>
template<typename CONTAINER, typename ITEM>
class Iterator : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline Iterator( CONTAINER* pContainer, s32 cursor )
	{
		m_pContainer = pContainer;
		m_Cursor = cursor;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Iterator(){}

	/// <summary>
	/// 初期化する
	/// </summary>
	inline void Init()
	{
		m_Cursor = 0;
	}

	/// <summary>
	/// 値取得
	/// </summary>
	inline skyBool GetValue( ITEM &item ) const
	{
		ITEM &data;
        skyBool result = skyFalse;
		if ( m_pContainer->SetItCursor( m_Cursor , data ) == skyTrue )
		{
			item = data;
            result = skyTrue;
		}

		return result;
	}

	/// <summary>
	/// ++演算子オーバーライド（前置式）
	/// </summary>
	inline void operator ++()
	{
		m_Cursor = Next();
	}
	/// <summary>
	/// ++演算子オーバーライド（後置式）
	/// </summary>
	inline void operator ++( int )
	{
		m_Cursor = Next();
	}

	/// <summary>
	/// --演算子オーバーライド（前置式）
	/// </summary>
	inline void operator --()
	{
		m_Cursor = Prev();
	}

	/// <summary>
	/// --演算子オーバーライド（後置式）
	/// </summary>
	inline void operator --( int )
	{
		m_Cursor = Prev();
	}

	/// <summary>
	/// ==演算子オーバーライド
	/// </summary>
	inline skyBool operator ==(const Iterator &it) const
	{
		return ( m_Cursor == it.m_Cursor );
	}

	/// <summary>
	/// !=演算子オーバーライド
	/// </summary>
	inline skyBool operator !=(const Iterator &it) const
	{
		return ( m_Cursor != it.m_Cursor );
	}

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	inline ITEM &operator *()
	{
		ITEM *data = skyNull;
		if ( m_pContainer->SetItCursor( m_Cursor , data ) == skyFalse )
		{
			SKY_PANIC_MSG( _T( "Data Access error occurred." ) );
		}
 		return *data;
	}

	/// <summary>
	/// 現在のカーソル位置の値を削除する
	/// </summary>
	inline s32 Delete()
	{
		m_pContainer->DeleteIdx( m_Cursor );
		return m_Cursor;
	}

	/// <summary>
	/// 現在のカーソル位置を返す
	/// </summary>
	inline s32 GetIdx()
	{
		return m_Cursor;
	}

	/// <summary>
	/// 次のカーソル位置を返す
	/// </summary>
	inline s32 Next()
	{
		s32 next = m_Cursor + 1;

		//上限を超えさせない
		u32 itemNum = m_pContainer->GetNum();
		if ( itemNum < ( u32 )next )
		{
			next = itemNum;
		}

		return next;
	}

	/// <summary>
	/// 前のカーソル位置を返す
	/// </summary>
	inline s32 Prev()
	{
		s32 prev = m_Cursor - 1;

		return prev;
	}

protected:

	/// <summary>
	/// 現在のカーソル位置
	/// </summary>
	s32			m_Cursor;

	/// <summary>
	/// 操作対象リスト
	/// </summary>
	CONTAINER*	m_pContainer;

};

} }
