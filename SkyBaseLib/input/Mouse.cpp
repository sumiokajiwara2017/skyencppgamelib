#include "StdAfx.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Script.h"

namespace sky { namespace lib { namespace input {

static u8 s_VkeyTbl[ eSkyMouseKeyKind_Max ] =
{
	VK_LBUTTON,
	VK_MBUTTON,
	VK_RBUTTON,
};


/// <summary>
/// ���s�[�g�Ԋu
/// </summary>
static s32 s_RepIntvlNum = 0;

/// <summary>
/// ���s�[�g�Ԋu
/// </summary>
static s32 s_OnIntvlNum= 0;

struct CMouseMember
{
	/// <summary>
	/// �L�[���͏��
	/// </summary>
	skyBool m_bKeybuf[ eSkyMouseKeyKind_Max ];

	/// <summary>
	/// �P�t���[���O�̃L�[���͏��
	/// </summary>
	skyBool m_bOldKeybuf[ eSkyMouseKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�t���O
	/// </summary>
	skyBool m_RepFlg[ eSkyMouseKeyKind_Max ];

	/// <summary>
	/// ���s�[�g�Ԋu�J�E���^�[
	/// </summary>
	s32 m_RepIntvl[ eSkyMouseKeyKind_Max ];

	/// <summary>
	/// �}�E�X���W
	/// </summary>
	POINT m_MousePoint;

	/// <summary>
	/// �P�t���[���O�̃}�E�X���W
	/// </summary>
	POINT m_OldMousePoint;

	/// <summary>
	/// �}�E�X�C�x���g�f���Q�[�g�n���h��
	/// <summary>
	delegate::CDelegateHandler	m_EventHandler;
};

//-------------------------------------< ���� >---------------------------------------------//

static CMousePtr*  m_pMouse  = skyNull;
static u32         m_PlayerNum = 0;

void CMouse::Initialize( u32 playerNum )
{
	m_pMouse	= NEW_ARRAY__( CMousePtr , playerNum );
	m_PlayerNum	= playerNum;

	s_RepIntvlNum = C_S32( input_DEF_REPEATE_INTVL );
	s_OnIntvlNum = C_S32( input_DEF_ON_INTVL );

	//�����̏������H�v���邱�ƂŁA�L�[�{�[�h�̂n�m/�n�e�eF���ł���
	for( u32 i = 0 ; i < playerNum ; i++ )
	{
		m_pMouse[ i ].SetPtr( NEW__( CMouse , CMouse() ) );
	}
}

CMouse::CMouse()
{
	m_pMember = NEW__( CMouseMember , CMouseMember() );
}

CMouse::~CMouse()
{
	DEL m_pMember;
}

void CMouse::Dispose()
{
	DEL_ARRAY	m_pMouse;
}

CMousePtr &CMouse::Get( u32 playerNo )
{
	return m_pMouse[ playerNo ];
}

void CMouse::SetBtnRepeat( s32 repIntvl , s32 onIntvl )
{
	s_RepIntvlNum = repIntvl;
	s_OnIntvlNum = onIntvl;
}

void CMouse::Update( const fps::CFpsController &fps )
{
	SKY_UNUSED_ARG( fps );

	//�O�̏���ۑ�����i�v���C���[���j
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		CMemoryUtil_::Copy( m_pMouse[ playerNo ]->m_pMember->m_bOldKeybuf , m_pMouse[ playerNo ]->m_pMember->m_bKeybuf , sizeof( skyBool ) * eSkyMouseKeyKind_Max );
	}

	//�L�[���͏��̎擾
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		for( s32 key = 0 ; key < eSkyMouseKeyKind_Max ; key++ )
		{
			if ( ::GetKeyState( s_VkeyTbl[ key ] ) & 0x80 )
			{
				m_pMouse[ CURRENT_PLAYNER_NO ]->m_pMember->m_bKeybuf[ key ] = skyTrue;
			}
			else
			{
				m_pMouse[ CURRENT_PLAYNER_NO ]->m_pMember->m_bKeybuf[ key ] = skyFalse;
			}
		}
	}

	//�O�̃}�E�X���W����ۑ�����i�v���C���[���j
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		m_pMouse[ playerNo ]->m_pMember->m_OldMousePoint = m_pMouse[ playerNo ]->m_pMember->m_MousePoint;
	}

	//���݂̃}�E�X���W���擾
	::GetCursorPos( &m_pMouse[ CURRENT_PLAYNER_NO ]->m_pMember->m_MousePoint );
}

void CMouse::AttachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_pMember->m_EventHandler += spListener;
}

void CMouse::DetachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_pMember->m_EventHandler -= spListener;
}


void CMouse::KeyOn( eSkyMouseKeyKind key ) 
{
	m_pMember->m_bKeybuf[ key ] = skyTrue;
}

skyBool CMouse::IsKeyOn( eSkyMouseKeyKind key ) 
{
	return ( m_pMember->m_bKeybuf[ key ] == skyTrue ) && ( m_pMember->m_bOldKeybuf[ key ] == skyTrue );
}

skyBool CMouse::IsKeyRelease( eSkyMouseKeyKind key ) 
{
	//�������Ă��đO�t���[�������Ă���
	return ( !( m_pMember->m_bKeybuf[ key ] == skyTrue ) && ( m_pMember->m_bOldKeybuf[  key ] == skyTrue ) );
}

skyBool CMouse::IsKeyPress( eSkyMouseKeyKind key ) 
{
	//�������Ă��đO�̃t���[�������ĂȂ�
	return ( m_pMember->m_bKeybuf[ key ] == skyTrue ) && ( m_pMember->m_bOldKeybuf[ key ] == skyFalse );
}

skyBool CMouse::IsKeyRepeat( eSkyMouseKeyKind key )
{
	if ( IsKeyPress( key ) == skyTrue )
	{
		m_pMember->m_RepFlg[ key ] = skyTrue;
		m_pMember->m_RepIntvl[ key ] = 0;
		return skyTrue;
	}
	else if ( m_pMember->m_RepFlg[ key ] == skyTrue && IsKeyOn( key ) == skyTrue )
	{
		if ( m_pMember->m_RepIntvl[ key ] >= s_RepIntvlNum )
		{
			m_pMember->m_RepIntvl[ key ] -= s_OnIntvlNum;
			return skyTrue;
		}
		else
		{
			m_pMember->m_RepIntvl[ key ]++;
		}
	}
	return skyFalse;
}

void CMouse::EventExec_LBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_LEFT_BTN_DOWN , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_LBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_LEFT_BTN_UP , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_RBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_RIGHT_BTN_DOWN , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_RBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_RIGHT_BTN_UP , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_CBtnDown( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_CENTER_BTN_DOWN , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_CBtnUp( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_CENTER_BTN_UP , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_HWRotation( u32 playerNo , s32 x , s32 y , s32 rotVal , eSkyMouseKeyWheelWithBtnKind eWWKind , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_WHEEL_ROTATION , x , y , rotVal , eWWKind , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

void CMouse::EventExec_Move( u32 playerNo , s32 x , s32 y , wndType windowId )
{
	delegate::CDelegateArgs args;
	args.Sets32( MOUSE_EVENT_MOVE , x , y , windowId );

	m_pMouse[ playerNo ]->m_pMember->m_EventHandler( CommonSmartPointer() , args );
}

} } }