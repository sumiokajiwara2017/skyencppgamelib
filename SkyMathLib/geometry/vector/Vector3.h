#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

/// <summary>
/// ３Ｄ頂点クラス
/// ベースはゲーム３Ｄ数学のサンプルから頂いてきました。
/// </summary>
template< typename T >
class CVector3 : public CGeometryBase
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
		};
		T v[ 3 ];
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVector3();				        //デフォルトのコンストラクタ
	inline CVector3( const CVector3& v );	//他のインスタンスからの要素の値コピーインスタンス生成
	inline CVector3( T nX , T nY , T nZ );	//要素番号を受け取りインスタンス生成
	inline CVector3( const dectype vec[ 3 ] );

	/// <summary>
	/// []演算子オーバーライド
	/// </summary>
	inline T operator[]( s32 idx ) const;

	/// <summary>
	/// ０初期化
	/// </summary>
	inline void Zero();

	/// <summary>
	/// 値のセット
	/// </summary>
	inline CVector3< T > &Set( T x , T y , T z );

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	inline CVector3& operator =( const CVector3& v );
	inline CVector3& operator =( const CVector2< T >& v );

	/// <summary>
	/// ==演算子オーバーライド（要素の比較）
	/// </summary>
	inline skyBool operator ==( const CVector3& v ) const;

	/// <summary>
	/// !=演算子オーバーライド
	/// </summary>
	inline skyBool operator !=( const CVector3& v ) const;

	/// <summary>
	/// +演算子オーバーライド
	/// </summary>
	inline CVector3 operator +( const CVector3& v ) const;

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	inline CVector3 operator -( const CVector3& v ) const;

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	inline CVector3 operator -() const;

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	inline CVector3 operator *( T val ) const;
	inline CVector3 operator *( const CVector3 &vec ) const;

	/// <summary>
	/// /演算子オーバーライド
	/// </summary>
	inline CVector3 operator /( T val ) const;
	inline CVector3 operator /( const CVector3 &vec ) const;

	/// <summary>
	/// +=演算子オーバーライド
	/// </summary>
	inline CVector3& operator +=( const CVector3 &v );

	/// <summary>
	/// -=演算子オーバーライド
	/// </summary>
	inline CVector3& operator -=( const CVector3 &v );

	/// <summary>
	/// *=演算子オーバーライド
	/// </summary>
	inline CVector3& operator *=( T val );

	/// <summary>
	/// /=演算子オーバーライド
	/// </summary>
	inline CVector3& operator /=( T val );

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	inline CVector3&	Normalize();

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
	/// </summary>
	inline T Dot( const CVector3& v ) const;

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
	inline CVector3< T > Cross( const CVector3< T >& v ) const;

    /// <summary>
    /// ベクトルのなす角(ラジアン）を求める
    /// </summary>
    inline T IncludedAngle( const CVector3 &v ) const;

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
	inline T QuaredLength() const; //2乗
    inline T LengthXZ() const;

	/// <summary>
	/// ベクトルの長さをコピーしたベクトルを返す
	/// </summary>
	inline CVector3< T > Vector3Length() const;

	/// <summary>
	/// f32型の配列に内容をコピーする
	/// </summary>
	inline void Copyf32Array( f32 *pDestArray ) const;

	/// <summary>
	/// 全ての要素が0かどうか調べる
	/// </summary>
	inline skyBool IsAllZero() const;

	/// <summary>
	/// 全ての要素が1かどうか調べる
	/// </summary>
	inline skyBool IsAllOne() const;
};

/// <summary>
/// ベクトルの大きさを計算する
/// </summary>
template< class T >
inline T VectorMag( const CVector3< T > &v );

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
inline CVector3< T > Cross( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// ベクトルの内積を計算する
///
/// 【内積】
/// 一般的にどんな次元のベクトルの内積も二つのベクトルがどれぐらい「似ているか」を表します。
/// 内積が大きいほど「似ている」ことになります。
///
/// +-----+------+------------------------------------------+
/// | > 0 | 鋭角 | おおむね同じ向きを向く					|
/// |  0  | 直角 | 直行している								|
/// | < 0 | 鈍角 | おおむね反対方向を向いている				|
/// +-----+------+------------------------------------------+
/// </summary>
template< class T >
inline T Dot( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// *演算子オーバーライド（値*CVector3インスタンス。スタック上に新しいインスタンスを生成します。）
/// </summary>
template< class T >
inline CVector3< T > Mult( T k , const CVector3< T > &v );

/// <summary>
/// ベクトル間の距離を返します。
/// </summary>
template< class T >
inline T Distance( const CVector3< T >& v1, const CVector3< T >& v2 );

/// <summary>
/// ベクトル間の距離を返します。
/// 平方根の計算を省いています。単純な比較の場合こちらを使ったほうが高速
/// </summary>
template< class T >
inline T DistanceSquared( const CVector3< T >& v1, const CVector3< T >& v2 );

/// <summary>
/// v1がv2よりも大きい場合skyTrueを返す。
/// </summary>
template< class T >
inline skyBool Vector3Greater( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// 各要素の逆数ベクトルを返す
/// </summary>
template< class T >
inline CVector3< T > VectorReciprocal( const CVector3< T > &v );

/// <summary>
/// 行列変換
/// </summary>
template< class T >
inline CVector3< T > Transform( const CVector3< T > &vec , const CMatrix4x3 &matrix );

//基本ベクトル３型
typedef CVector3< dectype >                 CBasicVector3;
typedef CList< u32 , CBasicVector3 >        CBasicVector3List;
typedef CVectorPrimitive< CBasicVector3 >	CBasicVector3Vector;	//配列
typedef CStack< CBasicVector3 >		        CBasicVector3Stack;
typedef kfm::CFCurve< CBasicVector3 >       CBasicVector3FCurve;
typedef kfm::CFCurvePlayer< CBasicVector3 > CBasicVector3FCurvePlayer;

/// <summary>
/// ３次元ZEROベクトル
/// </summary>
static const CBasicVector3 CBasicVector3_ZERO( 0.0f , 0.0f , 0.0f );
static const CBasicVector3 CBasicVector3_UPY( 0.0f , 1.0f , 0.0f );
static const CBasicVector3 CBasicVector3_UPZ( 0.0f , 0.0f , 1.0f );

/// <summary>
/// 軸定数
/// </summary>
static const CBasicVector3 VECTOR3_AXIS_X   ( 1.0f , 0.0f , 0.0f );
static const CBasicVector3 VECTOR3_AXIS_Y   ( 0.0f , 1.0f , 0.0f ); 
static const CBasicVector3 VECTOR3_AXIS_Z   ( 1.0f , 0.0f , 1.0f ); 

} } }

#include "Vector3.inl"