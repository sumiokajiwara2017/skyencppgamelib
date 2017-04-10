#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

CAABB3::CAABB3() : CGeometryBase()
{
}

skyBool CAABB3::IsZero() const
{
	return ( m_Max.Length() == 0 && m_Min.Length() == 0 );
}

CBasicVector3 CAABB3::Corner( s32 i ) const
{

    SKY_ASSERT_MSG( i >= 0, _T( "index over. [%d]" ) , i );
    SKY_ASSERT_MSG( i <= 7, _T( "index over. [%d]" ) , i );

	// �w��̃C���f�b�N�X�̍��W��Ԃ��B
	return CBasicVector3(
		( i & 1 ) ? m_Max.X : m_Min.X ,
		( i & 2 ) ? m_Max.Y : m_Min.Y,
		( i & 4 ) ? m_Max.Z : m_Min.Z
	);
}

void CAABB3::Empty()
{
	//�^�ő�l��ݒ肷�邱�ƂŃ{�b�N�X����ɂ���
	const dectype kBigNumber = 1e37f;
	m_Min.X = m_Min.Y = m_Min.Z = kBigNumber;
	m_Max.X = m_Max.Y = m_Max.Z = -kBigNumber;
}

void CAABB3::AddX( dectype x )
{
	m_Min.X += x;
	m_Max.X += x;
}

void CAABB3::AddY( dectype y )
{
	m_Min.Y += y;
	m_Max.Y += y;
}

void CAABB3::AddZ( dectype z )
{
	m_Min.Z += z;
	m_Max.Z += z;
}

void CAABB3::Merge( const CBasicVector3 &p )
{

	// ���̓_���܂ނ̂ɕK�v�Ȃ������̃{�b�N�X��傫������

	if ( p.X < m_Min.X ) m_Min.X = p.X;
	if ( p.X > m_Max.X ) m_Max.X = p.X;
	if ( p.Y < m_Min.Y ) m_Min.Y = p.Y;
	if ( p.Y > m_Max.Y ) m_Max.Y = p.Y;
	if ( p.Z < m_Min.Z ) m_Min.Z = p.Z;
	if ( p.Z > m_Max.Z ) m_Max.Z = p.Z;
}

void CAABB3::Merge( const CAABB3 &box )
{
	// �{�b�N�X�ɕK�v�Ȃ����傫������

	if ( box.m_Min.X < m_Min.X ) m_Min.X = box.m_Min.X;
	if ( box.m_Max.X > m_Max.X ) m_Max.X = box.m_Max.X;
	if ( box.m_Min.Y < m_Min.Y ) m_Min.Y = box.m_Min.Y;
	if ( box.m_Max.Y > m_Max.Y ) m_Max.Y = box.m_Max.Y;
	if ( box.m_Min.Z < m_Min.Z ) m_Min.Z = box.m_Min.Z;
	if ( box.m_Max.Z > m_Max.Z ) m_Max.Z = box.m_Max.Z;
}

void CAABB3::SetToTransformedBox( const CAABB3 &box , const math::CMatrix4x3 &m )
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

	if ( m.M13 > 0.0f )
	{
		m_Min.Z += m.M13 * box.m_Min.X; m_Max.Z += m.M13 * box.m_Max.X;
	}
	else
	{
		m_Min.Z += m.M13 * box.m_Max.X; m_Max.Z += m.M13 * box.m_Min.X;
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

	if ( m.M23 > 0.0f )
	{
		m_Min.Z += m.M23 * box.m_Min.Y; m_Max.Z += m.M23 * box.m_Max.Y;
	}
	else
	{
		m_Min.Z += m.M23 * box.m_Max.Y; m_Max.Z += m.M23 * box.m_Min.Y;
	}

	if ( m.M31 > 0.0f )
	{
		m_Min.X += m.M31 * box.m_Min.Z; m_Max.X += m.M31 * box.m_Max.Z;
	}
	else
	{
		m_Min.X += m.M31 * box.m_Max.Z; m_Max.X += m.M31 * box.m_Min.Z;
	}

	if ( m.M32 > 0.0f )
	{
		m_Min.Y += m.M32 * box.m_Min.Z; m_Max.Y += m.M32 * box.m_Max.Z;
	}
	else
	{
		m_Min.Y += m.M32 * box.m_Max.Z; m_Max.Y += m.M32 * box.m_Min.Z;
	}

	if ( m.M33 > 0.0f )
	{
		m_Min.Z += m.M33 * box.m_Min.Z; m_Max.Z += m.M33 * box.m_Max.Z;
	}
	else
	{
		m_Min.Z += m.M33 * box.m_Max.Z; m_Max.Z += m.M33 * box.m_Min.Z;
	}
}

skyBool	CAABB3::IsEmpty() const
{
	// �ǂꂩ�̎������]���Ă��Ȃ������`�F�b�N����
	return ( m_Min.X > m_Max.X ) || ( m_Min.Y > m_Max.Y ) || ( m_Min.Z > m_Max.Z );
}

CBasicVector3	CAABB3::ClosestPointTo( const CBasicVector3 &p ) const
{
	// p���e������̃{�b�N�X�Ƀv�b�V������

	CBasicVector3 r;

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

	if ( p.Z < m_Min.Z )
	{
		r.Z = m_Min.Z;
	}
	else if ( p.Z > m_Max.Z )
	{
		r.Z = m_Max.Z;
	}
	else
	{
		r.Z = p.Z;
	}

	return r;
}

s32	CAABB3::ClassifyPlane( const CBasicPlane &plane ) const
{
	//�@�����`�F�b�N���A�c�̒l�̍ŏ��l�ƍő�l���v�Z����	
	CBasicVector3 n = plane.m_Normal;
	dectype       d = plane.d;

	dectype	m_MinD, m_MaxD;

	if ( n.X > 0.0f )
	{
		m_MinD = n.X * m_Min.X; m_MaxD = n.X * m_Max.X;
	}
	else
	{
		m_MinD = n.X * m_Max.X; m_MaxD = n.X * m_Min.X;
	}

	if ( n.Y > 0.0f )
	{
		m_MinD += n.Y * m_Min.Y; m_MaxD += n.Y * m_Max.Y;
	}
	else
	{
		m_MinD += n.Y * m_Max.Y; m_MaxD += n.Y * m_Min.Y;
	}

	if ( n.Z > 0.0f )
	{
		m_MinD += n.Z * m_Min.Z; m_MaxD += n.Z * m_Max.Z;
	}
	else
	{
		m_MinD += n.Z * m_Max.Z; m_MaxD += n.Z * m_Min.Z;
	}

	// ���S�ɂ��̕��ʂ̑O�ʑ��ɂ��邩���`�F�b�N����

	if ( m_MinD >= d )
	{
		return +1;
	}

	// ���S�ɂ��̕��ʂ̗��ʑ��ɂ��邩���`�F�b�N����

	if (m_MaxD <= d)
	{
		return -1;
	}

	// ���̕��ʂƌ�������

	return 0;
}

dectype	CAABB3::IntersectPlane( const CBasicPlane &plane , const CBasicVector3 &dir ) const
{

	CBasicVector3 n      = plane.m_Normal;
	dectype       planeD = plane.d;


	// ���K�����ꂽ�x�N�g�����n����Ă��邩���m�F����

	SKY_ASSERT_MSG( CMathUtil::FAbs( n.Dot( n ) - 1.0f ) < 0.01f		, _T( "Argument vector n is not Normalize. " ) ); //�����x�N�g��n�����K������Ă��܂���B
	SKY_ASSERT_MSG( CMathUtil::FAbs( dir.Dot( dir ) - 1.0f) < 0.01f	, _T( "Argument vector n is not Normalize. " ) );

	// �������Ȃ��ꍇ���̋���Ȑ���Ԃ�

	const dectype kNoIntersection = 1e30f;

	// ���Ίp���v�Z����i���̕��ʂ̑O�ʂɌ������Ĉړ����Ă��邱�Ƃ��m�F����j

	dectype	dot = n.Dot( dir );
	if ( dot >= 0.0f )
	{
		return kNoIntersection;
	}

	// �@�����`�F�b�N���A�c�̍ŏ��l�ƍő�l���v�Z����
	// m_MiniD�͍őP�ʂɂ���p�̓_�̂c�̒l�ł���

	dectype	minD, maxD;

	if ( n.X > 0.0f )
	{
		minD = n.X * m_Min.X; maxD = n.X * m_Max.X;
	}
	else
	{
		minD = n.X * m_Max.X; maxD = n.X * m_Min.X;
	}

	if (n.Y > 0.0f)
	{
		minD += n.Y * m_Min.Y; maxD += n.Y * m_Max.Y;
	}
	else
	{
		minD += n.Y * m_Max.Y; maxD += n.Y * m_Min.Y;
	}

	if (n.Z > 0.0f)
	{
		minD += n.Z * m_Min.Z; maxD += n.Z * m_Max.Z;
	}
	else
	{
		minD += n.Z * m_Max.Z; maxD += n.Z * m_Min.Z;
	}

	//���S�ɂ��̕��ʂ̑��̑��ɂ��łɂ��邩���`�F�b�N����
	if ( maxD <= planeD )
	{
		return kNoIntersection;
	}

	//�őO�ʂ̊p�̓_��p���ĕW���̌����ǐՂ̕�����������
	dectype	t = ( planeD - minD ) / dot;

	// ���łɊђʂ��Ă��邩�H
	if ( t < 0.0f )
	{
		return 0.0f;
	}

	// > 1�̏ꍇ�́A���̎��ԓ��ł͏Փ˂��Ȃ�����
	// ����͌Ăяo�������`�F�b�N���ׂ������ł���B

	return t;
}

dectype	IntersectMovingAABB3 ( const CAABB3 &stationaryBox ,	const CAABB3 &movingBox , const CBasicVector3 &d )
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
	
	// Z�����`�F�b�N

	if ( d.Z == 0.0f )
	{

		// Z�̃C���^�[�o���͋󂩖����ł���

		if ( ( stationaryBox.m_Min.Z >= movingBox.m_Max.Z ) ||
			( stationaryBox.m_Max.Z <= movingBox.m_Min.Z ) )
		{
			// ���ԃC���^�[�o������Ȃ̂Ō������Ȃ�

			return kNoIntersection;
		}

		// ���ԃC���^�[�o�������� �| �X�V�͕K�v�Ȃ�

	}
	else
	{

		// ���Z���P��ōς܂���

		dectype	oneOverD = 1.0f / d.Z;

		// �J�n�ƏI�����d�Ȃ��Ă���ꍇ�A���Ԃ̒l���v�Z����

		dectype	zEnter = ( stationaryBox.m_Min.Z - movingBox.m_Max.Z ) * oneOverD;
		dectype	zLeave = ( stationaryBox.m_Max.Z - movingBox.m_Min.Z ) * oneOverD;

		// �C���^�[�o���̏��Ԃ��`�F�b�N����

		if ( zEnter > zLeave )
		{
			TemplateUtil::Swap( zEnter , zLeave );
		}

		// �C���^�[�o�����X�V����

		if ( zEnter > tEnter ) tEnter = zEnter;
		if ( zLeave < tLeave ) tLeave = zLeave;

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
