#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

class CAABB2;
typedef SmartPointer< CAABB2 > CAABB2Ptr;

/// <summary>
/// �`�`�a�a�iAxis Aligned Bounding Box , �������{�b�N�X�j�N���X
///---------------------------------------------------------------------------
/// AABB3::corner
///
/// �W�̊p�_�̂P��Ԃ�
/// �����̓_�͈ȉ��̂悤�ɔԍ��t�������B
///
///  2                            3 
///   +----------------------------+
///   |                            |
///   |                            |      +Y
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |
///   |                            |      |     
///   |                            |      |    
///   |                            |      |   
///   |                            |      |  
///   |                            |      | 
///   |                            |      |
///   |                            |      ----------------- +X
///   ------------------------------
///  0                              1
///
/// Bit 0 �� mini.X vs. max.X
/// Bit 1 �� mini.Y vs. max.Y
/// </summary>
class CAABB2 : public CGeometryBase
{
public:

	/// <summary>
	/// �ő�l�ƍŏ��l�i���ɃV���v���j
	/// </summary>
	CBasicVector2 m_Min;
	CBasicVector2 m_Max;

	CBasicVector2 Size  () const { return m_Max - m_Min; }
	dectype       SizeX ()       { return m_Max.X - m_Min.X; }
	dectype       SizeY ()       { return m_Max.Y - m_Min.Y; }
	CBasicVector2 Center() const { return (m_Min + m_Max) * 0.5f;	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAABB2();

	/// <summary>
	/// ��̂`�`�a�a���H
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// �ړ�
	/// </summary>
	void AddX( dectype x );
	void AddY( dectype y );

	/// <summary>
	/// �S�̊p�̓_�̂P��Ԃ�
	/// </summary>
	CBasicVector2 Corner( s32 i ) const;

// �{�b�N�X����

	/// <summary>
	/// �����̒l�Ɏ��ۂɑ傫���^������������ݒ肷�邱�ƂŃ{�b�N�X����ɂ���
	/// </summary>
	void Empty();

	/// <summary>
	/// �{�b�N�X�ɓ_��ǉ�����
	/// </summary>
	void Merge( const CBasicVector2 &p );

	/// <summary>
	/// AABB���{�b�N�X�ɒǉ�����
	/// </summary>
	void Merge( const CAABB2 &box );

	/// <summary>
	/// ���̃{�b�N�X�����W�ϊ����A�V�����`�`�a�a���v�Z����
	/// ����͏�ɁA���Ȃ��Ƃ����̃{�b�N�X�Ɠ����傫���̂`�`�a�a�ƂȂ邱�Ƃ�Y��Ȃ��悤��
	/// �����傫���Ȃ�ꍇ������
	/// </summary>
	void SetToTransformedBox( const CAABB2 &box , const math::CMatrix3x2 &m );

// ����^�����e�X�g

	/// <summary>
	/// ���̃{�b�N�X����̏ꍇ��skyTrue��Ԃ�
	/// </summary>
	skyBool IsEmpty() const;

	/// <summary>
	/// ����_�ɑ΂��Ă��̃{�b�N�X��ōł��߂��_��Ԃ�
	/// </summary>
	CBasicVector2 ClosestPointTo( const CBasicVector2 &p ) const;

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	CAABB2& operator =( const CAABB2& v )
	{
		m_Min = v.m_Min;
		m_Max = v.m_Max;
		return *this;
	}
};

/// <summary>
/// �����Ă���`�`�a�a�����q���Ă���`�`�a�a�ƌ������Ă���Ƃ��̃p�����g���b�N�ȓ_��Ԃ�
/// �������Ȃ��ꍇ�́��P��Ԃ�
/// </summary>
dectype		IntersectMovingAABB2( const CAABB2 &stationaryBox ,	const CAABB2 &movingBox ,	const CBasicVector2 &d );

} } }