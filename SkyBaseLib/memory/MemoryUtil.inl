namespace sky { namespace lib { namespace memory {

inline void *CMemoryUtil::MallocPointer( void *pSrc , u32 srcSize , u32 usedSize , u32 size )
{
        SKY_UNUSED_ARG( size );
        SKY_UNUSED_ARG( srcSize );

	SKY_ASSERT_MSG( srcSize - usedSize >= size , _T( "The range of the memory was exceeded." ) ); //確保しようとした領域が元領域の大きさを超えました。確保できません。

	u8 *pSrcu8 = ( u8 * )pSrc;

	u8* pDest = pSrcu8 + usedSize;

	return ( void * )pDest;
}

inline void *CMemoryUtil::BackMallocPointer( void *pSrc , u32 srcSize , u32 usedSize , u32 size )
{
	SKY_ASSERT_MSG( srcSize - usedSize >= size , _T( "The range of the memory was exceeded." ) );

	u8 *pSrcu8 = ( u8 * )pSrc;

	u8 *pSrcu8Adr = pSrcu8 + srcSize;

	u8* pDest = ( u8* )( pSrcu8Adr - usedSize - size );

	return ( void * )pDest;
}

inline void CMemoryUtil::GetPowTow( u32 value , u32 *pOutValue , u32 *pOutIdx )
{
	s32 i = 0;
	s32 isEnd = skyFalse;
	while( !isEnd )
	{
		*pOutValue = ( u32 )pow( 2.0f , i );
		if ( *pOutValue >= value )
		{
			*pOutIdx = i;
			break;
		}
		i++;
	}
}

//-------------------------------------< 実装 >---------------------------------------------//

#ifdef SKYLIB_PLATFORM_WIN32

inline void *CMemoryUtil::Malloc( u32 size , const skyMString *pFname , s32 line )
{
	pFname;
	line;
//	return MALLOC__( size , DEFAULT_ALIGN , pFname , line );
	return MALLOC__( size , pFname , line );
}

inline void *CMemoryUtil::Realloc( void *p , u32 size , const skyMString *pFname , s32 line )
{
	pFname;
	line;
//	return REALLOC__( p , DEFAULT_ALIGN , size , pFname , line );
	return REALLOC__( p , size , pFname , line );
}

inline void CMemoryUtil::Free( void *p )
{
	FREE__( p );
}

inline void CMemoryUtil::ClearZero( void *pData , u32 size )
{
	::memset( pData , 0 , size );
}

inline void CMemoryUtil::ClearValue( void* pData , u32 value , u32 size )
{
	::memset( pData , value , size );
}

inline void CMemoryUtil::Copy( void *pDestData , const void *pSrcData ,  u32 srcSize )
{
	::memcpy( pDestData , pSrcData , srcSize );
}

inline skyBool CMemoryUtil::Cmp( const void *pData1 , const void *pData2 , u32 size )
{
	return ::memcmp( pData1 , pData2 , size ) == 0;
}

#endif

} } }
