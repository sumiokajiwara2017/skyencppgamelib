#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ４Ｄ頂点クラス
/// </summary>
template< typename T >
class CVector4 : public CGeometryBase
{

public:

	/// <summary>
	/// ３Ｄ頂点
	/// </summary>
	union 
	{
		struct 
		{
			T		X;
			T		Y;
			T		Z;
			T       W;
		};
		T v[ 4 ];
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVector4();				//デフォルトのコンストラクタ
	inline CVector4( const CVector3< T >& v , T w );	//他のインスタンスからの要素の値コピーインスタンス生成
	inline CVector4( const CVector4& v );	//他のインスタンスからの要素の値コピーインスタンス生成
	inline CVector4( T nX , T nY , T nZ , T nW );	//要素番号を受け取りインスタンス生成

	/// <summary>
	/// []演算子オーバーライド
	/// </summary>
	inline T operator[]( s32 idx );

	/// <summary>
	/// ０初期化
	/// </summary>
	inline void Zero();

	/// <summary>
	/// 値のセット
	/// </summary>
	inline void Set( T x , T y , T z , T w );

	/// <summary>
	/// ３次元で取得
	/// </summary>
	inline CVector3<T> GetVector3() const;

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	inline CVector4& operator =( const CVector4& v );

	/// <summary>
	/// *=演算子オーバーライド
	/// </summary>
	inline CVector4& operator *=( T val );

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	inline CVector4&	Normalize();

	/// <summary>
	/// 長さを求める
	/// </summary>
    inline T Length() const;

	/// <summary>
	/// ベクトルの内積を計算する
	/// </summary>
	inline T Dot( const CVector4& v ) const;
};

//基本ベクトル３型
typedef CVector4< dectype >                 CBasicVector4;

/// <summary>
/// ４次元ZEROベクトル
/// </summary>
static const CBasicVector4 CBasicVector4_ZERO( 0.0f , 0.0f , 0.0f , 0.0f );

} } }

#include "Vector4.inl"