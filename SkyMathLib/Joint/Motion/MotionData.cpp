#include "StdAfx.h"
#include "SkyMathLib/Joint.h"

SkyImplementRTTI( sky::lib::math::CMotionData , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::math::CMotionDataContena , sky::lib::base::SkyRefObject );

namespace sky { namespace lib { namespace math {

CMotionData::~CMotionData()
{
}

CMotionData::CMotionData()
{
}

void CMotionData::SetAnimationPallet( const CAnimationDataPallet &pallet )
{
	m_AnimationDataPallet.Clear();
	for( u32 i = 0 ; i < pallet.GetNum() ; i++ )
	{
		m_AnimationDataPallet.Add( pallet[ i ] );
	}
}

void CMotionData::SetMaxTime( f32 maxTime )
{
	m_MaxTime = maxTime;
}

f32 CMotionData::GetMaxTime() const
{
	return m_MaxTime;
}

u32 CMotionData::GetAnimationNum() const
{
	return m_AnimationDataPallet.GetNum();
}

const CAnimationData &CMotionData::GetAnimationData( u32 index ) const
{
	return m_AnimationDataPallet[ index ];
}

skyBool CMotionData::SaveBinary_( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}
			
skyBool CMotionData::LoadBinary_( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

//-------------------------------------------------------------------------------------------------------

#define ANIMATION_DATA_DEFAULT_NUM ( 64 )

void CMotionDataContena::AddAnimation( const CMotionDataPtr &spAnimation )
{
	m_AnimationPallet.Add( spAnimation );
	m_AnimationHash.Set( spAnimation->Name.m_Name , spAnimation ); //ñºëOåüçıópHash
}

u32 CMotionDataContena::GetAnimationNum()
{
	return m_AnimationPallet.GetNum();
}

CMotionDataPtr CMotionDataContena::GetAnimation( u32 index )
{
	SKY_ASSERT( index < m_AnimationPallet.GetNum() );

	return m_AnimationPallet[ index ];
}

CMotionDataPtr CMotionDataContena::GetAnimation( const hash::CHashStringPtr &name )
{
	return m_AnimationHash[ name ];
}

CMotionDataContena::CMotionDataContena() : m_AnimationHash( ANIMATION_DATA_DEFAULT_NUM )
{
}

} } }