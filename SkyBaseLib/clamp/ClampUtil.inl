namespace sky { namespace lib {

namespace ClampUtil
{

	template< typename T >
	inline skyBool ValueClamp( T *pVal , T addVal , T clampMin, T clampMax , eClampLoopType loopType )
	{
		( *pVal ) += addVal;
		
		if ( ( *pVal ) < clampMin )
		{
			if ( loopType == eClampLoopType_Clamp ) 		( *pVal ) = clampMax - 1;							// 最大値
			else if ( loopType == eClampLoopType_Excess )	( *pVal ) = clampMax - ( clampMin - ( *pVal ) );	// 最大値　-　超過分
			else										( *pVal ) = clampMin;								// 最小値	

			return skyTrue;
		}
		else if ( ( *pVal ) >= clampMax )
		{
			if ( loopType == eClampLoopType_Clamp ) 		( *pVal ) = clampMin;								// 最小値
			else if ( loopType == eClampLoopType_Excess )	( *pVal ) = ( (*pVal) - clampMax ) + clampMin;		// 最小値　+　超過分
			else										( *pVal ) = clampMax - 1;							// 最大値	

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