#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �����N���X
/// </summary>
template< typename T >
class CSphere : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSphere();
	CSphere( T x , T y , T z , T radius );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CSphere();

	/// <summary>
	/// ��̋����H
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// ���Ƌ��̍���
	/// </summary>
	const CSphere< T > &Merge( const CSphere< T > &sphere );

	/// <summary>
	/// ���Ɠ_�̍���
	/// ���̒��S���␳�����Bappend�ɔ�ׂċ����傫���Ȃ��Ă��܂��B
	/// </summary>
	const CSphere< T > &Merge( const CVector3< T > &point );

	/// <summary>
	/// ���ɓ_��ǉ�
	/// ���̒��S�͓����Ȃ��B
	/// </summary>
	const CSphere< T > &Append( const CVector3< T > &point );

	/// <summary>
	/// ���_�z�񂩂�K�؂ȋ��𐶐�����
	/// </summary>
	const CSphere< T > &CreateFromPoints( const CVectorPrimitive< CVector3< T > > &points );

	/// <summary>
	/// directAccess
	/// </summary>
    T& X();
    const T& X() const;
    T& Y();
    const T& Y() const;
    T& Z();
    const T& Z() const;
    T& R();
    const T& R() const;

	/// <summary>
	/// getter/setter
	/// </summary>
	void			SetPos( const CVector3< T >& vec );
	void			AddPos( const CVector3< T >& vec );
	const CVector3< T >&	GetPos() const;
	void			SetRadius( dectype radius );
	void			AddRadius( dectype radius );
	T				GetRadius() const;

private:

	/// <summary>
	/// ���S���W
	/// </summary>
	CVector3< T >	m_Pos;

	/// <summary>
	/// ���a
	/// </summary>
	T			m_Radius;
};

//��{�~�^
typedef CSphere< dectype >           CBasicSphere;
typedef SmartPointer< CBasicSphere > CBasicSpherePtr;

} } }

#include "Sphere.inl"