#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

//�v���b�g�t�H�[�����̃����o�ϐ�(pimpl)
struct CWindowMember;

//�e���v���[�gTypedef�S
class CWindow;
typedef SmartPointer< CWindow >				CWindowPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList		< u32 , CWindowPtr >		CWindowPtrList;	//���X�g��`
typedef CHash		< CWindowPtr >				CWindowPtrHash;	//Hash��`
typedef CHashMT		< CWindowPtr >				CWindowPtrHashMT;	//Hash��`

//-----------------------------------< ���� >-------------------------------------------//

class CWindow : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWindow();

	/// <summary>
	/// ���ƍ����ƈʒu���擾����
	/// </summary>
	math::CBasicRect&           GetRect			()										{ return m_WindowRect; }
	
	/// <summary>
	/// �T�C�Y�ύX�m�F
	/// </summary>
	skyBool                     IsChangeSize    ( dectype width, dectype height )       { return ( m_WindowRect.W() != width || m_WindowRect.H() != height ); }

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	void                        ReSize          ( dectype width, dectype height )       { m_WindowRect.W() = width; m_WindowRect.H() = height; }

	/// <summary>
	/// �g�p����������
	/// </summary>
	void				        SetUp	        ( const math::CBasicRect &rect );
	void				        SetUp	        ( s32 hndl );

	/// <summary>
	/// �ړ����f���Q�[�g
	/// </summary>
	void Move( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );

	/// <summary>
	/// �E�B���h�E����ID�̎擾
	/// </summary>
	wndType GetWindowId();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline CWindowMember* GetMember(){ return m_pMember; }

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
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr  Clone();
	serialize::CSerializeInterfacePtr  CloneSharing();

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
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CWindowPtr			Create( const hash::CHashStringPtr &name )
	{
		
		CWindowPtr spWindow( NEW__( CWindow , CWindow() ) );
		if ( name.IsEmpty() == skyFalse ) spWindow->Name.SetName( name );
		return spWindow;
	}

protected:

	/// <summary>
	/// �Z�b�g�A�b�v���
	/// </summary>
	enum eSetUpKind
	{
		eSetUpKind_Create = 0 ,
		eSetUpKind_Hndl ,
	} 
	m_SetUpKind;

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CWindowMember*      m_pMember;

//---Member

	/// <summary>
	/// Window�̏c���A���W
	/// </summary>
	math::CBasicRect	     m_WindowRect;

//---Member

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CWindow();
};

} } }

#define CWindowCreate_()	        sky::lib::graphic::CWindow::Create()
#define CWindowCreateName_( name )	sky::lib::graphic::CWindow::Create( name )

//SkyBaseLib�Ŏ�������Ă���
LRESULT WINAPI SkyWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace sky { namespace lib { namespace graphic {

struct CWindowMember
{
	/// <summary>
	/// Window�n���h��
	/// </summary>
	HWND						m_nWindowHndl;

	/// <summary>
	/// Window�N���X
	/// </summary>
	WNDCLASSEX					m_kWc;

	/// <summary>
	/// �ړ����m�f���Q�[�g
	/// </summary>
	delegate::IDelegateObjectPtr m_spMoveDelegate;

	/// <summary>
	/// �T�C�Y�ύX���m�f���Q�[�g
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;

	/// <summary>
	/// Window�n���h�����擾����
	/// </summary>
	inline const HWND GetWindowHndl() const { return m_nWindowHndl; };
};

} } }
