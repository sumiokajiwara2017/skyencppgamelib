#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �V�[���O���t�X�}�[�g�|�C���^�^
/// </summary>
class CEffector;
typedef SmartPointer< CEffector > CEffectorPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �W���C���g�̋@�\�ɃX�P�[���l�A����f�A���˕ϊ������삷��v�f�������������̃N���X�B
/// �W���C���g�ƕ����čl���邽�߂ɂ����đ���܂����B
/// </summary>
class CEffector : public math::CJoint
{
    SkyTypeDefRTTI;

//-----------�􉽊w�ϊ�

	/// <summary>
	/// �w��̕��ʂɓ��˂���
	/// </summary>
	inline void SetProjection( const math::CBasicVector3 &n );
	inline void SetProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos );
	inline void MulProjection( const math::CBasicVector3 &n );
	inline void MulProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos );

	/// <summary>
	/// �w��̕��ʂɑ΂��ă��t���N�V��������
	/// </summary>
	inline void SetReflect( math::eAxis axis , dectype k = 0.0f );
	inline void SetReflect( const math::CBasicVector3 &n );
	inline void MulReflect( math::eAxis axis , dectype k = 0.0f );
	inline void MulReflect( const math::CBasicVector3 &n );

	/// <summary>
	/// ����f����
	/// </summary>
	inline void SetShear( math::eAxis axis , dectype s , dectype t ); 
	inline void MulShear( math::eAxis axis , dectype s , dectype t ); 

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CEffector( const CEffectorPtr &spParent = CEffectorPtr() );

};

} } }

#include "Effector.inl"