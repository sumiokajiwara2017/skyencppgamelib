#pragma once

namespace sky { namespace lib { namespace math {

//�ϊ��}�N��
#define DEGtoRAD( _s ) ( static_cast< dectype >( _s ) * sky::lib::math::cRadPerDeg )
#define RADtoDEG( _s ) ( ( _s ) * sky::lib::math::cDegPerRad )
#define RADtoANG( _s ) ( ( s16 )( ( _s ) * sky::lib::math::cAngPerRad ) )
#define ANGtoRAD( _s ) ( static_cast< dectype >( _s ) * sky::lib::math::cRadPerAng )
#define DEGtoANG( _s ) ( s16 )( static_cast< dectype >( _s ) * sky::lib::math::cAngPerDeg )
#define ANGtoDEG( _s ) ( static_cast< dectype >( _s ) * sky::lib::math::cDegPerAng )

//�萔
const dectype cRadPerDeg = util::KPI / 180.0f;   // radian per Degree
const dectype cDegPerRad = 180.0f / util::KPI;   // degree per radian
const dectype cRadPerAng = util::KPI / 32768.0f; // radian per angle
const dectype cAngPerRad = 32768.0f / util::KPI; // angle per radian
const dectype cAngPerDeg = 32678.0f / 180.0f;    // angle per degree
const dectype cDegPerAng = 180.0f / 32678.0f;    // degree per angle

/// <summary>
/// �p�x�N���X
/// </summary>
template< typename T >
class CAngle : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CAngle()               {}
	inline CAngle( T ang )        { m_Angle = ( s16 )ang; }
	inline CAngle( s32 ang )      { m_Angle = ( s16 )ang; }
	inline CAngle( s16 ang )      { m_Angle = ang; }

	/// <summary>
	/// �l�ϊ�
	/// </summary>
	inline T    ToRad() const     { return ANGtoRAD( m_Angle ); }
	inline T    ToDeg() const     { return ANGtoDEG( m_Angle ); }
	inline void FromRad( T rad )  { m_Angle = RADtoANG( rad ); }
	inline void FromDeg( T deg )  { m_Angle = DEGtoANG( deg ); }
	inline void AddFromRad( T rad )  { m_Angle += RADtoANG( rad ); }
	inline void AddFromDeg( T deg )  { m_Angle += DEGtoANG( deg ); }

	/// <summary>
	/// �ݒ�
	/// </summary>
	inline void  Set( CAngle ang ) { m_Angle = ang.m_Angle ; }

	/// <summary>
	/// ��Βl�v�Z
	/// </summary>
	inline T     Abs() { return ( T )( m_Angle < 0 ? -m_Angle : m_Angle ); }
	inline void  Abs( CAngle ang ) { m_Angle = ( ang.m_Angle < 0 ? -ang.m_Angle : ang.m_Angle ); }

	/// <summary>
	/// �T�C���E�R�T�C���l���擾
	/// </summary>
	static T Sin( const CAngle ang );
	static T Cos( const CAngle ang );
	inline T Sin() const { return CAngle::Sin( *this ) ; }
	inline T Cos() const { return CAngle::Cos( *this ) ; }

	/// <summary>
	/// ���Z�q�̃I�[�o�[���C�h
	/// </summary>
	inline CAngle operator =  ( const CAngle n ) { return CAngle( m_Angle  = n.m_Angle ); }
	inline CAngle operator =  ( T b )            { return CAngle( m_Angle = ( s16 )( b ) ); }
	inline CAngle operator += ( const CAngle n ) { return CAngle( m_Angle = ( s16 )( m_Angle + n.m_Angle ) ); }
	inline CAngle operator -= ( const CAngle n ) { return CAngle( m_Angle = ( s16 )( m_Angle - n.m_Angle ) ); }
	inline CAngle operator += ( T b )            { return CAngle( m_Angle = ( s16 )( m_Angle + b ) ); }
	inline CAngle operator -= ( T b )            { return CAngle( m_Angle = ( s16 )( m_Angle - b ) ); }
	inline CAngle operator *= ( T b )            { return CAngle( m_Angle = ( s16 )( m_Angle * b ) ); }
	inline CAngle operator /= ( T b )            { return CAngle( m_Angle = ( s16 )( m_Angle / b ) ); }
	friend inline CAngle operator + ( const CAngle a, const CAngle b ) { return CAngle( ( s16 )( a.m_Angle + b.m_Angle ) ); }
	friend inline CAngle operator - ( const CAngle a, const CAngle b ) { return CAngle( ( s16 )( a.m_Angle - b.m_Angle ) ); }
	friend inline CAngle operator * ( const CAngle a, const CAngle b ) { return CAngle( ( s16 )( a.m_Angle * b.m_Angle ) ); }
	friend inline CAngle operator + ( const CAngle a, T b ) { return CAngle( ( s16 )( a.m_Angle + b ) ); }
	friend inline CAngle operator - ( const CAngle a, T b ) { return CAngle( ( s16 )( a.m_Angle - b ) ); }
	friend inline CAngle operator + ( T a, const CAngle b ) { return CAngle( ( s16 )( a + b.m_Angle ) ); }
	friend inline CAngle operator - ( T a, const CAngle b ) { return CAngle( ( s16 )( a - b.m_Angle ) ); }
	friend inline CAngle operator * ( const CAngle a, T b ) { return CAngle( ( s16 )( a.m_Angle * b ) ); }
	friend inline CAngle operator / ( const CAngle a, T b ) { return CAngle( ( s16 )( a.m_Angle / b ) ); }

	friend inline bool operator ==( const CAngle a, const CAngle b ) { return a.m_Angle == b.m_Angle; }
	friend inline bool operator !=( const CAngle a, const CAngle b ) { return a.m_Angle != b.m_Angle; }
	friend inline bool operator < ( const CAngle a, const CAngle b ) { return a.m_Angle < b.m_Angle; }
	friend inline bool operator > ( const CAngle a, const CAngle b ) { return a.m_Angle > b.m_Angle; }
	friend inline bool operator <=( const CAngle a, const CAngle b ) { return a.m_Angle <= b.m_Angle; }
	friend inline bool operator >=( const CAngle a, const CAngle b ) { return a.m_Angle >= b.m_Angle; }
	friend inline bool operator < ( const CAngle a, T b ) { return a.m_Angle < b; }
	friend inline bool operator > ( const CAngle a, T b ) { return a.m_Angle > b; }
	friend inline bool operator <=( const CAngle a, T b ) { return a.m_Angle <= b; }
	friend inline bool operator >=( const CAngle a, T b ) { return a.m_Angle >= b; }
	friend inline bool operator < ( T a, const CAngle b ) { return a <  b.m_Angle; }
	friend inline bool operator > ( T a, const CAngle b ) { return a >  b.m_Angle; }
	friend inline bool operator <=( T a, const CAngle b ) { return a <= b.m_Angle; }
	friend inline bool operator >=( T a, const CAngle b ) { return a >= b.m_Angle; }

	inline const CAngle &operator +() const { return *this; }              // �P��+���Z�q.
	inline CAngle operator -() const        { return CAngle( ( s16 )-m_Angle ); } // �P��-���Z�q.

	/// <summary>
	/// to�̒l�Ɍ������ĕ�Ԃ���i1��n�������������ړ��j
	/// </summary>
	inline CAngle TurnN( const CAngle from, const CAngle to, T n );

	/// <summary>
	/// to�̒l�Ɍ������ĕ�Ԃ���i�c��̊p�x��n��������n�����i�ށB�j
	/// </summary>
	inline CAngle FaceN( const CAngle from, const CAngle to, T n );

	/// <summary>
	/// ��� dir �Ɍ������ĕ�Ԃ���
	/// </summary>
	inline void TurnN ( const CAngle dir, T n ) { *this = TurnN( *this , dir , n ) ;}
	inline void Turn2 ( const CAngle dir )      { TurnN( dir , 2 ); }
	inline void Turn4 ( const CAngle dir )      { TurnN( dir , 4 ); }
	inline void Turn8 ( const CAngle dir )      { TurnN( dir , 8 ); }
	inline void Turn16( const CAngle dir )      { TurnN( dir , 16 ); }

	inline void FaceN ( const CAngle dir, T n ) { *this = FaceN( *this , dir , n ); }
	inline void Face2 ( const CAngle dir )      { FaceN( dir , 2 ); }
	inline void Face4 ( const CAngle dir )      { FaceN( dir , 4 ); }
	inline void Face8 ( const CAngle dir )      { FaceN( dir , 8 ); }
	inline void Face16( const CAngle dir )      { FaceN( dir , 16 ); }

	/// <summary>
	/// 256�i�K�̊p�x�ɕϊ�����.
	/// </summary>
	inline u8 To256() const        { return ( u8 )( ( angle + 0x7f ) >> 8 ); }
	inline void From256( u8 from ) { angle = ( s16 )from << 8; }

	/// <summary>
	/// �����o�A�N�Z�X�֐�
	/// </summary>
    inline T& Angle()              { return m_Angle; }
    inline const T& Angle() const  { return m_Angle; }

private:

	/// <summary>
	/// �p�x( 32678.0f = 180�x�i3.14���W�A���j�@�ƂȂ�l�B���W�A���ł��x�ł��Ȃ��j
	/// </summary>
	s16 m_Angle;
};

typedef CAngle< dectype >  CBasicAngle;

//----------------------------�֗��񃁃��o�֐��Q--------------------------------��

/// <summary>
/// ��Βl������CAngle�ŕԂ�
/// </summary>
inline CBasicAngle AngleAbs( const CBasicAngle ang )
{
	return ang < 0 ? -ang : ang;
}

/// <summary>
/// �������v�Z����
/// </summary>
inline CBasicAngle FaceYTo( const CBasicVector3 &to )
{
	return RADtoANG( CMathUtil::Atan2( to.X , to.Z ) );
}
inline CBasicAngle FaceYFromTo( const CBasicVector3 &from, const CBasicVector3 &to )
{
	return FaceYTo( to - from );
}

inline CBasicAngle FaceXTo( const CBasicVector3 &to )
{
	dectype len = to.LengthXZ();
	if ( len > 0.0f )
    {
		return RADtoANG( CMathUtil::Atan( -to.Y / len ) );
	}
	return ( s16 )0;
}
inline CBasicAngle FaceXFromTo( const CBasicVector3 &from, const CBasicVector3 &to )
{
	return FaceXTo( to - from ) ;
}
inline CBasicAngle FaceXTo2( const CBasicVector3 &to )
{
	return RADtoANG( CMathUtil::Asin( -to.Y ) ) ;
}

//�x�N�g���ɕϊ�
inline CBasicVector3 ToVectorY( CBasicAngle a , dectype l )
{
	return CBasicVector3( -l * a.Sin() , 0 , l * a.Cos() ) ;
}

inline CBasicVector3 ToVectorX( CBasicAngle a , dectype l )
{
	return CBasicVector3(  0 , l * a.Sin() , l * a.Cos() ) ;
}

//destAngle�̊p�x���ő�maxAngle�l�܂�toAngle�ɋ߂Â���B
//�߂�l��destAngle��toAngle�̊p�x�̍���Ԃ�
inline CBasicAngle DiffTurn( CBasicAngle &destAngle , CBasicAngle toAngle , CBasicAngle maxAngle )
{
	CBasicAngle diffAngle = toAngle - destAngle;
	CBasicAngle result  = diffAngle;
	if ( diffAngle >  maxAngle ) 
	{
		diffAngle =  maxAngle;
	}
	if ( diffAngle < -maxAngle ) 
	{
		diffAngle = -maxAngle;
	}
	destAngle += diffAngle;
	return result;
}

} } }

#include "Angle.inl"
