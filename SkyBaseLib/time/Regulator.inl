namespace sky { namespace lib { namespace time {

inline CRegulator::~CRegulator()
{
}

inline CRegulator::CRegulator( updateTime radyIntvlTime ) : m_ReadyIntvlTime( radyIntvlTime ) , m_AccumulatedTime( 0.0f )
{
}

inline skyBool CRegulator::IsReady( updateTime time )
{
	skyBool result = skyFalse;
	m_AccumulatedTime += time;

	if ( m_AccumulatedTime >= m_ReadyIntvlTime )
	{
		result = skyTrue;
		m_AccumulatedTime = 0.0f;
	}

	return result;
}

inline void CRegulator::SetReadyIntvlTime( updateTime radyIntvlTime )
{
	m_ReadyIntvlTime = radyIntvlTime;
}

} } }