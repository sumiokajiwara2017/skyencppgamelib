#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >----------------------------------------------//

class CScene;
typedef SmartPointer< CScene >			    CScenePtr;			//�X�}�[�g�|�C���^�^��`
typedef CList		< dectype , CScenePtr > CScenePtrList;		//�X�}�[�g�|�C���^���X�g�^
typedef CHash		< CScenePtr >           CScenePtrHash;		//Hash��`

/// <summary>
/// �V�[���N���X
/// ���������̃N���X�̏�񂪂P�`��G���A��\��������
/// ������ʕ����őΐ�̃Q�[���̏ꍇ�A��ʕ����̃N���X������͂��ɂȂ�B
/// ���C�g�������o�ɉ����Ȃ��������R�F���C�g�͊e�V�[���O���t�̃m�[�h�ɑ΂��ē��Ă���̂�����B
/// �V�[���O���t�̃��[�g�������o�ɉ����Ȃ��������R�F�V�[���O���t�͈Ⴄ�V�[���œ������̂��g�p����\�������������߁B
/// </summary>
class CScene : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CScene(){}

	/// <summary>
	/// �g�p����
	/// </summary>
	void               SetUp();

//--Leaf

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�i�V�[�����e�N�X�`���ɏ����o���������̏ꍇ�̓e�N�X�`�������_�����O�^�[�Q�b�g��ݒ肷��j
	/// </summary>
	CRenderTargetPtr       GetRenderTarget( skyBool isRequired = skyTrue );
	void                   SetRenderTarget( const CRenderTargetPtr &spRenderTarget );

	/// <summary>
	/// ���[�g�̐[�x�X�e���V���o�b�t�@
	/// </summary>
	CDepthStencilBufferPtr GetDepthStencilBuffer( skyBool isRequired = skyTrue );
	void                   SetDepthStencilBuffer( const CDepthStencilBufferPtr &spDepthStencilBuffer );

	/// <summary>
	/// �r���[�|�[�g
	/// </summary>
	CViewPortPtr           GetViewPort( skyBool isRequired = skyTrue );
	void                   SetViewPort( const CViewPortPtr &spViewPort );

	/// <summary>
	/// �V�U�����O��`
	/// </summary>
	CScissorRectPtr        GetScissorRect( skyBool isRequired = skyTrue );
	void                   SetScissorRect( const CScissorRectPtr &spScissorRect );

	/// <summary>
	/// �J�����Ǘ�
	/// </summary>
	CameraManagerPtr       GetCameraManager( skyBool isRequired = skyTrue );
	void                   SetCameraManager( const CameraManagerPtr &spCamera );

	/// <summary>
	/// ���C�g
	/// </summary>
	CLightPtr              GetLight( skyBool isRequired = skyTrue );
	void                   SetLight( const CLightPtr &spLight );

	/// <summary>
	/// �����_�[�X�e�[�g
	/// </summary>
	CRenderStatePtr        GetRenderState( skyBool isRequired = skyTrue );
	void                   SetRenderState( const CRenderStatePtr &spRenderState );

	/// <summary>
	/// �|�X�g�G�t�F�N�g�Ǘ������i�g�p����Ƃ��͕K���Ăԁj
	/// </summary>
	void SetUpPostEffect();
	const CPostEffectManagerPtr &GetPostEffectManager(); //����̂��ߎ擾

	/// <summary>
	/// �V�[���O���t�̃��[�g
	/// </summary>
	CSceneGraphNodePtr     GetSceneGraphRoot( skyBool isRequired = skyTrue );

//---Member

	/// <summary>
	/// �V�[���������_�����O���Ȃ�
	/// </summary>
	skyBool			       m_IsOff;

	/// <summary>
	/// �V�[���������_�����O����ۂɐ[�x�o�b�t�@���N���A���邩�ǂ���
	/// </summary>
	skyBool			       m_IsDepthBufferClear;

	/// <summary>
	/// �V�[���������_�����O����ۂɃX�e���V���o�b�t�@���N���A���邩�ǂ���
	/// </summary>
	skyBool			       m_IsStencilBufferClear;

//---Member
		
	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	void Deactivate();

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CScenePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CScenePtr spScene( NEW__( CScene , CScene() ) );
		if ( name.IsEmpty() == skyFalse ) spScene->Name.SetName( name );
		return spScene;
	}

private:

	/// <summary>
	/// �|�X�g�G�t�F�N�g�Ǘ��i�C�Ӑݒ�j
	/// </summary>
	CPostEffectManagerPtr m_spPostEffectManager;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CScene() : m_IsDepthBufferClear( skyTrue ) , m_IsStencilBufferClear( skyFalse ) , m_IsOff( skyFalse ){}

};

#define CSceneCreate_()	            sky::lib::graphic::CScene::Create()
#define CSceneCreateName_( name )	sky::lib::graphic::CScene::Create( name )

} } }