#include "StdAfx.h"
#include "SkyMathLib/Matrix.h"

namespace sky { namespace lib { namespace math {

CMatrix4x3& CMatrix4x3::SetupLocalToParent(const CBasicVector3 &pos , const CEulerAngles &orient)
{
    //��]�s����쐬����

	CRotationMatrix orientMatrix;
	orientMatrix.Setup(orient);

    //�S�~�R�s����Z�b�g�A�b�v����
    //���ӁF���x���C�ɂȂ�ꍇ�́A�ꎞ�I��RotationMatrix�I�u�W�F�N�g��p���邱�ƂȂ�
    //�����̕ϐ��ɒ��ڍs����쐬���邱�Ƃ��ł���
    //��������΁A���̊֐��Ăяo���ƂQ�`�R�̃R�s�[�����ߖ�ł���

	SetupLocalToParent( pos , orientMatrix );

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupLocalToParent(const CBasicVector3 &pos , const CRotationMatrix &orient)
{
	Identity();

    //�s��̉�]�����R�s�[����
    //��]�͒ʏ�A������ԁ��I�u�W�F�N�g��Ԃւ̍s��ł���A���Ȃ킿�A�e�����[�J���ł���
    //���[�J�����e�ւ̉�]���~�����̂ŁA�R�s�[���ɓ]�u����K�v������

	M11 = orient.M11; M12 = orient.M21; M13 = orient.M31;
	M21 = orient.M12; M22 = orient.M22; M23 = orient.M32;
	M31 = orient.M13; M32 = orient.M23; M33 = orient.M33;

    //���s�ړ�������ݒ肷��
    //���s�ړ��́A�R�~�R�̕����̌��ŋN����̂ŁA
    //���̕����̃t�B�[���h�𒼐ڃR�s�[���邱�Ƃ��ł���

	Tx = pos.X; Ty = pos.Y; Tz = pos.Z;

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupParentToLocal(const CBasicVector3 &pos , const CEulerAngles &orient)
{
    //��]�s����쐬����

	CRotationMatrix orientMatrix;
	orientMatrix.Setup( orient );

    //�S�~�R�s����Z�b�g�A�b�v����

	SetupParentToLocal( pos , orientMatrix );

	return *this;
}

CMatrix4x3& CMatrix4x3::SetupParentToLocal(const CBasicVector3 &pos , const CRotationMatrix &orient)
{
	Identity();

    //���̍s��̉�]�������R�s�[����
    //���̗v�f�́A���C�A�E�g�ɏ]���āA�]�u���邱�ƂȂ����ڃR�s�[�ł���

	M11 = orient.M11; M12 = orient.M12; M13 = orient.M13;
	M21 = orient.M21; M22 = orient.M22; M23 = orient.M23;
	M31 = orient.M31; M32 = orient.M32; M33 = orient.M33;

    //�����ŁA���s�ړ�����ݒ肷��
    //�ʏ�A���̈ʒu�������t�ɕ��s�ړ����A���[���h��Ԃ��犵����Ԃ֕��s�ړ�����
    //�������Ȃ���A���̉�]���ŏ��ɋN����Ƃ���������␳����K�v������
    //�܂�A���s�ړ���������]����K�v������
    //����́A-pos�������s�ړ�����s��s�Ɖ�]�s��q���쐬���A
    //�A�������s�q�s����쐬���邱�ƂƓ����ł���

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

	//���s�ړ����������Z�b�g����

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

    //���`�㐔�ɂ�肷��Ԃ�

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

    //�㕔�̂R�~�R�i���`�ϊ��j�������v�Z����

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

	//���s�ړ������P�Ȃ�R�s�[
	r.Tx = m1.Tx;
	r.Ty = m1.Ty;
	r.Tz = m1.Tz;
	r.Tw = m1.Tw;

	//���ʂ�Ԃ�
	return r;
}

sky::lib::math::CMatrix4x3 operator*( const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CMatrix4x3 &m2 )
{

	sky::lib::math::CMatrix4x3 r;

    //�㕔�̂R�~�R�i���`�ϊ��j�������v�Z����

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

	//���ʂ�Ԃ�
	return r;
}

sky::lib::math::CMatrix4x3 &operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 )
{
	m1 = m1 * m2;
	return m1;
}

///////////////////////////////////////////////
// ��]�s�񁨃N�H�[�^�j�I���ϊ�
///////////////////////////////////////////////
skyBool TransformRotMatToQuaternion(
    f32 &qx, f32 &qy, f32 &qz, f32 &qw,
    f32 m11, f32 m12, f32 m13,
    f32 m21, f32 m22, f32 m23,
    f32 m31, f32 m32, f32 m33
) {
    // �ő听��������
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
        return skyFalse; // �����̍s��ɊԈႢ����I

    // �ő�v�f�̒l���Z�o
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

//2�̍s��̊Ԃ̐��`��Ԃ��v�Z���A���ʂ̍s���Ԃ� 
// �N�H�[�^�j�I������]�s��D3DXMatrixRotationQuaternion      ��]�s�񁨃N�H�[�^�j�I����D3DXQuaternionRotationMatrix
sky::lib::math::CMatrix4x3 Lerp(sky::lib::math::CMatrix4x3 a,sky::lib::math::CMatrix4x3 b,f32 t)
{
	sky::lib::math::CMatrix4x3 ret;
	ret.Identity();

	//���ꂼ��̍s�񂩂畽�s�ړ��������A���ꂼ��x�N�g���Ɏ��o��
	sky::lib::math::CBasicVector3 vA, vB;
	vA.X=a.Tx;
	vA.Y=a.Ty;
	vA.Z=a.Tz;
	vB.X=b.Tx;
	vB.Y=b.Ty;
	vB.Z=b.Tz;

	//���ꂼ��̍s�񂩂�X�P�[�����O�������A���ꂼ��x�N�g���Ɏ��o��
	sky::lib::math::CBasicVector3 vA1(a.M11,a.M12,a.M13);
	f32 Sax=vA1.Length();//�X�P�[�����OX����
	sky::lib::math::CBasicVector3 vA2(a.M21,a.M22,a.M23);
	f32 Say=vA2.Length();//�X�P�[�����OY����
	sky::lib::math::CBasicVector3 vA3(a.M31,a.M32,a.M33);
	f32 Saz=vA3.Length();//�X�P�[�����OZ����

	sky::lib::math::CBasicVector3 vB1(b.M11,b.M12,b.M13);
	f32 Sbx=vB1.Length();//�X�P�[�����OX����
	sky::lib::math::CBasicVector3 vB2(b.M21,b.M22,b.M23);
	f32 Sby=vB2.Length();//�X�P�[�����OY����
	sky::lib::math::CBasicVector3 vB3(b.M31,b.M32,b.M33);
	f32 Sbz=vB3.Length();//�X�P�[�����OZ����

	//���ꂼ��̍s�񂩂��]�������A���ꂼ��N�H�[�^�j�I���Ɏ��o��
	//��]�ƃX�P�[�����O�͍���������Ă��邽�߁A��]�����o���O�ɁA�s�񂩂�X�P�[�����O�������������Ȃ��Ă͂Ȃ�Ȃ��B�i���ꂼ��X�P�[�����O�W���ōs������j
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
	//���ꂼ����A���̔䗦�Ő��`��Ԃ���
	sky::lib::math::CBasicVector3 vR;
	sky::lib::interp::Linear( vR , vA , vB , t );
	sky::lib::math::CQuaternion qR;
	qR = Slerp( qA , qB , t );
	//�x�N�g���ƃN�H�[�^�j�I�����s��ɖ߂�
	ret.SetupRotate( qR ); //������D3DXMatrixRotationQuaternion�ƈႤ���ʂ�Ԃ��B
	ret.Tx=vR.X;
	ret.Ty=vR.Y;
	ret.Tz=vR.Z;
	ret.Tw=1.0;

	return ret;
}
