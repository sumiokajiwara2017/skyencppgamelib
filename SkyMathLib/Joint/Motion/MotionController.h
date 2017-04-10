#pragma once

/*
�y�X�L�j���O�̌����u��U�A���_�ɖ߂��Ă����]�Ȃǂ̕ϊ��������A�܂����̈ʒu�ɖ߂��z
�X�L�j���O�̌����͈�U�A���_�ɖ߂��Ă����]�Ȃǂ̕ϊ��������A���̈ʒu�ɖ߂��ł����B
��U�A���_�ɖ߂��ɂ́w�I�t�Z�b�g�s��x���g���܂��B�I�t�Z�b�g�s��͍��W���_���猩����Έʒu�̏����p�������_�Ɉړ�����s��ł��B
�w�����p���s��x�͈ړ����܂߂��s��ŁA�I�t�Z�b�g�s���������Ɖ�]��񂾂��ɂȂ�B

	//����ł����͂��B
	�ϊ�����W = ���W * �{�[���I�t�Z�b�g�s�� *�i �����p���s��i�������e�q�֌W�Ōv�Z�ς݂̂��Ɓj * �A�j���[�V�����s�� )
    //�����������p���D�荞�ݍς݂̏ꍇ�͂����Ȃ�
	�ϊ�����W = ���W * �{�[���I�t�Z�b�g�s�� * �A�j���[�V�����s��
*/

namespace sky { namespace lib { namespace graphic {

class CMotionJointDebugDisplayNode;

} } }

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���[�V�����R���g���[���[�i���^���w�i�t�H���[�h�L�l�}�e�B�N�X�j�ō���Ă���f�[�^���Đ��B�e�����N����q�����N�ւƌv�Z���Ă����j
/// ������
/// �E���[�V�������
/// �E���[�V�����u�����h
/// �E�f���A���N�I�[�^�j�I���Ή�
/// </summary>
class CMotionController
{
	//�f�o�b�O�\���@�\�͂��F�B
	friend class graphic::CMotionJointDebugDisplayNode;

public:

	/// <summary>
	/// �W���C���g�p���b�g�̊֘A�t��
	/// Mesh�̎��W���C���g�p���b�g��ݒ肷�邱�ƂŁA���b�V���Ƀ��[�V������������
	/// </summary>
	void AttachSkeleton( const CSkeletonPtr &spSkeleton );

	/// <summary>
	/// ���[�V�����f�[�^�z�񁦂�ݒ�
	/// �����̃��[�V�����i�����A����A����A�R��Ȃǁj�����������[�V�����R���e�i
	/// </summary>
	void SetAnimationContena( const CMotionDataContenaPtr &spAnimationContena );

	/// <summary>
	/// ���[�V�����f�[�^�̍Đ�
	/// </summary>
	void Play( u32 motionNo , updateTime blendTime = 0 );

	/// <summary>
	/// ���ݍĐ����̃��[�V�����f�[�^���擾
	/// </summary>
	CMotionDataPtr GetCurrentAnimationData();

	/// <summary>
	/// ���Ԃ̍X�V�i�߂�l�͍Đ����ꂽ���[�V�����̃t���[���ԍ��j
	/// </summary>
	void Update( updateTime time , skyBool relativeToCurTime = skyTrue );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMotionController();

private:

	/// <summary>
	/// ���[�V�����R���e�i
	/// </summary>
	CMotionDataContenaPtr m_spAnimationContena;

	/// <summary>
	/// �X�P���g��
	/// </summary>
	CSkeletonPtr m_spSkeleton;

	/// <summary>
	/// ���݂̎���
	/// </summary>
	updateTime m_CurrentTime;

	/// <summary>
	/// ���݂̃��[�V�����ԍ�
	/// </summary>
	u32 m_CurrentAnimationNo;

	/// <summary>
	/// ���݂̃��[�V�����f�[�^
	/// </summary>
	CMotionDataPtr m_CurrentAnimationData; 

	/// <summary>
	/// �u�����h����
	/// </summary>
	updateTime m_BlendTime;

//------�v�Z�o�b�t�@----��

	/// <summary>
	/// �W���C���g�X�^�b�N
	/// </summary>
	CJointPtrStack m_JointStack;

	/// <summary>
	/// ���[���h�ϊ��s��z��
	/// </summary>
	math::CMatrix4x3Array m_WorldTransforms;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �f�o�b�O�o�b�t�@
	/// </summary>
	math::CMatrix4x3Array m_DebugJointTransforms;

#endif	

//------�v�Z�o�b�t�@----��

	/// <summary>
	/// ���̍X�V
	/// </summary>
    void UpdateJointTransform( updateTime time , skyBool relativeToCurTime );
	static void CMotionController::UpdateWorldTransformRecursive( const CJointPtr &spJoint , const CJointPtr &spParentJoint );

	/// <summary>
	/// ���[���h���W�ʒu�̍X�V
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// �X�L�����W�̍X�V
	/// </summary>
    void UpdateSkinTransform();

	/// <summary>
	/// �W���C���g�ϊ��s��̏�����
	/// </summary>
	void InitJointTransformsMatrix();
};

} } }