namespace sky { namespace lib { namespace fps {

inline CFpsController::CFpsController() :
m_ElapsedFrame( 0 ) ,
m_StartTime( 0 ) ,
m_EndTime( 0 ) ,
m_BeforStartTime( 0 ) ,
m_OneFrameTime( 0 ) ,
m_ColAverageFrameNum( 0 ) ,
m_ColAverageSumTimeBuffer( 0 ) ,
m_ColAverageFrameCounter( 0 )

{
	SetFrameRate( DEFAULT_FPS );
	m_StartAppTime = CTimeUtil_::GetNowTime();
}

inline void	CFpsController::SetFrameRate( u16 frameRate )
{ 
	m_FrameRate = frameRate;
	m_OneFrameTime = 1000 / m_FrameRate;
}

inline u16 CFpsController::GetFrameRate()
{ 
	return m_FrameRate;
}

inline updateTime CFpsController::GetOneFrameTime()
{
	return m_OneFrameTime;
}

inline void	CFpsController::SetStartTime( u32 startTime )
{
	m_StartTime = startTime;
}

inline updateTime	CFpsController::GetStartTime()
{
	return m_StartTime;
}

inline void	CFpsController::IncElapsedFrame()
{ 
	m_ElapsedFrame++;
}

inline void	CFpsController::StartFrame()
{
	if ( m_BeforStartTime == 0.0f )
	{//�ŏ�������m_StartTime��m_BeforStartTime�͓����B�܂�S�Ă̏������o�ߎ���0�ɂȂ�i�~�܂��Ă�j
		m_BeforStartTime = CTimeUtil_::GetNowTime();
	}
	else
	{
		m_BeforStartTime = m_StartTime;
	}
	m_StartTime = CTimeUtil_::GetNowTime();
}

inline updateTime CFpsController::GetPassageTime_Msec() const 
{
	return m_StartTime - m_BeforStartTime;
}

inline updateTime CFpsController::GetPassageTime_Second() const
{
	return GetPassageTime_Msec() / 1000;
}

inline updateTime CFpsController::GetPassageAllTime_Msec() const
{
	return CTimeUtil_::GetNowTime() - m_StartAppTime;
}

inline updateTime CFpsController::GetPassageAllTime_Second() const
{
	return GetPassageAllTime_Msec() / 1000;
}

inline u32	CFpsController::GetElapsedFrame() const
{
	return m_ElapsedFrame; 
}

inline skyBool CFpsController::IsEndTimeStillRate()
{
	updateTime PassTime = CTimeUtil_::GetNowTime() - m_StartTime; // �o�ߎ��Ԃ̌v�Z
		
	return ( GetOneFrameTime() > PassTime );
}

inline void	CFpsController::EndFrame()
{
	m_EndTime = CTimeUtil_::GetNowTime();

	updateTime PassTime = m_EndTime - m_StartTime; // �o�ߎ��Ԃ̌v�Z

	//���Ԃ��]������҂�
	(  GetOneFrameTime() > PassTime ) ? CTimeUtil_::Wait( GetOneFrameTime() - PassTime ) : CTimeUtil_::Wait( 0 ); // �҂B

	IncElapsedFrame();
}

inline skyBool CFpsController::GetAverageOneFrameTime( u32 colFrameNum , updateTime &destOneFrameTime )
{
	if ( colFrameNum <= m_ColAverageFrameNum )
	{//�v�����ʂ��o��

		//���v�l���v���t���[�����Ŋ���
		destOneFrameTime = m_ColAverageSumTimeBuffer / ( updateTime )colFrameNum;

		//������
		m_ColAverageFrameNum = 0;
		m_ColAverageSumTimeBuffer = 0;

		return skyTrue;
	}
	else
	{//�v�����p������

		//�P�t���[��������̎��Ԃ����Z
		m_ColAverageSumTimeBuffer += GetPassageTime_Msec();

		m_ColAverageFrameNum++;

		return skyFalse;
	}
}


} } }