#pragma once

namespace sky { namespace lib { namespace math {

class CSkeleton;
typedef SmartPointer< CSkeleton >   CSkeletonPtr;

/// <summary>
/// �X�P���g���N���X
/// Joint�̃c���[�\�������B
/// </summary>
class CSkeleton : public base::SkyRefObject
{

public:

	/// <summary>
	/// �W���C���g�c���[
	/// �W���C���g�̐e�q�֌W��ۑ�����
	/// </summary>
	CJointPtr m_spJointRoot;

	/// <summary>
	/// �W���C���g�p���b�g
	/// �W���C���g��z���ɊǗ�����B�V�F�[�_�[�ɃE�F�C�g�̌v�Z��������Ƃ��Ȃǂ͔z��ŃW���C���g���V�F�[�_�[�ɓn���B
	/// ���̃e�[�u���̑��݈Ӌ`�́A�X�L�j���O�̂��߂����B
	/// </summary>
	CJointPalletPtr m_spJointPallet;

	/// <summary>
	/// �����N�n�؍\���I�u�W�F�N�g����̊֐߃��X�g����
	/// </summary>
	/// <param name="srcNode">���ڃm�[�h</param>
	/// <param name="parentInList">���X�g���m�[�h</param>
	inline void SerializeJoints( const CJointPtr &spNode );

    /// <summary>
    /// �������֐�
    /// </summary>
    /// <param name="root">���֐߃����N�n�̃��[�g</param>
    inline void Initialize( const CJointPtr &root );

    /// <summary>
    /// �֐߂̃��[�g���[�J�����W�̎擾
    /// </summary>
    /// <param name="jid">�֐߃C���f�N�X</param>
    /// <returns>���[�g���[�J�����W</returns>
    inline CBasicVector3 GetLocalPosition( s32 jid ) const;

    /// <summary>
    /// �֐߂̃O���[�o�����W�̎擾
    /// </summary>
    /// <param name="jid">�֐߃C���f�N�X</param>
    /// <returns>�O���[�o�����W</returns>
    inline CBasicVector3 GetGlobalPosition( s32 jid ) const;

    /// <summary>
    /// �֐߂̃O���[�o���g�����X�t�H�[���̎擾
    /// </summary>
    /// <param name="jid">�֐߃C���f�N�X</param>
    /// <returns>�O���[�o���g�����X�t�H�[���s��</returns>
    inline CMatrix4x3 GetGlobalCoordinate( s32 jid ) const;

    /// <summary>
    /// �֐߂̃��[�g���[�J���g�����X�t�H�[���̎擾
    /// </summary>
    /// <param name="jid">�֐߃C���f�N�X</param>
    /// <returns>���[�g���[�J���g�����X�t�H�[���s��</returns>
    inline CMatrix4x3 GetLocalCoordinate( s32 jid ) const;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CSkeleton( const CJointPtr &spJoint = CJointPtr() );

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// ���W�v�����g
	/// </summary>
	void PrintPosition();

#endif

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CSkeletonPtr Create( const CJointPtr &spJoint )
	{
		CSkeletonPtr spSkeleton( NEW__( CSkeleton , CSkeleton( spJoint ) ) );
		return spSkeleton;
	}
};

} } }

#define CSkeletonCreate_( root )	          sky::lib::math::CSkeleton::Create( root )

#include "Skeleton.inl"