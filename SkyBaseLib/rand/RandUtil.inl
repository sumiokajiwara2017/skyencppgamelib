namespace sky { namespace lib {

namespace RandUtil
{
	inline void SetTimeSeed()
	{
		//���Ԃ���V�[�h��ݒ肷��
		srand( ( u32 )CTimeUtil_::GetNowTime() );
	}

	inline void SetSeed( u32 seed )
	{
		//�[�������̎��ݒ肷��
		srand( seed );
	}

	inline u32 Rand( u32 maxValue )
	{
		s32 value = rand()%maxValue;

		//�������擾
		return ( u32 )value; //�擾����l��0�`maxValue�̊ԂƂ���
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