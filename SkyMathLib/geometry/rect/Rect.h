#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ��`���N���X(���W�͂����ĂR�����j
/// </summary>
template< typename T >
class CRect : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRect();
	CRect( T x , T y, T w, T h );

	/// <summary>
	/// �f�X�g���N�^
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
	/// �_����
	/// </summary>
	skyBool DotHitCheck( const CVector2< T >& dot ) const;

	/// <summary>
	/// ��`����
	/// </summary>
	skyBool RectHitCheck( const CRect& rect ) const;

	/// <summary>
	/// ���S�_�̎擾
	/// </summary>
	void GetCenterPoint( CVector2< T >& centerPos ) const;

	/// <summary>
	/// �������ݔ���
	/// </summary>
	skyBool InsideCheck( const CRect& rect ) const;

	/// <summary>
	/// �l����(x ,y,w,h���S��0�̏ꍇTRUE )
	/// </summary>
	skyBool	IsEmpty() const;

	/// <summary>
	/// �S�_���W���擾
	/// </summary>
	void GetPos( CVector2< T > rect[] );

private:

	/// <summary>
	/// ���W
	/// </summary>
	CVector2< T >	m_Pos;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	CSize< T >	m_kSize;

};

//��{��`�^
typedef CRect< dectype >             CBasicRect;
typedef SmartPointer< CBasicRect >   CBasicRectPtr;
typedef CList< u32 , CBasicRectPtr > CBasicRectPtrList;
typedef CRect< u32 >                 CRect_u32;

} } }

#include "Rect.inl"