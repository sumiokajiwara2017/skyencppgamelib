#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �t�^���w�i�C���o�[�X�L�l�}�e�B�N�X�j����������@�\�����Q
/// </summary>
namespace InverseKinematicsUtil
{
    /// <summary>
    /// Cyclic-Coordinate Descent IK �\���o
    /// </summary>
    /// <param name="skeleton">�X�P���g��</param>
    /// <param name="effector">�G�t�F�N�^�֐�</param>
    /// <param name="targetPos">�ڕW�ʒu</param>
    /// <param name="numMaxIteration">�ő唽����</param>
    /// <param name="errToleranceSq">�덷���e�l�̓��</param>
    void SolveCcd( const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration , dectype errToleranceSq);

	/// <summary>
	/// �p�[�e�B�N�� IK �\���o
	/// </summary>
	/// <param name="skeleton">�X�P���g��</param>
	/// <param name="effector">�G�t�F�N�^�֐�</param>
	/// <param name="targetPos">�ڕW�ʒu</param>
	/// <param name="numMaxIteration">�ő唽����</param>
	/// <returns>�p�[�e�B�N���ʒu</returns>
	CBasicVector3List &SolveParticle( CBasicVector3List &dest , const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration );

    /// <summary>
    /// ���[�E�s�b�`�E���[�����R�r�A�� IK �\���o
    /// </summary>
    /// <param name="skeleton">�X�P���g��</param>
    /// <param name="effector">�G�t�F�N�^�֐�</param>
    /// <param name="dest">�ڕW�ʒu</param>
    /// <param name="numMaxIteration">�ő唽����</param>
    /// <param name="step">�v�Z���ݕ�</param>
    void SolveEulerJacobian( const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration, dectype step );

};

} } }