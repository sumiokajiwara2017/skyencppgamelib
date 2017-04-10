#pragma once

namespace sky { namespace lib { namespace math {

class COBB2;
typedef SmartPointer< COBB2 > COBB2Ptr;

/// <summary>
/// �n�a�a�iOriented Bounding Box , �L�����E�{�b�N�X�j�N���X
/// </summary>
class COBB2 : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	COBB2();

	/// <summary>
	/// ��̂n�a�a���H
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// �쐬
	/// </summary>
	void Create( const CBasicVector2Vector &points );

	/// <summary>
	/// OBB�̃}�[�W
	/// </summary>
	void Merge( const COBB2 &obb );

	/// <summary>
	/// �C���f�b�N�X�łR�����擾����
	/// </summary>
	CBasicVector2 GetAxis( eAxis axis ) const;

	/// <summary>
	/// �C���f�b�N�X�łR���̑傫���̔����𓾂�
	/// </summary>
	dectype GetRadius( eAxis axis ) const;

	/// <summary>
	/// �_�̍Őڋߓ_�����߂�
	/// </summary>
	void ClosestPtPoint( const CBasicVector2 &point , CBasicVector2 &dest ) const;

	/// <summary>
	/// �_�Ƃ̋����̕����i�Q��j��Ԃ�
	/// </summary>
	dectype SqDistPoint( const CBasicVector2 &point ) const;

	/// <summary>
	/// �傫����K�p����X���̎擾
	/// </summary>
	CBasicVector2 GetExtendedAxisX() const;

	/// <summary>
	/// �傫����K�p����Y���̎擾
	/// </summary>
	CBasicVector2 GetExtendedAxisY() const;

	/// <summary>
	/// �������a�̎擾
	/// </summary>
	dectype GetEffectiveDiameter( const CBasicVector2& direction ) const;

	/// <summary>
	/// �������a�̎擾
	/// </summary>
	dectype GetEffectiveRadius( const CBasicVector2& direction ) const;

	/// <summary>
	/// �R�[�i�[�̎擾
	/// 
	/// �ȉ��̐}�̃C���f�b�N�X�ɉ����ăR�[�i�[���擾���܂��B
	/// 0�����S��4���傫���ł��B
    /// y+
    /// |
    /// 1-+--2
    /// |    |
    /// |    |
    /// 0----3-- x+	 
	/// </summary>
	CBasicVector2 GetCorner(int index) const;

	/// <summary>
	/// 
	/// �R�[�i�[�z��̎擾
	/// �ȉ��̐}�̃C���f�b�N�X�ɉ����ăR�[�i�[���擾���܂��B
	/// 0�����S��4���傫���ł��B
    /// y+
    /// |
    /// 1-+--2
    /// |    |
    /// |    |
    /// 0----3-- x+	 
	/// </summary>
	void GetCornerArray( CBasicVector2 corner[ 4 ] ) const;

	/// <summary>
	/// OBB�ɉ�]�}�g���b�N�X���|����
	/// </summary>
	COBB2 Transform( const CMatrix3x2& matrix ) const;

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

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	COBB2& operator =( const COBB2& v )
	{
		m_Pos =  v.m_Pos;
		m_Sc =   v.m_Sc;
		m_Matrix = v.m_Matrix;
		return *this;
	}

	/// <summary>
	/// ���S���W
	/// </summary>
	CBasicVector2 m_Pos;

	/// <summary>
	/// �R�������̑傫��
	/// </summary>
	CBasicVector2 m_Sc;

	/// <summary>
	/// �R���̕����i�R���̏�Ԃ͍s��̒�������o����j
	/// </summary>
	CMatrix3x2 m_Matrix;

};

} } }