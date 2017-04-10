namespace sky { namespace lib { namespace math {

template< class T >
void CMatrix4x3::CopyArray( T *dest ) const
{
	dest[ 0  ] = ( T )M11;
	dest[ 1  ] = ( T )M12;
	dest[ 2  ] = ( T )M13;
	dest[ 3  ] = ( T )M14;

	dest[ 4  ] = ( T )M21;
	dest[ 5  ] = ( T )M22;
	dest[ 6  ] = ( T )M23;
	dest[ 7  ] = ( T )M24;

	dest[ 8  ] = ( T )M31;
	dest[ 9  ] = ( T )M32;
	dest[ 10 ] = ( T )M33;
	dest[ 11 ] = ( T )M34;

	dest[ 12 ] = ( T )Tx;
	dest[ 13 ] = ( T )Ty;
	dest[ 14 ] = ( T )Tz;
	dest[ 15 ] = ( T )Tw;
}

inline CMatrix4x3::CMatrix4x3()
{
}

inline CMatrix4x3::CMatrix4x3( const dectype mat[ 16 ] )
{
	M11 = mat[ 0 ];
	M12 = mat[ 1 ];
	M13 = mat[ 2 ];
	M14 = mat[ 3 ];

	M21 = mat[ 4 ];
	M22 = mat[ 5 ];
	M23 = mat[ 6 ];
	M24 = mat[ 7 ];

	M31 = mat[ 8 ];
	M32 = mat[ 9 ];
	M33 = mat[ 10 ];
	M34 = mat[ 11 ];

	Tx = mat[ 12 ];
	Ty = mat[ 13 ];
	Tz = mat[ 14 ];
	Tw = mat[ 15 ];
}

inline CMatrix4x3::CMatrix4x3( dectype m11 , dectype m12 , dectype m13 , dectype m14 ,
            dectype m21 , dectype m22 , dectype m23 , dectype m24 , 
            dectype m31 , dectype m32 , dectype m33 , dectype m34 , 
            dectype m41 , dectype m42 , dectype m43 , dectype m44 )
{
	M11 = m11;
	M12 = m12;
	M13 = m13;
	M14 = m14;

	M21 = m21;
	M22 = m22;
	M23 = m23;
	M24 = m24;

	M31 = m31;
	M32 = m32;
	M33 = m33;
	M34 = m34;

	Tx = m41;
	Ty = m42;
	Tz = m43;
	Tw = m44;
}

inline CMatrix4x3& CMatrix4x3::Identity()
{
	M11 = 1.0f; M12 = 0.0f; M13 = 0.0f; M14 = 0.0f;
	M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;	M24 = 0.0f;
	M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;	M34 = 0.0f;
	Tx  = 0.0f; Ty  = 0.0f; Tz  = 0.0f;	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::operator =( const CMatrix4x3& m )
{
	M11 = m.M11;
	M12 = m.M12;
	M13 = m.M13;
	M14 = m.M14;

	M21 = m.M21;
	M22 = m.M22;
	M23 = m.M23;
	M24 = m.M24;

	M31 = m.M31;
	M32 = m.M32;
	M33 = m.M33;
	M34 = m.M34;

	Tx  = m.Tx;
	Ty  = m.Ty;
	Tz  = m.Tz;
	Tw  = m.Tw;

	return *this;
}

inline CMatrix4x3  CMatrix4x3::operator + ( const CMatrix4x3& src ) const
{
	return CMatrix4x3( M11 + src.M11, M12 + src.M12, M13 + src.M13, M14 + src.M14,
				       M21 + src.M21, M22 + src.M22, M23 + src.M23, M24 + src.M24,
				       M31 + src.M31, M32 + src.M32, M33 + src.M33, M34 + src.M34,
				       Tx  + src.Tx,  Ty + src.Ty, Tz + src.Tz, Tw + src.Tw );
}

inline CMatrix4x3  CMatrix4x3::operator - ( const CMatrix4x3& src ) const
{
	return CMatrix4x3( M11 - src.M11, M12 - src.M12, M13 - src.M13, M14 - src.M14,
				       M21 - src.M21, M22 - src.M22, M23 - src.M23, M24 - src.M24,
				       M31 - src.M31, M32 - src.M32, M33 - src.M33, M34 - src.M34,
				       Tx  - src.Tx,  Ty - src.Ty, Tz - src.Tz, Tw - src.Tw );
}

inline CMatrix4x3& CMatrix4x3::ZeroTranslation()
{
	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetTranslation( const CBasicVector3 &vec )
{
	Tx = vec.X; Ty = vec.Y; Tz = vec.Z;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupTranslation(const CBasicVector3 &vec) 
{

	Identity();

	//恒等行列設定
	M11 = 1.0f; M12 = 0.0f; M13 = 0.0f; M14 = 0.0f;
	M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;	M24 = 0.0f;
	M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;	M34 = 0.0f;
	Tw =  1.0f;

	//平行移動部分を設定する
	Tx = vec.X; Ty = vec.Y; Tz = vec.Z;

	return *this;
}

inline dectype    CMatrix4x3::GetTranslation( eAxis axis ) const
{
	switch ( axis )
	{

		case eAxis_x: //Ｘ軸の移動量

			return Tx;
			break;

		case eAxis_y: //Ｙ軸の移動量

			return Ty;
			break;

		case eAxis_z: //Ｚ軸の移動量

			return Tz;
			break;

		default:
            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ), axis );
	}
	return 0.0f;
}

inline CMatrix4x3& CMatrix4x3::SetupRotate( eAxis axis , dectype theta)
{
	Identity();

    //回転角のsinとcosを得る

	dectype	s , c;
	CMathUtil::SinCos( &s , &c, theta );

    //どの軸の周りを回転しているかをチェックする

	switch ( axis )
	{

		case eAxis_x: //Ｘ軸の周りの回転

			M11 = 1.0f; M12 = 0.0f; M13 = 0.0f;
			M21 = 0.0f; M22 = c;    M23 = s;
			M31 = 0.0f; M32 = -s;   M33 = c;
			break;

		case eAxis_y: //Ｙ軸の周りの回転

			M11 = c;    M12 = 0.0f; M13 = -s;
			M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;
			M31 = s;    M32 = 0.0f; M33 = c;
			break;

		case eAxis_z: //Ｚ軸の周りの回転
			M11 = c;    M12 = -s;    M13 = 0.0f;
			M21 = s;    M22 = c;    M23 = 0.0f;
			M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;
			break;

		default:
            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
	}

    //平行移動をリセットする
	Tx = Ty = Tz = 0.0f;

	return *this;
}


inline CMatrix4x3& CMatrix4x3::AddTranslation( const CBasicVector3 &vec )
{
	Tx += vec.X; Ty += vec.Y; Tz += vec.Z;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupRotateXYZ( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = x * y * z;

	return *this;
}
inline CMatrix4x3& CMatrix4x3::SetupRotateXZY( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = x * z * y;

	return *this;
}
inline CMatrix4x3& CMatrix4x3::SetupRotateYXZ( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = y * x * z;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupRotateYZX( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = y * z * x;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupRotateZXY( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = z * x * y;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupRotateZYX( const CBasicVector3 &vec )
{
	CMatrix4x3 x , y , z;
	x.SetupRotate( eAxis_x , vec.X );
	y.SetupRotate( eAxis_y , vec.Y );
	z.SetupRotate( eAxis_z , vec.Z );

	*this = z * y * z;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupRotate( const CBasicVector3 &axis , dectype theta )
{
	Identity();

    //軸を指定するのに単位ベクトルを渡されたかどうかをチェックする
	SKY_ASSERT_MSG( CMathUtil::FAbs( axis.Dot( axis ) - 1.0f) < 0.01f , _T( "The vector is not Normalize." ) , axis ); //引数axisベクトルが正規化されていません。

    //回転角のsinとcosを得る

	dectype	s , c;
	CMathUtil::SinCos(&s , &c, theta);

    //1-cos(theta)といくつかの共通している副次式を計算する

	dectype	a = 1.0f - c;
	dectype	ax = a * axis.X;
	dectype	ay = a * axis.Y;
	dectype	az = a * axis.Z;

    //行列の要素を設定する
    //共通に用いられる副次式が沢山あるため、もう少し最適化の余地がある
    //それはコンパイラに任せることにする。。

	M11 = ax*axis.X + c;
	M12 = ax*axis.Y + axis.Z*s;
	M13 = ax*axis.Z - axis.Y*s;

	M21 = ay*axis.X - axis.Z*s;
	M22 = ay*axis.Y + c;
	M23 = ay*axis.Z + axis.X*s;

	M31 = az*axis.X + axis.Y*s;
	M32 = az*axis.Y - axis.X*s;
	M33 = az*axis.Z + c;

	//平行移動部分をリセットする

	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline void    CMatrix4x3::GetDirection( eAxis axis , CBasicVector3 &dest , skyBool isNormalizeExec ) const
{
	switch ( axis )
	{

		case eAxis_x: //Ｘ軸の向きの取得

			dest.X = M11;
			dest.Y = M12;
			dest.Z = M13;

			break;

		case eAxis_y: //Ｙ軸の向きの取得

			dest.X = M21;
			dest.Y = M22;
			dest.Z = M23;

			break;

		case eAxis_z: //Ｚ軸の向きの取得

			dest.X = M31;
			dest.Y = M32;
			dest.Z = M33;

			break;

		default:
            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
    }

	//正規化して拡大縮小情報をそぎ落とす
	if ( isNormalizeExec == skyTrue )
	{
		dest.Normalize();
	}
}

inline CMatrix4x3& CMatrix4x3::SetupScale( const CBasicVector3 &sc )
{
	Identity();

    //行列の要素を設定する（非常に単純明快である）
	M11 = sc.X;  M12 = 0.0f; M13 = 0.0f;
	M21 = 0.0f; M22 = sc.Y;  M23 = 0.0f;
	M31 = 0.0f; M32 = 0.0f;  M33 = sc.Z;

    //平行移動部分をリセットする

	Tx = Ty = Tz = 0.0f;

	return *this;
}


inline CMatrix4x3& CMatrix4x3::SetScale( const CBasicVector3 &sc )
{
	Identity();

    //行列の要素を設定する（非常に単純明快である）

	M11 = sc.X;  M12 = 0.0f; M13 = 0.0f;
	M21 = 0.0f; M22 = sc.Y;  M23 = 0.0f;
	M31 = 0.0f; M32 = 0.0f;  M33 = sc.Z;

	return *this;
}

inline dectype    CMatrix4x3::GetScale( eAxis axis ) const
{
	CBasicVector3 direction;
	GetDirection( axis , direction , skyFalse );
	return direction.Length();
}

inline CBasicVector3 CMatrix4x3::GetScale() const
{
	return CBasicVector3( GetScale( eAxis_x ) , GetScale( eAxis_y ) , GetScale( eAxis_z ) );
}

inline CMatrix4x3& CMatrix4x3::SetupScaleAlongAxis(const CBasicVector3 &axis , dectype k)
{
	Identity();

    //軸を指定するのに単位ベクトルを渡されたかどうかをチェックする
	SKY_ASSERT_MSG( CMathUtil::FAbs( axis.Dot( axis ) - 1.0f ) < 0.01f , _T( "The vector is not Normalize." ) , axis ); //引数axisベクトルが正規化されていません。

     //k-1といくつかの共通して用いる副次式を計算する
	dectype	a = k - 1.0f;
	dectype	ax = a * axis.X;
	dectype	ay = a * axis.Y;
	dectype	az = a * axis.Z;

    //行列の要素を生める
    //ここで共通して用いる副次式に夜最適化を行う
    //対角線上で反対の行列の要素が等しいからである。

	M11 = ax*axis.X + 1.0f;
	M22 = ay*axis.Y + 1.0f;
	M32 = az*axis.Z + 1.0f;

	M12 = M21 = ax*axis.Y;
	M13 = M31 = ax*axis.Z;
	M23 = M32 = ay*axis.Z;

    //平行移動部分をリセットする

	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupShear(eAxis axis , dectype s , dectype t)
{
	Identity();

    //行いたいせん断の種類をチェックする

	switch (axis) {

		case eAxis_x: //Ｘを用いてせん断する

			M11 = 1.0f; M12 = s;    M13 = t;
			M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;
			M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;
			break;

		case eAxis_y: //Ｙを用いてせん断する

			M11 = 1.0f; M12 = 0.0f; M13 = 0.0f;
			M21 = s;    M22 = 1.0f; M23 = t;
			M31 = 0.0f; M32 = 0.0f; M33 = 1.0f;
			break;

		case eAxis_z:  //Ｚを用いてせん断する

			M11 = 1.0f; M12 = 0.0f; M13 = 0.0f;
			M21 = 0.0f; M22 = 1.0f; M23 = 0.0f;
			M31 = s;    M32 = t;    M33 = 1.0f;
			break;

		default:

            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
	}

    //平行移動部分をリセットする
	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProject( const CBasicVector3 &n )
{
	Identity();

    //軸を指定するのに単位ベクトルを渡されたかどうかをチェックする
	SKY_ASSERT_MSG( CMathUtil::FAbs( n.Dot( n ) - 1.0f ) < 0.01f , _T( "The vector is not Normalize." ) , CMathUtil::FAbs(n.Dot(n) - 1.0f) ); //引数nベクトルが正規化されていません。
	 
    //行列の要素を生める
    //ここで共通して用いる副次式による最適化を行う
    //対角線上で反対の行列の要素が等しいからである

	M11 = 1.0f - n.X*n.X;
	M22 = 1.0f - n.Y*n.Y;
	M33 = 1.0f - n.Z*n.Z;

	M12 = M21 = -n.X*n.Y;
	M13 = M31 = -n.X*n.Z;
	M23 = M32 = -n.Y*n.Z;

    //平行移動部分をリセットする
	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProject( const CBasicPlane &p , const CBasicVector3 &lightPos )
{//D3DXMatrixShadow関数処理を参考に作成

	CBasicVector4 P = p;
	P.Normalize();
	CBasicVector4 L( lightPos.X , lightPos.Y , lightPos.Z , 0.0f );
	dectype d = P.Dot( L );

	M11 = -P.X * L.X + d;
	M12 = -P.X * L.Y;
	M13 = -P.X * L.Z;
	M14 = -P.X * L.W;

	M21 = -P.Y * L.X;
	M22 = -P.Y * L.Y + d;
	M23 = -P.Y * L.Z;
	M24 = -P.Y * L.W;

	M31 = -P.Z * L.X;
	M32 = -P.Z * L.Y;
	M33 = -P.Z * L.Z + d;
	M34 = -P.Z * L.W;

	Tx  = -P.W * L.X;
	Ty  = -P.W * L.Y;
	Tz  = -P.W * L.Z;
	Tw  = -P.W * L.W + d;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupReflect( eAxis axis , dectype k )
{
	Identity();

    //どの面に関してリフレクションを実行するかをチェックする

	switch ( axis )
	{
		case eAxis_x: //面ｘ＝ｋに関するリフレクション

			M11 = -1.0f; M12 =  0.0f; M13 =  0.0f;
			M21 =  0.0f; M22 =  1.0f; M23 =  0.0f;
			M31 =  0.0f; M32 =  0.0f; M33 =  1.0f;

			Tx = 2.0f * k;
			Ty = 0.0f;
			Tz = 0.0f;

			break;

		case eAxis_y: //面ｙ＝ｋに関するリフレクション

			M11 =  1.0f; M12 =  0.0f; M13 =  0.0f;
			M21 =  0.0f; M22 = -1.0f; M23 =  0.0f;
			M31 =  0.0f; M32 =  0.0f; M33 =  1.0f;

			Tx = 0.0f;
			Ty = 2.0f * k;
			Tz = 0.0f;

			break;

		case eAxis_z: //面ｚ＝ｋに関するリフレクション

			M11 =  1.0f; M12 =  0.0f; M13 =  0.0f;
			M21 =  0.0f; M22 =  1.0f; M23 =  0.0f;
			M31 =  0.0f; M32 =  0.0f; M33 = -1.0f;

			Tx = 0.0f;
			Ty = 0.0f;
			Tz = 2.0f * k;

			break;

		default:
            SKY_ASSERT_MSG( skyFalse , _T( "The parameter[axis] is invalid. [%d]" ) , axis );
	}

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupReflect( const CBasicVector3 &n )
{
	Identity();

    //軸を指定するのに単位ベクトルを渡されたかどうかをチェックする
	SKY_ASSERT_MSG( CMathUtil::FAbs(n.Dot( n ) - 1.0f) < 0.01f , _T( "The vector is not Normalize." ) , CMathUtil::FAbs(n.Dot( n ) - 1.0f) ); //CMatrix4x3::SetupReflectの引数axisベクトルが正規化されていません。

    //共通して用いる副次式を計算する

	dectype	ax = -2.0f * n.X;
	dectype	ay = -2.0f * n.Y;
	dectype	az = -2.0f * n.Z;

    //行列の要素を埋める
    //ここで共通して用いる副次式による最適化を行う
    //対角線上で反対の行列の要素が等しいからである

	M11 = 1.0f + ax*n.X;
	M22 = 1.0f + ay*n.Y;
	M32 = 1.0f + az*n.Z;

	M12 = M21 = ax*n.Y;
	M13 = M31 = ax*n.Z;
	M23 = M32 = ay*n.Z;

    //平行移動部分をリセットする

	Tx = Ty = Tz = 0.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupPosToUv()
{
	M11 = 0.5f;
	M12 = 0.0f;
	M13 = 0.0f;
	M14 = 0.0f;

	M21 = 0.0f;
	M22 = -0.5f;
	M23 = 0.0f;
	M24 = 0.0f;

	M31 = 0.0f;
	M32 = 0.0f;
	M33 = 1.0f;
	M34 = 0.0f;

	Tx  = 0.5f;
	Ty  = 0.5f;
	Tz  = 0.0f;
	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupViewLH( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &up )
{
	math::CBasicVector3 zaxis = ( tag - eye ).Normalize();
	math::CBasicVector3 xaxis = ( math::Cross( up , zaxis ) ).Normalize();
	math::CBasicVector3 yaxis = math::Cross( zaxis , xaxis );

	M11 = ( FLOAT )xaxis.X;
	M12 = ( FLOAT )yaxis.X;
	M13 = ( FLOAT )zaxis.X;
	M14 = 0;

	M21 = ( FLOAT )xaxis.Y;
	M22 = ( FLOAT )yaxis.Y;
	M23 = ( FLOAT )zaxis.Y;
	M24 = 0;

	M31 = ( FLOAT )xaxis.Z;
	M32 = ( FLOAT )yaxis.Z;
	M33 = ( FLOAT )zaxis.Z;
	M34 = 0;

	Tx  = -( FLOAT )xaxis.Dot( eye );
	Ty  = -( FLOAT )yaxis.Dot( eye );
	Tz  = -( FLOAT )zaxis.Dot( eye );
	Tw  = 1;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupViewRH( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &up )
{//D3DXMatrixLookAtRH 関数の処理を参考に記述

	math::CBasicVector3 zaxis = ( eye - tag ).Normalize();
	math::CBasicVector3 xaxis = ( math::Cross( up , zaxis ) ).Normalize();
	math::CBasicVector3 yaxis = math::Cross( zaxis , xaxis );

	M11 = ( f32 )xaxis.X;
	M12 = ( f32 )yaxis.X;
	M13 = ( f32 )zaxis.X;
	M14 = 0;

	M21 = ( f32 )xaxis.Y;
	M22 = ( f32 )yaxis.Y;
	M23 = ( f32 )zaxis.Y;
	M24 = 0;

	M31 = ( f32 )xaxis.Z;
	M32 = ( f32 )yaxis.Z;
	M33 = ( f32 )zaxis.Z;
	M34 = 0;

	Tx  = -( f32 )xaxis.Dot( eye );
	Ty  = -( f32 )yaxis.Dot( eye );
	Tz  = -( f32 )zaxis.Dot( eye );
	Tw  = 1;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProjectionLH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf )
{
	SetupProjectionLH( fovY , width / height , zn , zf );

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProjectionRH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf )
{//D3DXMatrixPerspectiveFovRH 関数の処理を参考に記述
	SetupProjectionRH( fovY , width / height , zn , zf );

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProjectionLH( dectype fovY , dectype aspect , dectype zn , dectype zf )
{
	FLOAT h = ( FLOAT )CMathUtil_::Ctan( fovY / 2 );
	FLOAT w = ( FLOAT )( h / ( FLOAT )aspect );

	M11 = w;
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = h;
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = ( FLOAT )( zf / ( zf - zn ) );
	M34 = 1;

	Tx  = 0;
	Ty  = 0;
	Tz  = ( FLOAT )( -zn * zf / ( zf - zn ) );
	Tw  = 0;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProjectionRH( dectype fovY , dectype aspect , dectype zn , dectype zf )
{//D3DXMatrixPerspectiveFovRH 関数の処理を参考に記述

	FLOAT h = ( FLOAT )CMathUtil_::Ctan( fovY / 2 );
	FLOAT w = ( FLOAT )( h / ( FLOAT )aspect );

	M11 = w;
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = h;
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = ( f32 )( zf / ( zn - zf ) );
	M34 = -1.0f;

	Tx  = 0;
	Ty  = 0;
	Tz  = ( f32 )( zn * zf / ( zn - zf ) );
	Tw  = 0;

	return *this;
}


inline CMatrix4x3& CMatrix4x3::SetupProjectionOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf )
{//D3DXMatrixPerspectiveOffCenterLHを参考
	M11 = 2*zn/(right-left);
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = 2*zn/(top-bottom);
	M23 = 0;
	M24 = 0;

	M31 = (left+right)/(left-right);
	M32 = (top+bottom)/(bottom-top);
	M33 = zf/(zf-zn);
	M34 = 1;

	Tx  = 0;
	Ty  = 0;
	Tz  = zn*zf/(zn-zf);
	Tw  = 0;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupProjectionOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf )
{//D3DXMatrixPerspectiveOffCenterRHを参考
	M11 = 2*zn/(right-left);
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = 2*zn*(top-bottom);
	M23 = 0;
	M24 = 0;

	M31 = (left+right)/(right-left);
	M32 = (top+bottom)/(top-bottom);
	M33 = zf/(zn-zf);
	M34 = -1;

	Tx  = 0;
	Ty  = 0;
	Tz  = zn*zf/(zn-zf);
	Tw  = 0;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupOrthoMatrixLH( dectype width , dectype height , dectype zn , dectype zf )
{
	M11 = ( 2 / width );
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = ( 2 / height );
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = ( 1 / ( zf - zn ) );
	M34 = 0;

	Tx  = 0;
	Ty  = 0;
	Tz  = ( zn / ( zn - zf ) );
	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupOrthoMatrixRH( dectype width , dectype height , dectype zn , dectype zf )
{//D3DXMatrixOrthoRH 関数の処理を参考に記述

	M11 = ( 2 / width );
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = ( 2 / height );
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = ( 1 / ( zn - zf ) );
	M34 = 0;

	Tx  = 0;
	Ty  = 0;
	Tz  = ( zn / ( zn - zf ) );
	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupOrthoMatrixOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf )
{//D3DXMatrixOrthoOffCenterLH関数の処理を参考に記述
	M11 = 2/(right-left);
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = 2/(top-bottom);
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = 1/(zf-zn);
	M34 = 0;

	Tx  = (left+right)/(left-right);
	Ty  = (top+bottom)/(bottom-top);
	Tz  = zn/(zn-zf);
	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupOrthoMatrixOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf )
{//D3DXMatrixOrthoOffCenterRH関数の処理を参考に記述
	M11 = 2/(right-left);
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = 2/(top-bottom);
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = 1/(zn-zf);
	M34 = 0;

	Tx  = (left+right)/(left-right);
	Ty  = (top+bottom)/(bottom-top);
	Tz  = zn/(zn-zf);
	Tw  = 1.0f;

	return *this;
}

inline CMatrix4x3& CMatrix4x3::SetupTexturePositionMatrix()
{
	M11 = 0.5f;
	M12 = 0;
	M13 = 0;
	M14 = 0;

	M21 = 0;
	M22 = -0.5f;
	M23 = 0;
	M24 = 0;

	M31 = 0;
	M32 = 0;
	M33 = 1.0f;
	M34 = 0;

	Tx  = 0.5f;
	Ty  = 0.5f;
	Tz  = 0;
	Tw  = 1.0f;

	return *this;
}

inline CBasicVector3 &CMatrix4x3::GetCol0( CBasicVector3 &dest ) const 
{
	dest.Set( M11 , M12 , M13 );
	return dest;
}

inline CBasicVector3 &CMatrix4x3::GetCol1( CBasicVector3 &dest ) const
{
	dest.Set( M21 , M22 , M23 );
	return dest;
}

inline CBasicVector3 &CMatrix4x3::GetCol2( CBasicVector3 &dest ) const
{
	dest.Set( M31 , M32 , M33 );
	return dest;
}

inline CBasicVector3 &CMatrix4x3::GetCol3( CBasicVector3 &dest ) const
{
	dest.Set( Tx , Ty , Tz );
	return dest;
}

inline CMatrix4x3& CMatrix4x3::SetCol0( CBasicVector3 &src , dectype w )
{
	M11 = src.X;
	M12 = src.Y;
	M13 = src.Z;
	M14 = w;
	return *this;
}
inline CMatrix4x3& CMatrix4x3::SetCol1( CBasicVector3 &src , dectype w )
{
	M21 = src.X;
	M22 = src.Y;
	M23 = src.Z;
	M24 = w;
	return *this;
}
inline CMatrix4x3& CMatrix4x3::SetCol2( CBasicVector3 &src , dectype w )
{
	M31 = src.X;
	M32 = src.Y;
	M33 = src.Z;
	M34 = w;
	return *this;
}
inline CMatrix4x3& CMatrix4x3::SetCol3( CBasicVector3 &src , dectype w )
{
	Tx = src.X;
	Ty = src.Y;
	Tz = src.Z;
	Tw = w;
	return *this;
}

} } }


inline sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m )
{

    //線形代数によりすりつぶす

	return sky::lib::math::CBasicVector3(
		p.X*m.M11 + p.Y*m.M21 + p.Z*m.M31 + m.Tx ,
		p.X*m.M12 + p.Y*m.M22 + p.Z*m.M32 + m.Ty ,
		p.X*m.M13 + p.Y*m.M23 + p.Z*m.M33 + m.Tz
	);
}

inline sky::lib::math::CBasicVector3 &operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m )
{
	p = p * m;
	return p;
}

inline sky::lib::math::CMatrix4x3 operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2 )
{

	sky::lib::math::CMatrix4x3 r;

    //上部の３×３（線形変換）部分を計算する

	r.M11 = m1.M11 * m2.M11 + m1.M12 * m2.M21 + m1.M13 * m2.M31;
	r.M12 = m1.M11 * m2.M12 + m1.M12 * m2.M22 + m1.M13 * m2.M32;
	r.M13 = m1.M11 * m2.M13 + m1.M12 * m2.M23 + m1.M13 * m2.M33;
	r.M14 = 0;

	r.M21 = m1.M21 * m2.M11 + m1.M22 * m2.M21 + m1.M23 * m2.M31;
	r.M22 = m1.M21 * m2.M12 + m1.M22 * m2.M22 + m1.M23 * m2.M32;
	r.M23 = m1.M21 * m2.M13 + m1.M22 * m2.M23 + m1.M23 * m2.M33;
	r.M24 = 0;

	r.M31 = m1.M31 * m2.M11 + m1.M32 * m2.M21 + m1.M33 * m2.M31;
	r.M32 = m1.M31 * m2.M12 + m1.M32 * m2.M22 + m1.M33 * m2.M32;
	r.M33 = m1.M31 * m2.M13 + m1.M32 * m2.M23 + m1.M33 * m2.M33;
	r.M34 = 0;

	//平行移動部分を計算する

	r.Tx = m1.Tx  * m2.M11  + m1.Ty * m2.M21  + m1.Tz  * m2.M31 + m2.Tx;
	r.Ty = m1.Tx  * m2.M12  + m1.Ty * m2.M22  + m1.Tz  * m2.M32 + m2.Ty;
	r.Tz = m1.Tx  * m2.M13  + m1.Ty * m2.M23  + m1.Tz  * m2.M33 + m2.Tz;
	r.Tw = 1.0f;

	//結果を返す
	return r;
}

inline sky::lib::math::CMatrix4x3 &operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2 )
{
	m1 = m1 * m2;
	return m1;
}


inline sky::lib::math::CMatrix4x3 MulMatrix4x4( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 )
{
	//m1またはm2がdestと同じである可能性を考えて一旦バッファにrを貯めこむ（計算結果が狂うので）
	sky::lib::math::CMatrix4x3 r;

	r.M11 = m1.M11 * m2.M11 + m1.M12 * m2.M21 + m1.M13 * m2.M31;
	r.M12 = m1.M11 * m2.M12 + m1.M12 * m2.M22 + m1.M13 * m2.M32;
	r.M13 = m1.M11 * m2.M13 + m1.M12 * m2.M23 + m1.M13 * m2.M33;
	r.M14 = m1.M11 * m2.M14 + m1.M12 * m2.M24 + m1.M13 * m2.M34;

	r.M21 = m1.M21 * m2.M11 + m1.M22 * m2.M21 + m1.M23 * m2.M31;
	r.M22 = m1.M21 * m2.M12 + m1.M22 * m2.M22 + m1.M23 * m2.M32;
	r.M23 = m1.M21 * m2.M13 + m1.M22 * m2.M23 + m1.M23 * m2.M33;
	r.M24 = m1.M21 * m2.M14 + m1.M22 * m2.M24 + m1.M23 * m2.M34;

	r.M31 = m1.M31 * m2.M11 + m1.M32 * m2.M21 + m1.M33 * m2.M31;
	r.M32 = m1.M31 * m2.M12 + m1.M32 * m2.M22 + m1.M33 * m2.M32;
	r.M33 = m1.M31 * m2.M13 + m1.M32 * m2.M23 + m1.M33 * m2.M33;
	r.M34 = m1.M31 * m2.M14 + m1.M32 * m2.M24 + m1.M33 * m2.M34;

	r.Tx  = m1.Tx  * m2.M11 + m1.Ty  * m2.M21 + m1.Tz  * m2.M31 + m2.Tx;
	r.Ty  = m1.Tx  * m2.M12 + m1.Ty  * m2.M22 + m1.Tz  * m2.M32 + m2.Ty;
	r.Tz  = m1.Tx  * m2.M13 + m1.Ty  * m2.M23 + m1.Tz  * m2.M33 + m2.Tz;
	r.Tw  = m1.Tx  * m2.M14 + m1.Ty  * m2.M24 + m1.Tz  * m2.M34 + m2.Tw;

	return r;
}

inline sky::lib::math::CBasicVector3 MulMatrix4x4( const sky::lib::math::CBasicVector3 &v, const sky::lib::math::CMatrix4x3 &m )
{//通常の*演算子で行うのと違い、wの計算値を有効にして射影等の変換を生かす

	f32 x = m.M11 * v.X + m.M21 * v.Y + m.M31 * v.Z + m.Tx;
	f32 y = m.M12 * v.X + m.M22 * v.Y + m.M32 * v.Z + m.Ty;
	f32 z = m.M13 * v.X + m.M23 * v.Y + m.M33 * v.Z + m.Tz;
	f32 w = m.M14 * v.X + m.M24 * v.Y + m.M34 * v.Z + m.Tw;

	x /= w;
	y /= w;
	z /= w;
	return sky::lib::math::CBasicVector3( x, y, z );
}

inline sky::lib::math::CBasicVector4 MulMatrix4x4( const sky::lib::math::CBasicVector4 &v, const sky::lib::math::CMatrix4x3 &m )
{
	return sky::lib::math::CBasicVector4(
		( ( ( ( v.X * m.M11 ) + ( v.Y * m.M21 ) ) + ( v.Z * m.M31 ) ) + ( v.W * m.Tx ) ),
		( ( ( ( v.X * m.M12 ) + ( v.Y * m.M22 ) ) + ( v.Z * m.M32 ) ) + ( v.W * m.Ty ) ),
		( ( ( ( v.X * m.M13 ) + ( v.Y * m.M23 ) ) + ( v.Z * m.M33 ) ) + ( v.W * m.Tz ) ),
		( ( ( ( v.X * m.M14 ) + ( v.Y * m.M24 ) ) + ( v.Z * m.M34 ) ) + ( v.W * m.Tw ) ) );
}

inline sky::lib::math::CMatrix4x3 MulMatrix4x4Full( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 )
{
	//m1またはm2がdestと同じである可能性を考えて一旦バッファにrを貯めこむ（計算結果が狂うので）
	sky::lib::math::CMatrix4x3 r;

	for ( s32 i = 0 ; i < 4 ; i++ )
		for ( s32 j = 0 ; j < 4 ; j++ )
			r.m[ i ][ j ] = m1.m[ i ][ 0 ] * m2.m[ 0 ][ j ]
						  + m1.m[ i ][ 1 ] * m2.m[ 1 ][ j ]
						  + m1.m[ i ][ 2 ] * m2.m[ 2 ][ j ]
						  + m1.m[ i ][ 3 ] * m2.m[ 3 ][ j ];

	return r;
}

inline sky::lib::math::CMatrix4x3 MulMatrix4x4TransOnly( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 )
{
	//m1またはm2がdestと同じである可能性を考えて一旦バッファにrを貯めこむ（計算結果が狂うので）
	sky::lib::math::CMatrix4x3 r;

	//拡大縮小はm1のものをそのまま使用する

	r.M11 = m1.M11;
	r.M12 = m1.M12;
	r.M13 = m1.M13;
	r.M14 = m1.M14;

	r.M21 = m1.M21;
	r.M22 = m1.M22;
	r.M23 = m1.M23;
	r.M24 = m1.M24;

	r.M31 = m1.M31;
	r.M32 = m1.M32;
	r.M33 = m1.M33;
	r.M34 = m1.M34;

	//平行移動部分を計算する
	r.Tx = m1.Tx + m2.Tx;
	r.Ty = m1.Ty + m2.Ty;
	r.Tz = m1.Tz + m2.Tz;

	return r;
}

inline dectype	Determinant(const sky::lib::math::CMatrix4x3 &m)
{
	return
		  m.M11 * (m.M22*m.M33 - m.M23*m.M32)
		+ m.M12 * (m.M23*m.M31 - m.M21*m.M33)
		+ m.M13 * (m.M21*m.M32 - m.M22*m.M31);
}

inline dectype	Determinant4x4(const sky::lib::math::CMatrix4x3 &m)
{
	dectype det = m.M11*m.M22*m.M33*m.Tw + m.M11*m.M23*m.M34*m.Ty 
		+ m.M11*m.M24*m.M32*m.Tz	+ m.M12*m.M21*m.M34*m.Tz 
		+ m.M12*m.M23*m.M31*m.Tw	+ m.M12*m.M24*m.M33*m.Tx
		+ m.M13*m.M21*m.M32*m.Tw	+ m.M13*m.M22*m.M34*m.Tx

		+ m.M13*m.M24*m.M31*m.Ty + m.M14*m.M21*m.M33*m.Ty
		+ m.M14*m.M22*m.M31*m.Tz + m.M14*m.M23*m.M32*m.Tx
		- m.M11*m.M22*m.M34*m.Tz - m.M11*m.M23*m.M32*m.Tw
		- m.M11*m.M24*m.M33*m.Ty - m.M12*m.M21*m.M33*m.Tw

		- m.M12*m.M23*m.M34*m.Tx - m.M12*m.M24*m.M31*m.Tz
		- m.M13*m.M21*m.M34*m.Ty - m.M13*m.M22*m.M31*m.Tw
		- m.M13*m.M24*m.M32*m.Tx - m.M14*m.M21*m.M32*m.Tz
		- m.M14*m.M22*m.M33*m.Tx - m.M14*m.M23*m.M31*m.Ty;
	return det;

}

inline sky::lib::math::CMatrix4x3 Invert(const sky::lib::math::CMatrix4x3 &m)
{

	//行列式を計算する

	dectype	det = Determinant(m);

	SKY_ASSERT_MSG( sky::lib::math::CMathUtil::FAbs( det ) > 0.000001f , _T( "The matrix inverse doesn't exist for the singular matrix." ) ); //特異行列のため逆行列は存在しません。

    //行列式分の１（あるいは行列式の逆数）を計算する
    //これで、除算を１回だけにして、要素ごとには乗算することができる

	dectype	oneOverDet = 1.0f / det;

    //逆行列の３×３部分を計算する
    //この行列式により随伴行列を除算することで行う

	sky::lib::math::CMatrix4x3	r;

	r.M11 = ( m.M22 * m.M33 - m.M23 * m.M32 ) * oneOverDet;
	r.M12 = ( m.M13 * m.M32 - m.M12 * m.M33 ) * oneOverDet;
	r.M13 = ( m.M12 * m.M23 - m.M13 * m.M22 ) * oneOverDet;
	r.M14 = 0;

	r.M21 = ( m.M23 * m.M31 - m.M21 * m.M33 ) * oneOverDet;
	r.M22 = ( m.M11 * m.M33 - m.M13 * m.M31 ) * oneOverDet;
	r.M23 = ( m.M13 * m.M21 - m.M11 * m.M23 ) * oneOverDet;
	r.M24 = 0;

	r.M31 = ( m.M21 * m.M32 - m.M22 * m.M31 ) * oneOverDet;
	r.M32 = ( m.M12 * m.M31 - m.M11 * m.M32 ) * oneOverDet;
	r.M33 = ( m.M11 * m.M22 - m.M12 * m.M21 ) * oneOverDet;
	r.M34 = 0;

    //逆行列の平行移動部分を計算する

	r.Tx = -( m.Tx * r.M11 + m.Ty * r.M21 + m.Tz * r.M31 );
	r.Ty = -( m.Tx * r.M12 + m.Ty * r.M22 + m.Tz * r.M32 );
	r.Tz = -( m.Tx * r.M13 + m.Ty * r.M23 + m.Tz * r.M33 );
	r.Tw = 1.0f;

	return r;
}

inline sky::lib::math::CMatrix4x3 Invert4x4(const sky::lib::math::CMatrix4x3 &m)
{

	//行列式を計算する

	dectype	det = Determinant4x4(m);

	SKY_ASSERT_MSG( sky::lib::math::CMathUtil::FAbs( det ) > 0.000001f , _T( "The matrix inverse doesn't exist for the singular matrix." ) ); //特異行列のため逆行列は存在しません。

    //行列式分の１（あるいは行列式の逆数）を計算する
    //これで、除算を１回だけにして、要素ごとには乗算することができる

	dectype	oneOverDet = 1.0f / det;

	sky::lib::math::CMatrix4x3	r;

	r.M11  = oneOverDet * ( m.M22 * m.M33 * m.Tw  + m.M23 * m.M34 * m.Ty  + m.M24 * m.M32 * m.Tz  - m.M22 * m.M34 * m.Tz  - m.M23 * m.M32 * m.Tw  - m.M24 * m.M33 * m.Ty  );
	r.M12  = oneOverDet * ( m.M12 * m.M34 * m.Tz  + m.M13 * m.M32 * m.Tw  + m.M14 * m.M33 * m.Ty  - m.M12 * m.M33 * m.Tw  - m.M13 * m.M34 * m.Ty  - m.M14 * m.M32 * m.Tz  );
	r.M13  = oneOverDet * ( m.M12 * m.M23 * m.Tw  + m.M13 * m.M24 * m.Ty  + m.M14 * m.M22 * m.Tz  - m.M12 * m.M24 * m.Tz  - m.M13 * m.M22 * m.Tw  - m.M14 * m.M23 * m.Ty  );
	r.M14  = oneOverDet * ( m.M12 * m.M24 * m.M33 + m.M13 * m.M22 * m.M34 + m.M14 * m.M23 * m.M32 - m.M12 * m.M23 * m.M34 - m.M13 * m.M24 * m.M32 - m.M14 * m.M22 * m.M33 );
 
	r.M21  = oneOverDet * ( m.M21 * m.M34 * m.Tz  + m.M23 * m.M31 * m.Tw  + m.M24 * m.M33 * m.Tx  - m.M21 * m.M33 * m.Tw  - m.M23 * m.M34 * m.Tx  - m.M24 * m.M31 * m.Tz  );
	r.M22  = oneOverDet * ( m.M11 * m.M33 * m.Tw  + m.M13 * m.M34 * m.Tx  + m.M14 * m.M31 * m.Tz  - m.M11 * m.M34 * m.Tz  - m.M13 * m.M31 * m.Tw  - m.M14 * m.M33 * m.Tx  );
	r.M23  = oneOverDet * ( m.M11 * m.M24 * m.Tz  + m.M13 * m.M21 * m.Tw  + m.M14 * m.M23 * m.Tx  - m.M11 * m.M23 * m.Tw  - m.M13 * m.M24 * m.Tx  - m.M14 * m.M21 * m.Tz  );
	r.M24  = oneOverDet * ( m.M11 * m.M23 * m.M34 + m.M13 * m.M24 * m.M31 + m.M14 * m.M21 * m.M33 - m.M11 * m.M24 * m.M33 - m.M13 * m.M21 * m.M34 - m.M14 * m.M23 * m.M31 );
 
	r.M31  = oneOverDet * ( m.M21 * m.M32 * m.Tw  + m.M22 * m.M34 * m.Tx  + m.M24 * m.M31 * m.Ty  - m.M21 * m.M34 * m.Ty  - m.M22 * m.M31 * m.Tw  - m.M24 * m.M32 * m.Tx  );
	r.M32  = oneOverDet * ( m.M11 * m.M34 * m.Ty  + m.M12 * m.M31 * m.Tw  + m.M14 * m.M32 * m.Tx  - m.M11 * m.M32 * m.Tw  - m.M12 * m.M34 * m.Tx  - m.M14 * m.M31 * m.Ty  );
	r.M33  = oneOverDet * ( m.M11 * m.M22 * m.Tw  + m.M12 * m.M24 * m.Tx  + m.M14 * m.M21 * m.Ty  - m.M11 * m.M24 * m.Ty  - m.M12 * m.M21 * m.Tw  - m.M14 * m.M22 * m.Tx  );
	r.M34  = oneOverDet * ( m.M11 * m.M24 * m.M32 + m.M12 * m.M21 * m.M34 + m.M14 * m.M22 * m.M31 - m.M11 * m.M22 * m.M34 - m.M12 * m.M24 * m.M31 - m.M14 * m.M21 * m.M32 );
 
	r.Tx   = oneOverDet * ( m.M21 * m.M33 * m.Ty  + m.M22 * m.M31 * m.Tz  + m.M23 * m.M32 * m.Tx  - m.M21 * m.M32 * m.Tz  - m.M22 * m.M33 * m.Tx  - m.M23 * m.M31 * m.Ty  );
	r.Ty   = oneOverDet * ( m.M11 * m.M32 * m.Tz  + m.M12 * m.M33 * m.Tx  + m.M13 * m.M31 * m.Ty  - m.M11 * m.M33 * m.Ty  - m.M12 * m.M31 * m.Tz  - m.M13 * m.M32 * m.Tx  );
	r.Tz   = oneOverDet * ( m.M11 * m.M23 * m.Ty  + m.M12 * m.M21 * m.Tz  + m.M13 * m.M22 * m.Tx  - m.M11 * m.M22 * m.Tz  - m.M12 * m.M23 * m.Tx  - m.M13 * m.M21 * m.Ty  );
	r.Tw   = oneOverDet * ( m.M11 * m.M22 * m.M33 + m.M12 * m.M23 * m.M31 + m.M13 * m.M21 * m.M32 - m.M11 * m.M23 * m.M32 - m.M12 * m.M21 * m.M33 - m.M13 * m.M22 * m.M31 );

	return r;
}

inline sky::lib::math::CBasicVector3	GetTranslationVector(const sky::lib::math::CMatrix4x3 &m)
{
	return sky::lib::math::CBasicVector3(m.Tx , m.Ty, m.Tz);
}

inline sky::lib::math::CBasicVector3	getPositionFromParentToLocalMatrix(const sky::lib::math::CMatrix4x3 &m)
{

    //３×３の部分の転置と負の平行移動値を乗算する
    //転置を用いるので、行列は直行行列であることが前提である
    //この関数は、実際には剛体でない座標変換では意味がない
	return sky::lib::math::CBasicVector3(
		-(m.Tx*m.M11 + m.Ty*m.M12 + m.Tz*m.M13) ,
		-(m.Tx*m.M21 + m.Ty*m.M22 + m.Tz*m.M23) ,
		-(m.Tx*m.M31 + m.Ty*m.M32 + m.Tz*m.M33)
	);
}

inline sky::lib::math::CBasicVector3	getPositionFromLocalToParentMatrix(const sky::lib::math::CMatrix4x3 &m)
{
	//位置は、単に平行移動部分になる
	return sky::lib::math::CBasicVector3(m.Tx , m.Ty, m.Tz);
}
