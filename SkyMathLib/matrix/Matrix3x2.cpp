#include "StdAfx.h"
#include "SkyMathLib/Matrix.h"

namespace sky { namespace lib { namespace math {

CMatrix3x2::CMatrix3x2()
{
}

CMatrix3x2::~CMatrix3x2()
{
}

void CMatrix3x2::Identity()
{
	M11 = 1.0f; M12 = 0.0f;
	M21 = 0.0f; M22 = 1.0f;
	Tx  = 0.0f; Ty  = 0.0f;
}

void CMatrix3x2::ZeroTranslation()
{
	Tx = Ty = 0.0f;
}

void CMatrix3x2::SetTranslation( const sky::lib::math::CBasicVector2 &d )
{
	Tx = d.X; Ty = d.Y;
}

void CMatrix3x2::SetupRotate( dectype theta )
{
	//�x�����W�A����
	dectype rad = CMathUtil::DegToRad( theta );

	//�w��̊p�x��sin��cos�����߂�
	dectype	sin , cos;
	CMathUtil::SinCos( &sin , &cos , rad );

	//�s��v�f��ݒ肷��
	M11 = cos; M12 = -sin; 
	M21 = sin; M22 = cos;  

    //���s�ړ����������Z�b�g����

	Tx = Ty = 0.0f;
}

void CMatrix3x2::SetupScale( const sky::lib::math::CBasicVector2 &sc )
{
    //�s��̗v�f��ݒ肷��i���ɒP�������ł���j

	M11 = sc.X;  M12 = 0.0f; 
	M21 = 0.0f;  M22 = sc.Y;  

    //���s�ړ����������Z�b�g����

	Tx = Ty = 0.0f;
}

skyBool CMatrix3x2::operator ==( const CMatrix3x2 &m ) const
{
	return M11 == m.M11 && M12 == m.M12 && M21 == m.M21 && M22 == m.M22 && Tx == m.Tx && Ty == m.Ty;
}

skyBool CMatrix3x2::operator !=( const CMatrix3x2 &m ) const
{
	return M11 == m.M11 || M12 == m.M12 || M21 == m.M21 || M22 == m.M22 || Tx == m.Tx || Ty == m.Ty;
}

} } }

sky::lib::math::CBasicVector2	operator*(const sky::lib::math::CBasicVector2 &p, const sky::lib::math::CMatrix3x2 &m)
{
    //���`�㐔�ɂ�肷��Ԃ�

	return sky::lib::math::CBasicVector2(
		p.X*m.M11 + p.Y*m.M21 + m.Tx ,
		p.X*m.M12 + p.Y*m.M22 + m.Ty
	);
}

sky::lib::math::CBasicVector2 &operator*=(sky::lib::math::CBasicVector2 &p, const sky::lib::math::CMatrix3x2 &m)
{
	p = p * m;
	return p;
}

sky::lib::math::CMatrix3x2 operator*(const sky::lib::math::CMatrix3x2 &m1, const sky::lib::math::CMatrix3x2 &m2) {

	sky::lib::math::CMatrix3x2 r;

    //�㕔�̂R�~�R�i���`�ϊ��j�������v�Z����

	r.M11 = m1.M11*m2.M11 + m1.M12*m2.M21;
	r.M12 = m1.M11*m2.M12 + m1.M12*m2.M22;

	r.M21 = m1.M21*m2.M11 + m1.M22*m2.M21;
	r.M22 = m1.M21*m2.M12 + m1.M22*m2.M22;

	//���s�ړ��������v�Z����

	r.Tx = m1.Tx*m2.M11 + m1.Ty*m2.M21 + m2.Tx;
	r.Ty = m1.Tx*m2.M12 + m1.Ty*m2.M22 + m2.Ty;

	//���ʂ�Ԃ�
	return r;
}

sky::lib::math::CMatrix3x2 &operator*=(sky::lib::math::CMatrix3x2 &m1, const sky::lib::math::CMatrix3x2 &m2) {

	m1 = m1 * m2;
	return m1;
}

sky::lib::math::CBasicVector2	GetTranslationVector(const sky::lib::math::CMatrix3x2 &m)
{
	return sky::lib::math::CBasicVector2(m.Tx , m.Ty);
}
