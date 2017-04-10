#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// シーングラフスマートポインタ型
/// </summary>
class CEffector;
typedef SmartPointer< CEffector > CEffectorPtr;			//スマートポインタ型定義

/// <summary>
/// ジョイントの機能にスケール値、せん断、投射変換等操作する要素を加えただけのクラス。
/// ジョイントと分けて考えるためにあえて造りました。
/// </summary>
class CEffector : public math::CJoint
{
    SkyTypeDefRTTI;

//-----------幾何学変換

	/// <summary>
	/// 指定の平面に投射する
	/// </summary>
	inline void SetProjection( const math::CBasicVector3 &n );
	inline void SetProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos );
	inline void MulProjection( const math::CBasicVector3 &n );
	inline void MulProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos );

	/// <summary>
	/// 指定の平面に対してリフレクションする
	/// </summary>
	inline void SetReflect( math::eAxis axis , dectype k = 0.0f );
	inline void SetReflect( const math::CBasicVector3 &n );
	inline void MulReflect( math::eAxis axis , dectype k = 0.0f );
	inline void MulReflect( const math::CBasicVector3 &n );

	/// <summary>
	/// せん断する
	/// </summary>
	inline void SetShear( math::eAxis axis , dectype s , dectype t ); 
	inline void MulShear( math::eAxis axis , dectype s , dectype t ); 

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CEffector( const CEffectorPtr &spParent = CEffectorPtr() );

};

} } }

#include "Effector.inl"