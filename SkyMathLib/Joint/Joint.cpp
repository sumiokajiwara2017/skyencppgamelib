#include "StdAfx.h"
#include "SkyMathLib/Joint.h"

SkyImplementRTTI( sky::lib::math::CJoint , sky::lib::serialize::CSerializeInterface );

namespace sky { namespace lib { namespace math {

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M11 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M12 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M13 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M14 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M21 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M22 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M23 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M24 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M31 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M32 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M33 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.M34 , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.Tx  , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.Ty  , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.Tz  , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_LocalMatrix.Tw  , sizeof( d64 ) , isWrite ); \

skyBool CJoint::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin save. \n" ) );

	GETSET_BIN( skyTrue );

	return skyTrue;
}

skyBool CJoint::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin load. \n" ) );

	GETSET_BIN( skyFalse );

	return skyTrue;
}

const serialize::CSerializeInterfacePtr &CJoint::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	( ( CJointPtr )spContainer )->m_LocalMatrix = m_LocalMatrix;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CJoint::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CJoint::Clone()
{
	return Copy( CJointPtr( NEW__( CJoint , CJoint() ) ) );
}

serialize::CSerializeInterfacePtr CJoint::CloneSharing()
{
	return Clone();
}

CJointPtr CJoint::AttachChild( u32 key, const CJointPtr &spChild , const CBasicVector3 &pos )
{
	m_ChildList.AddTail( key, spChild );
	spChild->AttachParent( thisPtr( CJointPtr ) );
	m_OffsetMatrix.SetupTranslation( pos );

	return spChild;
}

CJointPtr CJoint::AttachChild( const CJointPtr &spChild , const CBasicVector3 &pos )
{
	m_ChildList.AddTail( spChild );
	spChild->AttachParent( thisPtr( CJointPtr ) );
	spChild->SetOffset( pos );

	return spChild;
}

CJointPtr CJoint::AttachChildFront( const CJointPtr &spChild , const CBasicVector3 &pos )
{
	m_ChildList.AddFront( spChild );
	spChild->AttachParent( thisPtr( CJointPtr ) );
	m_OffsetMatrix.SetupTranslation( pos );

	return spChild;
}

void CJoint::DetachChild( const CJointPtr &spChild )
{
	m_ChildList.Delete( spChild );
	spChild->DetachParent();
}

void CJoint::DetachAllChild()
{
	FOREACH( CJointPtrList , it , m_ChildList )
	{
		DetachChild( ( *it ) );
	}
}

CJointPtr CJoint::SearchChild( const hash::CHashStringPtr &name )
{
	CJointPtr spResult;

	CJointUtil::SeaachChildName< CJoint , CJointPtr >( thisPtr( CJointPtr ) , name , spResult );

	return spResult;
}


skyBool CJoint::IsRoot()
{
	return m_Parent.IsEmpty();
}

skyBool CJoint::IsChildOn()
{
	return ( m_ChildList.GetNum() > 0 );
}

void CJoint::AttachParent( const CJointPtr &spParent )
{
	m_Parent = spParent;
}

const CJointPtr &CJoint::Parent()
{
	return m_Parent;
}

void CJoint::DetachParent()
{
	m_Parent.Delete();
}

CJointPtrList &CJoint::ChildList()
{
	return m_ChildList;
}

CJoint::~CJoint()
{
	if ( m_pInitialTransform != skyNull )
	{
		DEL m_pInitialTransform;
	}
};


CJoint::CJoint( const CJointPtr &spParent ) : Index( INVALID_JOINT_INDEX ) , m_Postion( 0 , 0 , 0 ) , m_Scale( 1 , 1 , 1 ) , m_pInitialTransform( skyNull ) , m_IsLocalMatrixComputeOn( skyTrue ) , m_IsComputeOffsetMatrixToLocalMatrixOn( skyFalse )
{
	m_Parent = spParent;
	m_LocalMatrix.Identity();
    m_OffsetMatrix.Identity();
	m_Rotation.Identity();
}

} } }