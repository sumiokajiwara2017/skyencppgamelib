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
    //�����̊p�x���v�Z����

	dectype	radianOver2 = radian * 0.5f;

    //�l��ݒ肷��
	W = CMathUtil::Cos( radianOver2 );
	X = CMathUtil::Sin( radianOver2 );
	Y = 0.0f;
	Z = 0.0f;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutY( dectype radian )
{

    //�����̊p�x���v�Z����

	dectype	radianOver2 = radian * 0.5f;

    //�l��ݒ肷��

	W = CMathUtil::Cos( radianOver2 );
	X = 0.0f;
	Y = CMathUtil::Sin( radianOver2 );
	Z = 0.0f;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutZ( dectype radian )
{

    //�����̊p�x���v�Z����

	dectype	radianOver2 = radian * 0.5f;

    //�l��ݒ肷��

	W = CMathUtil::Cos( radianOver2 );
	X = 0.0f;
	Y = 0.0f;
	Z = CMathUtil::Sin( radianOver2 );

	return *this;
}

CQuaternion &CQuaternion::SetToRotateAboutAxis(const CBasicVector3 &axis , dectype radian)
{
    //�����w�肷��̂ɒP�ʃx�N�g����n���ꂽ���ǂ������`�F�b�N����
	SKY_ASSERT_MSG( CMathUtil::FAbs(VectorMag(axis) - 1.0f) < .01f , _T( "The vector is not Normalize. " ) , axis ); //����axis�x�N�g�������K������Ă��܂���B

	//�����̊p�x���v�Z����

	dectype	radianOver2 = radian * 0.5f;
	dectype	sinradianOver2 = CMathUtil::Sin( radianOver2 );

    //�l��ݒ肷��
	W = CMathUtil::Cos( radianOver2 );
	X = axis.X * sinradianOver2;
	Y = axis.Y * sinradianOver2;
	Z = axis.Z * sinradianOver2;

	return *this;
}

CQuaternion &CQuaternion::SetupVector( const CBasicVector3 &vec1 , const CBasicVector3 &vec2 )
{
	//�Ȃ��p�����߂�
	dectype radian = vec1.IncludedAngle( vec2 );

	//�O�ς����߂�
	CBasicVector3 axis = Cross( vec1 , vec2 );

	SKY_ASSERT_MSG( axis.Length() > 0 , "cross is zero." );

	//�O�ς����ɂȂ��p����]����N�I�[�^�j�I���𐶐�����
	SetToRotateAboutAxis( axis.Normalize() , radian );

	return *this;
}

CQuaternion &CQuaternion::SetToRotateObjectToInertial( const CEulerAngles &orientation )
{

    //�����̊p��sin��cos���v�Z����

	dectype	sp, sb, sh;
	dectype	cp, cb, ch;
	CMathUtil::SinCos( &sp , &cp , orientation.Pitch * 0.5f );
	CMathUtil::SinCos( &sb , &cb , orientation.Bank * 0.5f );
	CMathUtil::SinCos( &sh , &ch , orientation.Heading * 0.5f );

    //�l���v�Z����

	W =  ch * cp * cb + sh * sp * sb;
	X =  ch * sp * cb + sh * cp * sb;
	Y = -ch * sp * sb + sh * cp * cb;
	Z = -sh * sp * cb + ch * cp * sb;

	return *this;
}

CQuaternion &CQuaternion::SetToRotateInertialToObject(const CEulerAngles &orientation)
{
    //�����̊p��sin��cos���v�Z����

	dectype	sp, sb, sh;
	dectype	cp, cb, ch;
	CMathUtil::SinCos(&sp, &cp, orientation.Pitch * 0.5f);
	CMathUtil::SinCos(&sb, &cb, orientation.Bank * 0.5f);
	CMathUtil::SinCos(&sh, &ch, orientation.Heading * 0.5f);

    //�l���v�Z����

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
    //�l�����̑傫�����v�Z����

	dectype	mag = ( dectype )CMathUtil::Sqrt(W*W + X*X + Y*Y + Z*Z);

    //0���Z����ی삷�邽�߂ɖ����Ȓ������`�F�b�N����

	if (mag > 0.0f) {

		//���K������
		dectype	oneOverMag = 1.0f / mag;
		W *= oneOverMag;
		X *= oneOverMag;
		Y *= oneOverMag;
		Z *= oneOverMag;

	}
	else
	{

        //�q���[�X�g���A��肪���������B�i�Ȃɂ���W���[�N�H�j

		//���K���`�F�b�N
        SKY_ASSERT_MSG( skyFalse , _T( "The value is 0." ) );

		Identity();
	}

	return *this;
}

dectype	CQuaternion::GetRotationAngle() const
{

    //�����̊p�x���v�Z����
    // w = cos(radian /2)�ł��邱�Ƃɒ���

	dectype radianOver2 = CMathUtil::SafeAcos(W);

    //��]�p�x��Ԃ�

	return radianOver2 * 2.0f;
}

CQuaternion	&CQuaternion::Setup( const CBasicVector3 &vec )
{
	//�w�y���ʂɂ������]���Z�o
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

	//�x�y���ʂɂ������]���Z�o
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

    //sin^2(radian/2)���v�Z����
    //w = cos(radian/2)�Asin^2(x) + cos^(x) = 1�ł��邱�Ƃɒ���

	dectype sinradianOver2Sq = 1.0f - W*W;

    //���l�̕s���m������g�����
	if (sinradianOver2Sq <= 0.0f) {

        //�P���l�����A�܂��͕s���m�Ȑ��l
        //�C�ӂ̗L���ȃx�N�g����Ԃ������i�ǂ��ł��悢����ł���j
		return CBasicVector3(1.0f , 0.0f , 0.0f);
	}

    //�@1/sin(radian/2)���v�Z����
	dectype	oneOverSinradianOver2 = 1.0f / CMathUtil::Sqrt(sinradianOver2Sq);

    //��]�̎���Ԃ�
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
    //�͈͊O�̃p�����[�^�[���`�F�b�N���A���̏ꍇ�͒[�_��Ԃ�

	if (t <= 0.0f) return q0;
	if (t >= 1.0f) return q1;

	//���ς��������Ďl�����Ԃ̊p�x��cos���v�Z����

	dectype cosOmega = DotProduct(q0, q1);

    //���̓��ς̏ꍇ�A-q2��p����
    //�Q�̎l����q��-q�͓�����]��\�����A���ƂȂ鋅�ʐ��`�⊮�𐶐�����ꍇ������
    //���m�Ȋp�x��p���ĉ�]���邽�߂�q��-q��I������

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

    //�Q�̎l�����͒P�ʎl�����ł���K�v������A���ς�<=1.0�ɂȂ�͂��ł���
	SKY_ASSERT_MSG( cosOmega < 1.1f , _T( "The product in two Quaternion has exceeded 1.0d. ") ); //�Q�̃N�H�[�^�j�I���̓��ς�1.0d�𒴂��Ă��܂��܂����B

    //�⊮�p�̃t���N�V�������v�Z����
    //�l�������قƂ�ǂ܂����������ł��邪�`�F�b�N����

	dectype k0, k1;
	if (cosOmega > 0.9999f) {

		//���ɋ߂� --- �P���ɐ��`��Ԃ�p����i�����0���Z��h�����߁j

		k0 = 1.0f-t;
		k1 = t;

	} else {

        //�O�p�@�̓���sin^2(omega) + cos^2(omega) = 1��p����
        //���̊p�x��sin���v�Z����

		dectype sinOmega = sky::lib::math::CMathUtil::Sqrt(1.0f - cosOmega*cosOmega);

		 //����sin��cos����p�x���v�Z����

		dectype omega = sky::lib::math::CMathUtil::Atan2(sinOmega , cosOmega);

		//���Z��1��ōςނ悤�ɁA����̋t�����v�Z����

		dectype oneOverSinOmega = 1.0f / sinOmega;

		 //��ԃp�����[�^�[���v�Z����

		k0 = sky::lib::math::CMathUtil::Sin( (1.0f - t) * omega) * oneOverSinOmega;
		k1 = sky::lib::math::CMathUtil::Sin(t * omega) * oneOverSinOmega;
	}

	//�⊮����

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

	//������]��

	result.W = q.W;

	 //���΂̉�]��

	result.X = -q.X;
	result.Y = -q.Y;
	result.Z = -q.Z;

	//���ʂ�Ԃ�

	return result;
}

sky::lib::math::CQuaternion Pow(const sky::lib::math::CQuaternion &q, dectype exponent)
{

    //�P�ʎl�����̏ꍇ���`�F�b�N����
    //����͂O���Z��h�����߂ł���

	if (sky::lib::math::CMathUtil::FAbs(q.W) > .9999f) {
		return q;
	}

    //�����̊p�xalpha(alpha = radian/2)�����o��

	dectype	alpha = sky::lib::math::CMathUtil::Acos(q.W);

    //�V����alpha�l���v�Z����

	dectype	newAlpha = alpha * exponent;

    //�V����w�l���v�Z����

	sky::lib::math::CQuaternion result;
	result.W = sky::lib::math::CMathUtil::Cos(newAlpha);

    //�V����xyz�l���v�Z����

	dectype	mult = sky::lib::math::CMathUtil::Sin(newAlpha) / sky::lib::math::CMathUtil::Sin(alpha);
	result.X = q.X * mult;
	result.Y = q.Y * mult;
	result.Z = q.Z * mult;

	// Return it

	return result;
}

sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &vec, const sky::lib::math::CQuaternion &q )
{
	//�����l���������߂�
	sky::lib::math::CQuaternion rq = Conjugate( q );

	//�x�N�g�����l�����ɂ���
	sky::lib::math::CQuaternion vq;
	vq.SetVector( vec );

	//�|���Z������
	sky::lib::math::CQuaternion rvq = q * vq * rq;

	//�l�������x�N�g���ɕϊ����ĕԂ�
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
