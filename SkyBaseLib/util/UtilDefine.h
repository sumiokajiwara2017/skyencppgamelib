#pragma once

#ifdef SW_SKYLIB_HASH_VALUE_CRC
#define GET_HASH_VALUE( str )		sky::lib::util::Crc::Create( str )
typedef u32                         hashtype;
#define GET_HASH_VALUE_SIZE()		sizeof( hashtype )
#endif
#ifdef SW_SKYLIB_HASH_VALUE_MUMU
#define GET_HASH_VALUE( str )		sky::lib::util::MurmurHash::Create( str , CStringUtil_::LenByte( str ) , 1 )
typedef u32                         hashtype;
#define GET_HASH_VALUE_SIZE()		sizeof( hashtype )
#endif