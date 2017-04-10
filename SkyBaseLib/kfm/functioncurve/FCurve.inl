namespace sky { namespace lib { namespace kfm {

inline CFCurveBase::CFCurveBase() : m_TimeRange( 0.0f )
{
}

inline CFCurveBase::~CFCurveBase()
{
}

inline updateTime CFCurveBase::GetTimeRange() const
{
	return m_TimeRange;
}

inline delegate::CDelegateHandler &CFCurveBase::GetColValueEventHndl()
{
	return m_ColValueEventHndl;
}

template < class T >
void CFCurve< T >::SetInitValue( T value )
{
	m_InitValue = value;
	m_TimeValue = m_InitValue;
}

template < class T >
T&   CFCurve< T >::GetInitValue()
{
	return m_InitValue;
}

template < class T >
void CFCurve< T >::AddKey( updateTime time , T keyValue , eInterpolateType eType )
{
	//�L�[�쐬
	kfm::CFKey< T >* pKey = NEW kfm::CFKey< T >( keyValue );

	//�L�[�ɕK�v�ȏ����i�[
	pKey->SetTime( time );
	pKey->SetInterpType( eType );

	//�L�[�z��Ɋi�[
	m_KeyArray.Add( pKey );

	//�z����\�[�g���Ă���
	std::sort< CFKey< T >** >( m_KeyArray.GetTopAdr() , m_KeyArray.GetBottomAdr() , SortPred() );

	//�ő厞�Ԃ��v�Z
	m_TimeRange = 0;
	for ( u32 i = 0 ; i < m_KeyArray.GetNum() ; i++ )
	{
		m_TimeRange += m_KeyArray[ i ]->GetTime();
	}
}

template < class T >
inline CFCurve< T >::~CFCurve()
{
	ClearKey();
}

template < class T >
void CFCurve< T >::DeleteKey( CFKey< T >* &pKey )
{
	if ( m_KeyArray.Delete( pKey ) == skyTrue )
	{
		m_TimeRange -= pKey->GetTime();
	}
}

template < class T >
skyBool CFCurve< T >::GetKey( u32 index , CFKey< T >* &pDest )
{
	return m_KeyArray.Get( index , pDest );
}

template < class T >
CFKey< T > *CFCurve< T >::DeleteKey( u32 index )
{
	CFKey< T > *pKey = skyNull;
	if ( m_KeyArray.Get( index , pKey ) )
	{
		if ( m_KeyArray.DeleteIdx( index ) == skyTrue )
		{
			m_TimeRange -= pKey->GetTime();
		}
	}

	return pKey;
}

template < class T >
skyBool CFCurve< T >::ColTimeValue( updateTime time , f32 weight , skyBool isCallBackValue , const CFCurve< T > *pBlendFCurve )
{//�������̏������Ȃ�ׂ��y���������B

	//�L�[���Ȃ���ΏI���
	//�ő厞�Ԃ��o�߂��Ă��܂��Ă�����I���
	if (  m_KeyArray.GetNum() == 0 || m_TimeRange < time )
	{
		return skyFalse;
	}

	//�Y���̎��ԂɑΉ�����ŏ��L�[���擾����
	updateTime range       = 0;
	updateTime nowTime     = 0;
	eInterpolateType eType = eInterpolateType_None;
	T* pStart              = &m_InitValue;
	T* pEnd                = skyNull;
	updateTime addTime     = 0;
	skyBool isFound        = skyFalse;
	for( u32 i = 0 ; i < m_KeyArray.GetNum() ; i++ )
	{
		addTime += m_KeyArray[ i ]->GetTime();

		if( addTime >= time )
		{//���B�\��t���[���̔���

			range     = m_KeyArray[ i ]->GetTime();
			nowTime   = time - ( addTime - m_KeyArray[ i ]->GetTime() );
			eType     = m_KeyArray[ i ]->GetInterpType();
			pEnd      = &( *m_KeyArray[ i ] );
			isFound   = skyTrue;
			break;
		}
		else
		{
			pStart = &( *m_KeyArray[ i ] );
		}
	}

	if( isFound == skyTrue )
	{
		switch ( eType )
		{
			case eInterpolateType_Linear:
			{
				//���`���
				interp::Linear< T >( m_TimeValue , *pStart , *pEnd , ( f32 )( nowTime / range ) );
				break;
			}
			case eInterpolateType_None:
			{
				//��Ԗ���
				interp::None< T >( m_TimeValue , *pStart , *pEnd , ( f32 )( nowTime / range ) );
				break;
			}
			case eInterpolateType_Acceleration:
			{
				//�������
				interp::Acceleration< T >( m_TimeValue , *pStart , *pEnd , ( f32 )( nowTime / range ) );
				break;
			}
			case eInterpolateType_Deceleration:
			{
				//�������
				interp::Deceleration< T >( m_TimeValue , *pStart , *pEnd , ( f32 )( nowTime / range ) );
				break;
			}
			default:
			{
				isFound = skyFalse;
				break;
			}
		}

		if ( isFound == skyTrue )
		{
			m_TimeValue = ( T )( m_TimeValue * weight ); //�e���x��������
		}

		if ( pBlendFCurve != skyNull )
		{//�u�����h
			m_TimeValue = m_TimeValue + pBlendFCurve->GetTimeValue();
		}

		if ( isCallBackValue == skyTrue )
		{
			//�C�x���g����
			m_ColValueEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )&time , ( memSize )&m_TimeValue , ( memSize )&weight ) );
		}
	}

	return isFound;
}

template < class T >
void CFCurve< T >::ClearKey()
{
	FOREACH( CKeyVector , it , m_KeyArray )
	{
		DEL ( *it );
	}
	m_KeyArray.Reset( m_KeyArray.GetNum() );
	m_TimeRange = 0;
}

template < class T >
const T &CFCurve< T >::GetTimeValue() const
{
	return m_TimeValue;
}

template < class T >
void CFCurve< T >::Optimization()
{

}

template < class T >
typename CFCurve< T >::CKeyVector &CFCurve< T >::GetKeyArray()
{
	return m_KeyArray;
}

} } }