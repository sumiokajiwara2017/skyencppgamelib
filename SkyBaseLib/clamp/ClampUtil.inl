namespace sky { namespace lib {

namespace ClampUtil
{

	template< typename T >
	inline skyBool ValueClamp( T *pVal , T addVal , T clampMin, T clampMax , eClampLoopType loopType )
	{
		( *pVal ) += addVal;
		
		if ( ( *pVal ) < clampMin )
		{
			if ( loopType == eClampLoopType_Clamp ) 		( *pVal ) = clampMax - 1;							// �ő�l
			else if ( loopType == eClampLoopType_Excess )	( *pVal ) = clampMax - ( clampMin - ( *pVal ) );	// �ő�l�@-�@���ߕ�
			else										( *pVal ) = clampMin;								// �ŏ��l	

			return skyTrue;
		}
		else if ( ( *pVal ) >= clampMax )
		{
			if ( loopType == eClampLoopType_Clamp ) 		( *pVal ) = clampMin;								// �ŏ��l
			else if ( loopType == eClampLoopType_Excess )	( *pVal ) = ( (*pVal) - clampMax ) + clampMin;		// �ŏ��l�@+�@���ߕ�
			else										( *pVal ) = clampMax - 1;							// �ő�l	

			return skyTrue;
		}
		return skyFalse;
	}

	template< typename T >
	inline void BtweenValueMove( T *pVal , T addVal , skyBool *pIsToUp , T clampMin, T clampMax )
	{
		if ( *pIsToUp == TRUE )
		{
			if ( *pVal >= clampMax )
			{
				*pVal = clampMax;
				*pIsToUp = skyFalse;
			}
			else
			{
				*pVal += addVal;
			}
		}
		else
		{
			if ( *pVal <= clampMin )
			{
				*pVal = clampMin;
				*pIsToUp = skyTrue;
			}
			else
			{
				*pVal -= addVal;
			}
		}
	}
}

} }