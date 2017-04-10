#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;

class CQuaternion;
typedef CVector< CQuaternion >  CQuaternionVector;

/// <summary>
/// 四元数クラス
/// 
/// 四元数はW,X ,Y,Zの４つの数値を用いて回転を表現します。
/// 入力はX値,Y値,Z値か、軸ベクトルと回転角度です。
/// 注意）余弦数とベクトルの掛け算の結果のベクトルの値に「2.2204460492503131e-016」という値が入っていることがあるがこの値は「０に限りなく近い数字」であり、
///       SKY_PRINTFなどで出力すると0と出る。float.hで「DBL_EPSILON」というマクロで定義されている。「-4.3711390063094768e-008」も同様に0である。
/// </summary>
class CQuaternion
{

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
	CQuaternion();
	CQuaternion( dectype x , dectype y , dectype z , dectype w );
	CQuaternion( const dectype q[ 4 ] );

    /// <summary>
    /// デストラクタ
    /// </summary>
	virtual ~CQuaternion();

    /// <summary>
    /// プロパティ
    /// </summary>
	union 
	{
		struct 
		{
			dectype	W, X , Y, Z;
		};
		dectype q[ 4 ];
	};

    /// <summary>
    /// ベクトルを設定する
    /// </summary>
	CQuaternion &SetVector( const CBasicVector3 &vec );

    /// <summary>
    /// ベクトルを指定して作成する
    /// </summary>
	CQuaternion	&Setup( const CBasicVector3 &vec );

    /// <summary>
    /// 恒等四元数
    /// </summary>
	CQuaternion &Identity() { W = 1.0f; X = Y = Z = 0.0f; return *this; }

    /// <summary>
    /// X値から四元数を特定の角度にセットアップする
    /// </summary>
	CQuaternion &SetToRotateAboutX( dectype radian );

    /// <summary>
    /// Y値から四元数を特定の角度にセットアップする
    /// </summary>
	CQuaternion &SetToRotateAboutY( dectype radian );

    /// <summary>
    /// Z値から四元数を特定の角度にセットアップする
    /// </summary>
	CQuaternion &SetToRotateAboutZ( dectype radian );

    /// <summary>
    /// 軸ベクトルと回転角度から四元数を特定の角度にセットアップする
    /// 軸ベクトルaxisは正規化されていること！
    /// </summary>
	CQuaternion &SetToRotateAboutAxis( const CBasicVector3 &axis , dectype radian );

    /// <summary>
    /// ２つのベクトルからクオータニオンを生成する
    /// </summary>
	CQuaternion &SetupVector( const CBasicVector3 &vec1 , const CBasicVector3 &vec2 );

    /// <summary>
    /// オブジェクト空間→慣性空間への回転を実行する四元数をセットアップする
    /// 方向はオイラー角形式で与えられる
    /// </summary>
	CQuaternion &SetToRotateObjectToInertial( const CEulerAngles &orientation );

    /// <summary>
    /// 慣性空間→オブジェクト空間への回転を実行する四元数をセットアップする
    /// 方向はオイラー角形式で与えられる
    /// </summary>
	CQuaternion &SetToRotateInertialToObject( const CEulerAngles &orientation );

    /// <summary>
    /// *演算子のオーバーライド
    /// </summary>
	CQuaternion operator *( const CQuaternion &a ) const;
	CQuaternion &operator *( const dectype &v );

    /// <summary>
    /// *=演算子のオーバーライド
    /// </summary>
	CQuaternion &operator *=( const CQuaternion &a );

    /// <summary>
    /// +=演算子のオーバーライド
    /// </summary>
	CQuaternion &operator +=( const CQuaternion &a );

	/// <summary>
	/// 四元数を正規化する
	/// 注意すべき点は、通常、四元数は常に（数値の制度の範囲内で）正規化されていることである
	/// 詳細は9.4.6節参照
	///
	/// この関数は主に、浮動小数点の誤差の進入を防ぐために使われる
	/// 誤差の侵入は、たくさんの連続した四元数演算が適用された場合に発生する場合がある。
	/// </summary>
	CQuaternion &Normalize();

    /// <summary>
    /// 回転角θを返す
    /// </summary>
	dectype	GetRotationAngle() const;

    /// <summary>
    /// 回転軸を返す
    /// </summary>
	CBasicVector3	GetRotationAxis() const;
};

} } }

/// <summary>
/// 恒等四元数
/// </summary>
extern const sky::lib::math::CQuaternion skyQuaternionIdentity;

/// <summary>
/// 四元数の内積
/// 非メンバ関数を用いるので、ファンキーな構文を用いずにy減数表現を
/// 演算数として渡すことができる
/// </summary>
extern dectype DotProduct(const sky::lib::math::CQuaternion &a , const sky::lib::math::CQuaternion &b);

/// <summary>
/// 球面線形補間
/// q1→q2の間の線形補完をを0.0d→1.0dの間の数値で表現する。
/// tが0.1d 0.2dと1.0dに近づくにつれq2に近づくということだろう。
/// </summary>
extern sky::lib::math::CQuaternion Slerp(const sky::lib::math::CQuaternion &p, const sky::lib::math::CQuaternion &q, dectype t);

/// <summary>
/// 四元数の共役を計算する
/// これは、元の四元数の反対の回転を持つ四元数となる
/// </summary>
extern sky::lib::math::CQuaternion Conjugate(const sky::lib::math::CQuaternion &q);

/// <summary>
/// 四元数の累乗
/// </summary>
extern sky::lib::math::CQuaternion Pow(const sky::lib::math::CQuaternion &q, dectype exponent);

/// <summary>
/// ベクトルと四元数の掛け算
/// </summary>
sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &vec , const sky::lib::math::CQuaternion &q );
sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &vec , const sky::lib::math::CQuaternion &q );
sky::lib::math::CQuaternion   	operator*( const sky::lib::math::CQuaternion &q , dectype val );

