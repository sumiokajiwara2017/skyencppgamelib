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
	//度をラジアンに
	dectype rad = CMathUtil::DegToRad( theta );

	//指定の角度のsinとcosを求める
	dectype	sin , cos;
	CMathUtil::SinCos( &sin , &cos , rad );

	//行列要素を設定する
	M11 = cos; M12 = -sin; 
	M21 = sin; M22 = cos;  

    //平行移動部分をリセットする

	Tx = Ty = 0.0f;
}

void CMatrix3x2::SetupScale( const sky::lib::math::CBasicVector2 &sc )
{
    //行列の要素を設定する（非常に単純明快である）

	M11 = sc.X;  M12 = 0.0f; 
	M21 = 0.0f;  M22 = sc.Y;  

    //平行移動部分をリセットする

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
    //線形代数によりすりつぶす

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

    //上部の３×３（線形変換）部分を計算する

	r.M11 = m1.M11*m2.M11 + m1.M12*m2.M21;
	r.M12 = m1.M11*m2.M12 + m1.M12*m2.M22;

	r.M21 = m1.M21*m2.M11 + m1.M22*m2.M21;
	r.M22 = m1.M21*m2.M12 + m1.M22*m2.M22;

	//平行移動部分を計算する

	r.Tx = m1.Tx*m2.M11 + m1.Ty*m2.M21 + m2.Tx;
	r.Ty = m1.Tx*m2.M12 + m1.Ty*m2.M22 + m2.Ty;

	//結果を返す
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
