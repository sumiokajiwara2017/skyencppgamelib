#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct CRenderStateMember;

/// <summary>
/// �萔
/// </summary>
namespace renderstate
{
	//�����ɐݒ�ł���X�e�[�g�̍ő吔
	static const u32 STATE_STACK_MAX_NUM = 32;
}

//�e���v���[�gTypedef�S
class CRenderState;
typedef SmartPointer< CRenderState >	CRenderStatePtr;      //�X�}�[�g�|�C���^�Œ�`
typedef CStack< CRenderStatePtr >		CRenderStatePtrStack;

/// <summary>
/// �����_�[�X�e�[�g�N���X�i�����ŃX�^�b�N�Ǘ��j
/// ���̃N���X���g�p����ΐݒ肵���X�e�[�g��`���ɂ��ׂĖ߂����Ƃ��ł��邪�A���p�͂��Ȃ����ƁB�n���Ƀp�t�H�[�}���X�𗎂Ƃ��炵���B
/// </summary>
class CRenderState : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderState();

	/// <summary>
	/// ������
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	virtual void   vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	virtual void   vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// �J�����O�̗L���ݒ�
	/// </summary>
	void    EnableCulling ();

	/// <summary>
	/// �V�U�[�̗L���ݒ�
	/// </summary>
	void    EnableScissorTest ();

	/// <summary>
	/// �[�x�o�b�t�@�L��
	/// </summary>
	void    EnableDepthBuffer ();

	/// <summary>
	/// �[�x�o�b�t�@�L��
	/// </summary>
	void    DisableDepthBuffer ();

	/// <summary>
	/// �X�e���V���e�X�g�L��
	/// </summary>
	void    EnableStencilTest ();

	/// <summary>
	/// �X�e���V���}�X�N�L��
	/// </summary>
	void    EnableStencilMask ();

	/// <summary>
	/// �u�����h�X�e�[�g�ݒ�
	/// </summary>
	void    EnableTextureBlending ();

	/// <summary>
	/// �����_���[�ɃX�e�[�g��ݒ肷��
	/// </summary>
	void    SetRenderer();

	/// <summary>
	/// �����_���[�ɃX�e�[�g��ݒ肷��
	/// </summary>
	void    ReverseRenderer();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	CRenderStateMember* GetMember(){ return m_pMember; }

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
	void Activate();

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// �C���X�^���X�쐬
	/// </summary>
	static CRenderStatePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CRenderStatePtr spRenderState( NEW__( CRenderState , CRenderState() ) );
		if ( name.IsEmpty() == skyFalse ) spRenderState->Name.SetName( name );
		return spRenderState;
	}

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderState();
	void Constractor()
	{
		m_IsEnableCulling = skyFalse;
		m_IsEnableScissorTest = skyFalse;
		m_IsEnableDepthBuffer = skyFalse;
		m_IsEnableStencilTest = skyFalse;
		m_IsEnableStencilMask = skyFalse;
		m_IsEnableTextureBlending = skyFalse;
	}

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CRenderStateMember*   m_pMember;

//---Member

	/// <summary>
	/// �J�����O�̗L���ݒ�t���O
	/// </summary>
	skyBool  m_IsEnableCulling;

	/// <summary>
	/// �V�U�[�̗L���ݒ�t���O
	/// </summary>
	skyBool  m_IsEnableScissorTest;

	/// <summary>
	/// �[�x�o�b�t�@�L���t���O
	/// </summary>
	skyBool  m_IsEnableDepthBuffer;

	/// <summary>
	/// �[�x�o�b�t�@�L���t���O
	/// </summary>
	skyBool  m_IsDisableDepthBuffer;

	/// <summary>
	/// �X�e���V���e�X�g�L���t���O
	/// </summary>
	skyBool  m_IsEnableStencilTest;

	/// <summary>
	/// �X�e���V���}�X�N�L���t���O
	/// </summary>
	skyBool  m_IsEnableStencilMask;

	/// <summary>
	/// �u�����h�X�e�[�g�ݒ�t���O
	/// </summary>
	skyBool  m_IsEnableTextureBlending;

//---Member

};

} } }

#define CRenderStateCreate_()			sky::lib::graphic::CRenderState::Create()
#define CRenderStateCreateName_( name )	sky::lib::graphic::CRenderState::Create( name )