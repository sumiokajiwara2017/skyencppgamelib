#pragma once

namespace sky { namespace lib { namespace math {

class CQuaternion;
class CMatrix4x3;
class CRotationMatrix;

/// <summary>
/// オイラー角クラス
/// 
/// オイラー角はヘディング、ピッチ、バンクという３つの角度数値を用いて
/// 回転を表現します。（z-y-z軸系）
/// 
/// </summary>
class CEulerAngles
{
public:

    /// <summary>
    /// メンバ変数
    /// </summary>
	dectype	Heading;
	dectype	Pitch;
	dectype	Bank;

    /// <summary>
    /// コンストラクタ
    /// </summary>
	CEulerAngles() {}
	CEulerAngles( dectype h , dectype p , dectype b ) :
		Heading( h ) , Pitch( p ) , Bank( b ) {}

    /// <summary>
    /// 恒等化（全部０）する
    /// </summary>
	void	Identity() { Pitch = Bank = Heading = 0.0f; }


    /// <summary>
    /// 正準オイラー角の３組を決定する
    /// ヘディングとバンクを+-180度にピッチを+-90度に制限する
    /// つまり上記範囲にメンバ変数を変換する
    /// </summary>
	void	Canonize();

    /// <summary>
    /// 四元数を入力に受け取りオイラー角に変換する(オブジェクト空間→慣性空間）
    /// 慣性空間とはオブジェクト空間とワールド空間の中間、位置がオブジェクト空間で、回転がワールド空間の状態のこと。
    /// いや～この本がなんでこんな空間定義をしているのかうまく理解できていないような。要するに平行移動と回転を分けて解説したかったのでしょうか。
    /// </summary>
	void	FromObjectToInertialQuaternion( const CQuaternion &q );

    /// <summary>
    /// 四元数を入力に受け取りオイラー角に変換する(慣性空間→オブジェクト空間）
    /// </summary>
	void	FromInertialToObjectQuaternion( const CQuaternion &q );

    /// <summary>
    /// 4x3行列を入力にオイラー角に変換する（オブジェクト空間→ワールド空間）
    /// </summary>
	void	FromObjectToWorldMatrix( const CMatrix4x3 &m );

    /// <summary>
    /// 4x3行列を入力にオイラー角に変換する（ワールド空間→オブジェクト空間）
    /// </summary>
	void	FromWorldToObjectMatrix( const CMatrix4x3 &m );

    /// <summary>
    /// 3x3行列を入力にオイラー角に変換する
    /// </summary>
	void	FromRotationMatrix( const CRotationMatrix &m );
};

} } }

/// <summary>
/// 恒等行列
/// </summary>
extern const sky::lib::math::CEulerAngles skyCEulerAnglesIdentity;