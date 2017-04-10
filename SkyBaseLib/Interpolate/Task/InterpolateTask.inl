namespace sky { namespace lib { namespace interp {

template < class T >
inline void CInterpolateTask< T >::Start( T startValue , T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType , eInterpolateType eInterpType , updateTime speed )
{
	m_CurrentData.m_StartValue  = startValue;
	m_CurrentData.m_EndValue    = endValue;
	m_CurrentData.m_Time        = time;
	m_CurrentData.m_eInterpType = eInterpType;
	m_CurrentData.m_Speed       = speed;
	m_CurrentData.m_EndType     = endType;
	m_CurrentData.m_BlendTime   = 0.0f;
	m_CurrentData.m_NowTime     = 0;
	m_CurrentData.m_NowValue    = startValue;
	m_CurrentData.m_eState      = eState_Play_;
	m_BlendData.m_eState        = eState_Wait_;

	//同時に一つのプレイヤーにしか登録できない
	if ( m_pPlayer == skyNull )
	{
		m_pPlayer = pPlayer;
		m_pPlayer->RegisterTask( this );
	}
}

template < class T >
inline void CInterpolateTask< T >::Start( T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType , eInterpolateType eInterpType , updateTime speed )
{
	Start( m_CurrentData.m_NowValue , endValue , time , pPlayer , endType , eInterpType , speed );
}

template < class T >
inline void CInterpolateTask< T >::End()
{
	m_pPlayer->UnRegisterTask( this );
	m_pPlayer = skyNull;
}

template < class T >
inline void CInterpolateTask< T >::RequestEndValue( T endValue , updateTime time , updateTime blendTime , eInterpolateType eInterpType , updateTime speed , ePlayEndType endType )
{
	m_EndRequestQueue.Push();
	CExecData_ &data = m_EndRequestQueue.GetBack();

	data.m_EndValue    = endValue;
	data.m_Time        = time;
	data.m_eInterpType = eInterpType;
	data.m_Speed       = speed;
	data.m_EndType     = endType; 
	data.m_BlendTime   = blendTime; 
	data.m_eState      = eState_Wait_;
	data.m_NowTime     = 0;
}

template < class T >
inline void CInterpolateTask< T >::RequestEndValue( T endValue )
{
	RequestEndValue( endValue , m_CurrentData.m_Time , 0.0f , m_CurrentData.m_eInterpType , m_CurrentData.m_Speed , m_CurrentData.m_EndType );
}

template < class T >
inline void CInterpolateTask< T >::RequestEndValue( T endValue , updateTime time )
{
	RequestEndValue( endValue , time , 0.0f , m_CurrentData.m_eInterpType , m_CurrentData.m_Speed , m_CurrentData.m_EndType );
}

template < class T >
inline void CInterpolateTask< T >::CancelRequest()
{
	m_EndRequestQueue.DeleteAll();
}

template < class T >
inline void CInterpolateTask< T >::ChangeEndValue( T endValue )
{
	m_CurrentData.m_EndValue = endValue;
}

template < class T >
inline void CInterpolateTask< T >::ChangeInterpolateType ( eInterpolateType eInterpType )
{
	m_CurrentData.m_eInterpType = eInterpType;
}

template < class T >
inline void CInterpolateTask< T >::ChangeSpeed ( updateTime speed )
{
	m_CurrentData.m_Speed = speed;
}

template < class T >
inline void CInterpolateTask< T >::ChangeEndType( ePlayEndType endType )
{
	m_CurrentData.m_EndType = endType;
}

template < class T >
inline CInterpolateTask< T >::CInterpolateTask() : m_pPlayer( skyNull )
{

}

template < class T >
void *CInterpolateTask< T >::Update( updateTime time )
{
	updateTime leftTime = m_CurrentData.m_Time - m_CurrentData.m_NowTime;

	//ブレンド対象を捜す
	if ( m_BlendData.m_eState == eState_Wait_ &&
		 m_CurrentData.m_eState == eState_Play_ && 
		 m_CurrentData.m_EndType == ePlayEndType_End && 
		 m_EndRequestQueue.GetNum() > 0 &&
		 m_EndRequestQueue.GetFront().m_BlendTime > 0 )
	{
		m_BlendData              = m_EndRequestQueue.GetFront();

		m_BlendData.m_eState     = eState_Play_;
		m_BlendData.m_StartValue = m_CurrentData.m_NowValue;

		m_EndRequestQueue.Pop();
	}

	//ブレンドWeightを計算
	if ( m_BlendData.m_eState == eState_Play_ && m_BlendData.m_BlendTime > leftTime )
	{
		//ブレンド時間算出
		interp::Linear( m_CurrentWeights , 0.0f , 1.0f , ( f32 )( leftTime / m_BlendData.m_BlendTime ) );

		//ブレンド値の計算
		InterpolateCommon< T >( m_BlendData.m_NowValue , m_BlendData.m_StartValue , m_BlendData.m_EndValue , ( f32 )( m_BlendData.m_NowTime / m_BlendData.m_Time ) , m_BlendData.m_eInterpType );

		//時間の加算
		m_BlendData.m_NowTime += time;
	}
	else
	{
		//スタート位置を現在の位置にし続ける
		m_BlendData.m_StartValue = m_CurrentData.m_NowValue;

		m_CurrentWeights = 1.0f;
	}

	//現在の値を計算
	InterpolateCommon< T >( m_CurrentData.m_NowValue , m_CurrentData.m_StartValue , m_CurrentData.m_EndValue , ( f32 )( m_CurrentData.m_NowTime / m_CurrentData.m_Time ) , m_CurrentData.m_eInterpType );

	//ブレンド値を考慮
	if ( m_CurrentWeights != 1.0f )
	{
		m_CurrentData.m_NowValue = ( m_CurrentData.m_NowValue * m_CurrentWeights ) + ( m_BlendData.m_NowValue * ( 1.0f - m_CurrentWeights ) );
	}

	if ( m_CurrentData.m_NowTime == m_CurrentData.m_Time )
	{//終了
		
		switch( m_CurrentData.m_EndType )
		{
			case ePlayEndType_End:

				//ブレンドデータをカレントデータに設定
				if ( m_BlendData.m_eState == eState_Play_ )
				{
					m_CurrentData        = m_BlendData;
					m_BlendData.m_eState = eState_Wait_;
				}
				//次があったら次を再生なければ何もしない
				else if( m_EndRequestQueue.GetNum() > 0 )
				{
					//キューから先頭データを取得
					CExecData_ &data = m_EndRequestQueue.GetFront();

					m_CurrentData.m_StartValue  = m_CurrentData.m_NowValue;
					m_CurrentData.m_EndValue    = data.m_EndValue;
					m_CurrentData.m_Time        = data.m_Time;
					m_CurrentData.m_eInterpType = data.m_eInterpType;
					m_CurrentData.m_Speed       = data.m_Speed;
					m_CurrentData.m_EndType     = data.m_EndType;
					m_CurrentData.m_BlendTime   = data.m_BlendTime;
					m_CurrentData.m_eState      = eState_Play_;

					//頭から再生
					m_CurrentData.m_NowTime     = 0.0f;

					//キューからデータを削除
					m_EndRequestQueue.Pop();
				}
				else
				{
					m_CurrentData.m_eState      = eState_Wait_;
				}
				break;
			case ePlayEndType_Repetition:

				//現在値を開始に戻す
				m_CurrentData.m_NowValue = m_CurrentData.m_StartValue;

				//頭から再生
				m_CurrentData.m_NowTime = 0.0f;

				break;
			case ePlayEndType_Back:

				//開始と終了を交換
				TemplateUtil::Swap< T >( m_CurrentData.m_StartValue , m_CurrentData.m_EndValue );

				//頭から再生
				m_CurrentData.m_NowTime = 0.0f;
				break;
		};
	}
	else
	{//補間継続

		//コールバック
		m_GetValueEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )&m_CurrentData.m_NowTime , ( memSize )&m_CurrentData.m_NowValue ) );

		//時間を加算
		m_CurrentData.m_NowTime += time;

		//時間補正
		if ( m_CurrentData.m_NowTime > m_CurrentData.m_Time )
		{
			m_CurrentData.m_NowTime = m_CurrentData.m_Time;
		}
	}

	return &m_CurrentData.m_NowValue;
}

template < class T >
inline T CInterpolateTask< T >::GetValue( updateTime time )
{
	//時間をオーバーしたら補正
	if ( time > m_CurrentData.m_Time )
	{
		time = m_CurrentData.m_Time;
	} 
	else if ( time < 0.0f )
	{
		time = 0.0f;
	}

	T value;
	return InterpolateCommon< T >( value , m_CurrentData.m_StartValue , m_CurrentData.m_EndValue , time / m_CurrentData.m_Time , m_CurrentData.m_eInterpType );
}

template < class T >
skyBool CInterpolateTask< T >::IsEnd()
{
	return ( m_CurrentData.m_eState == eState_Play_ );
}

} } }