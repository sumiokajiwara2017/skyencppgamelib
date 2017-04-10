#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 角度ベクトルクラス
/// </summary>
template< typename T >
class CAngleVector2 : public base::SkyObject
{

public:

	/// <summary>
	/// メンバアクセス関数
	/// </summary>
    inline CAngle< T >&       Rot()                                                        { return m_Rot; }
    inline const CAngle< T >& Rot() const                                                  { return m_Rot; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CAngleVector2(){}
	inline CAngleVector2( CAngle< T > rot )                                                { m_Rot = rot; }
	inline CAngleVector2( T rot )                                                          { m_Rot = rot; }
	inline CAngleVector2( const CAngleVector2 &src )                                       { m_Rot = src.m_Rot; }

	/// <summary>
	/// 演算子オーバーライド
	/// </summary>
	inline CAngleVector2 &operator =( const CAngleVector2 &src )                           { m_Rot = src.m_Rot; return *this ; }	
	friend inline bool operator ==(const CAngleVector2 &src1 , const CAngleVector2 &src2 ) { return src1.m_Rot == src2.m_Rot; }
	friend inline bool operator !=(const CAngleVector2 &src1 , const CAngleVector2 &src2 ) { return src1.m_Rot != src2.m_Rot; }
	inline const CAngleVector2 &operator +() const                                         { return *this ; }
	inline CAngleVector2 operator -() const                                                { return CAngleVector2( -m_Rot ); }
	inline const CAngleVector2 operator +( const CAngleVector2 &src ) const                { return CAngleVector2( m_Rot + src.m_Rot ); }
	inline const CAngleVector2 operator -( const CAngleVector2 &src ) const                { return CAngleVector2( m_Rot - src.m_Rot );	}
	inline const CAngleVector2 operator *( const CAngleVector2 &src ) const                { return CAngleVector2( m_Rot * src.m_Rot );	}
	inline const CAngleVector2 operator +( T src ) const                                   { return CAngleVector2( m_Rot + src ); }
	inline const CAngleVector2 operator -( T src ) const                                   { return CAngleVector2( m_Rot - src ); }
	inline const CAngleVector2 operator *( T src ) const                                   { return CAngleVector2( m_Rot * src ); }
	inline const CAngleVector2 operator /( T src ) const                                   { return CAngleVector2( m_Rot / src ); }
	inline CAngleVector2 &operator+=( const CAngleVector2 &src )	                       { m_Rot += src.m_Rot; return *this; }
	inline CAngleVector2 &operator-=( const CAngleVector2 &src )	                       { m_Rot -= src.m_Rot; return *this; }
	inline CAngleVector2 &operator+=( T src )                                              { m_Rot += src; return *this; }
	inline CAngleVector2 &operator-=( T src )                                              { m_Rot -= src; return *this; }
	inline CAngleVector2 &operator/=( T src )                                              { m_Rot /= src; return *this; }
	inline CAngleVector2 &operator*=( T src )                                              { m_Rot *= src; return *this; }

	/// <summary>
	/// ゼロ初期化
	/// </summary>
	inline CAngleVector2 &ClearZero()	                                                   { m_Rot = 0; return *this ; }

	/// <summary>
	/// 指定の値に向かって補間する
	/// </summary>
	inline void TurnN ( const CAngleVector2 &dir , T n )                                   { m_Rot.TurnN( dir.m_Rot , n ); }
	inline void Turn2 ( const CAngleVector2 &dir )                                         { TurnN( dir , 2 );  }
	inline void Turn4 ( const CAngleVector2 &dir )                                         { TurnN( dir , 4 );  }
	inline void Turn8 ( const CAngleVector2 &dir )                                         { TurnN( dir , 8 );  }
	inline void Turn16( const CAngleVector2 &dir )                                         { TurnN( dir , 16 ); }

	inline void FaceN ( const CAngleVector2 &dir , T n )                                   { m_Rot.FaceN( dir.m_Rot , n ); }
	inline void Face2 ( const CAngleVector2 &dir )                                         { FaceN( dir , 2 );  }
	inline void Face4 ( const CAngleVector2 &dir )                                         { FaceN( dir , 4 );  }
	inline void Face8 ( const CAngleVector2 &dir )                                         { FaceN( dir , 8 );  }
	inline void Face16( const CAngleVector2 &dir )                                         { FaceN( dir , 16 ); }

	/// <summary>
	/// 行列に変換する
	/// </summary>
	inline CMatrix3x2 &ToMatrix( CMatrix3x2 &dest ) const;
	inline CMatrix4x3 &ToMatrix( CMatrix4x3 &dest ) const;

	/// <summary>
	/// ベクトルに変換する
	/// </summary>
	inline void          ToVector( CVector2< T > &dest , T len = 1 ) const;
	inline CVector2< T > ToVector( T len = 1 ) const;

	/// <summary>
	/// ベクトルを回転する
	/// </summary>
	inline void          RotVector( CVector2< T > &dest ) const;
	inline CVector2< T > RotVector( const CVector2< T > &src ) const;

private:

	/// <summary>
	/// メンバ変数
	/// </summary>
	CAngle< T > m_Rot;
};

//基本角度２型
typedef CAngleVector2< dectype > CBasicAngleVector2;

/// <summary>
/// ZERO角度２
/// </summary>
static const CBasicAngleVector2 CBasicAngleVector2_ZERO( 0.0f );

} } }

#include "AngleVec2.inl"