namespace sky { namespace lib { namespace math {

inline CRotationMatrix::CRotationMatrix()
{
}

inline CRotationMatrix	&CRotationMatrix::Identity()
{
	M11 = 1.0f; M12 = 0.0f; M13 = 0.0f;
	M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;
	M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;

	return *this;
}

inline CRotationMatrix	&CRotationMatrix::Setup( eAxis axis , dectype theta)
{
    //‰ñ“]Šp‚Ìsin‚Æcos‚ð“¾‚é

	dectype	s , c;
	CMathUtil::SinCos(&s , &c, theta);

    //‚Ç‚ÌŽ²‚ÌŽü‚è‚ð‰ñ“]‚µ‚Ä‚¢‚é‚©‚ðƒ`ƒFƒbƒN‚·‚é

	switch ( axis )
	{

		case eAxis_x: //‚wŽ²‚ÌŽü‚è‚Ì‰ñ“]

			M11 = 1.0f; M12 = 0.0f; M13 = 0.0f;
			M21 = 0.0f; M22 = c;    M23 = s;
			M31 = 0.0f; M32 = -s;   M33 = c;
			break;

		case eAxis_y: //‚xŽ²‚ÌŽü‚è‚Ì‰ñ“]

			M11 = c;    M12 = 0.0f; M13 = -s;
			M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;
			M31 = s;    M32 = 0.0f; M33 = c;
			break;

		case eAxis_z: //‚yŽ²‚ÌŽü‚è‚Ì‰ñ“]
			M11 = c;    M12 = -s;    M13 = 0.0f;
			M21 = s;   M22 = c;    M23 = 0.0f;
			M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;
			break;

		default:
        SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
	}

	return *this;
}

inline CRotationMatrix	&CRotationMatrix::Setup( const CBasicVector3 &axis , dectype theta )
{
    //Ž²‚ðŽw’è‚·‚é‚Ì‚É’PˆÊƒxƒNƒgƒ‹‚ð“n‚³‚ê‚½‚©‚Ç‚¤‚©‚ðƒ`ƒFƒbƒN‚·‚é
	SKY_ASSERT_MSG( CMathUtil::FAbs( axis.Dot( axis ) - 1.0f) < 0.01f , _T( "The vector is not Normalize." ) , axis ); //ˆø”axisƒxƒNƒgƒ‹‚ª³‹K‰»‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB

    //‰ñ“]Šp‚Ìsin‚Æcos‚ð“¾‚é

	dectype	s , c;
	CMathUtil::SinCos(&s , &c, theta);

    //1-cos(theta)‚Æ‚¢‚­‚Â‚©‚Ì‹¤’Ê‚µ‚Ä‚¢‚é•›ŽŸŽ®‚ðŒvŽZ‚·‚é

	dectype	a = 1.0f - c;
	dectype	ax = a * axis.X;
	dectype	ay = a * axis.Y;
	dectype	az = a * axis.Z;

    //s—ñ‚Ì—v‘f‚ðÝ’è‚·‚é
    //‹¤’Ê‚É—p‚¢‚ç‚ê‚é•›ŽŸŽ®‚ª‘òŽR‚ ‚é‚½‚ßA‚à‚¤­‚µÅ“K‰»‚Ì—]’n‚ª‚ ‚é
    //‚»‚ê‚ÍƒRƒ“ƒpƒCƒ‰‚É”C‚¹‚é‚±‚Æ‚É‚·‚éBB

	M11 = ax*axis.X + c;
	M12 = ax*axis.Y + axis.Z*s;
	M13 = ax*axis.Z - axis.Y*s;

	M21 = ay*axis.X - axis.Z*s;
	M22 = ay*axis.Y + c;
	M23 = ay*axis.Z + axis.X*s;

	M31 = az*axis.X + axis.Y*s;
	M32 = az*axis.Y - axis.X*s;
	M33 = az*axis.Z + c;

	return *this;
}

inline CBasicVector3	CRotationMatrix::InertialToObject(const CBasicVector3 &v) const
{
    //•W€“I‚È•û–@‚ÅAs—ñ‚ÌæŽZ‚ðŽÀs‚·‚é

	return CBasicVector3(
		M11*v.X + M21*v.Y + M31*v.Z,
		M12*v.X + M22*v.Y + M32*v.Z,
		M13*v.X + M23*v.Y + M33*v.Z
	);
}

inline CBasicVector3	CRotationMatrix::ObjectToInertial(const CBasicVector3 &v) const
{
    //“]’u‚ðæŽZ‚·‚é

	return CBasicVector3(
		M11*v.X + M12*v.Y + M13*v.Z,
		M21*v.X + M22*v.Y + M23*v.Z,
		M31*v.X + M32*v.Y + M33*v.Z
	);
}

inline skyBool CRotationMatrix::operator ==( const CRotationMatrix &m ) const
{
	return M11 == m.M11 && M12 == m.M12 && M13 == m.M13 && 
		   M21 == m.M21 && M22 == m.M22 && M23 == m.M23 &&
		   M31 == m.M31 && M32 == m.M32 && M33 == m.M23;
}

inline skyBool CRotationMatrix::operator !=( const CRotationMatrix &m ) const
{
	return M11 != m.M11 || M12 != m.M12 || M13 != m.M13 || 
		   M21 != m.M21 || M22 != m.M22 || M23 != m.M23 ||
		   M31 != m.M31 || M32 != m.M32 || M33 != m.M23;
}

inline CRotationMatrix	&CRotationMatrix::GetDirection( eAxis axis , CBasicVector3 &dest )
{
	switch ( axis )
	{

		case eAxis_x: //‚wŽ²‚ÌŒü‚«‚ÌŽæ“¾

			dest.X = M11;
			dest.Y = M12;
			dest.Z = M13;

			break;

		case eAxis_y: //‚xŽ²‚ÌŒü‚«‚ÌŽæ“¾

			dest.X = M21;
			dest.Y = M22;
			dest.Z = M23;

			break;

		case eAxis_z: //‚yŽ²‚ÌŒü‚«‚ÌŽæ“¾

			dest.X = M31;
			dest.Y = M32;
			dest.Z = M33;

			break;

		default:
            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
	}

	//Matrix4x3‚Æˆá‚Á‚ÄAâ‘Î‚ÉŠg‘åk¬î•ñ‚ª“ü‚Á‚Ä‚¢‚È‚¢‚Ì‚ÅA³‹K‰»‚Ì•K—v‚ª‚È‚¢

	return *this;
}

inline CRotationMatrix	&CRotationMatrix::SetUpXYZ(  const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = x * y * z;

	return *this;
}
inline CRotationMatrix	&CRotationMatrix::SetUpXZY( const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = x * z * y;

	return *this;
}
inline CRotationMatrix	&CRotationMatrix::SetUpYXZ( const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = y * x * z;

	return *this;
}
inline CRotationMatrix	&CRotationMatrix::SetUpYZX( const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = y * z * x;

	return *this;
}
inline CRotationMatrix	&CRotationMatrix::SetUpZXY( const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = z * x * y;

	return *this;
}
inline CRotationMatrix	&CRotationMatrix::SetUpZYX( const CBasicVector3 &vec )
{
	CRotationMatrix x , y , z;
	x.Setup( eAxis_x , vec.X );
	y.Setup( eAxis_y , vec.Y );
	z.Setup( eAxis_z , vec.Z );

	*this = z * y * x;

	return *this;
}

} } }

inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &m1, const sky::lib::math::CRotationMatrix &m2)
{
	sky::lib::math::CRotationMatrix r;

	r.M11 = m1.M11*m2.M11 + m1.M12*m2.M21 + m1.M13*m2.M31;
	r.M12 = m1.M11*m2.M12 + m1.M12*m2.M22 + m1.M13*m2.M32;
	r.M13 = m1.M11*m2.M13 + m1.M12*m2.M23 + m1.M13*m2.M33;

	r.M21 = m1.M21*m2.M11 + m1.M22*m2.M21 + m1.M23*m2.M31;
	r.M22 = m1.M21*m2.M12 + m1.M22*m2.M22 + m1.M23*m2.M32;
	r.M23 = m1.M21*m2.M13 + m1.M22*m2.M23 + m1.M23*m2.M33;

	r.M31 = m1.M31*m2.M11 + m1.M32*m2.M21 + m1.M33*m2.M31;
	r.M32 = m1.M31*m2.M12 + m1.M32*m2.M22 + m1.M33*m2.M32;
	r.M33 = m1.M31*m2.M13 + m1.M32*m2.M23 + m1.M33*m2.M33;

	//Œ‹‰Ê‚ð•Ô‚·
	return r;
}

inline sky::lib::math::CRotationMatrix &operator*=( sky::lib::math::CRotationMatrix &m1, const sky::lib::math::CRotationMatrix &m2 )
{
	m1 = m1 * m2;
	return m1;
}

inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &m , const sky::lib::math::CQuaternion &q )
{
	sky::lib::math::CRotationMatrix qm;
	qm.FromObjectToInertialQuaternion( q );

	return m * qm;
}

inline sky::lib::math::CRotationMatrix &operator*=( sky::lib::math::CRotationMatrix &m, const sky::lib::math::CQuaternion &q )
{
	m = m * q;
	return m;
}

inline sky::lib::math::CRotationMatrix operator+(const sky::lib::math::CRotationMatrix &m1, const sky::lib::math::CRotationMatrix &m2)
{

	sky::lib::math::CRotationMatrix r;

	//‘f’¼‚É‘«‚µŽZ

	r.M11 = m1.M11+m2.M11;
	r.M12 = m1.M11+m2.M12;
	r.M13 = m1.M11+m2.M13;

	r.M21 = m1.M21+m2.M11;
	r.M22 = m1.M21+m2.M12;
	r.M23 = m1.M21+m2.M13;

	r.M31 = m1.M31+m2.M11;
	r.M32 = m1.M31+m2.M12;
	r.M33 = m1.M31+m2.M13;

	//Œ‹‰Ê‚ð•Ô‚·
	return r;
}

inline sky::lib::math::CRotationMatrix &operator+=(sky::lib::math::CRotationMatrix &m1, const sky::lib::math::CRotationMatrix &m2) {
	m1 = m1 * m2;
	return m1;
}

inline sky::lib::math::CRotationMatrix Invert3x3( const sky::lib::math::CRotationMatrix &m )
{
	sky::lib::math::CRotationMatrix r;

    f32 a, b, c, d, e, f, g, h, i;
    a = m.m[ 0 ][ 0 ];
    b = m.m[ 0 ][ 1 ];
    c = m.m[ 0 ][ 2 ];
    d = m.m[ 1 ][ 0 ];
    e = m.m[ 1 ][ 1 ];
    f = m.m[ 1 ][ 2 ];
    g = m.m[ 2 ][ 0 ];
    h = m.m[ 2 ][ 1 ];
    i = m.m[ 2 ][ 2 ];

    r.m[ 0 ][ 0 ] = e * i - f * h;
    r.m[ 0 ][ 1 ] = c * h - b * i;
    r.m[ 0 ][ 2 ] = b * f - c * e;
    r.m[ 1 ][ 0 ] = f * g - d * i;
    r.m[ 1 ][ 1 ] = a * i - c * g;
    r.m[ 1 ][ 2 ] = c * d - a * f;
    r.m[ 2 ][ 0 ] = d * h - e * g;
    r.m[ 2 ][ 1 ] = b * g - a * h;
    r.m[ 2 ][ 2 ] = a * e - b * d;

    for ( s32 i0 = 0; i0 < 3; ++i0 )
    {
        for ( s32 i1 = 0; i1 < 3; ++i1 )
        {
            r.m[ i0 ][ i1 ] *= 1.0f / ((a * e * i + b * f * g + c * d * h) - (a * f * h + b * d * i + c * e * g));
        }
    }

	return r;
}

inline sky::lib::math::CRotationMatrix Transpose( sky::lib::math::CRotationMatrix m )
{
    sky::lib::math::CRotationMatrix dest;
    for ( s32 r = 0 ; r < 3 ; ++r )
    {
        for ( s32 c = 0 ; c < 3 ; ++c )
        {
            dest.m[ c ][ r ] = m.m[ r ][ c ];
        }
    }
    return dest;
}


inline sky::lib::math::CRotationMatrix PseudoInverse( const sky::lib::math::CRotationMatrix &m )
{
    sky::lib::math::CRotationMatrix tmp = Transpose( m ) * m;
    tmp = Invert3x3( tmp );
    return tmp * Transpose( m );
}
