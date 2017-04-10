#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// ���Ǐ����Q
/// ����Ŋw�ԃQ�[��AI�v���O���~���O���Q�l�ɍ쐬
/// </summary>
namespace CAISteeringBehaviorsUtil
{
	/// <summary>
	///�y�T���s���z
	/// </summary>
	skyBool Seek_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance = FLT_MAX );

	/// <summary>
	///�y�����s���z
	/// </summary>
	skyBool Flee_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance = FLT_MAX );

	/// <summary>
	///�y�����s���z
	/// dec�̒l���傫���قǊɂ₩�ɖڕW�ɓ��B����
	/// </summary>
	void Arrive_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , s32 dec = 8 );

	/// <summary>
	///�y�ǐՍs���z
	/// �ǐՑΏۂ̈ʒu�A���x�A�����A�����𗘗p���ĒǐՂ���
	/// </summary>
	void Pursuit_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj );
	void Pursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj );

	/// <summary>
	///�y�����s���z
	/// </summary>
	void Evade_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj );
	void Evade_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj );

	/// <summary>
	///�y�p�j�s���z
	/// </summary>
	void Wander_Move( const fps::CFpsController &fps , CAIMoveObject &obj , const math::CMatrix4x3 &objWorldMatrix , math::eAxis axis = ( math::eAxis )0 );

	/// <summary>
    ///�y��Q����s���z
	/// </summary>
	void ObstacleAvoidance_Move( const fps::CFpsController &fps , CAIMoveObject &obj , math::CollisionManager &collisionManager , const math::COBB3 &objObb3 );

	/// <summary>
	///�y�ǉ���s���z
	/// </summary>
	void WallAvoidance_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

	/// <summary>
	///�y����s���z
	/// </summary>
	void Interpose_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

	/// <summary>
	///�y�B�g�s���z
	/// </summary>
	void Hide_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CollisionDataPtrList &obstacleList , CAIMoveObject &obj );

	/// <summary>
	///�y�o�H�Ǐ]�s���z
	/// </summary>
	void FollowPath_Move( const fps::CFpsController &fps , CAIMoveObject &obj );

	/// <summary>
	///�y�I�t�Z�b�g�ǐՍs���z
	/// </summary>
	void OffsetPursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

//----�֗��֐�

	/// <summary>
	/// �I�u�W�F�N�g�̖ڕW�����ւ̉�]���Ԃ��v�Z����
	/// </summary>
	dectype TurnAroundTime( const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype coefficient = 0.8f );
}

} } }