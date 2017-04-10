namespace sky { namespace lib { namespace math {

inline void CSkeleton::SerializeJoints( const CJointPtr &spNode )
{
	m_spJointPallet->AddJoint( spNode );
	for( u32 i = 0 ; i < spNode->ChildList().GetNum() ; i++ )
	{
		SerializeJoints( spNode->ChildList().GetIdx( i ) );
	}
}

inline void CSkeleton::Initialize( const CJointPtr &root )
{
	if ( m_spJointPallet.IsEmpty() == skyTrue )
	{
		m_spJointPallet = CJointPalletCreate_();
	}
    SerializeJoints( root );
}

inline CBasicVector3 CSkeleton::GetLocalPosition( s32 jid ) const
{
    CBasicVector3 lpos = CBasicVector3_ZERO;
	CMatrix4x3 trans = m_spJointPallet->GetJoint( jid )->GetOffsetMatrix();
    lpos = Transform( lpos , m_spJointPallet->GetJoint( jid )->GetOffsetMatrix() );

	for ( CJointPtr node = m_spJointPallet->GetJoint( jid )->Parent();  node.IsEmpty() != skyTrue && node->Parent().IsEmpty() != skyTrue && node->Parent()->Index != INVALID_JOINT_INDEX ; node = node->Parent() )
    {
		lpos =  MulMatrix4x4( lpos , node->ComputeBoneTransForm() );
    }
    return lpos;
}

inline CMatrix4x3 CSkeleton::GetGlobalCoordinate( s32 jid ) const
{
//	return GetLocalCoordinate( jid ) * m_spJointRoot->ComputeBoneTransForm(); //このコンパイルが通らない？？
	return MulMatrix4x4( GetLocalCoordinate( jid ) , m_spJointRoot->ComputeBoneTransForm() );
}
inline CMatrix4x3 CSkeleton::GetLocalCoordinate( s32 jid ) const
{
    CMatrix4x3 coord = m_spJointPallet->GetJoint( jid )->GetOffsetMatrix();
	for ( CJointPtr node = m_spJointPallet->GetJoint( jid )->Parent() ; node.IsEmpty() != skyTrue && node->Parent().IsEmpty() != skyTrue && node->Parent()->Index != INVALID_JOINT_INDEX ; node = node->Parent() )
    {
		coord *= node->ComputeBoneTransForm();
    }
    return coord;
}

inline CBasicVector3 CSkeleton::GetGlobalPosition( s32 jid ) const 
{
	return MulMatrix4x4( GetLocalPosition( jid ) , m_spJointRoot->ComputeBoneTransForm() );
}

inline CSkeleton::CSkeleton( const CJointPtr &spJoint )
{
	if ( spJoint.IsEmpty() == skyTrue )
	{
		m_spJointRoot = CJointCreate_();
	}
	else
	{
		m_spJointRoot = spJoint;
		Initialize( spJoint );
	}
}

inline void CSkeleton::PrintPosition()
{
	for ( u32 i = 0 ; i < m_spJointPallet->GetJointNum() ; i++ )
	{
		SKY_PRINTF( "Pos = [ %f , %f , %f ]\n" , 
			m_spJointPallet->GetJoint( i )->GetPosition_XYZ().X ,
			m_spJointPallet->GetJoint( i )->GetPosition_XYZ().Y ,
			m_spJointPallet->GetJoint( i )->GetPosition_XYZ().Z );
	}
}

} } }
