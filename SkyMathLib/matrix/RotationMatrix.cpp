#include "StdAfx.h"
#include "SkyMathLib/Matrix.h"

#include "SkyMathLib/Graph.h"

namespace sky { namespace lib { namespace math {

CRotationMatrix	&CRotationMatrix::Setup(const CEulerAngles &orientation)
{
    //�p�x��sin��cos�����Ƃ߁A�v�Z���Ȃ���s��̗v�f�Ɋi�[����
	dectype	sh,ch, sp,cp, sb,cb;
	CMathUtil::SinCos(&sh, &ch, orientation.Heading);
	CMathUtil::SinCos(&sp, &cp, orientation.Pitch);
	CMathUtil::SinCos(&sb, &cb, orientation.Bank);

	M11 = ch * cb + sh * sp * sb;
	M12 = -ch * sb + sh * sp * cb;
	M13 = sh * cp;

	M21 = sb * cp;
	M22 = cb * cp;
	M23 = -sp;

	M31 = -sh * cb + ch * sp * sb;
	M32 = sb * sh + ch * sp * cb;
	M33 = ch * cp;

	return *this;
}

CRotationMatrix	&CRotationMatrix::Setup( const CBasicVector3 &vec )
{
	//�w�y���ʂɂ������]���Z�o
	CBasicVector2 baseK;
	CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( baseK , CBasicVector2( 0 , 1 ) );

	CRotationMatrix rotY;
	CBasicVector2 tempXZ( vec.X , vec.Z );
	if ( tempXZ.Length() > 0 )
	{
		CBasicVector2 tempXZK;
		CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( tempXZK , tempXZ );
		dectype radY = baseK.Y - tempXZK.Y;
		rotY.Setup( eAxis_y , radY );
	}

	//�x�y���ʂɂ������]���Z�o
	CRotationMatrix rotX;
	CBasicVector2 tempYZ( vec.Y , math::CMathUtil::Abs( vec.Z ) );
	if ( tempYZ.Length() > 0 )
	{
		CBasicVector2 tempYZK;
		CGraphUtil::ConvertFromRectangularCoordinateToPolarCoordinates( tempYZK , tempYZ );
		dectype radX = tempYZK.Y - baseK.Y;
		rotX.Setup( eAxis_x , radX );
	}

	*this = rotX * rotY;

	return *this;
}

CRotationMatrix	&CRotationMatrix::FromInertialToObjectQuaternion(const CQuaternion &q)
{

    //�s��̗v�f�𖄂߂�
    //����͂����炭�œK���\�ł���
    //��������̋��ʂ̕����������邩��ł���
    //����̓R���p�C���ɔC���邱�Ƃɂ���E�E

	M11 = 1.0f - 2.0f * (q.Y*q.Y + q.Z*q.Z);
	M12 = 2.0f * (q.X*q.Y + q.W*q.Z);
	M13 = 2.0f * (q.X*q.Z - q.W*q.Y);

	M21 = 2.0f * (q.X*q.Y - q.W*q.Z);
	M22 = 1.0f - 2.0f * (q.X*q.X + q.Z*q.Z);
	M23 = 2.0f * (q.Y*q.Z + q.W*q.X);

	M31 = 2.0f * (q.X*q.Z + q.W*q.Y);
	M32 = 2.0f * (q.Y*q.Z - q.W*q.X);
	M33 = 1.0f - 2.0f * (q.X*q.X + q.Y*q.Y);

	return *this;
}

CRotationMatrix	&CRotationMatrix::FromObjectToInertialQuaternion(const CQuaternion &q)
{

    //�s��̗v�f�𖄂߂�
    //����͂����炭�œK���\�ł���
    //��������̋��ʂ̕����������邩��ł���
    //����̓R���p�C���ɔC���邱�Ƃɂ���E�E

	M11 = 1.0f - 2.0f * (q.Y*q.Y + q.Z*q.Z);
	M12 = 2.0f * (q.X*q.Y - q.W*q.Z);
	M13 = 2.0f * (q.X*q.Z + q.W*q.Y);

	M21 = 2.0f * (q.X*q.Y + q.W*q.Z);
	M22 = 1.0f - 2.0f * (q.X*q.X + q.Z*q.Z);
	M23 = 2.0f * (q.Y*q.Z - q.W*q.X);

	M31 = 2.0f * (q.X*q.Z - q.W*q.Y);
	M32 = 2.0f * (q.Y*q.Z + q.W*q.X);
	M33 = 1.0f - 2.0f * (q.X*q.X + q.Y*q.Y);

	return *this;
}

} } }
