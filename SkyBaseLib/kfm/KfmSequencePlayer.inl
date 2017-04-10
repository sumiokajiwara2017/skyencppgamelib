namespace sky { namespace lib { namespace kfm {

template < class T >
inline CKfmSequencePlayer< T >::CKfmSequencePlayer() : m_CurrentPlaySequenceWeights( 1.0f ) , m_IsSaveExec( skyFalse )
{
	m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
	m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
}

template < class T >
inline void CKfmSequencePlayer< T >::PlaySequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�L���[�ɒǉ�
	m_SequencePlayInfoFifo.Push();

	//�L���[�̖������擾
	CSequencePlayInfo& playInfo = m_SequencePlayInfoFifo.GetBack();

	//�Đ����ݒ�
	SetSequencePlayInfo_( playInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlayReverseSequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�L���[�ɒǉ�
	m_SequencePlayInfoFifo.Push();

	//�L���[�̖������擾
	CSequencePlayInfo& playInfo = m_SequencePlayInfoFifo.GetBack();

	//�Đ����ݒ�
	SetSequencePlayReverseInfo_( playInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlaySequence( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�L���[�N���A
	m_SequencePlayInfoFifo.AllPop();

	//�u�����h�V�[�P���X���I������
	m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;

	//�Đ����㏑���ݒ�
	SetSequencePlayInfo_( m_CurrentPlaySequenceInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlayReverseSequence( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�L���[�N���A
	m_SequencePlayInfoFifo.AllPop();

	//�u�����h�V�[�P���X���I������
	m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;

	//�Đ����㏑���ݒ�
	SetSequencePlayReverseInfo_( m_CurrentPlaySequenceInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline CKfmSequencePlayer< T >* CKfmSequencePlayer< T >::PlayGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�v���C���[���쐬
	CKfmSequencePlayer< T >* spPlayer( NEW CKfmSequencePlayer() );

	//���X�g�ɒǉ�
	m_GlobalSequencePlayList.AddTail( spPlayer );

	//�Đ����ݒ�
	spPlayer->PlaySequence( spSequence , 0.0f , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );

	return spPlayer;
}

template < class T >
inline CKfmSequencePlayer< T >* CKfmSequencePlayer< T >::PlayReverseGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�v���C���[���쐬
	CKfmSequencePlayer< T >* spPlayer( NEW CKfmSequencePlayer() );

	//���X�g�ɒǉ�
	m_GlobalSequencePlayList.AddTail( spPlayer );

	//�Đ����ݒ�
	spPlayer->PlayReverseSequence( spSequence , 0.0f , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );

	return spPlayer;
}

template < class T >
inline void CKfmSequencePlayer< T >::StopCurrentSequence()
{
	m_CurrentPlaySequenceInfo.m_eState = ePlayState_Stop;
}

template < class T >
inline void CKfmSequencePlayer< T >::StopGlobalSequence()
{
	FOREACH( CKfmSequencePlayerPtrList , it , m_GlobalSequencePlayList )
	{
		( *it )->StopCurrentSequence();
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::StopSequenceAll()
{
	StopCurrentSequence();
	StopGlobalSequence();
}

template < class T >
inline void CKfmSequencePlayer< T >::NextCurrentSequence()
{
	m_CurrentPlaySequenceInfo.m_EndType = ePlayEndType_End;
}

template < class T >
inline void CKfmSequencePlayer< T >::NextGlobalSequence()
{
	FOREACH( CKfmSequencePlayerPtrList , it , m_GlobalSequencePlayList )
	{
		( *it )->NextCurrentSequence();
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::NextAllSequence()
{
	NextCurrentSequence();
	NextGlobalSequence();
}

template < class T >
inline void CKfmSequencePlayer< T >::CancelCurrentSequence()
{
	m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
}

template < class T >
inline void CKfmSequencePlayer< T >::CancelGlobalSequence()
{
	FOREACH( CKfmSequencePlayerPtrList , it , m_GlobalSequencePlayList )
	{
		( *it )->CancelCurrentSequence();
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::CancelSequenceAll()
{
	CancelCurrentSequence();
	CancelGlobalSequence();
}

template < class T >
inline void CKfmSequencePlayer< T >::ReStartCurrentSequence()
{
	m_CurrentPlaySequenceInfo.m_eState = ePlayState_Play;
}

template < class T >
inline void CKfmSequencePlayer< T >::ReStartGlobalSequence()
{
	FOREACH( CKfmSequencePlayerPtrList , it , m_GlobalSequencePlayList )
	{
		( *it )->ReStartGlobalSequence();
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::ReStartSequenceAll()
{
	ReStartCurrentSequence();
	ReStartGlobalSequence();
}

template < class T >
inline const CKfmSequence< T >* CKfmSequencePlayer< T >::Play( updateTime timeOffset )
{
	CKfmSequence< T >* pPlayedBlendSequenc = skyNull;
	CKfmSequence< T >* pPlayedSequenc = skyNull;

//----�O���[�o���V�[�P���X�̍Đ�

	FOREACH_NOINC( CKfmSequencePlayerPtrList , it , m_GlobalSequencePlayList )
	{
		const CKfmSequence< T >* pPlayedGlobalSequenc = ( *it )->Play( timeOffset );
		if ( pPlayedGlobalSequenc == skyNull )
		{
			it = m_GlobalSequencePlayList.Erases( it );
		}
		else
		{
			it++;
		}
	}

//----���݂̃V�[�P���X���Đ��I����Ԃ̏ꍇ�A�L���[����V�[�P���X���擾����

	if ( m_CurrentPlaySequenceInfo.m_eState == ePlayState_Wait )
	{
		//�L���[�Ƀf�[�^������΂�����J�����g�ɂ��čĐ�
		if ( m_SequencePlayInfoFifo.GetNum() > 0 )
		{
			m_SequencePlayInfoFifo.Pop( m_CurrentPlaySequenceInfo );
		}

		//����Ƀu�����h�����𖞂������ꍇ�u�����h���|�b�v���Ă���
		if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
			&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
			&& m_SequencePlayInfoFifo.GetNum() > 0 
			&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
		{
			m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
		}
	}

//----���݂̃E�F�C�g���v�Z

	//�u�����h���肪����ꍇ�u�����h�䗦���v�Z����
	updateTime timeLeft = 0.0f; 
	if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait
		&&  m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End	)
	{
		//�Đ��c�莞�Ԃ��v�Z
		switch ( m_CurrentPlaySequenceInfo.m_eState )
		{
			case ePlayState_Play :
			{
				timeLeft = m_CurrentPlaySequenceInfo.m_MaxTime - m_CurrentPlaySequenceInfo.m_NowTime;
				break;
			}
			case ePlayState_Play_Reverse:
			{
				timeLeft = m_CurrentPlaySequenceInfo.m_NowTime - m_CurrentPlaySequenceInfo.m_MinTime;
				break;
			}
		}

		//�c�莞�Ԃƃu�����h���Ԃ��r���Ďc�莞�Ԃ��u�����h���Ԃɓ��B���Ă�����u�����h���Ԃ��v�Z����
		if ( timeLeft <= m_CurrentPlayBlendSequenceInfo.m_BlendTime )
		{
			switch( m_CurrentPlaySequenceInfo.m_BlendInterpType )
			{
				case eInterpolateType_Linear:
				{
					//�u�����h���ԎZ�o
					interp::Linear( m_CurrentPlaySequenceWeights , 0.0f , 1.0f , ( f32 )( timeLeft / m_CurrentPlayBlendSequenceInfo.m_BlendTime ) );

					break;
				}
				default:
					SKY_PANIC_MSG( "This type no support. \n" );
					break;
			}
		}
	}

//----���ݍĐ����̃u�����h�V�[�P���X���Đ�

	if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait && timeLeft <= m_CurrentPlayBlendSequenceInfo.m_BlendTime )
	{
		if( m_CurrentPlayBlendSequenceInfo.m_StartIntvl <= 0.0f )
		{

			switch ( m_CurrentPlayBlendSequenceInfo.m_eState )
			{

//====================================================================================================================��

				case ePlayState_Play :
				{
					//���݂̎��ԂōĐ��i�u�����h���͒l��Ԃ��Ȃ��j
					m_CurrentPlayBlendSequenceInfo.m_spSequencePtr->Play( m_CurrentPlayBlendSequenceInfo.m_NowTime , 1.0f - m_CurrentPlaySequenceWeights , skyFalse );

					//�Đ��ς݂̃u�����h�V�[�P���X���i�[
					pPlayedBlendSequenc = m_CurrentPlayBlendSequenceInfo.m_spSequencePtr;

					//�������Ԃ��ŏI���Ԃ�������I�������i�ʏ�Ȃ��Ǝv�����ǁA�u�����h���ɏI����Ă��܂����Ƃ����邩���j
					if ( m_CurrentPlayBlendSequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MaxTime )
					{
						//�Đ��I��
						m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
					}
					else
					{//�܂�����

						//���Ԃ�i�߂�
						m_CurrentPlayBlendSequenceInfo.m_NowTime += timeOffset * m_CurrentPlayBlendSequenceInfo.m_Speed;

						//���Ԃ��Đ��ő厞�Ԃ𒴂�����Đ��ő厞�Ԃɉ�����i�ʏ�Ȃ��Ǝv�����ǃu�����h���ɏI�ɓ��B���邱�Ƃ����邩���j
						if ( m_CurrentPlayBlendSequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlayBlendSequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
					}
					break;
				}

//====================================================================================================================��

				case ePlayState_Play_Reverse:
				{
					//���݂̎��ԂōĐ�
					m_CurrentPlayBlendSequenceInfo.m_spSequencePtr->Play( m_CurrentPlayBlendSequenceInfo.m_NowTime , 1.0f - m_CurrentPlaySequenceWeights );

					//�Đ��ς݂̃u�����h�V�[�P���X���i�[
					pPlayedBlendSequenc = m_CurrentPlayBlendSequenceInfo.m_spSequencePtr;

					//�������Ԃ��ŏI���Ԃ�������I�������i�ʏ�Ȃ��Ǝv�����ǁA�u�����h���ɏI����Ă��܂����Ƃ����邩���j
					if ( m_CurrentPlayBlendSequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MinTime )
					{
						//�Đ��I��
						m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
					}
					else
					{//�܂�����

						//���Ԃ�i�߂�
						m_CurrentPlayBlendSequenceInfo.m_NowTime -= timeOffset * m_CurrentPlayBlendSequenceInfo.m_Speed;

						//���Ԃ��Đ��ő厞�Ԃ𒴂�����Đ��ő厞�Ԃɉ�����i�ʏ�Ȃ��Ǝv�����ǃu�����h���ɏI�ɓ��B���邱�Ƃ����邩���j
						if ( m_CurrentPlayBlendSequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlayBlendSequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
					}
					break;
				}

//====================================================================================================================��

				case ePlayState_Stop :
					//�Ȃɂ����Ȃ�
					break;
				default:
					SKY_PANIC_MSG( _T( "The state unused this proc." ) );
					break;
			}
		}
		else
		{
			m_CurrentPlayBlendSequenceInfo.m_StartIntvl -= timeOffset;
		}
	}

//----���ݍĐ����̃V�[�P���X���Đ�

	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		if( m_CurrentPlaySequenceInfo.m_StartIntvl <= 0.0f )
		{
			switch ( m_CurrentPlaySequenceInfo.m_eState )
			{

//====================================================================================================================��

				case ePlayState_Play :
				{
					//���݂̎��ԂōĐ�
					m_CurrentPlaySequenceInfo.m_spSequencePtr->Play( m_CurrentPlaySequenceInfo.m_NowTime , m_CurrentPlaySequenceWeights , skyTrue , pPlayedBlendSequenc );

					//�Đ��ς݂̃V�[�P���X�̎Q�Ƃ�Ԃ�
					pPlayedSequenc = m_CurrentPlaySequenceInfo.m_spSequencePtr;

					//�������Ԃ��ŏI���Ԃ�������I���������s��
					if ( m_CurrentPlaySequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MaxTime )
					{
						switch ( m_CurrentPlaySequenceInfo.m_EndType )
						{
						case ePlayEndType_End :
						{
							if ( m_IsSaveExec == skyTrue )
							{
								//�L���[�ɒǉ�
								m_SequencePlayedInfoFifo.Push();

								//�L���[�̖������擾
								CSequencePlayInfo& playInfo = m_SequencePlayedInfoFifo.GetBack();

								//�ۑ�
								playInfo = m_CurrentPlaySequenceInfo;
							}

							//�u�����h�V�[�P���X���������ꍇ�J�����g�V�[�P���X�Ɉڍs����
							if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait )
							{
								m_CurrentPlaySequenceInfo = m_CurrentPlayBlendSequenceInfo;
								m_CurrentPlaySequenceWeights = 1.0f; //��

								//����Ƀu�����h�����𖞂������ꍇ�u�����h���|�b�v���Ă���
								if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
									&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
									&& m_SequencePlayInfoFifo.GetNum() > 0 
									&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
								{//���̃u�����h�Ώۂ�����
									m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
								}
								else
								{
									m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
								}
							}
							else
							{//�I��
								m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
							}
							break;
						}
						case ePlayEndType_Repetition :

							//�ŏ�����Đ�
							m_CurrentPlaySequenceInfo.m_NowTime = 0.0f;

							break;
						case ePlayEndType_Back :

							//�t�Đ�
							m_CurrentPlaySequenceInfo.m_eState = ePlayState_Play_Reverse;

							//�ŏI�t���[���͍Đ��ς݂Ȃ̂ŁA�ŏI������O�̃t���[������k��
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime - timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

							break;
						}
					}
					else
					{//�܂�����

						//���Ԃ�i�߂�
						m_CurrentPlaySequenceInfo.m_NowTime += timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

						//���Ԃ��Đ��ő厞�Ԃ𒴂�����Đ��ő厞�Ԃɉ�����
						if ( m_CurrentPlaySequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
						//�t���ԍĐ��Ή�
						//���t���ԍĐ��Ŗ߂��̂͌��ݍĐ����̃V�[�P���X�̍ŏ��܂łł��B
						//  ���������V�[�P���X���t���ԍĐ����邱�Ƃŋt�]�̐��E��\���������ꍇ�A
						//  �Ώۂ̃V�[�P���X���t���ɋt�Đ��v���ŃL���[�ɐς�ŏ��Đ����Ă�������
						else if ( m_CurrentPlaySequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
					}
					break;
				}

//====================================================================================================================��

				case ePlayState_Play_Reverse:
				{
					//���݂̎��ԂōĐ�
					m_CurrentPlaySequenceInfo.m_spSequencePtr->Play( m_CurrentPlaySequenceInfo.m_NowTime , m_CurrentPlaySequenceWeights , skyTrue , pPlayedBlendSequenc );

					//�Đ��ς݂̃V�[�P���X�̎Q�Ƃ�Ԃ�
					pPlayedSequenc = m_CurrentPlaySequenceInfo.m_spSequencePtr;

					//�������Ԃ��ŏI���Ԃ�������I���������s��
					if ( m_CurrentPlaySequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MinTime )
					{
						switch ( m_CurrentPlaySequenceInfo.m_EndType )
						{
						case ePlayEndType_End :
						{
							//�u�����h�V�[�P���X���������ꍇ�J�����g�V�[�P���X�Ɉڍs����
							if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait )
							{
								m_CurrentPlaySequenceInfo = m_CurrentPlayBlendSequenceInfo;
								m_CurrentPlaySequenceWeights = 1.0f; //��

								//����Ƀu�����h�����𖞂������ꍇ�u�����h���|�b�v���Ă���
								if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
									&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
									&& m_SequencePlayInfoFifo.GetNum() > 0 
									&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
								{//���̃u�����h�Ώۂ�����
									m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
								}
								else
								{
									m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
								}
							}
							else
							{//�I��
								m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
							}
							break;
						}
						case ePlayEndType_Repetition :

							//�Ōォ��Đ�
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_spSequencePtr->GetTimeRange();
							break;

						case ePlayEndType_Back :

							//���Đ�
							m_CurrentPlaySequenceInfo.m_eState = ePlayState_Play;

							//�ŏ��̃t���[���͍Đ��ς݂Ȃ̂ŁA�ŏ������i�񂾃t���[��������s����
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime + timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

							break;
						}
					}
					else
					{//�܂�����

						//���Ԃ�i�߂�
						m_CurrentPlaySequenceInfo.m_NowTime -= timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

						//���Ԃ��Đ��ő厞�Ԃ𒴂�����Đ��ő厞�Ԃɉ�����
						if ( m_CurrentPlaySequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
						//�t���ԍĐ��Ή�
						else if ( m_CurrentPlaySequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
					}
					break;
				}

//====================================================================================================================��

				case ePlayState_Stop :
					//�Ȃɂ����Ȃ�
					break;
				default:
					SKY_PANIC_MSG( _T( "The state unused this proc." ) );
					break;
			}
		}
		else
		{
			m_CurrentPlaySequenceInfo.m_StartIntvl -= timeOffset;
		}
	}


	return pPlayedSequenc;
}

template < class T >
inline void CKfmSequencePlayer< T >::SetSequencePlayInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	dest.m_spSequencePtr = const_cast< CKfmSequence< T > * >( spSequence );
	dest.m_BlendTime = blendTime;
	dest.m_BlendInterpType = eInterpolateType_Linear;
	dest.m_StartIntvl = startIntvl;
	dest.m_Speed = speed;
	dest.m_NowTime = startSeqTime;
	dest.m_MinTime = startSeqTime;
	dest.m_MaxTime = spSequence->GetTimeRange();
	dest.m_EndType = eEndType;

	dest.m_eState = ePlayState_Play;
	dest.m_IsSequenceDelete = isSequenceDelete;
}

template < class T >
inline void CKfmSequencePlayer< T >::SetSequencePlayReverseInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//�}�C�i�X�P�̏ꍇ�͍ő厞�Ԃŋt�Đ��J�n
	if ( startSeqTime == -1.0f )
	{
		startSeqTime = spSequence->GetTimeRange();
	}

	dest.m_spSequencePtr = spSequence;
	dest.m_BlendTime = blendTime;
	dest.m_BlendInterpType = eInterpolateType_Linear;
	dest.m_StartIntvl = startIntvl;
	dest.m_Speed = speed;
	dest.m_NowTime = startSeqTime;
	dest.m_MinTime = 0.0f;
	dest.m_MaxTime = startSeqTime;
	dest.m_EndType = eEndType;

	dest.m_eState = ePlayState_Play_Reverse;
	dest.m_IsSequenceDelete = isSequenceDelete;
}

template < class T >
inline void CKfmSequencePlayer< T >::SetSpeed( updateTime speed )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_Speed = speed;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetNowTime( updateTime time )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_NowTime = time;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetPlayState( ePlayState state )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_eState = state;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetMiniTime( updateTime time )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_MinTime = time;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetMaxTime( updateTime time )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_MaxTime = time;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetEndType( ePlayEndType eEndType )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_EndType = eEndType;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetSequence( const CKfmSequence< T >* spSequence )
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		m_CurrentPlaySequenceInfo.m_spSequencePtr = spSequence;
	}
}

template < class T >
inline void CKfmSequencePlayer< T >::SetWeights( f32 weights )
{
	m_CurrentPlaySequenceWeights = weights;
}

template < class T >
inline void CKfmSequencePlayer< T >::IsSavePlayedSequence( skyBool isSaveExec )
{
	m_IsSaveExec = isSaveExec;
}

template < class T >
const CKfmSequence< T >* CKfmSequencePlayer< T >::GetCurrentSequence() const
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{//�Đ����̂��̂�����
		return m_CurrentPlaySequenceInfo.m_spSequencePtr;
	}
	else
	{
		return skyNull;
	}
}

template < class T >
updateTime CKfmSequencePlayer< T >::GetCurrentSequenceTime() const
{
	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{//�Đ����̂��̂�����
		return m_CurrentPlaySequenceInfo.m_NowTime;
	}
	else
	{
		return skyNull;
	}
}

template < class T >
inline typename CKfmSequencePlayer< T >::CSequencePlayInfoFifo& CKfmSequencePlayer< T >::GetPlayedSequenceFifo()
{
	return m_SequencePlayedInfoFifo;
}

} } }