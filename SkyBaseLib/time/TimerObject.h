#pragma once

namespace sky { namespace lib { namespace time {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���Ԍv���p�N���X
/// �R���X�g���N�^�Ōv�����J�n���ăf�X�g���N�^�Ōv�����ʂ��R���\�[���o�͂���
/// �X�R�[�v�̊J�n��TIMER_OBJCET��錾���āA�I���Ƀf�X�g���N�^�Ŏ��Ԃ��o��
/// �ʏ�͊֐��̍ŏ��ɐ錾���邪�A�֐����̋Ǐ��I�Ȍv�������L�̂悤�ɏ����΂ł���B
/// {
///		TIMER_OBJCET( "hoge" );//�v���J�n
///		
///		//���������������ׂ��C�ɂȂ鏈����������
///
///	} //�����Ōv���I���i�X�R�[�v���甲���A�f�X�g���N�^���Ă΂��j
/// </summary>
class CTimerObject : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CTimerObject( const skyString *pName );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline ~CTimerObject();

	/// <summary>
	/// ���O
	/// </summary>
	const skyString		*m_Name;

#ifdef SKYLIB_PLATFORM_WIN32
	/// <summary>
	/// ���Ԍv���J�n�����
	/// </summary>
	LARGE_INTEGER m_liBegin;

	/// <summary>
	/// ���Ԍv���I�������
	/// </summary>
	LARGE_INTEGER m_liEnd;

	/// <summary>
	/// ���Ԍv���������
	/// </summary>
	LARGE_INTEGER m_liFreq;
#endif

	/// <summary>
	/// ���Ԍv���J�n���ԁi�~���b�j
	/// </summary>
	updateTime m_msBegin;

};

#ifdef SKYLIB_PLATFORM_WIN32

inline CTimerObject::CTimerObject( const skyString *pName ) : m_Name( pName )
{
	QueryPerformanceCounter( &m_liBegin );

	m_msBegin = CTimeUtil_::GetNowTime();
}

inline CTimerObject::~CTimerObject()
{
	if ( m_Name == NULL ) return;

	QueryPerformanceCounter( &m_liEnd );
	QueryPerformanceFrequency( &m_liFreq );
	_tprintf( _T( "%s = %f ms( %f ) \n" ), m_Name , CTimeUtil_::GetNowTime() - m_msBegin , ( d64 )( m_liEnd.QuadPart - m_liBegin.QuadPart) / (d64)m_liFreq.QuadPart );

	m_Name = NULL;
}

#endif

#ifdef SW_SKYLIB_TIMER_ON

#define TIMER_OBJCET( name ) sky::lib::time::CTimerObject tm( name )

#else

#define TIMER_OBJCET( name ) 

#endif

} } }