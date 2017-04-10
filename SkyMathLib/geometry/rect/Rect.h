#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 矩形情報クラス(座標はあえて３次元）
/// </summary>
template< typename T >
class CRect : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRect();
	CRect( T x , T y, T w, T h );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRect();

	/// <summary>
	/// directAccess
	/// </summary>
    T& X();
    const T& X() const;
    T& Y();
    const T& Y() const;
    T& W();
    const T& W() const;
    T& H();
    const T& H() const;

	/// <summary>
	/// getter/setter
	/// </summary>
	void                    SetPos( const CVector2< T >& vec );
	void                    AddPos( const CVector2< T >& vec );
	const CVector2< T >&    GetPos() const;
	void                    SetSize( const CSize< T >& size );
	void                    SetSize( const CVector2< T > &size );
	const CSize< T >&       GetSize() const;
    CVector2< T >           GetSizeVec2() const;
	void                    Set( const CVector2< T >& vec , const CSize< T >& size );
	void                    Set( T x , T y, T w, T h );

	/// <summary>
	/// 点判定
	/// </summary>
	skyBool DotHitCheck( const CVector2< T >& dot ) const;

	/// <summary>
	/// 矩形判定
	/// </summary>
	skyBool RectHitCheck( const CRect& rect ) const;

	/// <summary>
	/// 中心点の取得
	/// </summary>
	void GetCenterPoint( CVector2< T >& centerPos ) const;

	/// <summary>
	/// 内側存在判定
	/// </summary>
	skyBool InsideCheck( const CRect& rect ) const;

	/// <summary>
	/// 値が空(x ,y,w,hが全部0の場合TRUE )
	/// </summary>
	skyBool	IsEmpty() const;

	/// <summary>
	/// ４点座標を取得
	/// </summary>
	void GetPos( CVector2< T > rect[] );

private:

	/// <summary>
	/// 座標
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// サイズ
	/// </summary>
	CSize< T >	m_kSize;

};

//基本矩形型
typedef CRect< dectype >             CBasicRect;
typedef SmartPointer< CBasicRect >   CBasicRectPtr;
typedef CList< u32 , CBasicRectPtr > CBasicRectPtrList;
typedef CRect< u32 >                 CRect_u32;

} } }

#include "Rect.inl"