#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// サイズクラス
/// </summary>
template< typename T >
class CSize : public base::SkyRefObject
{
public:

	/// <summary>
	/// サイズ
	/// </summary>
	T		W;
	T		H;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSize();
	CSize( T w, T h );


	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CSize();

	/// <summary>
	/// 空判定
	/// </summary>
	skyBool IsEmpty();

	/// <summary>
	/// 設定
	/// </summary>
	void Set( T w, T h );

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	CSize& operator =( const CSize& size );

	/// <summary>
	/// +演算子オーバーライド（要素の足し算,新しいインスタンスが生成されて返ります。）
	/// </summary>
	CVector2< T > operator +( const CVector2< T >& v1 ) const;

};

//基本矩形型
typedef CSize< dectype >     CBasicSize;
typedef CSize< u32 >         CSize_u32;

} } }

#include "Size.inl"