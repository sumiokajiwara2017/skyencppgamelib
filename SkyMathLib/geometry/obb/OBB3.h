#pragma once

namespace sky { namespace lib { namespace math {

class COBB3;
typedef SmartPointer< COBB3 > COBB3Ptr;

/// <summary>
/// �n�a�a�iOriented Bounding Box , �L�����E�{�b�N�X�j�N���X
/// </summary>
class COBB3 : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	COBB3();

	/// <summary>
	/// ��̂n�a�a���H
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// �쐬
	/// </summary>
	void Create( const CBasicVector3Vector &points );

	/// <summary>
	/// OBB�̃}�[�W
	/// </summary>
	void Merge( const COBB3 &obb );

	/// <summary>
	/// �C���f�b�N�X�łR�����擾����
	/// </summary>
	CBasicVector3 GetAxis( eAxis axis ) const;

	/// <summary>
	/// �C���f�b�N�X�łR���̑傫���̔����𓾂�
	/// </summary>
	dectype GetRadius( eAxis axis ) const;

	/// <summary>
	/// �_�̍Őڋߓ_�����߂�
	/// </summary>
	void ClosestPtPoint( const CBasicVector3 &point , CBasicVector3 &dest ) const;

	/// <summary>
	/// �_�Ƃ̋����̕����i�Q��j��Ԃ�
	/// </summary>
	dectype SqDistPoint( const CBasicVector3 &point ) const;

	/// <summary>
	/// �傫����K�p����X���̎擾
	/// </summary>
	CBasicVector3 GetExtendedAxisX() const;

	/// <summary>
	/// �傫����K�p����Y���̎擾
	/// </summary>
	CBasicVector3 GetExtendedAxisY() const;

	/// <summary>
	/// �傫����K�p����Z���̎擾
	/// </summary>
	CBasicVector3 GetExtendedAxisZ() const;

	/// <summary>
	/// �������a�̎擾
	/// </summary>
	dectype GetEffectiveDiameter( const CBasicVector3& direction ) const;

	/// <summary>
	/// �������a�̎擾
	/// </summary>
	dectype GetEffectiveRadius( const CBasicVector3& direction ) const;

	/// <summary>
	/// �R�[�i�[�̎擾
	/// 
	/// �ȉ��̐}�̃C���f�b�N�X�ɉ����ăR�[�i�[���擾���܂��B
	/// 0�����S��4���傫���ł��B
    ///     y+
    ///     |
    ///     1----2
    ///    /|   /|
    ///   5-+--4 |
    ///   | 0--+-3-- x+
    ///   |/   |/
    ///   6----7	 
	/// 
	/// </summary>
	CBasicVector3 GetCorner( u32 index ) const;

	/// <summary>
	/// 
	/// �R�[�i�[�z��̎擾
	/// �ȉ��̐}�̃C���f�b�N�X�ɉ����ăR�[�i�[���擾���܂��B
	/// 0�����S��4���傫���ł��B
	///    y+
	///    |
	///    1----2
	///   /|   /|
	///  5-+--4 |
	///  | 0--+-3-- x+
	///  |/   |/
	///  6----7
	/// /
	///z+
	/// 
	/// </summary>
	void GetCornerArray( CBasicVector3 corner[ 8 ] ) const;

	/// <summary>
	/// OBB�ɉ�]�}�g���b�N�X���|����
	/// </summary>
	COBB3 Transform( const CRotationMatrix& matrix ) const;

	/// <summary>
	/// ���ʂƂ̓��I�Ȍ���
	/// </summary>
	dectype IntersectPlane( const CBasicPlane &plane ) const;

	/// <summary>
	/// directAccess
	/// </summary>
    dectype& X()             { return m_Pos.X; }
    const dectype& X() const { return m_Pos.X; };
    dectype& Y()             { return m_Pos.Y; };
    const dectype& Y() const { return m_Pos.Y; };
    dectype& Z()             { return m_Pos.Z; };
    const dectype& Z() const { return m_Pos.Z; };

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	COBB3& operator =( const COBB3& v )
	{
		m_Pos =  v.m_Pos;
		m_Sc =   v.m_Sc;
		m_Matrix = v.m_Matrix;
		return *this;
	}

	/// <summary>
	/// ���S���W
	/// </summary>
	CBasicVector3 m_Pos;

	/// <summary>
	/// �R�������̑傫��
	/// </summary>
	CBasicVector3 m_Sc;

	/// <summary>
	/// �R���̕����i�R���̏�Ԃ͍s��̒�������o����j
	/// </summary>
	CRotationMatrix m_Matrix;

};

} } }