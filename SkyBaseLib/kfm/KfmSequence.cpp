#include "StdAfx.h"
#include "SkyBaseLib/Kfm.h"

namespace sky { namespace lib { namespace kfm {

updateTime CKfmSequenceBase::GetTimeRange() const
{
	return m_TimeRange;
}

CKfmSequenceBase::CKfmSequenceBase() : m_TimeRange( 0 ) , ID( INT_MAX )
{
}

} } }