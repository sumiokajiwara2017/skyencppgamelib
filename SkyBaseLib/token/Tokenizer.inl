namespace sky { namespace lib { namespace token {

template< class T >
inline CTokenizer< T >::CTokenizer( u32 tokenSize , u32 separatorSize , u32 cutoffSize )
{
	m_TokenSize     = tokenSize;
	m_SeparatorSize = separatorSize;
	m_CutoffSize    = cutoffSize;

	m_pBuffer = NULL;
	try 
	{ 
		m_pToken         = NEW T [ tokenSize ];
		m_pTokenUseCheck = NEW T [ tokenSize ];
		m_pSeparator     = NEW T [ separatorSize ]; 
		m_pCutOff        = NEW T [ cutoffSize ];
	} 
	catch( std::bad_alloc ba )
	{
		SKY_PANIC(); //MemoryError;
	}
}

template< class T >
inline CTokenizer< T >::~CTokenizer()
{
	DEL m_pCutOff;
	DEL m_pSeparator;
	DEL m_pToken;
	DEL m_pTokenUseCheck;
}

template< class T >
inline void CTokenizer< T >::SetSeparator( const T *separator )
{
	string::CStringUtil::Copy( m_pSeparator , m_SeparatorSize , separator );
}

template< class T >
inline void CTokenizer< T >::SetCutOff( const T *cutoff )
{
	string::CStringUtil::Copy( m_pCutOff , m_CutoffSize , cutoff );
}

template< class T >
inline void CTokenizer< T >::SetBuffer( T *buffer )
{
	m_pBuffer = buffer;
}

template< class T >
inline void CTokenizer< T >::Release()
{
	m_Separator.Clear();
	m_CutOff.Clear();
}

template<>
inline void CTokenizer< skyMString >::Next()
{
	skyMString *p = m_pBuffer;
	skyMString *q = m_pToken;

	// 区切り文字はスキップする
	while ( ( *p ) != '\0' && string::CStringUtil::Strchr( m_pSeparator , ( *p ) ) )
	{
		p++;
	}

	// 切り出し文字とヒットするか判定
	if ( string::CStringUtil::Strchr( m_pCutOff , ( *p ) ) )
	{
		//切り出し文字とヒットしたら，単体トークンとする
		( * ( q++ ) ) = ( * ( p++ ) );
	}
	else
	{
		//区切り文字または切り出し文字以外ならトークンとする
		std::string split =  m_pSeparator;
		split += m_pCutOff;
		while( ( *p ) != '\0' && !string::CStringUtil::Strchr( split.c_str(), ( *p ) ) )
		{
			( * ( q++ ) ) = ( * ( p++ ) );
		}
	}

	//抜き出した分だけバッファを進める
	m_pBuffer = p;

	//文字列として返すためにNULL終端文字を加える
	( *q ) = '\0';
}

template<>
inline void CTokenizer< skyWString >::Next()
{
	skyWString *p = m_pBuffer;
	skyWString *q = m_pToken;

	// 区切り文字はスキップする
	while ( ( *p ) != L'\0' && string::CStringUtil::Strchr( m_pSeparator , ( *p ) ) )
	{
		p++;
	}

	// 切り出し文字とヒットするか判定
	if ( string::CStringUtil::Strchr( m_pCutOff , ( *p ) ) )
	{
		//切り出し文字とヒットしたら，単体トークンとする
		( * ( q++ ) ) = ( * ( p++ ) );
	}
	else
	{
		//区切り文字または切り出し文字以外ならトークンとする
		string::CString split =  m_pSeparator;
		split += m_pCutOff;
		while( ( *p ) != L'\0' && !string::CStringUtil::Strchr( ( const skyString * )split , ( *p ) ) )
		{
			( * ( q++ ) ) = ( * ( p++ ) );
		}
	}

	//抜き出した分だけバッファを進める
	m_pBuffer = p;

	//文字列として返すためにNULL終端文字を加える
	( *q ) = L'\0';
}

template< class T >
inline skyBool CTokenizer< T >::Check( const T *pToken )
{
	return string::CStringUtil::Comp( m_pToken , pToken );
}

template< class T >
inline skyBool CTokenizer< T >::CheckNext( const T *pToken )
{
	skyBool result;

	//保存
	skyMString *savePointer = m_pBuffer;

	string::CStringUtil::Copy( m_pTokenUseCheck , m_TokenSize , m_pToken );

	Next();
	result = string::CStringUtil::Comp( m_pToken , pToken );

	m_pBuffer = savePointer;
	string::CStringUtil::Copy( m_pToken , m_TokenSize , m_pTokenUseCheck );

	return result;
}

template< class T >
inline skyBool CTokenizer< T >::CheckNowAndNext( const T *pToken )
{
	skyBool result;

	result = Check( pToken );
	result = result || CheckNext( pToken );

	return result;
}

template< class T >
inline skyBool CTokenizer< T >::IsEnd()
{
	skyBool result = skyFalse;
	if ( ( *m_pBuffer ) == '\0' || m_pBuffer == NULL )
	{
		result = skyTrue;
	}
	return result;
}

template< class T >
inline T* CTokenizer< T >::GetChar()
{
	return m_pToken;
}

template< class T >
inline d64 CTokenizer< T >::GetDouble()
{
	return string::CStringUtil::ConvStringTod64( m_pToken );
}

template< class T >
inline f32 CTokenizer< T >::GetFloat()
{
	return string::CStringUtil::ConvStringTof32( m_pToken );
}

template< class T >
inline s32 CTokenizer< T >::GetInt()
{
	return string::CStringUtil::ConvStringTos32( m_pToken );
}

template< class T >
inline T*  CTokenizer< T >::GetNextChar()
{
	Next();
	return GetChar();
}

template< class T >
inline d64 CTokenizer< T >::GetNextDouble()
{
	Next();
	return GetDouble();
}

template< class T >
inline f32 CTokenizer< T >::GetNextFloat()
{
	Next();
	return GetFloat();
}

template< class T >
inline s32 CTokenizer< T >::GetNextInt()
{
	Next();
	return GetInt();
}

} } }