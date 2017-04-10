#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;
class CQuaternion;

typedef CList< u32 , CRotationMatrix > CRotationMatrixList;
typedef CVector< CRotationMatrix >     CRotationMatrixVector;

/// <summary>
/// 回転行列
/// 
/// ------------------------------------------------------------------------------------------------------
/// このクラスのユーザーは、この行列がどのように構成されているかを気にしないでよい
/// しかしながら、内部的にすべてのことをきっちりさせておくことは重要である。
/// 
/// この行列は、回転行列だけであることを前提とし、したがって、直行している
/// 座標変換の正方向（それが実際にこの場合に当てはまるのであれば）は、
/// 慣性空間からオブジェクト空間へである
/// オブジェクト空間→慣性空間の回転を実行する場合は、その転置を乗算する。
/// 
/// 言い換えると、
/// 
/// 【慣性空間からオブジェクト空間】
/// 
///  慣性空間座標 ＊ 行列 ＝ オブジェクト空間座標
/// 
///               | m11 m12 m13 |
///  { ix iy iz } | m21 m22 m23 | = { ox oy oz }
///               | m31 m32 m33 |
/// 【オブジェクト空間から慣性空間】
/// 
///  オブジェクト空間座標 ＊ Ｔ行列 ＝ 慣性空間座標
/// 
///               | m11 m21 m31 |
///  { ox oy oz } | m12 m22 m32 | = { ix iy iz }
///               | m13 m23 m33 |
/// 
/// </summary>
class CRotationMatrix
{
public:

    /// <summary>
    /// プロパティ
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12, M13;
			dectype	M21, M22, M23;
			dectype	M31, M32, M33;
		};
		dectype m[ 3 ][ 3 ];
	};

    /// <summary>
    /// コンストラクタ
    /// </summary>
	inline CRotationMatrix();

    /// <summary>
    /// 行列を恒等行列を設定する
    /// </summary>
	inline CRotationMatrix	&Identity();

    /// <summary>
    /// 基本軸の周りの回転を実行する行列をセットアップする
    /// 
    /// 回転の軸は、以下のように１をベースにしたインデックスを用いて指定される
    /// 
    /// １→Ｘ軸に関する回転
    /// ２→Ｙ軸に関する回転
    /// ３→Ｚ軸に関する回転
    /// 
    /// thetaは回転の量（ラジアン）、左手のルールで正の回転を定義する
    /// 
    /// </summary>
	inline CRotationMatrix	&Setup( eAxis axis , dectype theta );

    /// <summary>
    /// 指定の軸にそって回転する行列をセットアップする
    /// </summary>
	inline CRotationMatrix	&Setup( const CBasicVector3 &axis , dectype theta );

    /// <summary>
    /// 指定された方向（オイラー角）で行列をセットアップする
    /// </summary>
	CRotationMatrix	&Setup( const CEulerAngles &orientation );

    /// <summary>
    /// 指定のベクトルから回転マトリックスを生成します。基準は( 0 , 0 , 1 )方向です。( 0 , 0 , 1 )を設定すると回転しない行列が生成されます。
    /// </summary>
	CRotationMatrix	&Setup( const CBasicVector3 &vec );

	/// <summary>
    /// 行列をセットアップする
    /// 慣性空間→オブジェクト空間の回転を実行する四元数を前提とする
    /// </summary>
	CRotationMatrix	&FromInertialToObjectQuaternion(const CQuaternion& q);

    /// <summary>
    /// 行列をセットアップする
    /// オブジェクト空間→慣性空間の回転を実行する四元数を前提とする
    /// </summary>
	CRotationMatrix	&FromObjectToInertialQuaternion(const CQuaternion& q);

    /// <summary>
    /// ３軸の向きを取得する(恒等行列の場合Ｘ軸は( 1.0f , 0.0f , 0.0f ) Ｙ軸は( 0.0f , 1.0f , 0.0f ) Ｚ軸は( 0.0f , 0.0f , 1.0f )
	/// isNormalizeExec:正規化するかしないか
    /// </summary>
	inline CRotationMatrix	&GetDirection( eAxis axis , CBasicVector3 &dest );

	/// <summary>
	/// ==演算子オーバーライド（要素の比較）
	/// </summary>
	inline skyBool operator ==( const CRotationMatrix &m ) const;

	/// <summary>
	/// !=演算子オーバーライド（要素の比較）
	/// </summary>
	inline skyBool operator !=( const CRotationMatrix &m ) const;

    /// <summary>
    /// ベクトルを慣性空間からオブジェクト空間へ回転する
    /// </summary>
	inline CBasicVector3	InertialToObject( const CBasicVector3& v ) const;

    /// <summary>
    /// ベクトルをオブジェクト空間から慣性空間へ回転する
    /// </summary>
	inline CBasicVector3	ObjectToInertial( const CBasicVector3& v ) const;

    /// <summary>
    /// ３軸の角度を受け取り、行列を作成する
    /// </summary>
	inline CRotationMatrix	&SetUpXYZ( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpXZY( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpYXZ( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpYZX( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpZXY( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpZYX( const CBasicVector3 &vec );
};

} } }

/// <summary>
/// 演算子のオーバーライド
/// </summary>
inline sky::lib::math::CRotationMatrix operator+(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CRotationMatrix &b);
inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CRotationMatrix &b);
inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CQuaternion &q);
inline sky::lib::math::CRotationMatrix &operator*=(const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CRotationMatrix &m2 );
inline sky::lib::math::CRotationMatrix &operator*=(const sky::lib::math::CRotationMatrix &m , const sky::lib::math::CQuaternion &q);
inline sky::lib::math::CRotationMatrix &operator+=(const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CRotationMatrix &m2 );
inline sky::lib::math::CRotationMatrix Invert3x3(const sky::lib::math::CRotationMatrix &m);
inline sky::lib::math::CRotationMatrix PseudoInverse( const sky::lib::math::CRotationMatrix &m ); //擬似逆行列を作成
inline sky::lib::math::CRotationMatrix Transpose( sky::lib::math::CRotationMatrix m );

#include "RotationMatrix.inl"