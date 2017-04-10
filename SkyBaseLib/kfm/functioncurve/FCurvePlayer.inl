namespace sky { namespace lib { namespace kfm {

template < class T >
inline void CFCurvePlayer< T >::PlayFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	m_SeqPlayer.PlaySequenceRequest( pSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline void CFCurvePlayer< T >::PlayReverseFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	m_SeqPlayer.PlayReverseSequenceRequest( pSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline void CFCurvePlayer< T >::PlayFCurve( const CFCurve< T >* pFCurve , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	m_SeqPlayer.PlaySequence( pSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline void CFCurvePlayer< T >::PlayReverseFCurve( const CFCurve< T >* pFCurve , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	m_SeqPlayer.PlayReverseSequence( pSequence , blendTime , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline CKfmSequencePlayer< T >* CFCurvePlayer< T >::PlayGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	return m_SeqPlayer.PlayGlobalSequence( pSequence , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline CKfmSequencePlayer< T >* CFCurvePlayer< T >::PlayReverseGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType )
{
	CKfmSequence< T > *pSequence = NEW kfm::CKfmSequence< T >;
	pSequence->AddFCurve( 0 , pFCurve );

	return m_SeqPlayer.PlayReverseGlobalSequence( pSequence , startIntvl , speed , startSeqTime , eEndType , skyTrue ); 
}

template < class T >
inline void CFCurvePlayer< T >::StopCurrentFCurve()
{
	m_SeqPlayer.StopCurrentSequence();
}

template < class T >
inline void CFCurvePlayer< T >::StopGlobalFCurve()
{
	m_SeqPlayer.StopGlobalSequence();
}

template < class T >
inline void CFCurvePlayer< T >::StopFCurveAll()
{
	m_SeqPlayer.StopSequenceAll();
}

template < class T >
inline void CFCurvePlayer< T >::NextCurrentFCurve()
{
	m_SeqPlayer.NextCurrentSequence();
}

template < class T >
inline void CFCurvePlayer< T >::NextGlobalFCurve()
{
	m_SeqPlayer.NextGlobalSequence();
}

template < class T >
inline void CFCurvePlayer< T >::NextAllFCurve()
{
	m_SeqPlayer.NextAllSequence();
}

template < class T >
inline void CFCurvePlayer< T >::CancelCurrentFCurve()
{
	m_SeqPlayer.CancelCurrentSequence();
}

template < class T >
inline void CFCurvePlayer< T >::CancelGlobalFCurve()
{
	m_SeqPlayer.CancelGlobalSequence();
}

template < class T >
inline void CFCurvePlayer< T >::CancelFCurveAll()
{
	m_SeqPlayer.CancelSequenceAll();
}

template < class T >
inline void CFCurvePlayer< T >::ReStartCurrentFCurve()
{
	m_SeqPlayer.ReStartCurrentSequence();
}

template < class T >
inline void CFCurvePlayer< T >::ReStartGlobalFCurve()
{
	m_SeqPlayer.ReStartGlobalSequence();
}

template < class T >
inline void CFCurvePlayer< T >::ReStartFCurveAll()
{
	m_SeqPlayer.ReStartSequenceAll();
}

template < class T >
inline const CKfmSequence< T >* CFCurvePlayer< T >::Play( updateTime timeOffset )
{
	const CKfmSequence< T >* pPlayedSequenc = m_SeqPlayer.Play( timeOffset );

	//再生中のシーケンスがあったらその
	if ( pPlayedSequenc != skyNull )
	{
		updateTime time                   = m_SeqPlayer.GetCurrentSequenceTime();
		f32        weight                 = 1.0f;
		m_ColValueEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )&time , ( memSize )&pPlayedSequenc->GetFCurve( 0 )->GetTimeValue() , ( memSize )&weight ) );
	}

	return pPlayedSequenc;
}

template < class T >
inline void CFCurvePlayer< T >::SetSpeed( updateTime speed )
{
	m_SeqPlayer.SetSpeed( speed );
}

template < class T >
inline void CFCurvePlayer< T >::SetNowTime( updateTime time )
{
	m_SeqPlayer.SetNowTime( time );
}

template < class T >
inline void CFCurvePlayer< T >::SetPlayState( ePlayState state )
{
	m_SeqPlayer.SetPlayState( state );
}

template < class T >
inline void CFCurvePlayer< T >::SetMiniTime( updateTime time )
{
	m_SeqPlayer.SetMiniTime( time );
}

template < class T >
inline void CFCurvePlayer< T >::SetMaxTime( updateTime time )
{
	m_SeqPlayer.SetMaxTime( time );
}

template < class T >
inline void CFCurvePlayer< T >::SetEndType( ePlayEndType eEndType )
{
	m_SeqPlayer.SetEndType( eEndType );
}

template < class T >
inline void CFCurvePlayer< T >::SetWeights( f32 weights )
{
	m_SeqPlayer.SetWeights( weights );
}

template < class T >
inline delegate::CDelegateHandler &CFCurvePlayer< T >::GetColValueEventHndl()
{
	return m_ColValueEventHndl;
}

} } }