#include "StdAfx.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Script.h"

#pragma message( "Incompleteness. It will mount neatly later. " ) //�������B���Ƃł����Ǝ�������B

namespace sky { namespace lib { namespace input {

//-------------------------------------< ���� >---------------------------------------------//

static CPadPtr*  m_pPad  = skyNull;
static u32       m_PlayerNum = 0;

static u8 s_VkeyTbl[ eSkyPadKeyKind_Max ] =
{
	PAD_KEY_UP ,
	PAD_KEY_BOTTOM ,
	PAD_KEY_LEFT ,
	PAD_KEY_RIGHT ,

	PAD_BTN_UP ,
	PAD_BTN_BOTTOM,
	PAD_BTN_LEFT ,
	PAD_BTN_RIGHT ,

	PAD_TRIG_L1 ,
	PAD_TRIG_L2 ,
	PAD_TRIG_R1 ,
	PAD_TRIG_R2 ,
};

void CPad::Initialize( u32 playerNum )
{
	m_pPad	= NEW_ARRAY__( CPadPtr , playerNum );
	m_PlayerNum	= playerNum;

	//�����̏������H�v���邱�ƂŁA�L�[�{�[�h�̂n�m/�n�e�eF���ł���
	for( u32 i = 0 ; i < playerNum ; i++ )
	{
		m_pPad[ i ].SetPtr( NEW__( CPad , CPad() ) );
	}
}

void CPad::Dispose()
{
	DEL_ARRAY	m_pPad;
}

CPadPtr &CPad::Get( u32 playerNo )
{
	return m_pPad[ playerNo ];
}

void CPad::SetBtnRepeat( s32 repIntvl , s32 onIntvl )
{
	m_nRepIntvlNum = repIntvl;
	m_nOnIntvlNum = onIntvl;
}

void CPad::_UpdateInputInfo()
{
}

void CPad::AttachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_EventHandler += spListener;
}

void CPad::DetachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_EventHandler -= spListener;
}

void CPad::_UpdateInputFunc( updateTime time )
{
	SKY_UNDER_CONSTRUCTION( time );
}

void CPad::Update( updateTime time )
{
	//�L�[���̍X�V
	_UpdateInputInfo();

	//���͎�t�֐��̃R�[���o�b�N
	_UpdateInputFunc( time );
}

void CPad::KeyOn( eSkyPadKeyKind key ) 
{
	m_bKeybuf[ key ] = skyTrue;
}

skyBool CPad::IsKeyOn( eSkyPadKeyKind key ) 
{
	return ( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[ key ] == skyTrue );
}

skyBool CPad::IsKeyRelease( eSkyPadKeyKind key ) 
{
	//�������Ă��đO�t���[�������Ă���
	return ( !( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[  key ] == skyTrue ) );
}

skyBool CPad::IsKeyPress( eSkyPadKeyKind key ) 
{
	//�������Ă��đO�̃t���[�������ĂȂ�
	return ( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[ key ] == skyFalse );
}

skyBool CPad::IsKeyRepeat( eSkyPadKeyKind key )
{
	if ( IsKeyPress( key ) == skyTrue )
	{
		m_RepFlg[ key ] = skyTrue;
		m_RepIntvl[ key ] = 0;
		return skyTrue;
	}
	else if ( m_RepFlg[ key ] == skyTrue && IsKeyOn( key ) == skyTrue )
	{
		if ( m_RepIntvl[ key ] >= m_nRepIntvlNum )
		{
			m_RepIntvl[ key ] -= m_nOnIntvlNum;
			return skyTrue;
		}
		else
		{
			m_RepIntvl[ key ]++;
		}
	}
	return skyFalse;
}

} } }