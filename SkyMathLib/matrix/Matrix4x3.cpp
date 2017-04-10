#include "StdAfx.h"
#include "SkyMathLib/Matrix.h"

namespace sky { namespace lib { namespace math {

CMatrix4x3& CMatrix4x3::SetupLocalToParent(const CBasicVector3 &pos , const CEulerAngles &orient)
{
    //回転行列を作成する

	CRotationMatrix orientMatrix;
	orientMatrix.Setup(orient);

    //４×３行列をセットアップする
    //注意：速度が気になる場合は、一時的なRotationMatrixオブジェクトを用いることなく
    //これらの変数に直接行列を作成することができる
    //そうすれば、一回の関数呼び出しと２～３のコピー操作を節約できる

	SetupLocalToParent( pos , orientMatrix );

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupLocalToParent(const CBasicVector3 &pos , const CRotationMatrix &orient)
{
	Identity();

    //行列の回転分をコピーする
    //回転は通常、慣性空間→オブジェクト空間への行列であり、すなわち、親→ローカルである
    //ローカル→親への回転が欲しいので、コピー中に転置する必要がある

	M11 = orient.M11; M12 = orient.M21; M13 = orient.M31;
	M21 = orient.M12; M22 = orient.M22; M23 = orient.M32;
	M31 = orient.M13; M32 = orient.M23; M33 = orient.M33;

    //平行移動部分を設定する
    //平行移動は、３×３の部分の後ろで起こるので、
    //この部分のフィールドを直接コピーすることができる

	Tx = pos.X; Ty = pos.Y; Tz = pos.Z;

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupParentToLocal(const CBasicVector3 &pos , const CEulerAngles &orient)
{
    //回転行列を作成する

	CRotationMatrix orientMatrix;
	orientMatrix.Setup( orient );

    //４×３行列をセットアップする

	SetupParentToLocal( pos , orientMatrix );

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupParentToLocal(const CBasicVector3 &pos , const CRotationMatrix &orient)
{
	Identity();

    //この行列の回転部分をコピーする
    //この要素は、レイアウトに従って、転置することなく直接コピーできる

	M11 = orient.M11; M12 = orient.M12; M13 = orient.M13;
	M21 = orient.M21; M22 = orient.M22; M23 = orient.M23;
	M31 = orient.M31; M32 = orient.M32; M33 = orient.M33;

    //ここで、平行移動分を設定する
    //通常、この位置分だけ逆に平行移動し、ワールド空間から慣性空間へ平行移動する
    //しかしながら、この回転が最初に起こるという事実を補正する必要がある
    //つまり、平行移動部分を回転する必要がある
    //これは、-posだけ並行移動する行列Ｔと回転行列Ｒを作成し、
    //連結したＴＲ行列を作成することと同じである

	Tx = -(pos.X*M11 + pos.Y*M21 + pos.Z*M31);
	Ty = -(pos.X*M12 + pos.Y*M22 + pos.Z*M32);
	Tz = -(pos.X*M13 + pos.Y*M23 + pos.Z*M33);

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupRotate( const CRotationMatrix &rotmat )
{
	Identity();

	M11 = rotmat.M11;
	M12 = rotmat.M12;
	M13 = rotmat.M13;

	M21 = rotmat.M21;
	M22 = rotmat.M22;
	M23 = rotmat.M23;

	M31 = rotmat.M31;
	M32 = rotmat.M32;
	M33 = rotmat.M33;

	//平行移動部分をリセットする

	Tx = Ty = Tz = 0.0f;

	return *this;
}


CMatrix4x3& CMatrix4x3::SetupRotate( const CQuaternion &q )
{
	CRotationMatrix rotMat;
	rotMat.FromInertialToObjectQuaternion( q );

	SetupRotate( rotMat );

	return *this;
}

const CRotationMatrix &CMatrix4x3::CreateRotateMatrix( CRotationMatrix &r ) const
{
	r.M11 = M11;
	r.M12 = M12;
	r.M13 = M13;

	r.M21 = M21;
	r.M22 = M22;
	r.M23 = M23;

	r.M31 = M31;
	r.M32 = M32;
	r.M33 = M33;

	return r;
}

void CMatrix4x3::DebugPrint() const
{
	SKY_PRINTF( _T( "[ 11[ %+-08.3f ] , 12[ %+-08.3f ] , 13[ %+-08.3f ] , 14[ %+-08.3f ] ] \n" ) , M11 , M12 , M13 , M14 );
	SKY_PRINTF( _T( "[ 21[ %+-08.3f ] , 22[ %+-08.3f ] , 23[ %+-08.3f ] , 24[ %+-08.3f ] ] \n" ) , M21 , M22 , M23 , M24 );
	SKY_PRINTF( _T( "[ 31[ %+-08.3f ] , 32[ %+-08.3f ] , 33[ %+-08.3f ] , 34[ %+-08.3f ] ] \n" ) , M31 , M32 , M33 , M34 );
	SKY_PRINTF( _T( "[ 41[ %+-08.3f ] , 42[ %+-08.3f ] , 43[ %+-08.3f ] , 44[ %+-08.3f ] ] \n" ) , Tx  , Ty  , Tz  , Tw );
}

void CMatrix4x3::IdentityArray( f32 *dest )
{
	dest[ 0 ]  = 1.0f; dest[ 1 ]  = 0.0f; dest[ 2 ]  = 0.0f; dest[ 3 ]  = 0.0f;
	dest[ 4 ]  = 0.0f; dest[ 5 ]  = 1.0f; dest[ 6 ]  = 0.0f; dest[ 7 ]  = 0.0f;
	dest[ 8 ]  = 0.0f; dest[ 9 ]  = 0.0f; dest[ 10 ] = 1.0f; dest[ 11 ] = 0.0f;
	dest[ 12 ] = 0.0f; dest[ 13 ] = 0.0f; dest[ 14 ] = 0.0f; dest[ 15 ] = 1.0f;
	return;
}

} } }

sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m )
{

    //線形代数によりすりつぶす

	return sky::lib::math::CBasicVector3(
		p.X*m.M11 + p.Y*m.M21 + p.Z*m.M31 ,
		p.X*m.M12 + p.Y*m.M22 + p.Z*m.M32 ,
		p.X*m.M13 + p.Y*m.M23 + p.Z*m.M33
	);
}

sky::lib::math::CBasicVector3 &operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m )
{
	p = p * m;
	return p;
}

sky::lib::math::CMatrix4x3 operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 )
{

	sky::lib::math::CMatrix4x3 r;

    //上部の３×３（線形変換）部分を計算する

	r.M11 = m1.M11*m2.M11 + m1.M12*m2.M21 + m1.M13*m2.M31;
	r.M12 = m1.M11*m2.M12 + m1.M12*m2.M22 + m1.M13*m2.M32;
	r.M13 = m1.M11*m2.M13 + m1.M12*m2.M23 + m1.M13*m2.M33;
	r.M14 = 0;

	r.M21 = m1.M21*m2.M11 + m1.M22*m2.M21 + m1.M23*m2.M31;
	r.M22 = m1.M21*m2.M12 + m1.M22*m2.M22 + m1.M23*m2.M32;
	r.M23 = m1.M21*m2.M13 + m1.M22*m2.M23 + m1.M23*m2.M33;
	r.M24 = 0;

	r.M31 = m1.M31*m2.M11 + m1.M32*m2.M21 + m1.M33*m2.M31;
	r.M32 = m1.M31*m2.M12 + m1.M32*m2.M22 + m1.M33*m2.M32;
	r.M33 = m1.M31*m2.M13 + m1.M32*m2.M23 + m1.M33*m2.M33;
	r.M34 = 0;

	//平行移動部分単なるコピー
	r.Tx = m1.Tx;
	r.Ty = m1.Ty;
	r.Tz = m1.Tz;
	r.Tw = m1.Tw;

	//結果を返す
	return r;
}

sky::lib::math::CMatrix4x3 operator*( const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CMatrix4x3 &m2 )
{

	sky::lib::math::CMatrix4x3 r;

    //上部の３×３（線形変換）部分を計算する

	r.M11 = m1.M11*m2.M11 + m1.M12*m2.M21 + m1.M13*m2.M31;
	r.M12 = m1.M11*m2.M12 + m1.M12*m2.M22 + m1.M13*m2.M32;
	r.M13 = m1.M11*m2.M13 + m1.M12*m2.M23 + m1.M13*m2.M33;
	r.M14 = 0;

	r.M21 = m1.M21*m2.M11 + m1.M22*m2.M21 + m1.M23*m2.M31;
	r.M22 = m1.M21*m2.M12 + m1.M22*m2.M22 + m1.M23*m2.M32;
	r.M23 = m1.M21*m2.M13 + m1.M22*m2.M23 + m1.M23*m2.M33;
	r.M24 = 0;

	r.M31 = m1.M31*m2.M11 + m1.M32*m2.M21 + m1.M33*m2.M31;
	r.M32 = m1.M31*m2.M12 + m1.M32*m2.M22 + m1.M33*m2.M32;
	r.M33 = m1.M31*m2.M13 + m1.M32*m2.M23 + m1.M33*m2.M33;
	r.M34 = 0;

	//結果を返す
	return r;
}

sky::lib::math::CMatrix4x3 &operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 )
{
	m1 = m1 * m2;
	return m1;
}

///////////////////////////////////////////////
// 回転行列→クォータニオン変換
///////////////////////////////////////////////
skyBool TransformRotMatToQuaternion(
    f32 &qx, f32 &qy, f32 &qz, f32 &qw,
    f32 m11, f32 m12, f32 m13,
    f32 m21, f32 m22, f32 m23,
    f32 m31, f32 m32, f32 m33
) {
    // 最大成分を検索
    f32 elem[ 4 ]; // 0:x, 1:y, 2:z, 3:w
    elem[ 0 ] = m11 - m22 - m33 + 1.0f;
    elem[ 1 ] = -m11 + m22 - m33 + 1.0f;
    elem[ 2 ] = -m11 - m22 + m33 + 1.0f;
    elem[ 3 ] = m11 + m22 + m33 + 1.0f;

    u32 biggestIndex = 0;
    for ( s32 i = 1; i < 4; i++ )
	{
        if ( elem[i] > elem[biggestIndex] )
            biggestIndex = i;
    }

    if ( elem[biggestIndex] < 0.0f )
        return skyFalse; // 引数の行列に間違いあり！

    // 最大要素の値を算出
    f32 *q[4] = {&qx, &qy, &qz, &qw};
    f32 v = sqrtf( elem[biggestIndex] ) * 0.5f;
    *q[biggestIndex] = v;
    f32 mult = 0.25f / v;

    switch ( biggestIndex )
	{
    case 0: // x
        *q[1] = (m12 + m21) * mult;
        *q[2] = (m31 + m13) * mult;
        *q[3] = (m23 - m32) * mult;
        break;
    case 1: // y
        *q[0] = (m12 + m21) * mult;
        *q[2] = (m23 + m32) * mult;
        *q[3] = (m31 - m13) * mult;
        break;
    case 2: // z
        *q[0] = (m31 + m13) * mult;
        *q[1] = (m23 + m32) * mult;
        *q[3] = (m12 - m21) * mult;
    break;
    case 3: // w
        *q[0] = (m23 - m32) * mult;
        *q[1] = (m31 - m13) * mult;
        *q[2] = (m12 - m21) * mult;
        break;
    }

    return skyTrue;
}

skyBool TransformRotMatToQuaternion( sky::lib::math::CQuaternion &dest , const sky::lib::math::CMatrix4x3 &src )
{
	return TransformRotMatToQuaternion( dest.X , dest.Y , dest.Z , dest.W , src.M11 , src.M12 , src.M13 , src.M21 , src.M22 , src.M23 , src.M31 , src.M32 , src.M33 );
}

//2つの行列の間の線形補間を計算し、結果の行列を返す 
// クォータニオン→回転行列＝D3DXMatrixRotationQuaternion      回転行列→クォータニオン＝D3DXQuaternionRotationMatrix
sky::lib::math::CMatrix4x3 Lerp(sky::lib::math::CMatrix4x3 a,sky::lib::math::CMatrix4x3 b,f32 t)
{
	sky::lib::math::CMatrix4x3 ret;
	ret.Identity();

	//それぞれの行列から平行移動成分を、それぞれベクトルに取り出す
	sky::lib::math::CBasicVector3 vA, vB;
	vA.X=a.Tx;
	vA.Y=a.Ty;
	vA.Z=a.Tz;
	vB.X=b.Tx;
	vB.Y=b.Ty;
	vB.Z=b.Tz;

	//それぞれの行列からスケーリング成分を、それぞれベクトルに取り出す
	sky::lib::math::CBasicVector3 vA1(a.M11,a.M12,a.M13);
	f32 Sax=vA1.Length();//スケーリングX成分
	sky::lib::math::CBasicVector3 vA2(a.M21,a.M22,a.M23);
	f32 Say=vA2.Length();//スケーリングY成分
	sky::lib::math::CBasicVector3 vA3(a.M31,a.M32,a.M33);
	f32 Saz=vA3.Length();//スケーリングZ成分

	sky::lib::math::CBasicVector3 vB1(b.M11,b.M12,b.M13);
	f32 Sbx=vB1.Length();//スケーリングX成分
	sky::lib::math::CBasicVector3 vB2(b.M21,b.M22,b.M23);
	f32 Sby=vB2.Length();//スケーリングY成分
	sky::lib::math::CBasicVector3 vB3(b.M31,b.M32,b.M33);
	f32 Sbz=vB3.Length();//スケーリングZ成分

	//それぞれの行列から回転成分を、それぞれクォータニオンに取り出す
	//回転とスケーリングは合成さｓれているため、回転を取り出す前に、行列からスケーリング成分を除去しなくてはならない。（それぞれスケーリング係数で行を割る）
	a.M11/=Sax;
	a.M12/=Sax;
	a.M13/=Sax;
	a.M21/=Say;
	a.M22/=Say;
	a.M23/=Say;
	a.M31/=Saz;
	a.M32/=Saz;
	a.M33/=Saz;

	b.M11/=Sbx;
	b.M12/=Sbx;
	b.M13/=Sbx;
	b.M21/=Sby;
	b.M22/=Sby;
	b.M23/=Sby;
	b.M31/=Sbz;
	b.M32/=Sbz;
	b.M33/=Sbz;

	sky::lib::math::CQuaternion qA(0,0,0,0);
	sky::lib::math::CQuaternion qB(0,0,0,0);

	TransformRotMatToQuaternion(qA,a);
	TransformRotMatToQuaternion(qB,b);
	//それぞれを、ｔの比率で線形補間する
	sky::lib::math::CBasicVector3 vR;
	sky::lib::interp::Linear( vR , vA , vB , t );
	sky::lib::math::CQuaternion qR;
	qR = Slerp( qA , qB , t );
	//ベクトルとクォータニオンを行列に戻す
	ret.SetupRotate( qR ); //ここがD3DXMatrixRotationQuaternionと違う結果を返す。
	ret.Tx=vR.X;
	ret.Ty=vR.Y;
	ret.Tz=vR.Z;
	ret.Tw=1.0;

	return ret;
}
