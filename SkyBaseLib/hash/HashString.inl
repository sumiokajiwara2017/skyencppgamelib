namespace sky { namespace lib { namespace hash {

inline void CHashString::SetString( const skyString *pKey , skyBool isNoCreateOn )
{
	m_IsNoCreateOn = isNoCreateOn;
	m_HashValue = GET_HASH_VALUE( pKey );

	if ( m_IsNoCreateOn == skyFalse )
	{
		m_String.pStringClass = NEW__( string::CString , string::CString( pKey ) );
		m_Size = m_String.pStringClass->GetSize();
	}
	else
	{
		m_String.pString = pKey;
		m_Size = CStringUtil_::LenByte( m_String.pString ) + sizeof( skyString );
	}
}
inline CHashString::~CHashString()
{
	if ( m_IsNoCreateOn == skyFalse )
	{
		DEL m_String.pStringClass;
	}
}
inline const skyString	*CHashString::GetString() const
{
	if ( m_IsNoCreateOn == skyFalse )
	{
		return m_String.pStringClass->Get();
	}
	else
	{
		return m_String.pString;
	}
}

inline u32 CHashString::GetSize() const
{
	return m_Size;
}

inline skyBool CHashString::operator ==( const skyString *pStr ) const
{
	return m_HashValue == GET_HASH_VALUE( pStr );
}

inline skyBool CHashString::operator ==( const CHashString &pStr ) const
{
	return m_HashValue == pStr.Get();
}

} } }