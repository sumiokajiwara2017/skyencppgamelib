namespace sky { namespace lib { namespace thread {

inline CriticalSection::CriticalSection()
{
	m_LoackThread = 0xffffffff;
	m_LockCount = 0;
	::InitializeCriticalSection( &m_Cs );
}

inline CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection( &m_Cs );

	//�����ŃG���[�������ꍇ�A���[�U�[��������~���s�����\��������܂��B
    SKY_ASSERT_MSG( m_LockCount == 0 , _T( "Lerave is necessary only for the number of Enter." ) ); //Enter�����񐔕�����Leave���Ȃ��܂܏I�����Ă���
}

inline void CriticalSection::Enter()
{
	::EnterCriticalSection( &m_Cs );

	m_LoackThread = CThreadUtil_::GetCurrentThreadId();
	m_LockCount++;
}

inline void CriticalSection::Leave()
{
    SKY_ASSERT_MSG( m_LockCount > 0 , _T( "Lerave is necessary only for the number of Enter. \n" ) ); //CriticalSection��Enter���Ă��Ȃ��̂�Leave���Ă���

	m_LockCount--;

	if ( m_LockCount == 0 )
	{
		m_LoackThread = 0xffffffff;
	}

	::LeaveCriticalSection( &m_Cs );
}

inline CriticalLock::CriticalLock( CriticalSection *pCs ) : m_LockCount( 0 ) , m_pCs( pCs )
{
	Enter();
}

inline CriticalLock::~CriticalLock()
{
	if ( m_pCs != skyNull ) 
	{
		//���b�N�J�E���g���N���e�B�J���Z�N�V������Leave����
		if ( 0 < m_LockCount )
		{
			for( u32 i = 0 ; i < m_LockCount ; i++ )
			{
				m_pCs->Leave();
			}
		}
	}
}

inline void CriticalLock::Enter()
{
	if ( m_pCs != skyNull ) 
	{
		m_LockCount++;
		m_pCs->Enter();
	}
}

inline void CriticalLock::Leave()
{
	if ( m_pCs != skyNull ) 
	{
		SKY_ASSERT_MSG( m_LockCount - 1 >= 0 , _T( "LockCount is invalid." ) );

		m_LockCount--;
		m_pCs->Leave();
	}
}

inline s32	CriticalLock::GetCount()
{
	return m_LockCount;
}

} } }