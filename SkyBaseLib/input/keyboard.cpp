#include "StdAfx.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Script.h"

namespace sky { namespace lib { namespace input {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// リピート間隔
/// </summary>
static s32 s_RepIntvlNum = 0;

/// <summary>
/// リピート間隔
/// </summary>
static s32 s_OnIntvlNum= 0;

static CkeyboardPtr*  m_pKeyBord  = skyNull;
static u32            m_PlayerNum = 0;

static u8 s_VkeyTbl[ eSkyKeybordKeyKind_Max ] =
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	VK_RETURN,
	VK_SHIFT,
	VK_ESCAPE,
	VK_HOME,
	VK_LEFT,
	VK_UP,
	VK_RIGHT,
	VK_DOWN,
	VK_PRIOR,
	VK_NEXT,
	VK_ESCAPE,
	VK_TAB,
};

void Ckeyboard::Initialize( u32 playerNum )
{
	m_pKeyBord	= NEW_ARRAY__( CkeyboardPtr , playerNum );
	m_PlayerNum	= playerNum;

	s_RepIntvlNum = C_S32( input_DEF_REPEATE_INTVL );
	s_OnIntvlNum = C_S32( input_DEF_ON_INTVL );

	//ここの処理を工夫することで、キーボードのＯＮ/ＯＦＦFができる
	for( u32 i = 0 ; i < playerNum ; i++ )
	{
		m_pKeyBord[ i ].SetPtr( NEW__( Ckeyboard , Ckeyboard() ) );
	}
}

void Ckeyboard::Dispose()
{
	DEL_ARRAY	m_pKeyBord;
}

CkeyboardPtr &Ckeyboard::Get( u32 playerNo )
{
	return m_pKeyBord[ playerNo ];
}

void Ckeyboard::SetBtnRepeat( s32 repIntvl , s32 onIntvl )
{
	s_RepIntvlNum = repIntvl;
	s_OnIntvlNum = onIntvl;
}

void Ckeyboard::_UpdateInputInfo()
{
	//前の情報を保存する（プレイヤー分）
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		CMemoryUtil_::Copy( m_pKeyBord[ playerNo ]->m_bOldKeybuf , m_pKeyBord[ playerNo ]->m_bKeybuf , sizeof( skyBool ) * eSkyKeybordKeyKind_Max );
		CMemoryUtil_::Copy( m_pKeyBord[ playerNo ]->m_bOldProgramKeybuf , m_pKeyBord[ playerNo ]->m_bProgramKeybuf , sizeof( skyBool ) * eSkyKeybordKeyKind_Max );
	}

	//キー入力情報の取得
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		for( s32 key = 0 ; key < eSkyKeybordKeyKind_Max ; key++ )
		{
			if ( ::GetKeyState( s_VkeyTbl[ key ] ) & 0x80 || 
				m_pKeyBord[ CURRENT_PLAYNER_NO ]->m_bProgramKeybuf[ key ] == skyTrue )
			{
				m_pKeyBord[ CURRENT_PLAYNER_NO ]->m_bKeybuf[ key ] = skyTrue;
			}
			else
			{
				m_pKeyBord[ CURRENT_PLAYNER_NO ]->m_bKeybuf[ key ] = skyFalse;
			}
			m_pKeyBord[ CURRENT_PLAYNER_NO ]->m_bProgramKeybuf[ key ] = skyFalse;
		}
	}
}

void Ckeyboard::AttachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_EventHandler += spListener;
}

void Ckeyboard::DetachInputListener( const delegate::IDelegateObjectPtr &spListener )
{
	m_EventHandler -= spListener;
}

void Ckeyboard::_UpdateInputFunc( const fps::CFpsController &fps )
{
	delegate::CDelegateArgs args;
	args.m_Fps = fps;

	//パラメーターは時間
	for ( u32 playerNo = 0 ; playerNo < m_PlayerNum ; playerNo++ )
	{
		m_pKeyBord[ playerNo ]->m_EventHandler( CommonSmartPointer() , args );
	}
}

void Ckeyboard::Update( const fps::CFpsController &fps )
{
	//キー情報の更新
	_UpdateInputInfo();

	//入力受付関数のコールバック
	_UpdateInputFunc( fps );
}

void Ckeyboard::KeyOn( eSkyKeybordKeyKind key ) 
{
	m_bProgramKeybuf[ key ] = skyTrue;
}

skyBool Ckeyboard::IsKeyOn( eSkyKeybordKeyKind key ) 
{
	return ( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[ key ] == skyTrue );
}

skyBool Ckeyboard::IsKeyRelease( eSkyKeybordKeyKind key ) 
{
	//今離していて前フレーム押している
	return ( !( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[  key ] == skyTrue ) );
}

skyBool Ckeyboard::IsKeyPress( eSkyKeybordKeyKind key ) 
{
	//今押していて前のフレーム押してない
	return ( m_bKeybuf[ key ] == skyTrue ) && ( m_bOldKeybuf[ key ] == skyFalse );
}

skyBool Ckeyboard::IsKeyRepeat( eSkyKeybordKeyKind key )
{
	if ( IsKeyPress( key ) == skyTrue )
	{
		m_RepFlg[ key ] = skyTrue;
		m_RepIntvl[ key ] = 0;
		return skyTrue;
	}
	else if ( m_RepFlg[ key ] == skyTrue && IsKeyOn( key ) == skyTrue )
	{
		if ( m_RepIntvl[ key ] >= s_RepIntvlNum )
		{
			m_RepIntvl[ key ] -= s_OnIntvlNum;
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

//-----スクリプトでも使う----▽

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

static SQInteger IsKeyPress( HSQUIRRELVM v )
{
	//引数を受け取る
	SQInteger val;
	if ( !SQ_SUCCEEDED( sq_getinteger( v , 2 , &val ) ) )
	{
		SKY_PANIC_MSG( _T( "The argument was not able to be acquired. " ) ); //引数を取得できませんでした。
	}

	//入力受付結果取得
	sq_pushinteger( v , ( SQInteger )sky::lib::input::m_pKeyBord[ sky::lib::input::CURRENT_PLAYNER_NO ]->IsKeyPress( ( eSkyKeybordKeyKind )val ) );

	return 1; //戻り値1個返したよ
};

sky::lib::squirrel::SqClassObject GetSqInputManager() //コピーコンストラクタが呼ばれるので毎フレームは呼ばないでください。おそらく一回の実行で数回しか呼ばないはずです。
{
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "Ckeyboard" ) ) );
	classobj.AddMethod( CHS( _T( "IsKeyPress" ) ) , IsKeyPress , CHS( _T( "yi" ) ) );

	return classobj;
}

#endif
