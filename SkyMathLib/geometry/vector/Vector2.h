#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ２Ｄ頂点クラス
/// ベースはゲーム３Ｄ数学のサンプルから頂いてきました。
/// </summary>
template< typename T >
class CVector2 : public CGeometryBase
{

public:

	/// <summary>
	/// ２Ｄ頂点
	/// </summary>
	union 
	{
		struct 
		{
			T		X;
			T		Y;
		};
		T v[ 2 ];
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVector2();				//デフォルトのコンストラクタ
	inline CVector2( const CVector2& v1 );		//他のインスタンスからの要素の値コピーインスタンス生成
	inline CVector2(T nX , T nY );			//要素番号を受け取りインスタンス生成

	/// <summary>
	/// []演算子オーバーライド
	/// </summary>
	inline T operator[]( s32 idx );

	/// <summary>
	/// 値設定
	/// </summary>
	inline CVector2< T > &Set( T x , T y );

	/// <summary>
	/// ０初期化
	/// </summary>
	inline void Zero();

	/// <summary>
	/// 空判定
	/// </summary>
	inline skyBool IsEmpty();

	/// <summary>
	/// =演算子オーバーライド
	/// </summary>
	inline CVector2& operator =( const CVector2& v1 );

	/// <summary>
	/// ==演算子オーバーライド
	/// </summary>
	inline skyBool operator ==( const CVector2& v1 ) const;

	/// <summary>
	/// !=演算子オーバーライド
	/// </summary>
	inline skyBool operator !=( const CVector2& v1 ) const;

	/// <summary>
	/// +演算子オーバーライド
	/// </summary>
	inline CVector2 operator +( const CVector2& v1 ) const;

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	inline CVector2 operator -( const CVector2& v1 ) const;

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	inline CVector2 operator -() const;

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	inline CVector2 operator *( T val ) const;
	inline CVector2 operator *( CVector2 vec ) const;

	/// <summary>
	/// /演算子オーバーライド
	/// </summary>
	inline CVector2 operator /( T val ) const;
	inline CVector2 operator /( CVector2 vec ) const;

	/// <summary>
	/// +=演算子オーバーライド
	/// </summary>
	inline CVector2& operator +=( const CVector2 &v1 );

	/// <summary>
	/// -=演算子オーバーライド
	/// </summary>
	inline CVector2& operator -=( const CVector2 &v1 );

	/// <summary>
	/// -=演算子オーバーライド
	/// </summary>
	inline CVector2& operator *=( T val );

	/// <summary>
	/// /=演算子オーバーライド
	/// </summary>
	inline CVector2& operator /=( T val );

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	inline void	Normalize();

	/// <summary>
	/// ベクトルの内積を計算する
	///
	/// 【内積】
	/// 一般的にどんな次元のベクトルの内積も二つのベクトルがどれぐらい「似ているか」を表します。
	/// 内積が大きいほど「似ている」ことになります。
	/// +-----+------+---------------------------------------------------------------------------------------------------+
	/// | > 0 | 鋭角 | おおむね同じ向きを向く(正規化したベクトル同士の場合は1.0fで完全に同じ方向ということになる)        |
	/// |  0  | 直角 | 直行している								                                                         |
	/// | < 0 | 鈍角 | おおむね反対方向を向いている（正規化したベクトル同士の場合は-1.0fで完全に反対方向ということになる |
	/// +-----+------+---------------------------------------------------------------------------------------------------+
	/// 例）CVector2( 2 , 0 ) と CVector( 3 , 0 )の内積は6。この2つのベクトルは平行である
	/// 例）CVector2( 2 , 0 ) と CVector( -3 , 0 )の内積は-6。この2つのベクトルは平行（逆方向）である
	/// 例）CVector2( 2 , 0 ) と CVector( 0 , 3 )の内積は0。この2つのベクトルは直行である
	/// </summary>
	T Dot( const CVector2& v1 ) const;

    /// <summary>
    /// ベクトルを回転させた後のベクトルを取得する
    /// </summary>
	inline CVector2< T > Rot( T rad );

    /// <summary>
    /// このベクトルに直角なベクトルを返す
    /// </summary>
    inline CVector2 Perp();

    /// <summary>
    /// ベクトルのなす角(ラジアン）を求める
    /// </summary>
    inline T IncludedAngle( const CVector2 &v1 );

    /// <summary>
    /// ベクトルを指定の長さにする
    /// </summary>
    inline T SetLength( T length );

    /// <summary>
    /// ベクトルが指定の長さを越えていたら指定の長さにする
    /// </summary>
	inline T Truncate( T length );

    /// <summary>
    /// ベクトルの長さを求める
    /// </summary>
    inline T Length() const;

    /// <summary>
    /// 値を切り捨てます
    /// </summary>
	inline void Floor();

    /// <summary>
    /// 値を四捨五入します。
    /// </summary>
	inline void Round();
};

/// <summary>
/// ベクトルの大きさを計算する
/// </summary>
template< class T >
inline T VectorMag( const CVector2< T > &v1 );

/// <summary>
/// ベクトルの外積を計算する
/// 
/// 【外積】 
///  外積は与えられた２つのベクトルに垂直なベクトルを返します。
///  内積と外積が同時に計算された場合外積のほうが先に計算されます。
///  
///  外積の大きさはベクトルのなす角のsinの大きさに比例します。
///  外積の大きさはもととなる２つのベクトルの大きさにsinθをかけた値に等しい。
///  ||a×b|| == ||a||||b||sinθ
/// </summary>
template< class T >
inline T Cross( const CVector2< T >& v1, const CVector2< T >& v2 );

/// <summary>
/// *演算子オーバーライド（値*Vector2インスタンス。スタック上に新しいインスタンスを生成します。）
/// </summary>
template< class T >
inline CVector2< T > Mult(f32 k, const CVector2< T > &v1);

/// <summary>
/// ベクトル間の距離を返します。
/// </summary>
template< class T >
inline T Distance(const CVector2< T >& v1, const CVector2< T >& v2);

/// <summary>
/// ベクトル間の距離を返します。
/// 平方根の計算を省いています。単純な比較の場合こちらを使ったほうが高速
/// </summary>
template< class T >
inline T DistanceSquared( const CVector2< T >& v1, const CVector2< T >& v2 );

//基本ベクトル２型
typedef CVector2< dectype >                 CBasicVector2;
typedef CVectorPrimitive< CBasicVector2 >	CBasicVector2Vector;			//配列
typedef kfm::CFCurve< CBasicVector2 >       CBasicVector2FCurve;
typedef kfm::CFCurvePlayer< CBasicVector2 > CBasicVector2FCurvePlayer;

/// <summary>
/// ２次元ゼロベクトル
/// </summary>
static const CBasicVector2 CBasicVector2_ZERO( 0.0f , 0.0f );

} } }

#include "Vector2.inl"