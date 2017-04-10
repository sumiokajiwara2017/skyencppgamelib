#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���f���N���X
/// ���\�[�X�t�@�C������`�悷��R�c�I�u�W�F�N�g�͂��̃N���X���Ǘ����܂��B
/// </summary>
class CModel : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual           ~CModel(){};

    /// <summary>
	/// �����_�����O����
    /// </summary>
	virtual void       vRendering          ( CRenderingParam &param );

    /// <summary>
	///  �����_�����O���������֐�
    /// </summary>
	void               RenderingImple      ( CRenderingParam &param , const CShaderObjectPtr &shader );

	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	CMaterialPtr       GetMaterial( u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �􉽊w�`�����DifuseMap���w��̐F�ɐݒ肷��
	/// </summary>
	void               AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �􉽊w�`�����Difuse�J���[���w��̐F�ɐݒ肷��
	/// </summary>
	void               SetDiffuse( const CColor &color , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// �A���t�@�l�̐ݒ�
	/// </summary>
	void               SetAlphaValue( f32 alpha , u32 meshNo = 0 , u32 materialNo = 0 );

//---Collision

	/// <summary>
	/// �������{�b�N�X�̎Z�o
	/// </summary>
	void               ComputeAABB3();

	/// <summary>
	/// �L�����E�{�b�N�X�̎Z�o
	/// </summary>
	void               ComputeOBB3();

	/// <summary>
	/// ���̎Z�o
	/// </summary>
	void               ComputeBS();

	/// <summary>
	///  �������{�b�N�X�̎擾�i���[�J�����W�j
	/// </summary>
	inline const math::CAABB3 &GetAABB3() const;

	/// <summary>
	///  �L�����E�{�b�N�X�̎擾�i���[�J�����W�j
	/// </summary>
	inline const math::COBB3 &GetOBB3() const;

	/// <summary>
	///  ���̎擾�i���[�J�����W�j
	/// </summary>
	inline const math::CBasicSphere &GetOBS() const;

	/// <summary>
	///  ���݂̃W���C���g�Ōv�Z�����������{�b�N�X�̎擾�i���[���h���W�j�i���s�ړ��̂ݔ��f�j
	///  ���Փ˔�����s���Ȃ炱����g�p����B
	/// </summary>
	inline const math::CAABB3 &GetAABB3ComputeJoint();

	/// <summary>
	///  ���݂̃W���C���g�Ōv�Z�����L�����E�{�b�N�X�̎擾�i���[���h���W�j�i���s�ړ��Ɖ�]���v�Z�j
	///  ���Փ˔�����s���Ȃ炱����g�p����B
	/// </summary>
	inline const math::COBB3  &GetOBB3ComputeJoint();

	/// <summary>
	///  ���݂̃W���C���g�Ōv�Z�������̎擾�i���[���h���W�j�i���s�ړ��̂ݔ��f�j
	///  ���Փ˔�����s���Ȃ炱����g�p����B
	/// </summary>
	inline const math::CBasicSphere &GetOBSComputeJoint();

	/// <summary>
	///  ���f���̃��[�J�����W�n�ɂ�����e��T�C�Y�A�ʒu���B�ϊ���̒l
	/// </summary>
	void                         ComputeSizeData();
	inline math::CBasicVector3   GetLocalSize();
	inline math::CBasicVector3   GetLocalHalfSize();
	inline f32                   GetLocalYUnderLength();


//----Branch

    /// <summary>
	///  Mesh�̊֘A�t���^�擾
    /// </summary>
	void				  AttachMeshContena( const CMeshContenaPtr &spMesh );
	CMeshContenaPtr		  GetMeshContena( skyBool isRequired = skyTrue );

//----Branch

//----Leaf

    /// <summary>
	///  �V�F�[�_�[���֘A�t���^�֘A�t������������
    /// </summary>
	void            AttachShader( const CShaderObjectPtr &effectPtr );
	void            ClearAndAttachShader( const CShaderObjectPtr &effectPtr ); //�V�F�[�_�[����U�N���A���A�^�b�`����
	void            DetachShader();

//----Leaf

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
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate();

#ifdef SW_SKYLIB_DEBUG_ON

//---Debug

	/// <summary>
	/// �@����`�悷��
	/// </summary>
	void SetDebugDisplayNormalOn( skyBool isOn , float length = 1.0f );

	/// <summary>
	/// �f�o�b�O�Ɏg�p���郏�[�N
	/// </summary>
	math::CJointPtr m_spDebugJoint;

#endif

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CModelPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CModelPtr spModel( NEW__( CModel , CModel() ) );
		if ( name.IsEmpty() == skyFalse ) spModel->Name.SetName( name );
		return spModel;
	}

protected:

	/// <summary>
	/// ���f���̑傫�����
	/// </summary>
	math::CBasicVector3  m_LocalSize;         //���[�J�����W�n�ɂ�����e���̒���
	math::CBasicVector3  m_LocalHalfSize;     //���[�J�����W�n�ɂ�����e���̔����̒���
	f32                  m_LocalYUnderLength; //���[�J�����W�n�ɂ�����x���̍ŏ����_�ʒu�ƃ��f���̒��S�ʒu�Ƃ̍��B���f����n�ʂ̏�ɂ̂���I�t�Z�b�g�����Ɏg���B
	                                          //���f�������[�J�����W�n�ɂ����āA���f���̒��S�_�ɍ��W�n�̒��S���������ꍇ�A���̒l��
	                                          //m_LocalHalfSize.Y�ƈ�v����B���[�J�����W�n�ɂ����Ă�Y�������ɂ��v���X�̈ʒu�A��
	                                          //���n�ʂɂ��悤�Ȍ`�ɂȂ��Ă����ꍇ�A���̒l�͌���Ȃ��O�ɋ߂��l�ɂȂ�B

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CModel();
	CModel( CSceneGraphNode &spNode );
	void Constractor();

	/// <summary>
	/// �������{�b�N�X�R�c
	/// </summary>
	math::CAABB3			     m_AABB3;
	math::CAABB3			     m_AABB3ComputeJoint; //���̎��_�ł̃��f���̊g��k���A��]�A���s�ړ����l�������l

	/// <summary>
	/// �L�����E�{�b�N�X�R�c
	/// </summary>
	math::COBB3                  m_OBB3;
	math::COBB3                  m_OBB3ComputeJoint; //���̎��_�ł̃��f���̊g��k���A��]�A���s�ړ����l�������l

	/// <summary>
	/// ���i�o�E���f�B���O�X�t�B�A�j�R�c
	/// </summary>
	math::CBasicSphere           m_BS;
	math::CBasicSphere           m_BSComputeJoint;  //���̎��_�ł̃��f���̊g��k���A��]�A���s�ړ����l�������l

	/// <summary>
	/// �f�o�b�O�@���\���n�m
	/// </summary>
	skyBool                      m_IsDebugNormalDispOn;
};

#define CModelCreate_()				sky::lib::graphic::CModel::Create()
#define CModelCreateName_( name )	sky::lib::graphic::CModel::Create( name )

} } }

#include "Model.inl"