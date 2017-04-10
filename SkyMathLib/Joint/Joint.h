#pragma once

#define INVALID_JOINT_INDEX ( -1 )

namespace sky { namespace lib { namespace math {

class CJoint;
typedef SmartPointer< CJoint >   CJointPtr;
typedef CList< u32 , CJointPtr > CJointPtrList;
typedef CVector< CJoint >        CJointVector;
typedef CHash< CJointPtr >       CJointPtrHash;
typedef CStack< CJointPtr >      CJointPtrStack;

/// <summary>
/// �W���C���g�N���X
/// </summary>
class CJoint : public serialize::CSerializeInterface
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CJoint();

	/// <summary>
	/// ���O
	/// </summary>
    CName Name;

	/// <summary>
	/// �C���f�b�N�X
	/// </summary>
    s32 Index;

	/// <summary>
	/// ���[�J���s���P�ʍs��ɂ���
	/// </summary>
	inline void Reset();

//-----------���s�ړ�

	//��Set �c �l���㏑������i���S�ɒl��u��������j���u��������Ώۂ̍s�񂪒P�ʍs��ł��邱�Ƃ�O��Ƃ��܂��B�s�񂪏�����ԂłȂ��ꍇ��Reset()���g�p���Ă���ݒ肵�Ă��������B
	//  Add �c �l�����Z����i���X�̒l�𐶂����j
	//  Mul �c �l����Z����i���X�̒l�𐶂����j

	/// <summary>
	/// X��
	/// </summary>
	inline void SetPosition_X( dectype value );
	inline void AddPosition_X( dectype value );

	/// <summary>
	/// Y��
	/// </summary>
	inline void SetPosition_Y( dectype value );
	inline void AddPosition_Y( dectype value );

	/// <summary>
	/// Z��
	/// </summary>
	inline void SetPosition_Z( dectype value );
	inline void AddPosition_Z( dectype value );

	/// <summary>
	/// XY��
	/// </summary>
	inline void SetPosition_XY( const CBasicVector2 &vec );
	inline void AddPosition_XY( const CBasicVector2 &vec );

	/// <summary>
	/// XZ��
	/// </summary>
	inline void SetPosition_XZ( const CBasicVector2 &vec );
	inline void AddPosition_XZ( const CBasicVector2 &vec );

	/// <summary>
	/// YZ��
	/// </summary>
	inline void SetPosition_YZ( const CBasicVector2 &vec );
	inline void AddPosition_YZ( const CBasicVector2 &vec );

	/// <summary>
	/// XYZ��
	/// </summary>
	inline void SetPosition_XYZ( const CBasicVector3 &vec );
	inline void AddPosition_XYZ( const CBasicVector3 &vec );

//-----------�g��k��

	/// <summary>
	/// X��
	/// </summary>
	inline void SetScale_X( dectype value );
	inline void MulScale_X( dectype value );

	/// <summary>
	/// Y��
	/// </summary>
	inline void SetScale_Y( dectype value );
	inline void MulScale_Y( dectype value );

	/// <summary>
	/// Z��
	/// </summary>
	inline void SetScale_Z( dectype value );
	inline void MulScale_Z( dectype value );

	/// <summary>
	/// XZ��
	/// </summary>
	inline void SetScale_XZ( const math::CBasicVector2 &vec );
	inline void AddScale_XZ( const math::CBasicVector2 &vec );

	/// <summary>
	/// YZ��
	/// </summary>
	inline void SetScale_YZ( const math::CBasicVector2 &vec );
	inline void AddScale_YZ( const math::CBasicVector2 &vec );

	/// <summary>
	/// XY��
	/// </summary>
	inline void SetScale_XY( const math::CBasicVector2 &vec );
	inline void MulScale_XY( const math::CBasicVector2 &vec );

	/// <summary>
	/// XYZ��
	/// </summary>
	inline void SetScale_XYZ( const math::CBasicVector3 &vec );
	inline void MulScale_XYZ( const math::CBasicVector3 &vec );
	inline void SetScale_XYZ( dectype sc );
	inline void MulScale_XYZ( dectype sc );

//-----------��]

	/// <summary>
	/// X��
	/// </summary>
	inline void SetRotation_X( dectype radian );
	inline void MulRotation_X( dectype radian );

	/// <summary>
	/// Y��
	/// </summary>
	inline void SetRotation_Y( dectype radian );
	inline void MulRotation_Y( dectype radian );

	/// <summary>
	/// Z��
	/// </summary>
	inline void SetRotation_Z( dectype radian );
	inline void MulRotation_Z( dectype radian );

	/// <summary>
	/// XY��
	/// </summary>
	inline void MulRotation_XY( const CBasicVector2 &vec );
	inline void MulRotation_YX( const CBasicVector2 &vec );

	/// <summary>
	/// XZ��
	/// </summary>
	inline void MulRotation_XZ( const CBasicVector2 &vec );
	inline void MulRotation_ZX( const CBasicVector2 &vec );

	/// <summary>
	/// YZ��
	/// </summary>
	inline void MulRotation_YZ( const CBasicVector2 &vec );
	inline void MulRotation_ZY( const CBasicVector2 &vec );

	/// <summary>
	/// XYZ��
	/// </summary>
	inline void MulRotation_XYZ( const CBasicVector3 &vec );
	inline void MulRotation_XZY( const CBasicVector3 &vec );
	inline void MulRotation_YXZ( const CBasicVector3 &vec );
	inline void MulRotation_YZX( const CBasicVector3 &vec );
	inline void MulRotation_ZXY( const CBasicVector3 &vec );
	inline void MulRotation_ZYX( const CBasicVector3 &vec );

	/// <summary>
	/// ���w��ŉ�]
	/// </summary>
	inline void SetRotation( const CBasicVector3 &axis , dectype radian );
	inline void MulRotation( const CBasicVector3 &axis , dectype radian );

	/// <summary>
	/// ���݂̉�]�����擾
	/// </summary>
	inline const CQuaternion &GetRotation() const;
	inline CQuaternion       &GetRotation();
	inline CRotationMatrix    GetRotationMatrix() const;
	inline CMatrix4x3         GetRotationMatrix4x3() const;

//----------�N�I�[�^�j�I��

	inline void MulQuaternion( const CQuaternion &q );

//-----------�l�擾(�s�񂩂�͉��L�̂悤�ȏ��������j

	/// <summary>
	/// �ʒu���
	/// </summary>
	inline dectype GetPosition_X() const;
	inline dectype GetPosition_Y() const;
	inline dectype GetPosition_Z() const;
	inline CBasicVector2 GetPosition_XY() const;
	inline CBasicVector2 GetPosition_XZ() const;
	inline CBasicVector2 GetPosition_YZ() const;
	inline CBasicVector3 GetPosition_XYZ() const;

	/// <summary>
	/// �X�P�[�����
	/// </summary>
	inline dectype GetScale_X() const;
	inline dectype GetScale_Y() const;
	inline dectype GetScale_Z() const;
	inline CBasicVector2 GetScale_XY() const;
	inline CBasicVector2 GetScale_XZ() const;
	inline CBasicVector2 GetScale_YZ() const;
	inline CBasicVector3 GetScale_XYZ() const;

	/// <summary>
	/// ���̃W���C���g�̋�ԃ}�g���b�N�X���������i��ɕ`��Ɏg�p�j
	/// ��ԏ������ׂ��y���̂̓V�X�e�����͑S��GetLocalMatrix���g���āA
	/// �A�v���͊e��g�����X�t�H�[�����s������K��ComputeLocalMatrix()���ĂԂ悤�ɂ���B
	/// ����ŕω���������������ComputeLocalMatrix()���Ă΂��悤�ɂȂ�]�v�ȕ��ׂ��ւ�͂��B
	/// </summary>
	inline const CMatrix4x3 &ComputeLocalMatrix(); //�����l���v�Z���ĕԂ�
	inline const CMatrix4x3 &GetLocalMatrix() const;     //�o�b�t�@�̎Q�Ƃ�Ԃ�
	inline void              SetLocalMatrix( const CMatrix4x3 &matrix );

	/// <summary>
	/// ���݂̃W���C���g�̃��[�J�����W��X���i��������ɃN�I�[�^�j�I���ŉ�]�����肱�̑O���ɐi�ނ��߂Ƀx�N�g���Ƃ��Ďg�p�����肷��ƕ֗��j
	/// ����f�����[�J�����W�n�̎����̂��c�ފ􉽕ϊ����s���Ă����ꍇ�A�c�񂾎����擾����Ă��܂�
	/// </summary>
	inline CBasicVector3 GetAxisX() const;

	/// <summary>
	/// ���݂̃W���C���g�̃��[�J�����W��Y���i��������ɃN�I�[�^�j�I���ŉ�]�����肱�̑O���ɐi�ނ��߂Ƀx�N�g���Ƃ��Ďg�p�����肷��ƕ֗��j
	/// ����f�����[�J�����W�n�̎����̂��c�ފ􉽕ϊ����s���Ă����ꍇ�A�c�񂾎����擾����Ă��܂�
	/// </summary>
	inline CBasicVector3 GetAxisY() const;

	/// <summary>
	/// ���݂̃W���C���g�̃��[�J�����W��Z���i��������ɃN�I�[�^�j�I���ŉ�]�����肱�̑O���ɐi�ނ��߂Ƀx�N�g���Ƃ��Ďg�p�����肷��ƕ֗��j
	/// ����f�����[�J�����W�n�̎����̂��c�ފ􉽕ϊ����s���Ă����ꍇ�A�c�񂾎����擾����Ă��܂�
	/// </summary>
	inline CBasicVector3 GetAxisZ() const;

	/// <summary>
	/// ���[�J���s��̌v�Z���n�m�^�n�e�e����i�Ȃ�炩�̗��R�ŊO������SetLocalMatrix�Őݒ肵���l���g�p�������ꍇ���j
	/// </summary>
	inline void SetComputeLocalMatrixOn( skyBool isComputeOn = skyTrue );

	/// <summary>
	/// ���[�J���s��̌v�Z�ɃI�t�Z�b�g�s����g�p���邩�ǂ����B
	/// </summary>
	inline void SetComputeOffsetMatrixToLocalMatrixOn( skyBool isComputeOn = skyTrue );

	/// <summary>
	/// ���W�}�g���b�N�X�̎擾
	/// </summary>
	inline CMatrix4x3 GetPositionMatrix() const;

	/// <summary>
	/// �{�[���ϊ��s��i��]�~�I�t�Z�b�g�s��~���W�s��j��Ԃ��B
	/// IK�Ɏg�p�B���[�V�����̏ꍇ�L�[�t���[���f�[�^���炱����Z�o����B
	/// </summary>
	inline const CMatrix4x3 &ComputeBoneTransForm(); //�����l����v�Z
	inline const CMatrix4x3 &GetJointTransForm() const;
	inline void SetJointTransForm( const CMatrix4x3 &matrix ); //MotionController���L�[��񂩂�Z�o�����l�Ȃ�

	/// <summary>
	/// �����p���s��擾�^�ݒ�
	/// </summary>
	inline const CMatrix4x3 &GetInitialTransform() const;
	inline void SetInitialTransform( const CMatrix4x3 &matrix );

	/// <summary>
	/// �I�t�Z�b�g�s��擾�^�ݒ�
	/// </summary>
	inline const CMatrix4x3 &GetOffsetMatrix() const;
	inline void              SetOffsetMatrix( const CMatrix4x3 &matrix );
	inline void              SetOffset( const CBasicVector3 &offset );
	inline CBasicVector3     GetOffset() const;

	/// <summary>
	/// �I�t�Z�b�g�x�N�g�����擾�^�ݒ�
	/// �����ł̓I�t�Z�b�g�s��Ƃ��Ă���
	/// </summary>
	inline CBasicVector3 GetOffsetVector() const;
	inline void SetOffsetVector( const CBasicVector3 &vector );

//---Serialize
		
	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CJointPtr Create( const CBasicVector3 offset = CBasicVector3( 0 , 0 , 0 ) , const CJointPtr &spParent = CJointPtr() )
	{
		CJointPtr spJoint( NEW__( CJoint , CJoint() ) );

		spJoint->SetOffset( offset );
		spJoint->AttachParent( spParent );

		return spJoint;
	}

protected:

	/// <summary>
	/// ���[�J���}�g���b�N�X
	/// </summary>
	CMatrix4x3  m_LocalMatrix; //������̓|�C���^�ɂ��Ďg�p�����ʂ̂݃C���X�^���X������

	/// <summary>
	/// ���[�J���}�g���b�N�X���v�Z�ŏo���t���O
	/// </summary>
	skyBool m_IsLocalMatrixComputeOn;

	/// <summary>
	/// ���[�J���}�g���b�N�X�̌v�Z�ɃI�t�Z�b�g�s����g�p����t���O
	/// </summary>
	skyBool m_IsComputeOffsetMatrixToLocalMatrixOn;

	/// <summary>
	/// �g��k��
	/// </summary>
	math::CBasicVector3 m_Scale;

	/// <summary>
	/// ��]���N�I�[�^�j�I��
	/// </summary>
	CQuaternion m_Rotation;

	/// <summary>
	/// �ʒu
	/// </summary>
	CBasicVector3 m_Postion;

//------�ȉ��̓��[�V�������ɂ����g�p���Ȃ�-----��

	/// <summary>
	/// �{�[���ϊ��s��
	/// </summary>
	CMatrix4x3  m_JointTransformMatrix; //������̓|�C���^�ɂ��Ďg�p�����ʂ̂݃C���X�^���X������

	/// <summary>
	/// �����{�[���g�����X�t�H�[���s��
	/// </summary>
	CMatrix4x3* m_pInitialTransform; //������̓|�C���^�ɂ��Ďg�p�����ʂ̂݃C���X�^���X������

	/// <summary>
	/// �I�t�Z�b�g�s��i�eJoint����ǂꂾ���ω����Ă��邩�Ƃ������j
	/// </summary>
	CMatrix4x3  m_OffsetMatrix; //������̓|�C���^�ɂ��Ďg�p�����ʂ̂݃C���X�^���X������

//------�ȉ��̓A�j���[�V�������ɂ����g�p���Ȃ�-----��

//-------------�e�q�֌W�֘A-------------------//

public:

	/// <summary>
	/// �ŏ�ʂ��ǂ������肷��
	/// </summary>
	skyBool IsRoot();

	/// <summary>
	/// �q�����݂��邩�m�F����
	/// </summary>
	skyBool IsChildOn();

	/// <summary>
	/// �e�̊֘A�t��
	/// </summary>
	void AttachParent( const CJointPtr &spParent );

	/// <summary>
	/// �e�̊֘A�t������
	/// </summary>
	void DetachParent();

	/// <summary>
	/// �e�̎擾
	/// </summary>
	const CJointPtr &Parent();

	/// <summary>
	/// �q�̃��X�g��Ԃ�
	/// </summary>
	CJointPtrList &ChildList();

	/// <summary>
	/// �q�̊֘A�t��
	/// </summary>
	CJointPtr AttachChild     ( u32 key, const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// �q�̊֘A�t��
	/// </summary>
	CJointPtr AttachChild     ( const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// �q�̊֘A�t��
	/// </summary>
	CJointPtr AttachChildFront( const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// �q�̊֘A�t������
	/// </summary>
	void               DetachChild     ( const CJointPtr &spChild );

	/// <summary>
	/// �q�̊֘A�t������
	/// </summary>
	void               DetachAllChild  ();

	/// <summary>
	/// �q�𖼑O�Ō���
	/// </summary>
	CJointPtr SearchChild( const hash::CHashStringPtr &name );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CJoint( const CJointPtr &spParent = CJointPtr() );

protected:

	/// <summary>
	/// �e
	/// </summary>
	CJointPtr   m_Parent;

	/// <summary>
	/// �q���X�g
	/// </summary>
	CJointPtrList	m_ChildList;

//-------------�e�q�֌W�֘A-------------------//


};

} } }

#define CJointCreate_()	                            sky::lib::math::CJoint::Create()
#define CJointCreateWithOffset_( offset , parent )  sky::lib::math::CJoint::Create( offset , parent )

#include "Joint.inl"