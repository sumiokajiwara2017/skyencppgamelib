namespace sky { namespace lib { namespace math {

#define JOINT_DATA_HASH_SIZE ( 17 )

inline CJointPallet::CJointPallet() : m_JointDataHash( JOINT_DATA_HASH_SIZE )
{
}

inline u32 CJointPallet::GetJointNum()
{
	return m_JointPallet.GetNum();
}

inline void CJointPallet::AddJoint( const CJointPtr &spJoint )
{
	CJointData_ data;

	spJoint->Index = m_JointPallet.GetNum();
	data.m_spJoint = spJoint;

	//パレットに格納する
	m_JointPallet.Add( data );

	//Hashにも放り込む
	m_JointDataHash.Set( spJoint->Name.GetString() , data );

	//計算ッバッファも数に応じて増やす
	m_SkinTransforms.Add();
}

inline void CJointPallet::SetJoint( u32 no , const CJointPtr &spJoint )
{
	SKY_ASSERT( no < m_JointPallet.GetNum() );

	m_JointPallet[ no ].m_spJoint = spJoint;
}

inline const CJointPtr &CJointPallet::GetJoint( u32 no )
{
	return m_JointPallet[ no ].m_spJoint;
}

inline void CJointPallet::Clear()
{
	m_JointPallet.Clear();
	m_JointDataHash.DeleteAll();
}

inline CJointPtr CJointPallet::GetJoint( const hash::CHashStringPtr &name )
{
	CJointData_ jointData;
	m_JointDataHash.Get( name , jointData );
	return jointData.m_spJoint;
}

inline math::CMatrix4x3Array &CJointPallet::GetSkinTransformMatrixArray()
{
	return m_SkinTransforms;
}

} } }