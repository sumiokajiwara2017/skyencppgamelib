#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 角度ベクトルクラス
/// </summary>
template< typename T >
class CAngleVector3 : public base::SkyObject
{

public:

	/// <summary>
	/// メンバアクセス関数
	/// </summary>
    inline CAngle< T >&       X()                                                          { return m_X; }
    inline const CAngle< T >& X() const                                                    { return m_X; }
    inline CAngle< T >&       Y()                                                          { return m_Y; }
    inline const CAngle< T >& Y() const                                                    { return m_Y; }
    inline CAngle< T >&       Z()                                                          { return m_Z; }
    inline const CAngle< T >& Z() const                                                    { return m_Z; }
    inline CAngle< T >&       W()                                                          { return m_W; }
    inline const CAngle< T >& W() const                                                    { return m_W; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CAngleVector3(){}
	inline CAngleVector3( CAngle< T > x , CAngle< T > y , CAngle< T > z )                  { m_X = x , m_Y = y , m_Z = z; }
	inline CAngleVector3( u8 initType , T x , T y , T z ); //initType 0:angle 1:radian 2:degree
	inline CAngleVector3( const CAngleVector3 &src )                                       { m_X = src.m_X , m_Y = src.m_Y , m_Z = src.m_Z; }

	/// <summary>
	/// 演算子オーバーライド
	/// </summary>
	inline CAngleVector3 &operator =( const CAngleVector3 &src )                           { m_X = src.m_X , m_Y = src.m_Y , m_Z = src.m_Z; return *this ; }	
	friend inline bool operator ==(const CAngleVector3 &src1 , const CAngleVector3 &src2 ) { return src1.m_X == src2.m_X && src1.m_Y == src2.m_Y && src1.m_Z == src2.m_Z; }
	friend inline bool operator !=(const CAngleVector3 &src1 , const CAngleVector3 &src2 ) { return src1.m_X != src2.m_X || src1.m_Y != src2.m_Y || src1.m_Z != src2.m_Z; }
	inline const CAngleVector3 &operator +() const                                         { return *this; }
	inline CAngleVector3 operator -() const                                                { return CAngleVector3( -m_X , -m_Y , -m_Z ); }
	inline const CAngleVector3 operator +( const CAngleVector3 &src ) const                { return CAngleVector3( m_X + src.m_X, m_Y + src.m_Y , m_Z + src.m_Z ); }
	inline const CAngleVector3 operator -( const CAngleVector3 &src ) const                { return CAngleVector3( m_X - src.m_X, m_Y - src.m_Y , m_Z - src.m_Z );	}
	inline const CAngleVector3 operator *( const CAngleVector3 &src ) const                { return CAngleVector3( m_X * src.m_X, m_Y * src.m_Y , m_Z * src.m_Z );	}
	inline const CAngleVector3 operator +( T src ) const                                   { return CAngleVector3( m_X + src , m_Y + src , m_Z + src ); }
	inline const CAngleVector3 operator -( T src ) const                                   { return CAngleVector3( m_X - src , m_Y - src , m_Z - src ); }
	inline const CAngleVector3 operator *( T src ) const                                   { return CAngleVector3( m_X * src , m_Y * src , m_Z * src ); }
	inline const CAngleVector3 operator /( T src ) const                                   { return CAngleVector3( m_X / src , m_Y / src , m_Z / src ); }
	inline CAngleVector3 &operator+=( const CAngleVector3 &src )	                       { m_X += src.m_X , m_Y += src.m_Y , m_Z += src.m_Z; return *this; }
	inline CAngleVector3 &operator-=( const CAngleVector3 &src )	                       { m_X -= src.m_X , m_Y -= src.m_Y , m_Z -= src.m_Z; return *this; }
	inline CAngleVector3 &operator+=( T src )                                              { m_X += src , m_Y += src , m_Z += src; return *this; }
	inline CAngleVector3 &operator-=( T src )                                              { m_X -= src , m_Y -= src , m_Z -= src; return *this; }
	inline CAngleVector3 &operator/=( T src )                                              { m_X /= src , m_Y /= src , m_Z /= src; return *this; }
	inline CAngleVector3 &operator*=( T src )                                              { m_X *= src , m_Y *= src , m_Z *= src; return *this; }

	/// <summary>
	/// ゼロ初期化
	/// </summary>
	inline CAngleVector3 &ClearZero()	                                                  { m_X = 0 , m_Y = 0 , m_Z = 0; return *this ; }

	/// <summary>
	/// 指定の値に向かって補間する
	/// </summary>
	inline void TurnN ( const CAngleVector3 &dir , T n )                                  { m_X.TurnN( dir.m_X , n ); m_Y.TurnN( dir.m_Y , n ); m_Z.TurnN( dir.m_Z , n ); }
	inline void Turn2 ( const CAngleVector3 &dir )                                        { TurnN( dir , 2 );  }
	inline void Turn4 ( const CAngleVector3 &dir )                                        { TurnN( dir , 4 );  }
	inline void Turn8 ( const CAngleVector3 &dir )                                        { TurnN( dir , 8 );  }
	inline void Turn16( const CAngleVector3 &dir )                                        { TurnN( dir , 16 ); }

	inline void FaceN ( const CAngleVector3 &dir , T n )                                  { m_X.FaceN( dir.m_X , n ); m_Y.FaceN( dir.m_Y , n ); m_Z.FaceN( dir.m_Z , n ); }
	inline void Face2 ( const CAngleVector3 &dir )                                        { FaceN( dir , 2 );  }
	inline void Face4 ( const CAngleVector3 &dir )                                        { FaceN( dir , 4 );  }
	inline void Face8 ( const CAngleVector3 &dir )                                        { FaceN( dir , 8 );  }
	inline void Face16( const CAngleVector3 &dir )                                        { FaceN( dir , 16 ); }

	/// <summary>
	/// 行列に変換する
	/// </summary>
	inline CRotationMatrix &ToMatrixXYZ( CRotationMatrix &dest ) const;
	inline CRotationMatrix &ToMatrixXZY( CRotationMatrix &dest ) const;
	inline CRotationMatrix &ToMatrixYXZ( CRotationMatrix &dest ) const;
	inline CRotationMatrix &ToMatrixYZX( CRotationMatrix &dest ) const;
	inline CRotationMatrix &ToMatrixZXY( CRotationMatrix &dest ) const;
	inline CRotationMatrix &ToMatrixZYX( CRotationMatrix &dest ) const;

	inline CMatrix4x3 &ToMatrixXYZ( CMatrix4x3 &dest ) const;
	inline CMatrix4x3 &ToMatrixXZY( CMatrix4x3 &dest ) const;
	inline CMatrix4x3 &ToMatrixYXZ( CMatrix4x3 &dest ) const;
	inline CMatrix4x3 &ToMatrixYZX( CMatrix4x3 &dest ) const;
	inline CMatrix4x3 &ToMatrixZXY( CMatrix4x3 &dest ) const;
	inline CMatrix4x3 &ToMatrixZYX( CMatrix4x3 &dest ) const;

	/// <summary>
	/// ベクトルに変換する
	/// </summary>
	inline void          ToVector( CVector3< T > &dest , T len = 1 ) const;
	inline CVector3< T > ToVector( T len = 1 ) const;

	/// <summary>
	/// ベクトルを回転する
	/// </summary>
	inline void          RotVector( CVector3< T > &dest ) const;
	inline CVector3< T > RotVector( const CVector3< T > &src ) const;

private:

	/// <summary>
	/// メンバ変数
	/// </summary>
	CAngle< T > m_X;
	CAngle< T > m_Y;
	CAngle< T > m_Z;
	CAngle< T > m_W; //SPU等のＣＰＵは128bit単位でデータを扱うため、T型でメンバを持つ際４成分あったほうが処理効率が良い。
};

//基本角度３型
typedef CAngleVector3< dectype > CBasicAngleVector3;

/// <summary>
/// ZERO角度３
/// </summary>
static const CBasicAngleVector3 CBasicAngleVector3_ZERO( 0.0f , 0.0f , 0.0f );

//----------------------------便利非メンバ関数群--------------------------------▽

/// <summary>
/// 角度の補間( t = 0.0f ～ 1.0f )
/// </summary>
inline CBasicAngleVector3 LerpAngleVec_Liner( dectype t , const CBasicAngleVector3 &from , const CBasicAngleVector3 &to );

inline CBasicAngleVector3 FaceTo( const CBasicVector3 &to );

inline CBasicAngleVector3 FaceTo2( const CBasicVector3 &to );

inline CBasicAngleVector3 FaceFromTo( const CBasicVector3 &from, const CBasicVector3 &to );

inline CBasicAngleVector3 faceTo( const CMatrix4x3 &rotMat );

} } }

#include "AngleVec3.inl"