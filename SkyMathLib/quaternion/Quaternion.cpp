#include "StdAfx.h"
#include "SkyMathLib/Quaternion.h"

#include "SkyMathLib/Graph.h"

const sky::lib::math::CQuaternion skyQuaternionIdentity;

namespace sky { namespace lib { namespace math {

CQuaternion::CQuaternion()
{
	Identity();
}

CQuaternion::~CQuaternion()
{
}

CQuaternion::CQuaternion( dectype x , dectype y , dectype z , dectype w )
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

CQuaternion::CQuaternion( const dectype q[ 4 ] )
{
	X = q[ 0 ];
	Y = q[ 1 ];
	Z = q[ 2 ];
	W = q[ 3 ];
}

CQuaternion &CQuaternion::SetVector( const CBasicVector3 &vec )
{
	W = 0;
	X = vec.X;
	Y = vec.Y;
	Z = vec.Z;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutX( dectype radian )
{
    //半分の角度を計算する

	dectype	radianOver2 = radian * 0.5f;

    //値を設定する
	W = CMathUtil::Cos( radianOver2 );
	X = CMathUtil::Sin( radianOver2 );
	Y = 0.0f;
	Z = 0.0f;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutY( dectype radian )
{

    //半分の角度を計算する

	dectype	radianOver2 = radian * 0.5f;

    //値を設定する

	W = CMathUtil::Cos( radianOver2 );
	X = 0.0f;
	Y = CMathUtil::Sin( radianOver2 );
	Z = 0.0f;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutZ( dectype radian )
{

    //半分の角度を計算する

	dectype	radianOver2 = radian * 0.5f;

    //値を設定する

	W = CMathUtil::Cos( radianOver2 );
	X = 0.0f;
	Y = 0.0f;
	Z = CMathUtil::Sin( radianOver2 );

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutAxis(const CBasicVector3 &axis , dectype radian)
{
    //軸を指定するのに単位ベクトルを渡されたかどうかをチェックする
	SKY_ASSERT_MSG( CMathUtil::FAbs(VectorMag(axis) - 1.0f) < .01f , _T( "The vector is not Normalize. " ) , axis ); //引数axisベクトルが正規化されていません。

	//半分の角度を計算する

	dectype	radianOver2 = radian * 0.5f;
	dectype	sinradianOver2 = CMathUtil::Sin( radianOver2 );

    //値を設定する
	W = CMathUtil::Cos( radianOver2 );
	X = axis.X * sinradianOver2;
	Y = axis.Y * sinradianOver2;
	Z = axis.Z * sinradianOver2;

	return *this;
}

CQuaternion &CQuaternion::SetupVector( const CBasicVector3 &vec1 , const CBasicVector3 &vec2 )
{
	//なす角を求める
	dectype radian = vec1.IncludedAngle( vec2 );

	//外積を求める
	CBasicVector3 axis = Cross( vec1 , vec2 );

	SKY_ASSERT_MSG( axis.Length() > 0 , "cross is zero." );

	//外積を軸になす角分回転するクオータニオンを生成する
	SetToRotateAboutAxis( axis.Normalize() , radian );

	return *this;
}

CQuaternion &CQuaternion::SetToRotateObjectToInertial( const CEulerAngles &orientation )
{

    //半分の角のsinとcosを計算する

	dectype	sp, sb, sh;
	dectype	cp, cb, ch;
	CMathUtil::SinCos( &sp , &cp , orientation.Pitch * 0.5f );
	CMathUtil::SinCos( &sb , &cb , orientation.Bank * 0.5f );
	CMathUtil::SinCos( &sh , &ch , orientation.Heading * 0.5f );

    //値を計算する

	W =  ch * cp * cb + sh * sp * sb;
	X =  ch * sp * cb + sh * cp * sb;
	Y = -ch * sp * sb + sh * cp * cb;
	Z = -sh * sp * cb + ch * cp * sb;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateInertialToObject(const CEulerAngles &orientation)
{
    //半分の角のsinとcosを計算する

	dectype	sp, sb, sh;
	dectype	cp, cb, ch;
	CMathUtil::SinCos(&sp, &cp, orientation.Pitch * 0.5f);
	CMathUtil::SinCos(&sb, &cb, orientation.Bank * 0.5f);
	CMathUtil::SinCos(&sh, &ch, orientation.Heading * 0.5f);

    //値を計算する

	W =  ch*cp*cb + sh*sp*sb;
	X = -ch*sp*cb - sh*cp*sb;
	Y =  ch*sp*sb - sh*cb*cp;
	Z =  sh*sp*cb - ch*cp*sb;

	return *this;
}

CQuaternion CQuaternion::operator *(const CQuaternion &a) const
{

	CQuaternion result;

	result.W = W*a.W - X*a.X - Y*a.Y - Z*a.Z;
	result.X = W*a.X + X*a.W + Z*a.Y - Y*a.Z;
	result.Y = W*a.Y + Y*a.W + X*a.Z - Z*a.X;
	result.Z = W*a.Z + Z*a.W + Y*a.X - X*a.Y;

	return result;
}

CQuaternion &CQuaternion::operator *=(const CQuaternion &a)
{

	// Multuiply and assign

	*this = *this * a;

	// Return reference to l-value

	return *this;
}

CQuaternion &CQuaternion::operator *( const dectype &v )
{
	W *= v;
	X *= v;
	Y *= v;
	Z *= v;

	return *this;
}

CQuaternion &CQuaternion::operator +=( const CQuaternion &q )
{
	W += q.W;
	X += q.X;
	Y += q.Y;
	Z += q.Z;

	return *this;
}

CQuaternion &CQuaternion::Normalize()
{
    //四元数の大きさを計算する

	dectype	mag = ( dectype )CMathUtil::Sqrt(W*W + X*X + Y*Y + Z*Z);

    //0除算から保護するために無効な長さをチェックする

	if (mag > 0.0f) {

		//正規化する
		dectype	oneOverMag = 1.0f / mag;
		W *= oneOverMag;
		X *= oneOverMag;
		Y *= oneOverMag;
		Z *= oneOverMag;

	}
	else
	{

        //ヒューストン、問題が発生した。（なにこれジョーク？）

		//正規化チェック
        SKY_ASSERT_MSG( skyFalse , _T( "The value is 0." ) );

		Identity();
	}

	return *this;
}

dectype	CQuaternion::GetRotationAngle() const
{

    //半分の角度を計算する
    // w = cos(radian /2)であることに注意

	dectype radianOver2 = CMathUtil::SafeAcos(W);

    //回転角度を返す

	return radianOver2 * 2.0f;
}

CQuaternion	&CQuaternion::Setup( const CBasicVector3 &vec )
{
	//ＸＺ平面における回転を算出
	CBasicVector2 baseK;
	CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( baseK , CBasicVector2( 0 , 1 ) );

	CQuaternion rotY;
	CBasicVector2 tempXZ( vec.X , vec.Z );
	if ( tempXZ.Length() > 0 )
	{
		CBasicVector2 tempXZK;
		CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( tempXZK , tempXZ );
		dectype radY = baseK.Y - tempXZK.Y;
		rotY.SetToRotateAboutY( radY );
	}

	//ＹＺ平面における回転を算出
	CQuaternion rotX;
	CBasicVector2 tempYZ( vec.Y , math::CMathUtil::Abs( vec.Z ) );
	if ( tempYZ.Length() > 0 )
	{
		CBasicVector2 tempYZK;
		CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( tempYZK , tempYZ );
		dectype radX = tempYZK.Y - baseK.Y;
		rotX.SetToRotateAboutX( radX );
	}

	*this = rotX * rotY;

	return *this;
}

CBasicVector3	CQuaternion::GetRotationAxis() const
{

    //sin^2(radian/2)を計算する
    //w = cos(radian/2)、sin^2(x) + cos^(x) = 1であることに注意

	dectype sinradianOver2Sq = 1.0f - W*W;

    //数値の不正確さから身を守る
	if (sinradianOver2Sq <= 0.0f) {

        //恒等四元数、または不正確な数値
        //任意の有効なベクトルを返すだけ（どうでもよいからである）
		return CBasicVector3(1.0f , 0.0f , 0.0f);
	}

    //　1/sin(radian/2)を計算する
	dectype	oneOverSinradianOver2 = 1.0f / CMathUtil::Sqrt(sinradianOver2Sq);

    //回転の軸を返す
	return CBasicVector3(
		X * oneOverSinradianOver2,
		Y * oneOverSinradianOver2,
		Z * oneOverSinradianOver2
	);
}

} } }

dectype DotProduct(const sky::lib::math::CQuaternion &a , const sky::lib::math::CQuaternion &b)
{
	return a.W*b.W + a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}

sky::lib::math::CQuaternion Slerp(const sky::lib::math::CQuaternion &q0, const sky::lib::math::CQuaternion &q1, dectype t)
{
    //範囲外のパラメーターをチェックし、その場合は端点を返す

	if (t <= 0.0f) return q0;
	if (t >= 1.0f) return q1;

	//内積をもちいて四元数間の角度のcosを計算する

	dectype cosOmega = DotProduct(q0, q1);

    //負の内積の場合、-q2を用いる
    //２つの四元数qと-qは同じ回転を表すが、ことなる球面線形補完を生成する場合がある
    //正確な角度を用いて回転するためにqか-qを選択する

	dectype q1w = q1.W;
	dectype q1x = q1.X;
	dectype q1y = q1.Y;
	dectype q1z = q1.Z;
	if (cosOmega < 0.0f) {
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
	}

    //２つの四元数は単位四元数である必要があり、内積は<=1.0になるはずである
	SKY_ASSERT_MSG( cosOmega < 1.1f , _T( "The product in two Quaternion has exceeded 1.0d. ") ); //２つのクォータニオンの内積が1.0dを超えてしまいました。

    //補完用のフラクションを計算する
    //四元数がほとんどまったく同じであるがチェックする

	dectype k0, k1;
	if (cosOmega > 0.9999f) {

		//非常に近い --- 単純に線形補間を用いる（これは0除算を防ぐため）

		k0 = 1.0f-t;
		k1 = t;

	} else {

        //三角法の等式sin^2(omega) + cos^2(omega) = 1を用いて
        //この角度のsinを計算する

		dectype sinOmega = sky::lib::math::CMathUtil::Sqrt(1.0f - cosOmega*cosOmega);

		 //このsinとcosから角度を計算する

		dectype omega = sky::lib::math::CMathUtil::Atan2(sinOmega , cosOmega);

		//除算が1回で済むように、分母の逆数を計算する

		dectype oneOverSinOmega = 1.0f / sinOmega;

		 //補間パラメーターを計算する

		k0 = sky::lib::math::CMathUtil::Sin( (1.0f - t) * omega) * oneOverSinOmega;
		k1 = sky::lib::math::CMathUtil::Sin(t * omega) * oneOverSinOmega;
	}

	//補完する

	sky::lib::math::CQuaternion result;
	result.X = k0*q0.X + k1*q1x;
	result.Y = k0*q0.Y + k1*q1y;
	result.Z = k0*q0.Z + k1*q1z;
	result.W = k0*q0.W + k1*q1w;

	// Return it

	return result;
}

sky::lib::math::CQuaternion Conjugate(const sky::lib::math::CQuaternion &q)
{
	sky::lib::math::CQuaternion result;

	//同じ回転量

	result.W = q.W;

	 //反対の回転軸

	result.X = -q.X;
	result.Y = -q.Y;
	result.Z = -q.Z;

	//結果を返す

	return result;
}

sky::lib::math::CQuaternion Pow(const sky::lib::math::CQuaternion &q, dectype exponent)
{

    //単位四元数の場合をチェックする
    //これは０除算を防ぐためである

	if (sky::lib::math::CMathUtil::FAbs(q.W) > .9999f) {
		return q;
	}

    //半分の角度alpha(alpha = radian/2)を取り出す

	dectype	alpha = sky::lib::math::CMathUtil::Acos(q.W);

    //新しいalpha値を計算する

	dectype	newAlpha = alpha * exponent;

    //新しいw値を計算する

	sky::lib::math::CQuaternion result;
	result.W = sky::lib::math::CMathUtil::Cos(newAlpha);

    //新しいxyz値を計算する

	dectype	mult = sky::lib::math::CMathUtil::Sin(newAlpha) / sky::lib::math::CMathUtil::Sin(alpha);
	result.X = q.X * mult;
	result.Y = q.Y * mult;
	result.Z = q.Z * mult;

	// Return it

	return result;
}

sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &vec, const sky::lib::math::CQuaternion &q )
{
	//共役四元数を求める
	sky::lib::math::CQuaternion rq = Conjugate( q );

	//ベクトルを四元数にする
	sky::lib::math::CQuaternion vq;
	vq.SetVector( vec );

	//掛け算をする
	sky::lib::math::CQuaternion rvq = q * vq * rq;

	//四元数をベクトルに変換して返す
	return sky::lib::math::CBasicVector3( rvq.X , rvq.Y , rvq.Z );
}

sky::lib::math::CBasicVector3 &operator*=(sky::lib::math::CBasicVector3 &vec, const sky::lib::math::CQuaternion &q)
{
	vec = vec * q;
	return vec;
}

sky::lib::math::CQuaternion   	operator*( const sky::lib::math::CQuaternion &q , dectype val )
{
	sky::lib::math::CQuaternion r;

	r.W = q.W * val;
	r.X = q.X * val;
	r.Y = q.Y * val;
	r.Z = q.Z * val;

	return r;
}
