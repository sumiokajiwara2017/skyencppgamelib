#pragma once

namespace sky { namespace lib {

namespace hashmap
{
	static const u32 ITEM_POOL_DEFAULT_NUM = 8;
}

/// <summary>
/// Hashコンテナクラス（このクラスは代入には対応していません。代入するような使い方をしないでください）
/// </summary>
template< typename T , typename KEY = u32 , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CHash : public IteratorIf< T > , public ThreadingModel
{
	
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CHash( s32 hashSize , u32 poolNum = hashmap::ITEM_POOL_DEFAULT_NUM );

	/// <summary>
	/// コピーコンストラクタ（未対応）
	/// </summary>
	inline CHash( const CHash& src ){ SKY_PANIC(); }

	/// <summary>
	/// 代入演算子（未対応）
	/// </summary>
	inline CHash& operator =( const CHash& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CHash();

	/// <summary>
	/// 値を設定
	/// </summary>
	inline T &Set( const hash::CHashStringPtr &hash );
	inline T &Set( const skyString *strId );
	inline T &Set( KEY id );
	inline void Set( const hash::CHashStringPtr &hash , const T &data );
	inline void Set( const skyString *strId , const T &data );
	inline void Set( KEY id , const T &data );

	/// <summary>
	/// 値が存在するか
	/// </summary>
	inline skyBool IsGet( KEY id );
	inline skyBool IsGet( const skyString *strId );
	inline skyBool IsGet( const hash::CHashStringPtr &hash );

	/// <summary>
	/// 値を取得
	/// </summary>
	inline skyBool Get( const skyString *strId , T &data ) const;
	inline T &Get( const skyString *strId ) const;
	inline T &operator[]( const skyString *strId ) const;

	inline skyBool Get( const hash::CHashStringPtr &hash , T &data ) const;
	inline T &Get( const hash::CHashStringPtr &hash ) const;
	inline T &operator[]( const hash::CHashStringPtr &hash ) const;

	inline skyBool Get( KEY id , T &data ) const;
	inline T &Get( KEY id ) const;
	inline T &operator[]( KEY id ) const;

	/// <summary>
	/// 値を削除
	/// </summary>
	inline skyBool Delete( const hash::CHashStringPtr &hash );
	inline skyBool Delete( const skyString *strId );
	inline skyBool Delete( KEY id );

	/// <summary>
	/// 値を削除
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// 現在の格納数
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// 現在の格納数
	/// </summary>
	inline u32 GetHashSize() const;

	/// <summary>
	/// KeyListの取得
	/// </summary>
	inline void GetkeyList( hash::CHashStringPtrList &keyList ) const;

	/// <summary>
	/// リストが空かどうか判定する
	/// </summary>
	inline skyBool IsEmpty() const;

//---------------------イテレーター関連-----------------------▽

	/// <summary>
	/// イテレーター
	/// </summary>
	class iterator : public Iterator< CHash< T , KEY , ThreadingModel > , T >
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline iterator( CHash< T , KEY , ThreadingModel > *pHash , s32 cursor );
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
	/// 指定のカーソルの値を取得する
	/// @@TODOこのロジックは高速化のために工夫の余地が残っている・・と思う。
	/// </summary>
	inline skyBool SetItCursor( u32 idx , T* &data ) const;
	inline skyBool SetItCursor( u32 idx , T* &data , hash::CHashStringPtr &idst ) const;

	/// <summary>
	/// 指定のカーソルの値を削除する
	/// @@TODOこのロジックは高速化のために工夫の余地が残っている・・と思う。
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

protected:

	struct _HashListTbl
	{
		KEY  					id;     					// 識別ID（数字）
		hash::CHashStringPtr	idst;						// 識別ID（文字）※数字だけだと情報が劣化するため
		T 						data;   					// 値
		struct _HashListTbl		*pNext; 					// 次のデータ

		virtual ~_HashListTbl(){}
	};

	_HashListTbl				**m_ppListTbl;			//ハッシュ本体
	u32							m_nSetNum;				//現在の設定数
	u32							m_nHashSize;			//ハッシュキー範囲（素数がいいらしい）
	CMemoryPool< _HashListTbl >	m_kItemPool;			//リスト要素のプール

	/// <summary>
	/// Hash値(実際のHashテーブルの格納先）計算
	/// </summary>
	inline u32 GetHashId( const skyString *pKeyStr ) const;

	/// <summary>
	/// ID比較
	/// </summary>
	inline skyBool EqId( KEY id , _HashListTbl *pList ) const;

	/// <summary>
	/// 値の設定
	/// </summary>
	inline skyBool _Set( u32 id , u32 hashId , const T &data , const hash::CHashStringPtr &idst );

	/// <summary>
	/// 値を取得
	/// </summary>
	inline skyBool _Get( u32 id , u32 hashId , T* &data ) const;

	/// <summary>
	/// 値を削除
	/// </summary>
	inline skyBool _Delete( u32 id , u32 hashId );

	/// <summary>
	/// 指定の値を消す
	/// </summary>
	inline void _DelList( _HashListTbl *pList );
};

template< typename T , typename KEY = s32 >
class CHashMT : public CHash< T , KEY , thread::ThreadingModels::CMultiThread >
{
	
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CHashMT( s32 hashSize );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CHashMT();
};

} }

#include "Hash.inl"