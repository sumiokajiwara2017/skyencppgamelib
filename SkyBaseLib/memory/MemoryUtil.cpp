#include "StdAfx.h"
#include "SkyBaseLib/Memory.h"

namespace sky { namespace lib { namespace memory {

static u32 s_AllocatorId = 0;

u32 CMemoryUtil::CreateAllocatorId()
{
	return s_AllocatorId++;
}

void CMemoryUtil::ResetAllocatorId()
{
	s_AllocatorId = 0;
}

SkyAllocatorInfo_ g_BaseAllocatorInfo;

void SetBaseAllocatorInfo( const SkyAllocatorInfo_ &info )
{
	g_BaseAllocatorInfo = info;
}

const SkyAllocatorInfo_ &GetBaseAllocatorInfo()
{
	return g_BaseAllocatorInfo;
}

void ClearBaseAllocatorInfo()
{
	g_BaseAllocatorInfo = SkyAllocatorInfo_();
}

} } }