#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

class CMotionJointDebugDisplayNode;
typedef SmartPointer< CMotionJointDebugDisplayNode >	CMotionJointDebugDisplayNodePtr;			//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// ���[�V�����̃f�o�b�O�\���N���X�B
/// </summary>
class CMotionJointDebugDisplayNode : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void   vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �`��I��
	/// </summary>
	void SetDrawOn( skyBool isOn = skyTrue );

	/// <summary>
	/// ��{�X�P�[���ݒ�
	/// </summary>
	void SetBaseScale( const math::CBasicVector3 &scale );

	/// <summary>
	/// ��{��]�ݒ�
	/// </summary>
	void SetBaseRot( math::eAxis axis , dectype theta );

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CMotionJointDebugDisplayNode& operator =( const CMotionJointDebugDisplayNode& src )
	{
		SKY_UNUSED_ARG( src );

		//���̃N���X�͑���ł���
		SKY_ASSERT( skyFalse );
        return *this;
	}

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CMotionJointDebugDisplayNodePtr Create( const math::CMotionController &pMotionController )
	{
		return CMotionJointDebugDisplayNodePtr( NEW CMotionJointDebugDisplayNode( pMotionController ) );
	}

private:

	/// <summary>
	/// �`��t���O
	/// </summary>
	skyBool m_IsDrawOn;

	/// <summary>
	/// ��{�X�P�[��
	/// </summary>
	math::CBasicVector3 m_BaseScale;

	/// <summary>
	/// ��{�X�P�[��
	/// </summary>
	math::CRotationMatrix m_BaseRot;

	/// <summary>
	/// ���f���c���[���[�g
	/// </summary>
	CSceneGraphNodePtr m_spModelTreeRoot;

	/// <summary>
	/// ���[�V�����R���g���[���[�̎Q��
	/// </summary>
	const math::CMotionController &m_pMotionController;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMotionJointDebugDisplayNode( const math::CMotionController &m_pMotionController );

	/// <summary>
	/// ���f���c���[�̍쐬
	/// </summary>
	static void CreateTreeModelFromJointTree( const math::CJointPtr &spJoint , const CSceneGraphNodePtr &spModelTree , const math::CBasicVector3 &basicScale , const math::CRotationMatrix &rot );

	/// <summary>
	/// ���f���c���[�ɃW���C���g�̌v�Z���ʂ�ݒ肵�Ă����B
	/// </summary>
	static void SetTreeModelFromJointTree( const math::CMotionController &pMotionController , const CSceneGraphNodePtr &spModelTree );
};

} } }

#define CMotionJointDebugDisplayNodeCreate_( controller )  sky::lib::graphic::CMotionJointDebugDisplayNode::Create( controller );

#endif