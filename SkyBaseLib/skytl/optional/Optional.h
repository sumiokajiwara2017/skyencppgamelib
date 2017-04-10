#pragma once

namespace sky { namespace lib {

class COptional_empty {};

/// <summary>
/// 戻り値クラスベース
/// </summary>
template < u64 size >
class COptionalBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
    COptionalBase();
    COptionalBase( COptionalBase const & other );

	/// <summary>
	/// =演算子のオーバーライド
	/// </summary>
    COptionalBase &operator = ( COptionalBase const & t );

	/// <summary>
	/// 有効判定
	/// </summary>
	skyBool const Valid() const;

	/// <summary>
	/// 無効判定
	/// </summary>
	skyBool const Invalid() const;

protected:

	/// <summary>
	/// 有効フラグ
	/// </summary>
    bool m_bValid;

	/// <summary>
	/// データ
	/// </summary>
    u8 m_data[ size ];
};

/// <summary>
/// 戻り値クラス
/// </summary>
template < class T >
class COptional : public COptionalBase< sizeof( T ) >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
    COptional	();
    COptional	( T const & t );
	COptional	( COptional_empty const &);
    COptional	( COptional const & other );

	/// <summary>
	/// =演算子のオーバーライド
	/// </summary>
    COptional & operator = ( T const & t );
    COptional & operator = ( COptional const & other );

	/// <summary>
	/// ==演算子のオーバーライド
	/// </summary>
	skyBool const operator == ( COptional const & other ) const;

	/// <summary>
	/// <演算子のオーバーライド
	/// </summary>
	skyBool const operator < ( COptional const & other) const;

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~COptional();

	// Accessors.

	/// <summary>
	/// *演算子のオーバーライド
	/// </summary>
	T const & operator * () const;
	T & operator * ();

	/// <summary>
	/// ->演算子のオーバーライド
	/// </summary>
	T const * const operator -> () const;
	T		* const operator -> ();

	/// <summary>
	/// 初期化
	/// </summary>
	void Clear();

	/// <summary>
	/// 有効か？
	/// </summary>
	skyBool const Valid() const;

	/// <summary>
	/// 無効か？
	/// </summary>
	skyBool const Invalid() const;

private:

	/// <summary>
	/// 型の取得
	/// </summary>
    T const * const GetT		() const;
    T * const		GetT		();

	/// <summary>
	/// コンストラクタ処理
	/// </summary>
	void			Construct	( T const & t );

	/// <summary>
	/// 破棄処理
	/// </summary>
    void			Destroy		();
};

} }

#include "Optional.inl"