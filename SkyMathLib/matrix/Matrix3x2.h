#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ２Ｄ処理用３Ｘ２行列クラス
/// </summary>
class CMatrix3x2
{
public:

    /// <summary>
    /// プロパティ
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12;
			dectype	M21, M22;
			dectype	M31, M32;
			dectype	Tx ,  Ty;
		};
		dectype m[ 3 ][ 2 ];
	};

	/// <summary>
    /// コンストラクタ
    /// </summary>
	CMatrix3x2();

	/// <summary>
    /// デストラクタ
    /// </summary>
	virtual ~CMatrix3x2();

    /// <summary>
    /// 恒等行列を設定する
    /// </summary>
	void	Identity();

    /// <summary>
    /// 行列の平行移動部分を直接アクセスする
    /// 最後の行を[0,0]に設定し線形変換部分（２×２の部分）をそのまま残すことで、
    /// この行列の平行移動部分を取り除きます。
    /// </summary>
	void	ZeroTranslation();

    /// <summary>
    /// 行列の平行移動部分を直接アクセスする
	///
	/// ３行目部分を下記のようにする
	/// | posX ,posY |
    /// 行列の平行移動部分を指定された値に設定し、２×２の部分は変更しません。
	///
    /// </summary>
	void	SetTranslation( const CBasicVector2 &d );

    /// <summary>
    /// 回転行列を設定する
	/// 
	/// ２×２部分を下記のようにする
	/// | cosθ,-sinθ|
	/// | sinθ,cosθ |
	/// 平行移動部分は変更しません。
	///
    /// </summary>
	void	SetupRotate( dectype theta );

    /// <summary>
    /// 拡縮行列を作成する
	/// 
	/// ２×２部分を下記のようにする
	/// | sc, 0  |
	/// | 0,  sy |
	/// 平行移動部分は変更しません。
	///
    /// </summary>
	void	SetupScale( const CBasicVector2 &sc );

	/// <summary>
	/// ==演算子オーバーライド（要素の比較）
	/// </summary>
	skyBool operator ==( const CMatrix3x2 &m ) const;

	/// <summary>
	/// !=演算子オーバーライド（要素の比較）
	/// </summary>
	skyBool operator !=( const CMatrix3x2 &m ) const;

};

} } }

/// <summary>
/// 演算子のオーバーライド
/// これは、紙の上の線形代数記法に似せてベクトルクラスを使えるようにする
/// </summary>
sky::lib::math::CBasicVector2	 operator*( const sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CMatrix3x2		 operator*( const sky::lib::math::CMatrix3x2 &m1 , const sky::lib::math::CMatrix3x2 &m2 );
sky::lib::math::CBasicVector2	&operator*=( sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CMatrix3x2		&operator*=( sky::lib::math::CMatrix3x2 &m1 , const sky::lib::math::CMatrix3x2 &m2 );
sky::lib::math::CBasicVector2	GetTranslationVector( const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CBasicVector2	operator*( const sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
