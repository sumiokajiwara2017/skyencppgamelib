#include "StdAfx.h"
#include "SkyBaseLib/Hash.h"
#include "SkyBaseLib/SKYTL.h"

namespace sky { namespace lib { namespace hash {

	void CHashStringUtil::GeTokenList( CHashStringPtrList &dest , const skyString *pSrc , const skyString key )
	{
        SKY_ASSERT_MSG( pSrc != skyNull , _T( "Please specify the character string." ) ); //文字列を指定してください

		u32 len = CStringUtil_::LenByte( pSrc );

		if ( len == 0 )
		{
			return;
		}

        string::CStringBuffer buf = _T( "" );
		for ( u32 i = 0 ; i < len ; i++ )
		{
			if ( pSrc[ i ] == key )
			{
                if ( buf != _T( "" ) )
				{//トークンをリストに追加
					dest.AddTail( CHS( buf.Get() ) );
                    buf = _T( "" );
				}
			}
			else
			{
				buf += pSrc[ i ];
			}
		}
	}


} } }
