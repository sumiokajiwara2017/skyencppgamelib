#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �􉽊w�Փ˕֗������Q
/// memo
/// �Q�[���̏Փ˔���͍ŏ��ɒP����`�A���ɉ~�̏Փ˔�����s���B
/// </summary>
namespace GeometryCollisionUtil {

	/// <summary>
	/// 2D�_��2D�_�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Pos2CrossPos2( const CVector2< T > &pos1 , const CVector2< T > &pos2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D�_��2D�_��ʂ钼���̊p�x�����W�A���ŕԂ�
	/// </summary>
	template< typename T >
	inline T Pos2AndPos2Angle( const CVector2< T > &pos1 , const CVector2< T > &pos2 );

	/// <summary>
	/// 2D�_����2D���̐����x�N�g�������߂�( �_�Ɛ������s�Ō����Ȃ��ꍇ�A�����x�N�g���͑��݂��Ȃ��̂�skyFalse���Ԃ�)
	/// </summary>
	template< typename T >
	inline skyBool Pos2ToLineVector( const CVector2< T > &pos , const CLine2< T > &line , CVector2< T > &dest );

	/// <summary>
	/// 2D�_��2D���̎n�_����I�_�֐L�т�x�N�g����i�s�����Ƃ��č�( 1 )�A�E( - 1 )�ǂ��瑤�ɂ��邩�A������( 0 )�ɂ��邩�𔻒肷��
	/// </summary>
	template< typename T >
	inline T Pos2ToLineSide( const CVector2< T > &pos , const CLine2< T > &line );

	/// <summary>
	/// 2D����2D���̌�������
	/// 0: �������Ă��Ȃ� 1: ��_�Ō������Ă��� 2: ��_�ȏ�Ō������Ă���
	/// </summary>
	template< typename T >
	inline s32 Line2CrossLine2( const CLine2< T > &line1 , const CLine2< T > &line2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D�_��2D���̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossLine2( const CVector2< T > &point ,  const CLine2< T > &line );

	/// <summary>
	/// �~�Ɖ~�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool CircleCrossCircle( const CCircle< T > &circle1 , const CCircle< T > &circle2 );

	/// <summary>
	/// 2D�_�Ƌ�`�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRect( const CVector2< T > &pos , const CRect< T > &rect );

	/// <summary>
	/// ��`�Ƌ�`�̏Փ˔���(������W�ƕ��ƍ����j
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CRect< T > &rect1 , const CRect< T > &rect2 );

	/// <summary>
	/// �l�p�`�Ǝl�p�`�̏Փ˔���(���S���W�ƕ��ƍ����j
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CVector2< T > &center1 , const CSize< T > &size1 , const CVector2< T > &center2 , const CSize< T > &size2 );

	/// <summary>
	/// 2D�_�Ɖ~�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossCircle( const CVector2< T > &point , const CCircle< T > &circle );

	/// <summary>
	/// 2D���Ɖ~�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Line2CrossCircle( const CLine2< T > &line , const CCircle< T > &circle );

	/// <summary>
	/// 2D�_��2D���C�Ƃ̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRay2( const CVector2< T > &point , const CRay2< T > &ray2 );

	/// <summary>
	/// 2D�_�Ƒȉ~�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossEllipse( const CVector2< T > &point , const CEllipse< T > &ellipse );

	/// <summary>
	/// �ȉ~�Ƒȉ~�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool EllipseCrossEllipse( const CEllipse< T > &ellipse1 , const CEllipse< T > &ellipse2 );

	/// <summary>
	/// 3D�_��3D�_�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossPos3( const CVector3< T > &pos1 , const CVector3< T > &pos2 );

	/// <summary>
	/// 3D�_��3D���C�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossRay3( const CVector3< T > &pos , const CRay3< T > &ray );

	/// <summary>
	/// 3D�_��3D���̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossLine3( const CVector3< T > &pos , const CLine3< T > &line );

	/// <summary>
	/// 3D�_�Ƌ��̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossSphere( const CVector3< T > &pos , const CSphere< T > &sphere );

	/// <summary>
	/// 3D����3D���̏Փ�
	/// </summary>
	template< typename T >
	inline skyBool Line3CrossLine3( const CLine3< T > &line1 , const CLine3< T > &line2 );

	/// <summary>
	/// 3D����3D���Q�̏Փ�
	/// </summary>
	inline skyBool Line3CrossLine3Fence( const CBasicLine3 &line , const math::CBasicVector3Vector &fence , skyBool isHitCheck2D = skyFalse );

	/// <summary>
	/// ���ʏ��3D�_�ƎO�p�`�̓��O����
	/// </summary>
	template< typename T >
	inline skyBool PlanePos3ToTriangleInSide( const CPlane< T > &plane , const CTriangle3< T > &triangle );

	/// <summary>
	/// 3D���C�ƕ��ʂ̏Փˁ���_�擾(skyTrue:��_����BskyFalse:��_�Ȃ��j
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossPlane( const CRay3< T > &ray , const CPlane< T > &plane , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 3D���C�ƎO�p�`�̏Փˁ���_�擾(skyTrue:��_����BskyFalse:��_�Ȃ��j
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossTriangle3( const CRay3< T > &ray , const CTriangle3< T > &triangle , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ���Ƌ��̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossSphere( const CSphere< T > &spher1 , const CSphere< T > &spher2 );

	/// <summary>
	/// ���ƎO�p�`�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossTriangle3( const CSphere< T > &sphere , const CTriangle3< T > &triangle );

	/// <summary>
	/// ���ƕ��ʂ̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossPlane( const CSphere< T > &sphere , const CPlane< T > &plane );

	/// <summary>
	/// ����3D�����̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossLine3( const CSphere< T > &sphere , const CLine3< T > &line );

	/// <summary>
	/// ����3D���C�̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossRay3( const CSphere< T > &sphere , const CRay3< T > &ray , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossCapsule3( const CSphere< T > &sphere , const Capsule3< T > &capsule );

	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	template< typename T >
	inline skyBool Capsule3CrossCapsule3( const Capsule3< T > &capsule1 , const Capsule3< T > &capsule2 );

	/// <summary>
	/// �`�`�a�a�Q�Ɠ_�̏Փ˔���
	/// </summary>
	inline skyBool AABB2CrossPoint2( const CAABB2 &aabb2 , const CBasicVector2 &p );

	/// <summary>
	/// �`�`�a�a�Q�Ɛ����Q�̏Փ˔���
	/// </summary>
	inline skyBool AABB2CrossLine2( const CAABB2 &aabb2 , const CBasicLine2 &line , CollisionResultInfo *info = skyNull );
	
	/// <summary>
	/// �`�`�a�a�Q�Ƃ`�`�a�a�Q�̏Փ˔���i�������Ă��Ă������Ɏw�肪����Ό����ɂ���Đ������`�`�a�a�Q��Ԃ��j
	/// </summary>
	inline skyBool AABB2CrossAABB2( const CAABB2 &aabb1 ,  const CAABB2 &aabb2 , CAABB2 *boxIntersect = skyNull );

	/// <summary>
	/// �`�`�a�a�R�Ɠ_�̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossPoint3( const CAABB3 &aabb3 , const CBasicVector3 &p );

	/// <summary>
	/// �`�`�a�a�R�Ƌ��̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossSphere( const CAABB3 &aabb3 , const CBasicSphere &sphere );

	/// <summary>
	/// �`�`�a�a�R�Ɛ����R�̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossLine3( const CAABB3 &aabb3 , const CBasicLine3 &line , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// �`�`�a�a�R�ƕ��ʂ̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossPlane( const CAABB3 &aabb3 , const CBasicPlane &plane );

	/// <summary>
	/// �`�`�a�a�R�ƎO�p�`�R�̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossTriangle3( const CAABB3 &aabb3 , const CBasicTriangle3 &triangle );

	/// <summary>
	/// �`�`�a�a�R�Ƃ`�`�a�a�R�̏Փ˔���
	/// </summary>
	inline skyBool AABB3CrossAABB3( const CAABB3 &aabb1 , const CAABB3 &aabb2 , CAABB3 *boxsIntersect = skyNull );

	/// <summary>
	/// �`�`�a�a�R�Ƃn�a�a�R�̏Փ˔���i�������`�`�a�a�A�n�a�a���m���Ɣ����܂��B�ʂ̏Փ˔�����s���Ă��Ȃ��ׂł��B������肩�̓}�V�Ƃ��������ł��B�j
	/// </summary>
	inline skyBool AABB3CrossOBB3( const CAABB3 &aabb3 , const COBB3 &obb );

	/// <summary>
	/// �n�a�a�Q�Ɛ����Q�̏Փ˔���
	/// </summary>
	inline skyBool OBB2CrossLine2( const COBB2 &obb2 , const CBasicLine2 &line );

	/// <summary>
	/// �n�a�a�Q�Ƃn�a�a�Q�̏Փ˔���
	/// </summary>
	inline skyBool OBB2CrossOBB2( const COBB2 &obb1 , const COBB2 &obb2 );

	/// <summary>
	/// �n�a�a�R�Ƌ��̏Փ˔���
	/// </summary>
	inline skyBool OBB3CrossSphere( const COBB3 &obb , const CBasicSphere &sphere , CollisionResultInfo *info = skyNull ); //clossPoint�͎��ۂ͂n�a�a�R�̓_�̒��ł����Ƃ����̒��S�ɋ߂��_�Փ˓_�̋ߎ��l

	/// <summary>
	/// �n�a�a�R�Ɛ����R�̏Փ˔���
	/// </summary>
	inline skyBool OBB3CrossLine3( const COBB3 &obb , const CBasicLine3 &line );

	/// <summary>
	/// �n�a�a�R�Ə��̏Փ˔���
	/// </summary>
	inline skyBool OBB3CrossPlane( const COBB3 &obb , const CBasicPlane &plane );

	/// <summary>
	/// �n�a�a�R�Ƃn�a�a�R�̏Փ˔���
	/// </summary>
	inline skyBool OBB3CrossOBB3( const COBB3 &obb1 , const COBB3 &obb2 );

}

} } }

#include "GeometryCollisionUtil.inl"
