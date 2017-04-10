#pragma once

namespace sky { namespace lib {

/// <summary>
/// 動的２次元配列コンテナクラス
/// </summary>
template< class T >
class CDictionary : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CDictionary();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CDictionary( u32 row = 0 , u32 col = 0 );

	/// <summary>
	/// リセット
	/// </summary>
	inline void Reset( u32 row , u32 col );

	/// <summary>
	/// 列数の取得
	/// </summary>
	inline u32 GetColNum() const;

	/// <summary>
	/// 行数の取得
	/// </summary>
	inline u32 GetRowNum() const;

	/// <summary>
	/// 値の参照の取得
	/// </summary>
	inline T &Get( u32 row , u32 col ) const;

	/// <summary>
	/// 値の参照の取得
	/// </summary>
	inline void Set( const T &val , u32 row , u32 col );

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	inline CDictionary& operator =( const CDictionary& src );

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CDictionary( const CDictionary& obj );

protected:

	/// <summary>
	/// 列数
	/// </summary>
	u32 m_Col;

	/// <summary>
	/// 行数
	/// </summary>
	u32 m_Row;

	/// <summary>
	/// データ
	/// </summary>
	T *m_pData;

	/// <summary>
	/// サイズ変更
	/// </summary>
	inline void SizeChange( u32 row , u32 col );

};

/// <summary>
/// 動的２次元配列コンテナクラス
/// </summary>
template< class T >
class CDictionaryPrimitive : public CDictionary< T >
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CDictionaryPrimitive(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CDictionaryPrimitive( u32 row = 0 , u32 col = 0 );

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CDictionaryPrimitive( const CDictionaryPrimitive& obj );

	/// <summary>
	/// 値の参照の取得
	/// </summary>
	inline void Set( const T &val , u32 row , u32 col );

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	inline CDictionaryPrimitive& operator =( const CDictionaryPrimitive& src );

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// デバッグプリント
	/// </summary>
	inline void DebugPrint();

#endif

private:

	/// <summary>
	/// サイズ変更
	/// </summary>
	inline void SizeChange( u32 row , u32 col );
};

typedef CDictionaryPrimitive< dectype > CDictionaryDec;

} }


#include "dictionary.inl"