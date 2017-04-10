#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

class CAABB3;
typedef SmartPointer< CAABB3 > CAABB3Ptr;

/// <summary>
/// �`�`�a�a�iAxis Aligned Bounding Box , �������{�b�N�X�j�N���X
///---------------------------------------------------------------------------
/// AABB3::corner
///
/// �W�̊p�_�̂P��Ԃ�
/// �����̓_�͈ȉ��̂悤�ɔԍ��t�������B
///
///            6                                7
///              ------------------------------
///             /|                           /|
///            / |                          / |
///           /  |                         /  |
///          /   |                        /   |
///         /    |                       /    |
///        /     |                      /     |
///       /      |                     /      |
///      /       |                    /       |
///     /        |                   /        |
///  2 /         |                3 /         |
///   /----------------------------/          |
///   |          |                 |          |
///   |          |                 |          |      +Y
///   |        4 |                 |          | 
///   |          |-----------------|----------|      |
///   |         /                  |         /  5    |
///   |        /                   |        /        |       +Z
///   |       /                    |       /         |
///   |      /                     |      /          |     /
///   |     /                      |     /           |    /
///   |    /                       |    /            |   /
///   |   /                        |   /             |  /
///   |  /                         |  /              | /
///   | /                          | /               |/
///   |/                           |/                ----------------- +X
///   ------------------------------
///  0                              1
///
/// Bit 0 �� mini.X vs. max.X
/// Bit 1 �� mini.Y vs. max.Y
/// Bit 2 �� mini.Z vs. max.Z
/// </summary>
class CAABB3 : public CGeometryBase
{
public:

	/// <summary>
	/// �ő�l�ƍŏ��l�i���ɃV���v���j
	/// </summary>
	CBasicVector3 m_Min;
	CBasicVector3 m_Max;

	CBasicVector3 Size  () const { return m_Max - m_Min; }
	dectype       SizeX ()       { return m_Max.X - m_Min.X; }
	dectype       SizeY ()       { return m_Max.Y - m_Min.Y; }
	dectype       SizeZ ()       { return m_Max.Z - m_Min.Z; }
	CBasicVector3 Center() const { return (m_Min + m_Max) * 0.5f;	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAABB3();

	/// <summary>
	/// ��̂`�`�a�a���H
	/// </summary>
	skyBool IsZero() const;

	/// <summary>
	/// �ړ�
	/// </summary>
	void AddX( dectype x );
	void AddY( dectype y );
	void AddZ( dectype z );

	/// <summary>
	/// �W�̊p�̓_�̂P��Ԃ�
	/// </summary>
	CBasicVector3 Corner( s32 i ) const;

// �{�b�N�X����

	/// <summary>
	/// �����̒l�Ɏ��ۂɑ傫���^������������ݒ肷�邱�ƂŃ{�b�N�X����ɂ���
	/// </summary>
	void Empty();

	/// <summary>
	/// �{�b�N�X�ɓ_��ǉ�����
	/// </summary>
	void Merge( const CBasicVector3 &p );

	/// <summary>
	/// AABB���{�b�N�X�ɒǉ�����
	/// </summary>
	void Merge( const CAABB3 &box );

	/// <summary>
	/// ���̃{�b�N�X�����W�ϊ����A�V�����`�`�a�a���v�Z����
	/// ����͏�ɁA���Ȃ��Ƃ����̃{�b�N�X�Ɠ����傫���̂`�`�a�a�ƂȂ邱�Ƃ�Y��Ȃ��悤��
	/// �����傫���Ȃ�ꍇ������
	/// </summary>
	void SetToTransformedBox( const CAABB3 &box , const math::CMatrix4x3 &m );

// ����^�����e�X�g

	/// <summary>
	/// ���̃{�b�N�X����̏ꍇ��skyTrue��Ԃ�
	/// </summary>
	skyBool IsEmpty() const;

	/// <summary>
	/// ����_�ɑ΂��Ă��̃{�b�N�X��ōł��߂��_��Ԃ�
	/// </summary>
	CBasicVector3 ClosestPointTo( const CBasicVector3 &p ) const;

	/// <summary>
	/// �{�b�N�X�����ʂ̂ǂ��瑤�ɂ��邩�𕪗ނ���
	/// </summary>
	s32 ClassifyPlane( const CBasicPlane &plane ) const;

	/// <summary>
	/// ���ʂƂ̓��I�Ȍ���
	/// 
	/// ���I��AABB�\ ���ʂ̌����e�X�g�����s����
	/// 
	/// n�͂��̕��ʂ̖@��(���K������Ă���Ƃ���)
	/// planeD�́A���̕��ʂ̕�����p�En=d��D�l�ł���
	/// dir��AABB�̈ړ�����
	/// 
	/// ���̕��ʂ͐Î~���Ă���Ɖ��肷��
	/// 
	/// �����̃p�����g���b�N�ȓ_(�������N����O�Ɉړ���������)��Ԃ�
	/// �������Ȃ��ꍇ�́A�ƂĂ��傫�Ȑ���Ԃ�
	/// �ړ��̒����ɑ΂��ă`�F�b�N����K�v������
	/// 
	/// ���̕��ʂ̑O�ʂƂ̌������������o�����
	/// </summary>
	dectype IntersectPlane( const CBasicPlane &plane , const CBasicVector3 &dir ) const;

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	CAABB3& operator =( const CAABB3& v )
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
dectype		IntersectMovingAABB3( const CAABB3 &stationaryBox ,	const CAABB3 &movingBox ,	const CBasicVector3 &d );

} } }