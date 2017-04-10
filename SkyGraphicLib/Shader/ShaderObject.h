#pragma once

namespace sky { namespace lib { namespace graphic {

class   CShaderObject;
typedef SmartPointer< CShaderObject >		CShaderObjectPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< dectype , CShaderObjectPtr >	CShaderObjectPtrList;		//�X�}�[�g�|�C���^���X�g�^

//-----------------------------------< ���ێ��� >-------------------------------------------//

/// <summary>
/// �V�F�[�_�[�̊�{�N���X
/// ���ׂẴV�F�[�_�[�N���X�͂��̃N���X���p�����邱�ƁB
/// </summary>
class CShaderObject : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ������
	/// </summary>
	static void			Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void			Dispose();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CShaderObject(){};

	/// <summary>
	/// �p�X���̎擾
	/// </summary>
	inline u32 GetPassNum(){ return m_PassNum; };

	/// <summary>
	/// �����_�����O�p�X�ԍ��ݒ�
	/// </summary>
	inline void       SetRenderingPass( u32 passNo ){ m_RenderingPassNo = passNo; }

	/// <summary>
	/// ���_���W�ϊ��s���ݒ肷��i�ǂ̃V�F�[�_�[�ɂ����_���W�ϊ��͕K����������j
	/// </summary>
	inline void       SetWorldMatrix ( const math::CMatrix4x3 &w )  { m_WorldMatrix = w; };

	/// <summary>
	/// �J��������ݒ�
	/// </summary>
	inline void       SetCamera      ( const CameraPtr &spCamera ) { m_spCamera = spCamera;}

	/// <summary>
	/// ���b�V����ݒ�
	/// </summary>
	inline void       SetMesh        ( const CMeshPtr   &spMesh )   { m_spMesh = spMesh; }

	/// <summary>
	/// ���b�V����ݒ�
	/// </summary>
	inline void       SetExtraData   ( const CExtraDataPtr   &spExtra )   { m_spExtraData = spExtra; }

	/// <summary>
	/// ���C�g��ݒ�
	/// </summary>
	inline void       SetLight   ( const CLightPtr   &spLight )   { m_spLight = spLight; }

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void	  vRendering	 ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	virtual void              vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// �`��i�e�V�F�[�_�[�������j
	/// </summary>
	virtual void              vDraw     () = 0;

protected:

	/// <summary>
	/// �p�X��
	/// </summary>
	u32                       m_PassNum;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

	/// <summary>
	/// �p�X��
	/// </summary>
	u32                       m_RenderingPassNo;

	/// <summary>
	/// ���[���h�}�g���b�N�X
	/// </summary>
	math::CMatrix4x3		  m_WorldMatrix;

	/// <summary>
	/// �J�������
	/// </summary>
    CameraPtr                 m_spCamera;

	/// <summary>
	/// ���b�V�����
	/// </summary>
    CMeshPtr                  m_spMesh;

	/// <summary>
	/// �G�L�X�g���f�[�^���
	/// </summary>
    CExtraDataPtr             m_spExtraData;

	/// <summary>
	/// ���C�g���
	/// </summary>
    CLightPtr                 m_spLight;

    /// <summary>
	///  ���ݎg�p���Ă���V�F�[�_�[�������h�c
    /// </summary>
	u32						  m_ShaderId;

    /// <summary>
	/// �V�F�[�_�[���
    /// </summary>
	CShaderInfoPtr            m_spShaderInfo;

//---���s���ɐ��������o�b�t�@�ϐ��i�ۑ��̕K�v�Ȃ��j

    /// <summary>
	/// �R���X�g���N�^
    /// </summary>
	CShaderObject();
};

/// <summary>
/// �V�F�[�_�[���i���̃V�F�[�_�[�����蓖�Ă�ꂽ�ꍇ�����\�����܂���j
/// </summary>
class CNullShader : public CShaderObject
{

public:

	//�`��
	virtual void vDraw(){};

	//�쐬
	static CShaderObjectPtr Create()
	{
		return CShaderObjectPtr( NEW CNullShader() );
	}
};

#define CNullShaderCreate_() sky::lib::graphic::CNullShader::Create()

} } }