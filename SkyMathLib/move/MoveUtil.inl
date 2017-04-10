namespace sky { namespace lib { namespace math {

template< typename VECTOR3 >
inline VECTOR3 CMoveUtil::VectorXMatrix( const VECTOR3 &v , const CMatrix4x3 &m )
{
	return v * m;
}

template< typename VECTOR3 >
inline VECTOR3 CMoveUtil::RotPoint( const VECTOR3 &point , const VECTOR3 &axis , dectype theta )
{
	CQuaternion q;
	q.SetToRotateAboutAxis( axis , theta );

	CRotationMatrix rm;
	rm.FromObjectToInertialQuaternion( q );

	return rm.InertialToObject( point );
}

} } }