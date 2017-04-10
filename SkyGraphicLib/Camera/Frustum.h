#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// 視野台クラス
/// </summary>
class CFrustum : public base::SkyRefObject
{

public:

	/// <summary>
	/// 視野台の各面ID
	/// </summary>
	enum Side 
	{ 
		Near, 
		Far, 
		Top, 
		Right, 
		Bottom, 
		Left, 
		NumPlanes
	};

	/// <summary>
	/// 視野台の各面
	/// </summary>
	math::CBasicPlane m_Planes[ NumPlanes ];

	/// <summary>
	/// 視野台の手前の面
	/// </summary>
	math::CBasicVector3 m_NearClip[ 4 ];

	/// <summary>
	/// 視野台の奥の面
	/// </summary>
	math::CBasicVector3 m_FarClip[ 4 ];

	/// <summary>
	/// 視野角
	/// </summary>
	dectype m_Fov;

	/// <summary>
	/// アスペクト比
	/// </summary>
	dectype m_Aspect;

	/// <summary>
	/// 近くの面の距離
	/// </summary>
	dectype m_Near;

	/// <summary>
	/// 奥の面の距離
	/// </summary>
	dectype m_Far;

	/// <summary>
	/// 点が視野台内か調べる
	/// </summary>
	skyBool Inside( const math::CBasicVector3 &point );

	/// <summary>
	/// 球が視野台内か調べる
	/// </summary>
	skyBool Inside( const math::CBasicVector3 &point, const dectype radius );

	/// <summary>
	/// 視野台の面を取得する
	/// </summary>
	const math::CBasicPlane &Get( Side side ) { return m_Planes[ side ]; }

	/// <summary>
	/// 視野角を設定する
	/// </summary>
	void SetFOV( dectype fov ) { m_Fov=fov; }

	/// <summary>
	/// アスペクト比を設定する
	/// </summary>
	void SetAspect(dectype aspect) { m_Aspect=aspect; }

	/// <summary>
	/// 近い面の距離設定
	/// </summary>
	void SetNear(dectype nearClip) { m_Near=nearClip; }

	/// <summary>
	/// 遠い面の距離設定
	/// </summary>
	void SetFar(dectype farClip) { m_Far = farClip; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Update( const dectype fov , const dectype aspect , const dectype near , const dectype far , const math::CBasicVector3 &right = math::CBasicVector3( 1.0f, 0.0, 0.0f ) , const math::CBasicVector3 &up = math::CBasicVector3( 0.0f, 1.0f, 0.0f ) , const math::CBasicVector3 &foward = math::CBasicVector3( 0.0f, 0.0f, 1.0f ) );

	/// <summary>
	/// 視野台表示ＯＮ／ＯＦＦ
	/// </summary>
	void SetDisp( skyBool isOn = skyTrue );
};

} } }