namespace sky { namespace lib { namespace math {

inline void CJoint::Reset()
{
	m_Rotation.Identity();
	m_Postion.Zero();
	m_Scale = CBasicVector3( 1 , 1 , 1 );
}

inline void CJoint::SetPosition_X( dectype value )
{
	m_Postion.X = value;
}

inline void CJoint::AddPosition_X( dectype value )
{
	m_Postion.X += value;
}

inline void CJoint::SetPosition_Y( dectype value )
{
	m_Postion.Y = value;
}

inline void CJoint::AddPosition_Y( dectype value )
{
	m_Postion.Y += value;
}

inline void CJoint::SetPosition_Z( dectype value )
{
	m_Postion.Z = value;
}

inline void CJoint::AddPosition_Z( dectype value )
{
	m_Postion.Z += value;
}

inline void CJoint::SetPosition_XY( const CBasicVector2 &vec )
{
	m_Postion.X = vec.X;
	m_Postion.Y = vec.Y;
}

inline void CJoint::AddPosition_XY( const CBasicVector2 &vec )
{
	m_Postion.X += vec.X;
	m_Postion.Y += vec.Y;
}

inline void CJoint::SetPosition_XZ( const CBasicVector2 &vec )
{
	m_Postion.X = vec.X;
	m_Postion.Z = vec.Y;
}

inline void CJoint::AddPosition_XZ( const CBasicVector2 &vec )
{
	m_Postion.X += vec.X;
	m_Postion.Z += vec.Y;
}

inline void CJoint::SetPosition_YZ( const CBasicVector2 &vec )
{
	m_Postion.Y = vec.X;
	m_Postion.Z = vec.Y;
}

inline void CJoint::AddPosition_YZ( const CBasicVector2 &vec )
{
	m_Postion.Y += vec.X;
	m_Postion.Z += vec.Y;
}

inline void CJoint::SetPosition_XYZ( const CBasicVector3 &vec )
{
	m_Postion.X = vec.X;
	m_Postion.Y = vec.Y;
	m_Postion.Z = vec.Z;
}

inline void CJoint::AddPosition_XYZ( const CBasicVector3 &vec )
{
	m_Postion.X += vec.X;
	m_Postion.Y += vec.Y;
	m_Postion.Z += vec.Z;
}

inline void CJoint::SetScale_X( dectype value )
{
	m_Scale.X = value;
}

inline void CJoint::MulScale_X( dectype value )
{
	m_Scale.X *= value;
}

inline void CJoint::SetScale_Y( dectype value )
{
	m_Scale.Y = value;
}

inline void CJoint::MulScale_Y( dectype value )
{
	m_Scale.Y *= value;
}

inline void CJoint::SetScale_Z( dectype value )
{
	m_Scale.Z = value;
}

inline void CJoint::MulScale_Z( dectype value )
{
	m_Scale.Z *= value;
}

inline void CJoint::SetScale_XY( const CBasicVector2 &vec )
{
	SetScale_X( vec.X );
	SetScale_Y( vec.Y );
}

inline void CJoint::MulScale_XY( const CBasicVector2 &vec )
{
	MulScale_X( vec.X );
	MulScale_Y( vec.Y );
}

inline void CJoint::SetScale_XZ( const CBasicVector2 &vec )
{
	SetScale_X( vec.X );
	SetScale_Z( vec.Y );
}

inline void CJoint::AddScale_XZ( const CBasicVector2 &vec )
{
	MulScale_X( vec.X );
	MulScale_Z( vec.Y );
}

inline void CJoint::SetScale_YZ( const CBasicVector2 &vec )
{
	SetScale_Y( vec.X );
	SetScale_Z( vec.Y );
}

inline void CJoint::AddScale_YZ( const CBasicVector2 &vec )
{
	MulScale_Y( vec.X );
	MulScale_Z( vec.Y );
}

inline void CJoint::SetScale_XYZ( const CBasicVector3 &vec )
{
	SetScale_X( vec.X );
	SetScale_Y( vec.Y );
	SetScale_Z( vec.Z );
}

inline void CJoint::MulScale_XYZ( const CBasicVector3 &vec )
{
	MulScale_X( vec.X );
	MulScale_Y( vec.Y );
	MulScale_Z( vec.Z );
}

inline void CJoint::SetScale_XYZ( dectype sc )
{
	SetScale_X( sc );
	SetScale_Y( sc );
	SetScale_Z( sc );
}

inline void CJoint::MulScale_XYZ( dectype sc )
{
	MulScale_X( sc );
	MulScale_Y( sc );
	MulScale_Z( sc );
}

inline void CJoint::SetRotation_X( dectype radian )
{
	m_Rotation.SetToRotateAboutX( radian );
}

inline void CJoint::MulRotation_X( dectype radian )
{
	CQuaternion rot;
	rot.SetToRotateAboutX( radian );
	m_Rotation *= rot;
}

inline void CJoint::SetRotation_Y( dectype radian )
{
	m_Rotation.SetToRotateAboutY( radian );
}

inline void CJoint::MulRotation_Y( dectype radian )
{
	CQuaternion rot;
	rot.SetToRotateAboutY( radian );
	m_Rotation *= rot;
}

inline void CJoint::SetRotation_Z( dectype radian )
{
	m_Rotation.SetToRotateAboutZ( radian );
}

inline void CJoint::MulRotation_Z( dectype radian )
{
	CQuaternion rot;
	rot.SetToRotateAboutZ( radian );
	m_Rotation *= rot;
}

inline void CJoint::MulRotation_XY( const CBasicVector2 &vec )
{
	MulRotation_X( vec.X );
	MulRotation_Y( vec.Y );
}

inline void CJoint::MulRotation_YX( const CBasicVector2 &vec )
{
	MulRotation_Y( vec.Y );
	MulRotation_X( vec.X );
}

inline void CJoint::MulRotation_XZ( const CBasicVector2 &vec )
{
	MulRotation_X( vec.X );
	MulRotation_Z( vec.Y );
}

inline void CJoint::MulRotation_ZX( const CBasicVector2 &vec )
{
	MulRotation_Z( vec.Y );
	MulRotation_X( vec.X );
}

inline void CJoint::MulRotation_YZ( const CBasicVector2 &vec )
{
	MulRotation_Y( vec.X );
	MulRotation_Z( vec.Y );
}

inline void CJoint::MulRotation_ZY( const CBasicVector2 &vec )
{
	MulRotation_Z( vec.Y );
	MulRotation_Y( vec.X );
}

inline void CJoint::MulRotation_XYZ( const CBasicVector3 &vec )
{
	MulRotation_X( vec.X );
	MulRotation_Y( vec.Y );
	MulRotation_Z( vec.Z );
}

inline void CJoint::MulRotation_XZY( const CBasicVector3 &vec )
{
	MulRotation_X( vec.X );
	MulRotation_Z( vec.Z );
	MulRotation_Y( vec.Y );
}

inline void CJoint::MulRotation_YXZ( const CBasicVector3 &vec )
{
	MulRotation_Y( vec.Y );
	MulRotation_X( vec.X );
	MulRotation_Z( vec.Z );
}

inline void CJoint::MulRotation_YZX( const CBasicVector3 &vec )
{
	MulRotation_Y( vec.Y );
	MulRotation_Z( vec.Z );
	MulRotation_X( vec.X );
}

inline void CJoint::MulRotation_ZXY( const CBasicVector3 &vec )
{
	MulRotation_Z( vec.Z );
	MulRotation_X( vec.X );
	MulRotation_Y( vec.Y );
}

inline void CJoint::MulRotation_ZYX( const CBasicVector3 &vec )
{
	MulRotation_Z( vec.Z );
	MulRotation_Y( vec.Y );
	MulRotation_X( vec.X );
}

inline void CJoint::SetRotation( const CBasicVector3 &axis , dectype radian )
{
	m_Rotation.SetToRotateAboutAxis( axis , radian );
}

inline void CJoint::MulRotation( const CBasicVector3 &axis , dectype radian )
{
	CQuaternion rot;
	rot.SetToRotateAboutAxis( axis , radian );
	m_Rotation *= rot;
}

inline const CQuaternion &CJoint::GetRotation() const
{
	return m_Rotation;
}

inline CQuaternion &CJoint::GetRotation()
{
	return m_Rotation;
}

inline CRotationMatrix CJoint::GetRotationMatrix() const
{
	CRotationMatrix r;
	r.FromObjectToInertialQuaternion( m_Rotation );
	return r;
}

inline CMatrix4x3 CJoint::GetRotationMatrix4x3() const
{
	CMatrix4x3 r;
	r.SetupRotate( GetRotationMatrix() );
	return r;
}

inline void CJoint::MulQuaternion( const CQuaternion &q )
{
	m_Rotation *= q;
}

inline CBasicVector3 CJoint::GetAxisX() const
{
	return CBasicVector3( m_LocalMatrix.M11 , m_LocalMatrix.M12 , m_LocalMatrix.M13 ).Normalize();
}

inline CBasicVector3 CJoint::GetAxisY() const
{
	return CBasicVector3( m_LocalMatrix.M21 , m_LocalMatrix.M22 , m_LocalMatrix.M23 ).Normalize();
}

inline CBasicVector3 CJoint::GetAxisZ() const
{
	return CBasicVector3( m_LocalMatrix.M31 , m_LocalMatrix.M32 , m_LocalMatrix.M33 ).Normalize();
}

inline dectype CJoint::GetPosition_X() const
{
	return m_LocalMatrix.Tx;
}
inline dectype CJoint::GetPosition_Y() const
{
	return m_LocalMatrix.Ty;
}
inline dectype CJoint::GetPosition_Z() const
{
	return m_LocalMatrix.Tz;
}
inline CBasicVector2 CJoint::GetPosition_XY() const
{
	return CBasicVector2( GetPosition_X() , GetPosition_Y() );
}
inline CBasicVector2 CJoint::GetPosition_XZ() const
{
	return CBasicVector2( GetPosition_X() , GetPosition_Z() );
}
inline CBasicVector2 CJoint::GetPosition_YZ() const
{
	return CBasicVector2( GetPosition_Y() , GetPosition_Z() );
}
inline CBasicVector3 CJoint::GetPosition_XYZ() const
{
	return CBasicVector3( GetPosition_X() , GetPosition_Y() , GetPosition_Z() );
}

inline dectype CJoint::GetScale_X() const
{
	return CBasicVector3( m_LocalMatrix.M11 , m_LocalMatrix.M12 , m_LocalMatrix.M13 ).Length();
}
inline dectype CJoint::GetScale_Y() const
{
	return CBasicVector3( m_LocalMatrix.M21 , m_LocalMatrix.M22 , m_LocalMatrix.M23 ).Length();
}
inline dectype CJoint::GetScale_Z() const
{
	return CBasicVector3( m_LocalMatrix.M31 , m_LocalMatrix.M32 , m_LocalMatrix.M33 ).Length();
}
inline CBasicVector2 CJoint::GetScale_XY() const
{
	return CBasicVector2( GetScale_X() , GetScale_Y() );
}
inline CBasicVector2 CJoint::GetScale_XZ() const
{
	return CBasicVector2( GetScale_X() , GetScale_Z() );
}
inline CBasicVector2 CJoint::GetScale_YZ() const
{
	return CBasicVector2( GetScale_Y() , GetScale_Z() );
}
inline CBasicVector3 CJoint::GetScale_XYZ() const
{
	return CBasicVector3( GetScale_X() , GetScale_Y() , GetScale_Z() );
}

inline const CMatrix4x3 &CJoint::ComputeLocalMatrix()
{
	if ( m_IsLocalMatrixComputeOn == skyTrue )
	{
		//拡縮情報設定（処理負荷低い）行列の初期化も兼ねている
		m_LocalMatrix.SetupScale( m_Scale );

		//回転（処理負荷高い）高いけど正しく回転するには仕方がない
		CRotationMatrix rot;
		rot.FromInertialToObjectQuaternion( m_Rotation );
		m_LocalMatrix *= rot;

		//移動設定（処理負荷低い）
		m_LocalMatrix.SetTranslation( m_Postion );

		//オフセット行列と初期姿勢行列をかけるか？
		if ( m_IsComputeOffsetMatrixToLocalMatrixOn == skyTrue )
		{
			m_LocalMatrix = MulMatrix4x4Full( m_LocalMatrix , m_OffsetMatrix );
		}
	}

	return m_LocalMatrix;
}

inline const CMatrix4x3 &CJoint::GetLocalMatrix() const
{
	return m_LocalMatrix;
}

inline void CJoint::SetLocalMatrix( const CMatrix4x3 &matrix )
{
	m_LocalMatrix = matrix;
}

inline void CJoint::SetComputeLocalMatrixOn( skyBool isComputeOn )
{
	m_IsLocalMatrixComputeOn = isComputeOn;
}

inline void CJoint::SetComputeOffsetMatrixToLocalMatrixOn( skyBool isComputeOn )
{
	m_IsComputeOffsetMatrixToLocalMatrixOn = isComputeOn;
}

inline const CMatrix4x3 &CJoint::ComputeBoneTransForm()
{
	m_JointTransformMatrix.Identity();

	//回転（処理負荷高い）高いけど正しく回転するには仕方がない
	CRotationMatrix rot;
	rot.FromInertialToObjectQuaternion( m_Rotation );
	m_JointTransformMatrix *= rot;

	//オフセット行列をかける
	m_JointTransformMatrix *= m_OffsetMatrix;

	//移動設定（処理負荷低い）
	m_JointTransformMatrix.AddTranslation( m_Postion );

	return m_JointTransformMatrix;
}

inline const CMatrix4x3 &CJoint::GetJointTransForm() const
{
	return m_JointTransformMatrix;
}

inline void CJoint::SetJointTransForm( const CMatrix4x3 &matrix )
{
	m_JointTransformMatrix = matrix;
}

inline const CMatrix4x3 &CJoint::GetInitialTransform() const
{
	SKY_ASSERT( m_pInitialTransform != skyNull ); //使いたいときは必ずセットしてね

	return *m_pInitialTransform;
}

inline void CJoint::SetInitialTransform( const CMatrix4x3 &matrix )
{
	if ( m_pInitialTransform == skyNull )
	{
		m_pInitialTransform = NEW CMatrix4x3();
	}

    *m_pInitialTransform = matrix;
}

inline CMatrix4x3 CJoint::GetPositionMatrix() const
{
	CMatrix4x3 r;
	return r.SetupTranslation( m_Postion );
}

inline const CMatrix4x3 &CJoint::GetOffsetMatrix() const
{
	return m_OffsetMatrix;
}

inline void CJoint::SetOffsetMatrix( const CMatrix4x3 &matrix )
{
	m_OffsetMatrix = matrix;
}

inline void CJoint::SetOffset( const CBasicVector3 &offset )
{
	m_OffsetMatrix.SetupTranslation( offset );
}

inline CBasicVector3 CJoint::GetOffset() const
{
	CBasicVector3 pos;
	return m_OffsetMatrix.GetCol3( pos );
}

inline CBasicVector3 CJoint::GetOffsetVector() const
{
	CBasicVector3 dest;
	return m_OffsetMatrix.GetCol3( dest );
}

inline void CJoint::SetOffsetVector( const CBasicVector3 &vec )
{
	m_OffsetMatrix.SetupTranslation( vec );
}

} } }