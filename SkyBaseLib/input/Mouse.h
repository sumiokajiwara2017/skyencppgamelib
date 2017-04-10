#pragma once

namespace sky { namespace lib { namespace input {

//-------------------------------------< ���� >---------------------------------------------//

struct CMouseMember;

class CMouse;
typedef SmartPointer< CMouse >	CMousePtr;

/// <summary>
/// ���͎�t�Ǘ�
/// <summary>
class CMouse  : public base::SkyRefObject
{

public:

	/// <summary>
	/// �{�^�����s�[�g�Ԋu�̐ݒ�
	/// <summary>
	static void SetBtnRepeat	( s32 repIntvl , s32 onIntvl );

	/// <summary>
	/// ���͎�t���X�i�[��o�^
	/// </summary>
	void AttachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// ���͎�t���X�i�[��o�^����
	/// </summary>
	void DetachInputListener	( const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMouse            ();

//---------< static >----------

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize		( u32 playerNum );

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose			();

	/// <summary>
	/// �X�V
	/// </summary>
	static void Update			( const fps::CFpsController &fps );

	/// <summary>
	/// �}�E�X�擾
	/// </summary>
	static CMousePtr& Get       ( u32 playerNo = CURRENT_PLAYNER_NO );

	/// <summary>
	/// �v���O�����L�[���́i�f�o�C�X����̓��͂������Ă��A�L�[���͂����������Ƃɂ���j
	/// <summary>
	void	KeyOn				( eSkyMouseKeyKind key );

	/// <summary>
	/// �L�[����������
	/// <summary>
	skyBool IsKeyOn				( eSkyMouseKeyKind key );

	/// <summary>
	/// �L�[��������
	/// <summary>
	skyBool IsKeyRelease		( eSkyMouseKeyKind key );

	/// <summary>
	/// �L�[�������㔻��
	/// <summary>
	skyBool IsKeyPress			( eSkyMouseKeyKind key );

	/// <summary>
	/// �L�[���s�[�g����
	/// <summary>
	skyBool IsKeyRepeat			( eSkyMouseKeyKind key );

//---�C�x���g�������m�͊e�v���b�g�t�H�[���l�X���낤�����BWindows�̓E�C���h�E�v���V�[�W���ł���Ă܂�-----

	/// <summary>
	/// �}�E�X���{�^���_�E���C�x���g����
	/// </summary>
	static void       EventExec_LBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �}�E�X���{�^���A�b�v�C�x���g����
	/// </summary>
	static void       EventExec_LBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �}�E�X�E�{�^���_�E���C�x���g����
	/// </summary>
	static void       EventExec_RBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �}�E�X�E�{�^���A�b�v�C�x���g����
	/// </summary>
	static void       EventExec_RBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �}�E�X�����{�^���_�E���C�x���g����
	/// </summary>
	static void       EventExec_CBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �}�E�X�����{�^���A�b�v�C�x���g����
	/// </summary>
	static void       EventExec_CBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId );

	/// <summary>
	/// �z�C�[����]�C�x���g����
	/// </summary>
	static void       EventExec_HWRotation( u32 playerNo , s32 x , s32 y , s32 rotVal , eSkyMouseKeyWheelWithBtnKind eWWKind , wndType windowId );

	/// <summary>
	/// �}�E�X�ړ��C�x���g����
	/// </summary>
	static void       EventExec_Move( u32 playerNo , s32 x , s32 y , wndType windowId );

protected:

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CMouseMember *m_pMember;

	/// <summary>
	/// �R���X�g���N�^
	/// <summary>
	CMouse();
	CMouse( const CMouse& ){};							//�R�s�[�R���X�g���N�^
	CMouse &operator=(const CMouse& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

} } }

#define CMouseInitialize_( playerNum ) sky::lib::input::CMouse::Initialize( playerNum )
#define CMouse_                        sky::lib::input::CMouse::Get()
#define CMouseDispose_()               sky::lib::input::CMouse::Dispose()
