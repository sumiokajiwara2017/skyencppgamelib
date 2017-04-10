namespace sky { namespace lib {  namespace bit {

namespace BinaryUtil
{

#define _U8_BIT_SIZE ( 8 ) 
void SetBitDataOn( u8 *pData , u32 idx , u32 bitSize )
{
	SKY_UNUSED_ARG( bitSize );
	SKY_ASSERT_MSG( bitSize > idx , _T( "It is beyond the limits of data." ) ); //bitを立てようとした位置はデータの範囲外です。

	//bit位置とbitインデックスを取得する
	u32 pos = idx % _U8_BIT_SIZE;
	u32 widx = idx / _U8_BIT_SIZE;
	
	u8 value = pData[ idx ];
	value = ( u8 )( value | ( 0x1 << ( _U8_BIT_SIZE - 1 - pos ) ) );
	
	//指定のbitをOnにする
	pData[ widx ] = value;
}

void SetBitDataOff( u8 *pData , u32 idx , u32 bitSize )
{
	SKY_UNUSED_ARG( bitSize );
	SKY_ASSERT_MSG( bitSize > idx , _T( "It is beyond the limits of data." ) ); //bitを寝かせようとした位置はデータの範囲外です。

	//bit位置とbitインデックスを取得する
	u32 pos = idx % _U8_BIT_SIZE;
	u32 widx = idx / _U8_BIT_SIZE;
	
	u8 mask = 0x0;
	switch( pos )
	{
		case 0:	mask = 0x7F; break;	//01111111
		case 1:	mask = 0xBF; break;	//10111111
		case 2:	mask = 0xDF; break;	//11011111
		case 3:	mask = 0xEF; break;	//11101111
		case 4:	mask = 0xF7; break;	//11110111
		case 5:	mask = 0xFB; break;	//11111011
		case 6:	mask = 0xFD; break;	//11111101
		case 7:	mask = 0xFE; break;	//11111110
	} 
	u8 value = ( u8 )( pData[ widx ] & mask);
	
	//指定のbitをOffにする
	pData[ widx ] = value;
}

skyBool IsBitDataOn( u8 *pData , u32 idx , u32 bitSize )
{
	SKY_UNUSED_ARG( bitSize );
	SKY_ASSERT_MSG( bitSize > idx , _T( "It is beyond the limits of data." ) ); //チェックをしようとした位置はデータの範囲外です。

	//bit位置とbitインデックスを取得する
	u32 pos = idx % _U8_BIT_SIZE;
	u32 widx = idx / _U8_BIT_SIZE;
	u32 ret = ( u32 )( ( pData[ widx ] >> pos ) & 0x1 );
	return ( skyBool )ret;
}

inline u8 CountBit( u32 data )
{
	u8 c = 0;

	while ( data )
	{
		++c;
		data &= ( data - 1 );
	}

	return c;
}


}

} } }