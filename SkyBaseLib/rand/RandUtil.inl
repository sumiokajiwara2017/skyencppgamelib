namespace sky { namespace lib {

namespace RandUtil
{
	inline void SetTimeSeed()
	{
		//ŠÔ‚©‚çƒV[ƒh‚ğİ’è‚·‚é
		srand( ( u32 )CTimeUtil_::GetNowTime() );
	}

	inline void SetSeed( u32 seed )
	{
		//‹[——”‚Ìí‚ğİ’è‚·‚é
		srand( seed );
	}

	inline u32 Rand( u32 maxValue )
	{
		s32 value = rand()%maxValue;

		//—”‚ğæ“¾
		return ( u32 )value; //æ“¾‚·‚é’l‚ğ0`maxValue‚ÌŠÔ‚Æ‚·‚é
	}

	inline d64 Rand()
	{ 
		return ( ( rand() )/ ( RAND_MAX + 1.0 ) );
	}

	inline d64 RandInRange( d64 x, d64 y )
	{
		return x + Rand() * ( y - x );
	}

	inline skyBool RandBool()
	{
		if ( Rand() > 0.5 ) return skyTrue;
		else return skyFalse;
	}

	inline d64 RandomClamped()
	{
		return Rand() - Rand();
	}
}

} }