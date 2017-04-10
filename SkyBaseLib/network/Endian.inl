namespace sky { namespace lib { namespace net {

namespace EndianUtil
{

inline u32 DataConvert_HostToNetWork( u32 data )
{
	return htonl( data );
}

inline u16 DataConvert_HostToNetWork( u16 data )
{
	return htons( data );
}

inline u32 DataConvert_NetWorkToHost( u32 data )
{
	ntohl( data );
}

inline u16 DataConvert_NetWorkToHost( u16 data )
{
	ntohs( data );
}

}

} } }