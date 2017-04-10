#pragma once

namespace sky { namespace lib { namespace math {

#define BONE_PALLET_DEFAULT_SIZE ( 64 )

class CJointPallet;
typedef SmartPointer< CJointPallet >	CJointPalletPtr;					//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �W���C���g�z��N���X
/// �W���C���g��z��ŊǗ�����̂͒��_�E�F�C�g�̂��߁w�����x�B���[�V�����̎��ۂ̓����Ƃ͊֌W���Ȃ��B
/// ���̃N���X�͐��w���C�u�����ɗL��Ȃ���`�惉�C�u�����ł����g�p���Ȃ��Ƃ�������ȃN���X�B
/// </summary>
class CJointPallet : public base::SkyRefObject
{
	friend class CMotionController;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CJointPallet(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CJointPallet();

	/// <summary>
	/// �W���C���g���擾
	/// </summary>
	inline u32 GetJointNum();

	/// <summary>
	/// �N���A����
	/// </summary>
	inline void Clear();

	/// <summary>
	/// �X�L���ϊ��s��z��̐擪�|�C���^��Ԃ��i���̂܂܃V�F�[�_�[�ɓn���j
	/// </summary>
	inline math::CMatrix4x3Array &GetSkinTransformMatrixArray();

	/// <summary>
	/// �W���C���g��ǉ��^�ݒ�^�擾����
	/// </summary>
	inline void  AddJoint( const CJointPtr &spJoint );
	inline void  SetJoint( u32 no , const CJointPtr &spJoint );
	inline const CJointPtr &GetJoint( u32 no );
	inline CJointPtr GetJoint( const hash::CHashStringPtr &name );

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CJointPalletPtr Create()
	{
		CJointPalletPtr spJoint( NEW__( CJointPallet , CJointPallet() ) );
		return spJoint;
	}

private:

	/// <summary>
	/// �W���C���g���
	/// </summary>
	struct CJointData_
	{
		/// <summary>
		/// �W���C���g���
		/// </summary>
		CJointPtr         m_spJoint;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		CJointData_(){}

		/// <summary>
		/// ������Z�q�I�[�o�[���C�h
		/// </summary>
		virtual CJointData_& operator =( const CJointData_& src )
		{
			m_spJoint         = src.m_spJoint;
			return *this;
		}
	};
	typedef CVector< CJointData_ > CJointDataPallet;
	typedef CHash< CJointData_ >   CJointDataHash;

	/// <summary>
	/// �W���C���g���OHash
	/// </summary>
	CJointDataHash m_JointDataHash;

	/// <summary>
	/// �W���C���g�z��
	/// </summary>
	CJointDataPallet m_JointPallet;

	/// <summary>
	/// �X�L���ϊ��s��z��(�v�Z�o�b�t�@�B���[�V�����ł�IK�ł��������`�悳��邽�߂ɂ������X�V���Ă��������j
	/// </summary>
	math::CMatrix4x3Array m_SkinTransforms;
};

} } }


#define CJointPalletCreate_()	  sky::lib::math::CJointPallet::Create()

#include "JointPallet.inl"