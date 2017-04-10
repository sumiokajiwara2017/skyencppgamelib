#include "StdAfx.h"
#include "SkyFrameWorkLib/AI.h"

namespace sky { namespace lib { namespace framework {

math::CQuaternion CAIMoveObject::GetDirectionQuaternion()
{
	math::CQuaternion result;
	result.Setup( m_Direction );
	return result;
}

void CAIMoveObject::Update( const fps::CFpsController &fps , const math::CBasicVector3 &force )
{
	// 加速度 = 力／質量
	math::CBasicVector3 acceleration = force / m_Mass;

	// 速度の更新（加速度は時間でスケールする）
	m_Velocity += acceleration * fps.GetPassageTime_Second();

	//最大速度を超えないようにする
	m_Velocity.Truncate( m_MaxSpeed );

	//位置の更新（速度は時間でスケールする）
	m_Position  += m_Velocity * fps.GetPassageTime_Second();

	//速度から現在の向きを計算する
	if ( m_Velocity.Length() > 0.00000001 )
	{
		m_Direction = m_Velocity;
		m_Direction.Normalize();
	}
}

} } }