#pragma once

namespace sky { namespace lib {

namespace clist
{
	static const u32 ITEM_POOL_DEFAULT_NUM = 8;
}

/// <summary>
/// 汎用型のリストクラス（このクラスは代入には対応していません。代入するような使い方をしないでください）
/// </summary>

template< typename KEY , typename DATA , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CList : public IteratorIf< DATA > , public ThreadingModel
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CList( u32 poolNum = clist::ITEM_POOL_DEFAULT_NUM );

	/// <summary>
	/// コピーコンストラクタ（未対応）
	/// </summary>
	inline CList( const CList& src ) : m_kItemPool( src.m_kItemPool.GetPoolNum() ) { SKY_PANIC(); }

	/// <summary>
	/// 代入演算子（未対応）
	/// </summary>
	inline CList& operator =( const CList& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CList();

	/// <summary>
	/// リスト末尾に追加
	/// </summary>
	inline DATA &AddFront();
	inline void AddFront( const DATA &data );

	/// <summary>
	/// リスト末尾に追加
	/// </summary>
	inline DATA &AddTail();
	inline void AddTail( const DATA &data );

	/// <summary>
	/// 指定のデータの後に追加
	/// </summary>
	inline void AddNext( const DATA &frontData , const DATA &data );

	/// <summary>
	/// id順に登録する。idが重複した場合id郡の末尾に登録される。つまり上書きはされません
	/// </summary>
	inline void AddTail( KEY id , const DATA &data );

	/// <summary>
	/// インデックスでリスト取得
	/// </summary>
	inline skyBool GetIdx( u32 idx , DATA &data ) const;
	inline DATA &GetIdx( u32 idx ) const;
	inline DATA &operator[]( s32 idx ) const;

	/// <summary>
	/// 引数と同じオブジェクトが存在するかどうか探す。見つかったら戻り値にインデックスを返す
	/// </summary>
	inline KEY	Find( const DATA &data ) const;

	/// <summary>
	/// キーで値取得
	/// </summary>
	inline skyBool GetKey( KEY id , DATA &data ) const;

	/// <summary>
	/// 値でリスト削除
	/// </summary>
	inline skyBool Delete( const DATA &data );

	/// <summary>
	/// 末尾の要素を削除する
	/// </summary>
	inline skyBool DeleteTail();

	/// <summary>
	/// 全削除
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// 現在の格納数
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// リストが空かどうか判定する
	/// </summary>
	inline skyBool IsEmpty() const;

//---------------------イテレーター関連-----------------------▽

	/// <summary>
	/// イテレーター
	/// </summary>
	class iterator : public Iterator< CList< KEY , DATA , ThreadingModel > , DATA >
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline iterator( CList< KEY , DATA , ThreadingModel > *pList , s32 cursor );
	};

	/// <summary>
	/// カーソルがBegin()より手前にある時True（FOREACH_REVERSのため）
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// 初期位置カーソルを指すイテレーター取得
	/// </summary>
	inline iterator Begin() const;

	/// <summary>
	/// イテレーター取得
	/// </summary>
	inline iterator End() const;

	/// <summary>
	/// イテレーターの指し示す位置を削除
	/// </summary>
	inline iterator Erases( iterator &it );

	/// <summary>
	/// 指定のイテレーターの次のイテレーターを取得
	/// </summary>
	inline iterator GetNextPos( iterator &it ) const;

	/// <summary>
	/// 指定のインデックスにカーソルを当てる
	/// </summary>
	inline skyBool SetItCursor( u32 idx , DATA* &data ) const;

	/// <summary>
	/// インデックスでリスト削除
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

//---------------------イテレーター関連-----------------------△

protected:

	/// <summary>
	/// リストを先頭から探索して指定のＩＤにカーソルを当てる(なかったらskyFalseを返す）
	/// </summary>
	inline DATA &SetCursorSearchStartHead( KEY id );

	/// <summary>
	/// リストを末尾から探索して指定のＩＤにカーソルを当てる(なかったらskyFalseを返す）
	/// </summary>
	inline DATA &SetCursorSearchStartTail( KEY id );

	/// <summary>
	/// リストの要素の型
	/// </summary>
	struct ListItem
	{
		DATA			data;
		KEY				id;
		ListItem*		pPrev;
		ListItem*		pNext;
		virtual ~ListItem(){}
	};

	/// <summary>
	/// メンバー
	/// </summary>
	ListItem					*m_pHead;		//リストの先頭
	ListItem					*m_pTail;		//リストの末尾
	u32							m_nAddNum;		//リストの登録数
	CMemoryPool< ListItem >		m_kItemPool;	//リスト要素のプール

	/// <summary>
	/// 新しいアイテムを作製して返す
	/// </summary>
	inline ListItem* _CreateItem( KEY id , const DATA &data );
	inline ListItem* _CreateItem( KEY id );
	inline ListItem* _CreateItem( const DATA &data );
	inline ListItem* _CreateItem();

	/// <summary>
	/// 指定のインデックスの値を先頭から線形探査して探す(かならず見つかるはず）
	/// </summary>
	inline void _SetCursorSearchStartHeadIdx( u32 idx, ListItem* &pCur ) const;

	/// <summary>
	/// 指定のインデックスの値を末尾から線形探査して探す（かならず見つかるはず）
	/// </summary>
	inline void _SetCursorSearchStartTailIdx( u32 idx, ListItem* &pCur ) const;

	/// <summary>
	/// 指定のＩＤの値を先頭から線形探査して探す（見つからないことも）
	/// </summary>
	inline void _SetCursorSearchStartHead( KEY id, ListItem* &pCur ) const;

	/// <summary>
	/// 指定のＩＤの値を末尾から線形探査して探す
	/// </summary>
	inline void _SetCursorSearchStartTail( KEY id, ListItem* &pCur ) const;

	/// <summary>
	/// 指定のポインタの値を先頭から線形探査して探す（見つからないことも）
	/// </summary>
	inline skyBool _SetCursor( DATA p, ListItem* &pCur );

	/// <summary>
	/// 現在のカーソル値を開放する
	/// </summary>
	inline void _DelCursor( ListItem* &pCur );
};

template< typename KEY, typename DATA >
class CListMT : public CList< KEY , DATA , thread::ThreadingModels::CMultiThread >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CListMT();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CListMT();
};

} }

#include "List.inl"