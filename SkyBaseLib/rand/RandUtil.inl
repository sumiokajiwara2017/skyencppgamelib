namespace sky { namespace lib {

namespace RandUtil
{
	inline void SetTimeSeed()
	{
		//時間からシードを設定する
		srand( ( u32 )CTimeUtil_::GetNowTime() );
	}

	inline void SetSeed( u32 seed )
	{
		//擬似乱数の種を設定する
		srand( seed );
	}

	inline u32 Rand( u32 maxValue )
	{
		s32 value = rand()%maxValue;

		//乱数を取得
		return ( u32 )value; //取得する値を0〜maxValueの間とする
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