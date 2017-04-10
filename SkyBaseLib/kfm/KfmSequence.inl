namespace sky { namespace lib { namespace kfm {

template < class T >
inline void CKfmSequence< T >::Play( updateTime time , f32 weight , skyBool isCallBackValue , const CKfmSequence< T > *pBlendSequence )
{
	if ( m_FCurveList.GetNum() > 0 )
	{
		u32 i = 0;
		FOREACH( CFcurveInfoPtrList , it , m_FCurveList )
		{
			if ( ( *it ).m_StartTime <= time )
			{
				if ( pBlendSequence != skyNull && pBlendSequence->GetFCurveNum() > i )
				{
					( *it ).m_pFCurvePtr->ColTimeValue( time - ( *it ).m_StartTime , weight , isCallBackValue , pBlendSequence->GetFCurve( i ) );
				}
				else
				{
					( *it ).m_pFCurvePtr->ColTimeValue( time - ( *it ).m_StartTime , weight , isCallBackValue );
				}
			}
			i++;
		}
	}

	//最初なら開始イベント発生
	if( time == 0.0f )
	{
		//再生開始イベント
		m_PlayEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( s32 )ID , ( s32 )ePlayState_Start ) );
	}

	//最初なら最後なら終了イベント発生
	if( time == m_TimeRange )
	{
		//再生終了イベント
		m_PlayEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( s32 )ID , ( s32 )ePlayState_End ) );
	}
}

template < class T >
inline CKfmSequence< T >::CKfmSequence()
{
}

template < class T >
inline u32 CKfmSequence< T >::AddFCurve( updateTime startTime , const CFCurve< T > *pFCurve )
{
	CFcurveInfo &info = m_FCurveList.AddTail();
	info.m_StartTime = startTime;
	info.m_pFCurvePtr = const_cast< CFCurve< T > * >( pFCurve );

	if ( m_TimeRange < pFCurve->GetTimeRange() + startTime ) m_TimeRange = pFCurve->GetTimeRange() + startTime;

	return m_FCurveList.GetNum() - 1;
}

template < class T >
inline void CKfmSequence< T >::DeleteAll()
{
	m_FCurveList.DeleteAll();
	m_TimeRange = 0;
}

template < class T >
inline const CFCurve< T > *CKfmSequence< T >::GetFCurve( u32 index ) const
{
	return m_FCurveList[ index ].m_pFCurvePtr;
}

template < class T >
inline u32 CKfmSequence< T >::GetFCurveNum() const
{
	return m_FCurveList.GetNum();
}

} } }