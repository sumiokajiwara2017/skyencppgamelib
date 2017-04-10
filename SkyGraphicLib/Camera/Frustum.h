#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �����N���X
/// </summary>
class CFrustum : public base::SkyRefObject
{

public:

	/// <summary>
	/// �����̊e��ID
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
	/// �����̊e��
	/// </summary>
	math::CBasicPlane m_Planes[ NumPlanes ];

	/// <summary>
	/// �����̎�O�̖�
	/// </summary>
	math::CBasicVector3 m_NearClip[ 4 ];

	/// <summary>
	/// �����̉��̖�
	/// </summary>
	math::CBasicVector3 m_FarClip[ 4 ];

	/// <summary>
	/// ����p
	/// </summary>
	dectype m_Fov;

	/// <summary>
	/// �A�X�y�N�g��
	/// </summary>
	dectype m_Aspect;

	/// <summary>
	/// �߂��̖ʂ̋���
	/// </summary>
	dectype m_Near;

	/// <summary>
	/// ���̖ʂ̋���
	/// </summary>
	dectype m_Far;

	/// <summary>
	/// �_�������������ׂ�
	/// </summary>
	skyBool Inside( const math::CBasicVector3 &point );

	/// <summary>
	/// ���������������ׂ�
	/// </summary>
	skyBool Inside( const math::CBasicVector3 &point, const dectype radius );

	/// <summary>
	/// �����̖ʂ��擾����
	/// </summary>
	const math::CBasicPlane &Get( Side side ) { return m_Planes[ side ]; }

	/// <summary>
	/// ����p��ݒ肷��
	/// </summary>
	void SetFOV( dectype fov ) { m_Fov=fov; }

	/// <summary>
	/// �A�X�y�N�g���ݒ肷��
	/// </summary>
	void SetAspect(dectype aspect) { m_Aspect=aspect; }

	/// <summary>
	/// �߂��ʂ̋����ݒ�
	/// </summary>
	void SetNear(dectype nearClip) { m_Near=nearClip; }

	/// <summary>
	/// �����ʂ̋����ݒ�
	/// </summary>
	void SetFar(dectype farClip) { m_Far = farClip; }

	/// <summary>
	/// ������
	/// </summary>
	void Update( const dectype fov , const dectype aspect , const dectype near , const dectype far , const math::CBasicVector3 &right = math::CBasicVector3( 1.0f, 0.0, 0.0f ) , const math::CBasicVector3 &up = math::CBasicVector3( 0.0f, 1.0f, 0.0f ) , const math::CBasicVector3 &foward = math::CBasicVector3( 0.0f, 0.0f, 1.0f ) );

	/// <summary>
	/// �����\���n�m�^�n�e�e
	/// </summary>
	void SetDisp( skyBool isOn = skyTrue );
};

} } }