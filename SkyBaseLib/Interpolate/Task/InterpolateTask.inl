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

	//�����Ɉ�̃v���C���[�ɂ����o�^�ł��Ȃ�
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

	//�u�����h�Ώۂ�{��
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

	//�u�����hWeight���v�Z
	if ( m_BlendData.m_eState == eState_Play_ && m_BlendData.m_BlendTime > leftTime )
	{
		//�u�����h���ԎZ�o
		interp::Linear( m_CurrentWeights , 0.0f , 1.0f , ( f32 )( leftTime / m_BlendData.m_BlendTime ) );

		//�u�����h�l�̌v�Z
		InterpolateCommon< T >( m_BlendData.m_NowValue , m_BlendData.m_StartValue , m_BlendData.m_EndValue , ( f32 )( m_BlendData.m_NowTime / m_BlendData.m_Time ) , m_BlendData.m_eInterpType );

		//���Ԃ̉��Z
		m_BlendData.m_NowTime += time;
	}
	else
	{
		//�X�^�[�g�ʒu�����݂̈ʒu�ɂ�������
		m_BlendData.m_StartValue = m_CurrentData.m_NowValue;

		m_CurrentWeights = 1.0f;
	}

	//���݂̒l���v�Z
	InterpolateCommon< T >( m_CurrentData.m_NowValue , m_CurrentData.m_StartValue , m_CurrentData.m_EndValue , ( f32 )( m_CurrentData.m_NowTime / m_CurrentData.m_Time ) , m_CurrentData.m_eInterpType );

	//�u�����h�l���l��
	if ( m_CurrentWeights != 1.0f )
	{
		m_CurrentData.m_NowValue = ( m_CurrentData.m_NowValue * m_CurrentWeights ) + ( m_BlendData.m_NowValue * ( 1.0f - m_CurrentWeights ) );
	}

	if ( m_CurrentData.m_NowTime == m_CurrentData.m_Time )
	{//�I��
		
		switch( m_CurrentData.m_EndType )
		{
			case ePlayEndType_End:

				//�u�����h�f�[�^���J�����g�f�[�^�ɐݒ�
				if ( m_BlendData.m_eState == eState_Play_ )
				{
					m_CurrentData        = m_BlendData;
					m_BlendData.m_eState = eState_Wait_;
				}
				//�����������玟���Đ��Ȃ���Ή������Ȃ�
				else if( m_EndRequestQueue.GetNum() > 0 )
				{
					//�L���[����擪�f�[�^���擾
					CExecData_ &data = m_EndRequestQueue.GetFront();

					m_CurrentData.m_StartValue  = m_CurrentData.m_NowValue;
					m_CurrentData.m_EndValue    = data.m_EndValue;
					m_CurrentData.m_Time        = data.m_Time;
					m_CurrentData.m_eInterpType = data.m_eInterpType;
					m_CurrentData.m_Speed       = data.m_Speed;
					m_CurrentData.m_EndType     = data.m_EndType;
					m_CurrentData.m_BlendTime   = data.m_BlendTime;
					m_CurrentData.m_eState      = eState_Play_;

					//������Đ�
					m_CurrentData.m_NowTime     = 0.0f;

					//�L���[����f�[�^���폜
					m_EndRequestQueue.Pop();
				}
				else
				{
					m_CurrentData.m_eState      = eState_Wait_;
				}
				break;
			case ePlayEndType_Repetition:

				//���ݒl���J�n�ɖ߂�
				m_CurrentData.m_NowValue = m_CurrentData.m_StartValue;

				//������Đ�
				m_CurrentData.m_NowTime = 0.0f;

				break;
			case ePlayEndType_Back:

				//�J�n�ƏI��������
				TemplateUtil::Swap< T >( m_CurrentData.m_StartValue , m_CurrentData.m_EndValue );

				//������Đ�
				m_CurrentData.m_NowTime = 0.0f;
				break;
		};
	}
	else
	{//��Ԍp��

		//�R�[���o�b�N
		m_GetValueEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )&m_CurrentData.m_NowTime , ( memSize )&m_CurrentData.m_NowValue ) );

		//���Ԃ����Z
		m_CurrentData.m_NowTime += time;

		//���ԕ␳
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
	//���Ԃ��I�[�o�[������␳
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