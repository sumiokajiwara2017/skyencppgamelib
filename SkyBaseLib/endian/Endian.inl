union CheckEndian_
{
	u32 bits32;
	u8  bytes[4];
};

inline u8	EndianUtil::GetEndian()
{
	CheckEndian_ val;
	val.bytes[ 0 ] = 0;
	val.bytes[ 1 ] = 1;
	val.bytes[ 2 ] = 0;
	val.bytes[ 3 ] = 0;
	return ( val.bits32 == 256 ) ? 0 : 1;
}

inline void _Swap( u8 *pcA, u8 *pcB )
{
    skyMString acValue = pcA[ 0 ];
    pcA[ 0 ] = pcB[ 0 ];
    pcB[ 0 ] = acValue;
}

inline void EndianUtil::Swap16( u8 *pSrcDest , u32 num )
{
    for ( u32 i = 0 ; i < num ; i++ )
    {
        _Swap( &pSrcDest[ 0 ] , &pSrcDest[ 1 ] );
        pSrcDest += 2;
    }
}

inline void EndianUtil::Swap32( u8 *pSrcDest , u32 num )
{
    for (u32 i = 0; i < num; i++ )
    {
        _Swap( &pSrcDest[ 0 ], &pSrcDest[ 3 ] );
        _Swap( &pSrcDest[ 1 ], &pSrcDest[ 2 ] );
        pSrcDest += 4; 
    }
}

inline void EndianUtil::Swap64( u8 *pSrcDest , u32 num )
{
    for (u32 i = 0; i < num; i++)
    {
        _Swap( &pSrcDest[ 0 ] , &pSrcDest[ 7 ] );
        _Swap( &pSrcDest[ 1 ] , &pSrcDest[ 6 ] );
        _Swap( &pSrcDest[ 2 ] , &pSrcDest[ 5 ] );
        _Swap( &pSrcDest[ 3 ] , &pSrcDest[ 4 ] );
        pSrcDest += 8;
    }
}
