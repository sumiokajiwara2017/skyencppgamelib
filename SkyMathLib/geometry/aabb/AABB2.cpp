#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

CAABB2::CAABB2() : CGeometryBase()
{
}

skyBool CAABB2::IsZero() const
{
	return ( m_Max.Length() == 0 && m_Min.Length() == 0 );
}

CBasicVector2 CAABB2::Corner( s32 i ) const
{

    SKY_ASSERT_MSG( i >= 0, _T( "index over. [%d]" ) , i );
    SKY_ASSERT_MSG( i <= 7, _T( "index over. [%d]" ) , i );

	// �w��̃C���f�b�N�X�̍��W��Ԃ��B
	return CBasicVector2(
		( i & 1 ) ? m_Max.X : m_Min.X ,
		( i & 2 ) ? m_Max.Y : m_Min.Y
	);
}

void CAABB2::Empty()
{
	//�^�ő�l��ݒ肷�邱�ƂŃ{�b�N�X����ɂ���
	const dectype kBigNumber = 1e37f;
	m_Min.X = m_Min.Y = kBigNumber;
	m_Max.X = m_Max.Y = -kBigNumber;
}

void CAABB2::AddX( dectype x )
{
	m_Min.X += x;
	m_Max.X += x;
}

void CAABB2::AddY( dectype y )
{
	m_Min.Y += y;
	m_Max.Y += y;
}

void CAABB2::Merge( const CBasicVector2 &p )
{

	// ���̓_���܂ނ̂ɕK�v�Ȃ������̃{�b�N�X��傫������

	if ( p.X < m_Min.X ) m_Min.X = p.X;
	if ( p.X > m_Max.X ) m_Max.X = p.X;
	if ( p.Y < m_Min.Y ) m_Min.Y = p.Y;
	if ( p.Y > m_Max.Y ) m_Max.Y = p.Y;
}

void CAABB2::Merge( const CAABB2 &box )
{
	// �{�b�N�X�ɕK�v�Ȃ����傫������

	if ( box.m_Min.X < m_Min.X ) m_Min.X = box.m_Min.X;
	if ( box.m_Max.X > m_Max.X ) m_Max.X = box.m_Max.X;
	if ( box.m_Min.Y < m_Min.Y ) m_Min.Y = box.m_Min.Y;
	if ( box.m_Max.Y > m_Max.Y ) m_Max.Y = box.m_Max.Y;
}

void CAABB2::SetToTransformedBox( const CAABB2 &box , const math::CMatrix3x2 &m )
{
	// ��̏ꍇ�͉������Ȃ�

	if (box.IsEmpty() )
	{
		Empty();
		return;
	}

	// ���s�ړ���������n�߂�

	m_Min = m_Max = GetTranslationVector(m);

	// �X�̍s��̗v�f�����ꂼ�꒲�ׁA�V�����`�`�a�a���v�Z����
	if ( m.M11 > 0.0f )
	{
		m_Min.X += m.M11 * box.m_Min.X; m_Max.X += m.M11 * box.m_Max.X;
	}
	else
	{
		m_Min.X += m.M11 * box.m_Max.X; m_Max.X += m.M11 * box.m_Min.X;
	}

	if ( m.M12 > 0.0f )
	{
		m_Min.Y += m.M12 * box.m_Min.X; m_Max.Y += m.M12 * box.m_Max.X;
	}
	else
	{
		m_Min.Y += m.M12 * box.m_Max.X; m_Max.Y += m.M12 * box.m_Min.X;
	}

	if ( m.M21 > 0.0f )
	{
		m_Min.X += m.M21 * box.m_Min.Y; m_Max.X += m.M21 * box.m_Max.Y;
	}
	else
	{
		m_Min.X += m.M21 * box.m_Max.Y; m_Max.X += m.M21 * box.m_Min.Y;
	}

	if ( m.M22 > 0.0f )
	{
		m_Min.Y += m.M22 * box.m_Min.Y; m_Max.Y += m.M22 * box.m_Max.Y;
	}
	else
	{
		m_Min.Y += m.M22 * box.m_Max.Y; m_Max.Y += m.M22 * box.m_Min.Y;
	}
}

skyBool	CAABB2::IsEmpty() const
{
	// �ǂꂩ�̎������]���Ă��Ȃ������`�F�b�N����
	return ( m_Min.X > m_Max.X ) || ( m_Min.Y > m_Max.Y );
}

CBasicVector2	CAABB2::ClosestPointTo( const CBasicVector2 &p ) const
{
	// p���e������̃{�b�N�X�Ƀv�b�V������

	CBasicVector2 r;

	if ( p.X < m_Min.X )
	{
		r.X = m_Min.X;
	}
	else if ( p.X > m_Max.X )
	{
		r.X = m_Max.X;
	}
	else
	{
		r.X = p.X;
	}

	if ( p.Y < m_Min.Y )
	{
		r.Y = m_Min.Y;
	} 
	else if ( p.Y > m_Max.Y )
	{
		r.Y = m_Max.Y;
	}
	else
	{
		r.Y = p.Y;
	}

	return r;
}

dectype	IntersectMovingAABB2 ( const CAABB2 &stationaryBox ,	const CAABB2 &movingBox , const CBasicVector2 &d )
{
	//�������Ȃ��ꍇ�͂��̋���Ȑ���Ԃ�

	const dectype kNoIntersection = 1e30f;

	//�ΏۂƂȂ鎞�Ԃ��ׂĂ��܂ނ悤�ɃC���^�[�o��������������
	dectype	tEnter = 0.0f;
	dectype	tLeave = 1.0f;

	//
	// �e�����̏d�Ȃ�̃C���^�[�o�����v�Z���A����܂łɗݐς����C���^�[�o����
	// ���̃C���^�[�o���̋��ʕ������v�Z����
	// ��̃C���^�[�o�������o�����Ƃ����ɁA���̌��ʁi�������Ȃ��j��Ԃ�
	// ���ꂼ��̏ꍇ�ŁA�e�����ŁA��܂��͖����̃C���^�[�o���ɒ��ӂ���
	//

	// X�����`�F�b�N����

	if ( d.X == 0.0f )
	{

		// X�̃C���^�[�o���͋󂩖����ł���

		if (
			( stationaryBox.m_Min.X >= movingBox.m_Max.X ) ||
			( stationaryBox.m_Max.X <= movingBox.m_Min.X )
		) {

			// ���ԃC���^�[�o������Ȃ̂Ō������Ȃ�

			return kNoIntersection;
		}

		// ���ԃC���^�[�o�������� - �X�V�͕K�v�Ȃ�

	} else {

		// ���Z���P��ōς܂���

		dectype	oneOverD = 1.0f / d.X;

		// �J�n�ƏI�����d�Ȃ��Ă���ꍇ�A���Ԃ̒l���v�Z����

		dectype	xEnter = ( stationaryBox.m_Min.X - movingBox.m_Max.X ) * oneOverD;
		dectype	xLeave = ( stationaryBox.m_Max.X - movingBox.m_Min.X ) * oneOverD;

		// �C���^�[�o���̏��Ԃ��`�F�b�N����

		if (xEnter > xLeave)
		{
			TemplateUtil::Swap( xEnter, xLeave );
		}

		// �C���^�[�o�����X�V����

		if ( xEnter > tEnter ) tEnter = xEnter;
		if ( xLeave < tLeave ) tLeave = xLeave;

		// ���̌��ʂ��󂩃`�F�b�N����

		if ( tEnter > tLeave )
		{
			return kNoIntersection;
		}
	}
	
	// Y�����`�F�b�N����

	if (d.Y == 0.0f) {

		// Y�̃C���^�[�o���͋󂩖����ł���

		if (
			( stationaryBox.m_Min.Y >= movingBox.m_Max.Y ) ||
			( stationaryBox.m_Max.Y <= movingBox.m_Min.Y )
		)
		{

			// ���ԃC���^�[�o������Ȃ̂Ō������Ȃ�

			return kNoIntersection;
		}

		// ���ԃC���^�[�o�������� - �X�V�͕K�v�Ȃ�

	}
	else
	{

		// ���Z���P��ōς܂���

		dectype	oneOverD = 1.0f / d.Y;

		// �J�n�ƏI�����d�Ȃ��Ă���ꍇ�A���Ԃ̒l���v�Z����

		dectype	yEnter = ( stationaryBox.m_Min.Y - movingBox.m_Max.Y ) * oneOverD;
		dectype	yLeave = ( stationaryBox.m_Max.Y - movingBox.m_Min.Y ) * oneOverD;

		// �C���^�[�o���̏��Ԃ��`�F�b�N����

		if ( yEnter > yLeave )
		{
			TemplateUtil::Swap( yEnter , yLeave );
		}

		// �C���^�[�o�����X�V����

		if ( yEnter > tEnter ) tEnter = yEnter;
		if ( yLeave < tLeave ) tLeave = yLeave;

		// ���̌��ʂ��󂩃`�F�b�N����

		if ( tEnter > tLeave )
		{
			return kNoIntersection;
		}
	}

	//�n�j�C��������
	//���̌������N�������Ƃ��̃p�����g���b�N�ȓ_��Ԃ�

	return tEnter;
}

} } }
