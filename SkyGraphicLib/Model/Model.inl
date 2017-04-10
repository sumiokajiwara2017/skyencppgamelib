namespace sky { namespace lib { namespace graphic {

inline const math::CAABB3 &CModel::GetAABB3() const { return m_AABB3; };

inline const math::COBB3 &CModel::GetOBB3() const { return m_OBB3; };

inline const math::CBasicSphere &CModel::GetOBS() const { return m_BS; };

inline const math::CAABB3 &CModel::GetAABB3ComputeJoint() 
{ 
	m_AABB3ComputeJoint = m_AABB3;
	m_AABB3ComputeJoint.AddX( GetPosition_X() );
	m_AABB3ComputeJoint.AddY( GetPosition_Y() );
	m_AABB3ComputeJoint.AddZ( GetPosition_Z() );
	return m_AABB3ComputeJoint;
};

inline const math::COBB3  &CModel::GetOBB3ComputeJoint()
{ 
	m_OBB3ComputeJoint = m_OBB3;
	m_OBB3ComputeJoint.m_Pos    += GetPosition_XYZ();
	m_OBB3ComputeJoint.m_Sc     = m_OBB3ComputeJoint.m_Sc * GetScale_XYZ();
	m_OBB3ComputeJoint.m_Matrix = m_OBB3ComputeJoint.m_Matrix * GetRotation();
	return m_OBB3ComputeJoint;
};

inline const math::CBasicSphere &CModel::GetOBSComputeJoint()
{ 
	m_BSComputeJoint = m_BS;
	m_BSComputeJoint.AddPos( GetPosition_XYZ() );
	return m_BSComputeJoint;
};

inline math::CBasicVector3 CModel::GetLocalSize()
{
	return m_LocalSize * GetScale_XYZ();
}

inline math::CBasicVector3 CModel::GetLocalHalfSize()
{
	return m_LocalHalfSize * GetScale_XYZ();
}

inline f32 CModel::GetLocalYUnderLength()
{
	return m_LocalYUnderLength * GetScale_Y();
}

} } }