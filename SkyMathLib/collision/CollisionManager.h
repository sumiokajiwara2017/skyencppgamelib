#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �R���W�����Ǘ�
/// �R���W�����Ǘ��𕡐������Ƃŏ����𕪎U�������ق���������������Ȃ��ł��B
/// ���Ӂj�`�`�a�a�Ƃn�a�a�𗼕��g�p���Ĉ�̃A�v���P�[�V�������쐬����̂͊�{�m�f�ł��B
/// �`�`�a�a�Ƃn�a�a�ŏՓ˔��肪���肵�Ď��Ȃ�����ł��i���r���[�Ȃ��͎̂������Ă���B�m���ɔ�����j
/// </summary>
class CollisionManager : base::SkyObject
{

public:

	/// <summary>
	/// �R���W�����f�[�^�̓o�^�˗�
	/// ���ۂ̓o�^��Update���Ă΂ꂽ�ۂɍs����B�R���W�����ʒm�̃R�[���o�b�N���ɃR���W�����f�[�^��
	/// �o�^�܂��͗������s����ƃ��X�g����Ɏx�Ⴊ�ł�\��������̂ŁA�Ԃ������Ĉ�Ăɓo�^�A�폜����������
	/// </summary>
	void ResisterDataRequest( const CollisionDataPtr &data );

	/// <summary>
	/// �R���W�����f�[�^�̓o�^����
	/// ���ۂ̓o�^��Update���Ă΂ꂽ�ۂɍs����B�R���W�����ʒm�̃R�[���o�b�N���ɃR���W�����f�[�^��
	/// �o�^�܂��͗������s����ƃ��X�g����Ɏx�Ⴊ�ł�\��������̂ŁA�Ԃ������Ĉ�Ăɓo�^�A�폜����������
	/// </summary>
	void UnResisterDataRequest( const CollisionDataPtr &data );

	/// <summary>
	/// ����i�o�^���ꂢ�Ă�f�[�^���m�ŏՓ˔�������{�j
	/// </summary>
	void Detection( s32 kindId = COLLISION_KIND_ID_ALL , u32 tagBit = 0 );

	/// <summary>
	/// ����i�o�^���ꂢ�Ă�f�[�^���m�ŏՓ˔�������{�j
	/// �q�b�g�����f�[�^�Ƀ^�O�𗧂Ă邱�Ƃ��ł���
	/// </summary>
	skyBool Detection( const CollisionDataPtr &data , s32 kindId = COLLISION_KIND_ID_ALL , u32 tagBit = 0 , CollisionDataPtrList *hitList = skyNull );

	/// <summary>
	/// �^�O�N���A
	/// </summary>
	void ClearTagBit();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �w��̎�ʂ̃f�[�^�����X�g�ŕԂ�
	/// </summary>
	const CollisionDataPtrList &GetKindDataList( s32 kindId , CollisionDataPtrList &dest );

private:

	/// <summary>
	/// ���N�G�X�g���ߎ��
	/// </summary>
	enum eRequestKind
	{
		eRequestKind_Resister = 0 ,
		eRequestKind_UnResister ,
	};

	/// <summary>
	/// ���N�G�X�g���ߏ��
	/// </summary>
	struct stRequestData
	{
		eRequestKind     m_eRequestKind;
		CollisionDataPtr m_spCollisionData;
	};
	typedef CList< u32 , stRequestData >	CRequestDataList;

	/// <summary>
	/// �v�����X�g
	/// </summary>
	CRequestDataList m_RequestList;

	/// <summary>
	/// �R���W�����f�[�^���X�g
	/// </summary>
	CollisionDataPtrList m_CollisionDataList;

	/// <summary>
	/// �e��W�I���g���̏Փ˔���
	/// </summary>
	skyBool Collision_point2( const CBasicVector2 *pPoint2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_point3( const CBasicVector3 *pPoint3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_point4( const CBasicVector4 *pPoint4 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_line2( const CBasicLine2 *pLine2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_line3( const CBasicLine3 *pLine3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ray2( const CBasicRay2 *pRay2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ray3( const CBasicRay3 *pRay3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_circle( const CBasicCircle *pCircle , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_sphere( const CBasicSphere *pSphere , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ellipse( const CBasicEllipse *pEllipse , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_plane( const CBasicPlane *pPlane , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_triangle2( const CBasicTriangle2 *pTriangle2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_triangle3( const CBasicTriangle3 *pTriangle3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_capsule2( const CBasicCapsule2 *pCapsule2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_capsule3( const CBasicCapsule3 *pCapsule3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_rect( const CBasicRect *pRect , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_aabb2( const CAABB2 *pAABB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_aabb3( const CAABB3 *pAABB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_obb2( const COBB2 *pOBB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_obb3( const COBB3 *pOBB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
};

} } }
