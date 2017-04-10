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
	//キューに追加
	m_SequencePlayInfoFifo.Push();

	//キューの末尾を取得
	CSequencePlayInfo& playInfo = m_SequencePlayInfoFifo.GetBack();

	//再生情報設定
	SetSequencePlayInfo_( playInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlayReverseSequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//キューに追加
	m_SequencePlayInfoFifo.Push();

	//キューの末尾を取得
	CSequencePlayInfo& playInfo = m_SequencePlayInfoFifo.GetBack();

	//再生情報設定
	SetSequencePlayReverseInfo_( playInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlaySequence( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//キュークリア
	m_SequencePlayInfoFifo.AllPop();

	//ブレンドシーケンスも終了する
	m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;

	//再生情報上書き設定
	SetSequencePlayInfo_( m_CurrentPlaySequenceInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline void CKfmSequencePlayer< T >::PlayReverseSequence( const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//キュークリア
	m_SequencePlayInfoFifo.AllPop();

	//ブレンドシーケンスも終了する
	m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;

	//再生情報上書き設定
	SetSequencePlayReverseInfo_( m_CurrentPlaySequenceInfo , spSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );
}

template < class T >
inline CKfmSequencePlayer< T >* CKfmSequencePlayer< T >::PlayGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//プレイヤーを作成
	CKfmSequencePlayer< T >* spPlayer( NEW CKfmSequencePlayer() );

	//リストに追加
	m_GlobalSequencePlayList.AddTail( spPlayer );

	//再生情報設定
	spPlayer->PlaySequence( spSequence , 0.0f , startIntvl , speed , startSeqTime , eEndType , isSequenceDelete );

	return spPlayer;
}

template < class T >
inline CKfmSequencePlayer< T >* CKfmSequencePlayer< T >::PlayReverseGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete )
{
	//プレイヤーを作成
	CKfmSequencePlayer< T >* spPlayer( NEW CKfmSequencePlayer() );

	//リストに追加
	m_GlobalSequencePlayList.AddTail( spPlayer );

	//再生情報設定
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

//----グローバルシーケンスの再生

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

//----現在のシーケンスが再生終了状態の場合、キューからシーケンスを取得する

	if ( m_CurrentPlaySequenceInfo.m_eState == ePlayState_Wait )
	{
		//キューにデータがあればそれをカレントにして再生
		if ( m_SequencePlayInfoFifo.GetNum() > 0 )
		{
			m_SequencePlayInfoFifo.Pop( m_CurrentPlaySequenceInfo );
		}

		//さらにブレンド条件を満たした場合ブレンドもポップしておく
		if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
			&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
			&& m_SequencePlayInfoFifo.GetNum() > 0 
			&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
		{
			m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
		}
	}

//----現在のウェイトを計算

	//ブレンド相手がいる場合ブレンド比率を計算する
	updateTime timeLeft = 0.0f; 
	if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait
		&&  m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End	)
	{
		//再生残り時間を計算
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

		//残り時間とブレンド時間を比較して残り時間がブレンド時間に到達していたらブレンド時間を計算する
		if ( timeLeft <= m_CurrentPlayBlendSequenceInfo.m_BlendTime )
		{
			switch( m_CurrentPlaySequenceInfo.m_BlendInterpType )
			{
				case eInterpolateType_Linear:
				{
					//ブレンド時間算出
					interp::Linear( m_CurrentPlaySequenceWeights , 0.0f , 1.0f , ( f32 )( timeLeft / m_CurrentPlayBlendSequenceInfo.m_BlendTime ) );

					break;
				}
				default:
					SKY_PANIC_MSG( "This type no support. \n" );
					break;
			}
		}
	}

//----現在再生中のブレンドシーケンスを再生

	if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait && timeLeft <= m_CurrentPlayBlendSequenceInfo.m_BlendTime )
	{
		if( m_CurrentPlayBlendSequenceInfo.m_StartIntvl <= 0.0f )
		{

			switch ( m_CurrentPlayBlendSequenceInfo.m_eState )
			{

//====================================================================================================================▽

				case ePlayState_Play :
				{
					//現在の時間で再生（ブレンド中は値を返さない）
					m_CurrentPlayBlendSequenceInfo.m_spSequencePtr->Play( m_CurrentPlayBlendSequenceInfo.m_NowTime , 1.0f - m_CurrentPlaySequenceWeights , skyFalse );

					//再生済みのブレンドシーケンスを格納
					pPlayedBlendSequenc = m_CurrentPlayBlendSequenceInfo.m_spSequencePtr;

					//もし時間が最終時間だったら終了処理（通常ないと思うけど、ブレンド中に終わってしまうこともあるかも）
					if ( m_CurrentPlayBlendSequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MaxTime )
					{
						//再生終了
						m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
					}
					else
					{//まだ続く

						//時間を進める
						m_CurrentPlayBlendSequenceInfo.m_NowTime += timeOffset * m_CurrentPlayBlendSequenceInfo.m_Speed;

						//時間が再生最大時間を超えたら再生最大時間に下げる（通常ないと思うけどブレンド中に終に到達することもあるかも）
						if ( m_CurrentPlayBlendSequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlayBlendSequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
					}
					break;
				}

//====================================================================================================================▽

				case ePlayState_Play_Reverse:
				{
					//現在の時間で再生
					m_CurrentPlayBlendSequenceInfo.m_spSequencePtr->Play( m_CurrentPlayBlendSequenceInfo.m_NowTime , 1.0f - m_CurrentPlaySequenceWeights );

					//再生済みのブレンドシーケンスを格納
					pPlayedBlendSequenc = m_CurrentPlayBlendSequenceInfo.m_spSequencePtr;

					//もし時間が最終時間だったら終了処理（通常ないと思うけど、ブレンド中に終わってしまうこともあるかも）
					if ( m_CurrentPlayBlendSequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MinTime )
					{
						//再生終了
						m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
					}
					else
					{//まだ続く

						//時間を進める
						m_CurrentPlayBlendSequenceInfo.m_NowTime -= timeOffset * m_CurrentPlayBlendSequenceInfo.m_Speed;

						//時間が再生最大時間を超えたら再生最大時間に下げる（通常ないと思うけどブレンド中に終に到達することもあるかも）
						if ( m_CurrentPlayBlendSequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlayBlendSequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
					}
					break;
				}

//====================================================================================================================▽

				case ePlayState_Stop :
					//なにもしない
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

//----現在再生中のシーケンスを再生

	if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait )
	{
		if( m_CurrentPlaySequenceInfo.m_StartIntvl <= 0.0f )
		{
			switch ( m_CurrentPlaySequenceInfo.m_eState )
			{

//====================================================================================================================▽

				case ePlayState_Play :
				{
					//現在の時間で再生
					m_CurrentPlaySequenceInfo.m_spSequencePtr->Play( m_CurrentPlaySequenceInfo.m_NowTime , m_CurrentPlaySequenceWeights , skyTrue , pPlayedBlendSequenc );

					//再生済みのシーケンスの参照を返す
					pPlayedSequenc = m_CurrentPlaySequenceInfo.m_spSequencePtr;

					//もし時間が最終時間だったら終了処理を行う
					if ( m_CurrentPlaySequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MaxTime )
					{
						switch ( m_CurrentPlaySequenceInfo.m_EndType )
						{
						case ePlayEndType_End :
						{
							if ( m_IsSaveExec == skyTrue )
							{
								//キューに追加
								m_SequencePlayedInfoFifo.Push();

								//キューの末尾を取得
								CSequencePlayInfo& playInfo = m_SequencePlayedInfoFifo.GetBack();

								//保存
								playInfo = m_CurrentPlaySequenceInfo;
							}

							//ブレンドシーケンスがあった場合カレントシーケンスに移行する
							if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait )
							{
								m_CurrentPlaySequenceInfo = m_CurrentPlayBlendSequenceInfo;
								m_CurrentPlaySequenceWeights = 1.0f; //回復

								//さらにブレンド条件を満たした場合ブレンドもポップしておく
								if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
									&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
									&& m_SequencePlayInfoFifo.GetNum() > 0 
									&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
								{//次のブレンド対象が来た
									m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
								}
								else
								{
									m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
								}
							}
							else
							{//終了
								m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
							}
							break;
						}
						case ePlayEndType_Repetition :

							//最初から再生
							m_CurrentPlaySequenceInfo.m_NowTime = 0.0f;

							break;
						case ePlayEndType_Back :

							//逆再生
							m_CurrentPlaySequenceInfo.m_eState = ePlayState_Play_Reverse;

							//最終フレームは再生済みなので、最終から一つ手前のフレームから遡る
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime - timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

							break;
						}
					}
					else
					{//まだ続く

						//時間を進める
						m_CurrentPlaySequenceInfo.m_NowTime += timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

						//時間が再生最大時間を超えたら再生最大時間に下げる
						if ( m_CurrentPlaySequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
						//逆時間再生対応
						//※逆時間再生で戻れるのは現在再生中のシーケンスの最初までです。
						//  もし複数シーケンスを逆時間再生することで逆転の世界を表現したい場合、
						//  対象のシーケンスを逆順に逆再生要求でキューに積んで順再生してください
						else if ( m_CurrentPlaySequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
					}
					break;
				}

//====================================================================================================================▽

				case ePlayState_Play_Reverse:
				{
					//現在の時間で再生
					m_CurrentPlaySequenceInfo.m_spSequencePtr->Play( m_CurrentPlaySequenceInfo.m_NowTime , m_CurrentPlaySequenceWeights , skyTrue , pPlayedBlendSequenc );

					//再生済みのシーケンスの参照を返す
					pPlayedSequenc = m_CurrentPlaySequenceInfo.m_spSequencePtr;

					//もし時間が最終時間だったら終了処理を行う
					if ( m_CurrentPlaySequenceInfo.m_NowTime == m_CurrentPlaySequenceInfo.m_MinTime )
					{
						switch ( m_CurrentPlaySequenceInfo.m_EndType )
						{
						case ePlayEndType_End :
						{
							//ブレンドシーケンスがあった場合カレントシーケンスに移行する
							if ( m_CurrentPlayBlendSequenceInfo.m_eState != ePlayState_Wait )
							{
								m_CurrentPlaySequenceInfo = m_CurrentPlayBlendSequenceInfo;
								m_CurrentPlaySequenceWeights = 1.0f; //回復

								//さらにブレンド条件を満たした場合ブレンドもポップしておく
								if ( m_CurrentPlaySequenceInfo.m_eState != ePlayState_Wait 
									&& m_CurrentPlaySequenceInfo.m_EndType == ePlayEndType_End
									&& m_SequencePlayInfoFifo.GetNum() > 0 
									&& m_SequencePlayInfoFifo.GetFront().m_BlendTime > 0.0f )
								{//次のブレンド対象が来た
									m_SequencePlayInfoFifo.Pop( m_CurrentPlayBlendSequenceInfo );
								}
								else
								{
									m_CurrentPlayBlendSequenceInfo.m_eState = ePlayState_Wait;
								}
							}
							else
							{//終了
								m_CurrentPlaySequenceInfo.m_eState = ePlayState_Wait;
							}
							break;
						}
						case ePlayEndType_Repetition :

							//最後から再生
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_spSequencePtr->GetTimeRange();
							break;

						case ePlayEndType_Back :

							//順再生
							m_CurrentPlaySequenceInfo.m_eState = ePlayState_Play;

							//最初のフレームは再生済みなので、最初から一つ進んだフレームから実行する
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime + timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

							break;
						}
					}
					else
					{//まだ続く

						//時間を進める
						m_CurrentPlaySequenceInfo.m_NowTime -= timeOffset * m_CurrentPlaySequenceInfo.m_Speed;

						//時間が再生最大時間を超えたら再生最大時間に下げる
						if ( m_CurrentPlaySequenceInfo.m_NowTime < m_CurrentPlaySequenceInfo.m_MinTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MinTime;
						}
						//逆時間再生対応
						else if ( m_CurrentPlaySequenceInfo.m_NowTime > m_CurrentPlaySequenceInfo.m_MaxTime )
						{
							m_CurrentPlaySequenceInfo.m_NowTime = m_CurrentPlaySequenceInfo.m_MaxTime;
						}
					}
					break;
				}

//====================================================================================================================▽

				case ePlayState_Stop :
					//なにもしない
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
	//マイナス１の場合は最大時間で逆再生開始
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
	{//再生中のものがある
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
	{//再生中のものがある
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