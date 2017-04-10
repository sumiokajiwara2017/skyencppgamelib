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
	// �����x = �́^����
	math::CBasicVector3 acceleration = force / m_Mass;

	// ���x�̍X�V�i�����x�͎��ԂŃX�P�[������j
	m_Velocity += acceleration * fps.GetPassageTime_Second();

	//�ő呬�x�𒴂��Ȃ��悤�ɂ���
	m_Velocity.Truncate( m_MaxSpeed );

	//�ʒu�̍X�V�i���x�͎��ԂŃX�P�[������j
	m_Position  += m_Velocity * fps.GetPassageTime_Second();

	//���x���猻�݂̌������v�Z����
	if ( m_Velocity.Length() > 0.00000001 )
	{
		m_Direction = m_Velocity;
		m_Direction.Normalize();
	}
}

} } }