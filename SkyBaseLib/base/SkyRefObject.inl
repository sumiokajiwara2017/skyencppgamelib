namespace sky { namespace lib { namespace base {

inline SkyRefObject::SkyRefObject() :
m_nRefCnt( 0 )
{
}

inline SkyRefObject::~SkyRefObject()
{
}

inline u32  SkyRefObject::GetRefCnt() const
{
	return m_nRefCnt;
}

inline void SkyRefObject::SetRefCnt( u32 cnt )
{
	m_nRefCnt = cnt;
}

inline u32 SkyRefObject::RefCntUp()
{
	m_nRefCnt++;

	return m_nRefCnt;
}

inline skyBool SkyRefObject::RefCntDown()
{
	m_nRefCnt--;

	return m_nRefCnt == 0;
}

} } }