#pragma once

namespace sky { namespace lib { namespace graphic {

const u32 SCREEN_LAYER_MAX_NUM = 16;

/// <summary>
/// �X�N���[���Z�b�g�A�b�v�f�[�^
/// </summary>
struct CScreenObjectSetUpData
{
	hash::CHashStringPtr      Name;                  //�X�N���[����
	math::CBasicRect          WindowRect;            //Window��`
	CameraPtr                 Camera;                //�J����
	CColor                    ClearColor;            //��ʃN���A�J���[
	CWindowPtr                Window;                //�E�B���h�E�i�����ō쐬����Window���g�p�������ꍇ�͐ݒ肷��j
	CRenderTargetPtr	      RenderTarget;          //�����_�[�^�[�Q�b�g
	CDepthStencilBufferPtr    DepthStencilBuffer;    //�[�x�X�e���V���o�b�t�@
    CRenderStatePtr           RenderState;           //�����_�[�X�e�[�g
    skyBool                   IsSetParentWindowSize; //�e��Window�̃T�C�Y��Vieport��Sissor�T�C�Y�Ɏg�p����

	CScreenObjectSetUpData() : IsSetParentWindowSize( skyFalse ){}
};

class CScreenObject;
typedef SmartPointer< CScreenObject >                 CScreenObjectPtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< s32 , CScreenObjectPtr >               CScreenObjectPtrList;		//�X�}�[�g�|�C���^���X�g�^
typedef CHash< CScreenObjectPtr >	                  CScreenObjectPtrHash;		//�X�}�[�g�|�C���^���X�g�^
typedef CStack< CScreenObjectPtr >	                  CScreenObjectPtrStack;    //�X�}�[�g�|�C���^���X�g�^

/// <summary>
/// �X�N���[���I�u�W�F�N�g�N���X
/// �X�N���[���I�u�W�F�N�g�́A�ȒP�ɉ�ʂ��o�����߂̃t���[�����[�N�ł��B
/// </summary>
class CScreenObject : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CScreenObject();

	/// <summary>
	/// �X�N���[���̖����ݒ�i�����ɂ��ꂽ�X�N���[���͕`�悳��܂���B�j
	/// </summary>
	void SetDisableOn( skyBool isDisable );
	skyBool IsDisableOn();

	/// <summary>
	/// �X�N���[���̖����ݒ�i�����ɂ��ꂽ�X�N���[���͕`�悳��܂���B�j
	/// </summary>
	void SetReSizeOn( skyBool isReSize );
	skyBool IsReSizeOn();

	/// <summary>
	/// �V�[���̂��擾����
	/// </summary>
	CScenePtr GetScene( skyBool isRequired = skyTrue );

	/// <summary>
	/// �q�X�N���[����ǉ�����i�I�t���C�������_�����O�ȂǂɎg�p�B�ǉ����ꂽ�X�N���[������ɕ`�悳���j
	/// </summary>
	CScreenObjectPtr AddChild( const CScreenObjectSetUpData &param );

	/// <summary>
	/// �X�N���[������悹����i�R�c�`��̏�ɂQ�c�`����̂���ȂǂɎg�p�B��悹���ꂽ�X�N���[������ɕ`�悳���j
	/// </summary>
	CScreenObjectPtr PushLayer( const CScreenObjectSetUpData &param );

	/// <summary>
	/// �`��
	/// </summary>
	void Rendering( const fps::CFpsController &fps );

//-------------Root�̂ݎ����Ă���(Branch)----------------��

	/// <summary>
	/// �Z�b�g�A�b�v���ɐ������ꂽ�E�B���h�E
	/// </summary>
	CWindowPtr             GetWindow( skyBool isRequired = skyTrue );

	/// <summary>
	/// �Z�b�g�A�b�v���ɐ������ꂽ�����_�����O�^�[�Q�b�g
	/// </summary>
	CRenderTargetPtr       GetRenderTarget( skyBool isRequired = skyTrue );

	/// <summary>
	/// �Z�b�g�A�b�v���ɐ������ꂽ�[�x�X�e���V���o�b�t�@
	/// </summary>
	CDepthStencilBufferPtr GetDepthStencilBuffer( skyBool isRequired = skyTrue );

	/// <summary>
	/// �Z�b�g�A�b�v���ɐ������ꂽ�S�̂ɂ����郌���_�[�X�e�[�g
	/// </summary>
	CRenderStatePtr        GetRenderState( skyBool isRequired = skyTrue );

//-------------Root�̂ݎ����Ă���----------------��

//---Serialize
			
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
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

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

//---Static

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose();

	/// <summary>
	/// ���O�ŃX�N���[�����擾�i���C���[�X�N���[���ɂ��q�X�N���[���ɂ����O�ł�������O���[�o���A�N�Z�X�ł���j
	/// </summary>
	static CScreenObjectPtr Get( const hash::CHashStringPtr &name );

	/// <summary>
	/// �X�N���[���𐶐�����
	/// </summary>
	static CScreenObjectPtr Create( const CScreenObjectSetUpData &param );

	/// <summary>
	/// Loader����Ă�ScreenObject�̐���
	/// </summary>
	static CScreenObjectPtr Create( const hash::CHashStringPtr &name )
	{
		
		CScreenObjectPtr spScreenObject( NEW__( CScreenObject , CScreenObject() ) );
		if ( name.IsEmpty() == skyFalse ) spScreenObject->Name.SetName( name );
		return spScreenObject;
	}

private:

	/// <summary>
	/// �������t���O
	/// </summary>
	skyBool               m_IsDisable;

	/// <summary>
	/// ���T�C�Y���s
	/// </summary>
	skyBool               m_IsReSizeOn;

	/// <summary>
	/// ���C���[�X�N���[���X�^�b�N
	/// </summary>
	CScreenObjectPtrStack m_LayerScreenStack;

	/// <summary>
	/// �q�X�N���[�����X�g
	/// </summary>
	CScreenObjectPtrList  m_ChildScreenList;

	/// <summary>
	/// �T�C�Y�ύX���m�f���Q�[�g
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;

	/// <summary>
	/// �����_�����O�X�e�b�v�Ǘ�
	/// </summary>
	CRenderStepManagerPtr m_RenderStepManager;

	/// <summary>
	/// �R���X�g���N�^�i����j
	/// </summary>
	CScreenObject();
	CScreenObject( const CScreenObjectPtr &spParent );
	void Constractor();

	/// <summary>
	/// ���T�C�Y���f���Q�[�g
	/// �}���`�E�B���h�E�^�C�v�̂n�r�ł̓E�B���h�E���[�h�ŃQ�[�����s�����ꍇ�A�E�B���h�E�̃T�C�Y�ύX���s����\��������B
	/// �E�B���h�E�̃T�C�Y�ύX�ɂ̓����_�����O�^�[�Q�b�g�̕ύX�A�r���[�|�[�g�̕ύX�A�J�����̃A�X�y�N�g��̕ύX�iDirectX�ł̓X�e���V���o�b�t�@
	/// �̕ύX�A�X���b�v�`�F�C���̃����[�h�A�X���b�v�`�F�C������o�b�N�o�b�t�@�̍Ď擾�AD3D�I�u�W�F�N�g�̔j���A�Đ����Ȃǁj
	/// �Ȃǂ𓯎��ɍs��Ȃ���΂Ȃ炸�ʓ|�ȏ�A�������ă��[�U�[�Ƀ����b�g���Ȃ��̂ŁA�Œ�𑜓x�ŁA����ނ����[�U�[���I�ׂ�
	/// �𑜓x�p�^�[����p�ӂ��Ă�����Ƃ����̂������B�ꉞ����CScreenObject�N���X�ł̓��T�C�Y�Ή����Ă݂��B
	/// </summary>
    void ReSize      ( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );
    void ReSizeChild ( dectype scw , dectype sch );

	/// <summary>
	/// �X�N���[������悹����i�R�c�`��̏�ɂQ�c�`����̂���ȂǂɎg�p�B��悹���ꂽ�X�N���[������ɕ`�悳���j
	/// </summary>
	CScreenObjectPtr CreateSubScreen( const CScreenObjectSetUpData &param );
};

} } }

#define CScreenObjectInitialize_()	sky::lib::graphic::CScreenObject::Initialize()
#define CScreenObject_				sky::lib::graphic::CScreenObject
#define CScreenObjectDispose_()		sky::lib::graphic::CScreenObject::Dispose()

#define CScreenObjectCreateName_( name )  sky::lib::graphic::CScreenObject::Create( name );
