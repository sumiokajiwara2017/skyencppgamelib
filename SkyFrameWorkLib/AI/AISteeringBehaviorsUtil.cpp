#include "StdAfx.h"
#include "SkyFrameWorkLib/AI.h"

namespace sky { namespace lib { namespace framework {

/// <summary>
/// ���Ǐ����Q
/// </summary>
namespace CAISteeringBehaviorsUtil
{

skyBool Seek_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance )
{
	skyBool result = skyFalse;

	math::CBasicVector3 force = math::CBasicVector3_ZERO;
	if ( math::DistanceSquared( targetPos , obj.m_Position ) <= enableDistance * enableDistance ) //�������̌v�Z���Ȃ����ߕ�����ԂŔ�r
	{
		math::CBasicVector3 desiredVelocity = ( targetPos - obj.m_Position ).Normalize() * obj.m_MaxSpeed;
		force = ( desiredVelocity - obj.m_Velocity );
		result = skyTrue;
	}

	//�X�V
	obj.Update( fps , force );

	return result;
}

skyBool Flee_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance )
{
	skyBool result = skyFalse;

	math::CBasicVector3 force = math::CBasicVector3_ZERO;
	if ( math::DistanceSquared( targetPos , obj.m_Position ) <= enableDistance * enableDistance ) //�������̌v�Z���Ȃ����ߕ�����ԂŔ�r
	{
		math::CBasicVector3 desiredVelocity = ( obj.m_Position - targetPos ).Normalize() * obj.m_MaxSpeed;
		force = ( desiredVelocity - obj.m_Velocity );
		result = skyTrue;
	}

	//�X�V
	obj.Update( fps , force );

	return result;
}

void Arrive_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , s32 dec )
{
	math::CBasicVector3 force = math::CBasicVector3_ZERO;

	//�ڕW�̕����Ɍ������߂̃x�N�g�����v�Z
	math::CBasicVector3 toTargetVec = targetPos - obj.m_Position;

	//�ڕW�ʒu�܂ł̋������v�Z����
	dectype dist = toTargetVec.Length();

	if ( dist > 0 )
	{
		//Deceleration��int�^�ŗ񋓒萔������Ă���̂ŁA
		//���̒l�͌����Ɋւ��闱�x�ׂ̍���������񋟂��邽�߂ɕK�v�Ƃ����
		const dectype decelerationTweaker = 0.3;

		//�w�肳�ꂽ�]�܂��������ŖڕW�ɓ��B����̂ɕK�v�ȑ��x���v�Z����
		dectype speed = dist / ( ( dectype )dec * decelerationTweaker );

		//���x���ő呬�x�𒴂��Ȃ��悤�ɂ���
		speed = TemplateUtil::Min< dectype >( speed , obj.m_MaxSpeed );

		//��������͒T���s���̂悤�ɏ����������
		//�������A�����idist)���킴�킴�v�Z���Ă���ToTarget�x�N�g���𐳋K������K�v�͂Ȃ�
		math::CBasicVector3 desiredVelocity = toTargetVec * speed / dist;

		force = desiredVelocity - obj.m_Velocity;
	}

	//�X�V
	obj.Update( fps , force );
}

void Pursuit_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj )
{
	Pursuit_Move( fps , target.m_Position , target.m_Velocity , target.m_MaxSpeed , target.m_Direction , obj );
}

void Pursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj )
{
	//�����鑤���O�ɍ~��A���̃G�[�W�F���g�������Ă���ꍇ�A�P�ɂ���Ɍ������ĒT���s������
	math::CBasicVector3 toEvader = targetPos - obj.m_Position;

	dectype relativeHeading = obj.m_Direction.Dot( direction );

	if ( ( toEvader.Dot( obj.m_Direction ) > 0 ) &&
		( relativeHeading < -0.95 ) ) //acos( 0.95 )= 18degs
	{
		Seek_Move( fps , targetPos , obj );
	}
	else
	{
		//���ʂɋ��Ȃ��̂œ����鑤���ǂ��ɂ��邩��\������
		//��ǂݎ��Ԃ͓����邪��ƒǂ�������K��̊Ԃ̋����ɔ�Ⴕ�A�G�[�W�F���g�̑��x�̍��v�ɔ���Ⴗ��
		dectype lookAheadTime = toEvader.Length() / ( obj.m_MaxSpeed + targetSpeed );

		//��]���Ԃ��l���ɓ����
		lookAheadTime += TurnAroundTime( targetPos , obj );

		//�����ŗ\�����ꂽ������K��̈ʒu�ɒT���s��������
		Seek_Move( fps , targetPos + targetVelocity * lookAheadTime , obj );
	}
}

void Evade_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj )
{
	Evade_Move( fps , target.m_Position , target.m_Velocity , target.m_MaxSpeed , target.m_Direction , obj );
}

void Evade_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj )
{
	math::CBasicVector3 toPursuer = targetPos - obj.m_Position;

	//��ǂݎ��Ԃ͒ǂ����Ɠ����邪��̊Ԃ̋����ɔ�Ⴕ�A�G�[�W�F���g�̑��x�̍��v�ɔ���Ⴗ��
	dectype lookAheadTime = toPursuer.Length() / ( obj.m_MaxSpeed + targetSpeed );

	//��]���Ԃ��l���ɓ����
//	lookAheadTime += TurnAroundTime( targetPos , obj ); //������������Ɠ����鐸�x������

	//�����ŉ����Ɋւ��ė\�����������̈ʒu���瓦����
	Flee_Move( fps , targetPos + targetVelocity * lookAheadTime , obj );
}

void Wander_Move( const fps::CFpsController &fps , CAIMoveObject &obj , const math::CMatrix4x3 &objWorldMatrix , math::eAxis lockAxis )
{
	//�����ȃ��C���_���x�N�g����ڕW�̈ʒu�ɉ�����
	obj.m_WanderTarget += math::CBasicVector3
		( 
		( lockAxis & math::eAxis_x ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter , 
		( lockAxis & math::eAxis_y ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter , 
		( lockAxis & math::eAxis_z ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter );

	//�V�����x�N�g����P�ʉ~�̌��ɍē��e����
	obj.m_WanderTarget.Normalize();
	
	//�x�N�g���̒�����p�j�~�̔��a�Ɠ����ɂȂ�܂ő��₷
	obj.m_WanderTarget *= obj.m_WanderRadius;

	//�ڕW���G�[�W�F���g�̑O�̈ʒu�Ɉړ�����
	math::CBasicVector3 targetLocal = obj.m_WanderTarget + math::CBasicVector3( 0 , 0 , obj.m_WanderDistance );

	//�ڕW�����[���h��Ԃ֓��e����
	math::CBasicVector3 targetWorld = targetLocal * objWorldMatrix;

	math::CBasicVector3 force = targetWorld - obj.m_Position;

	//�X�V
	obj.Update( fps , force );
}

void ObstacleAvoidance_Move( const fps::CFpsController &fps , CAIMoveObject &obj , math::CollisionManager &collisionManager , const math::COBB3 &objObb3 )
{
	//���x�ɍ��킹�Ăn�a�a���쐬����

	//�n�a�a�ƏՓ˔�����s��

	//�q�b�g�����f�[�^��obj�̃��[�J����ԂɈړ�����

	//���[�J����ԂɈړ������I�u�W�F�N�g�Ƃy�����C�̏Փ˔��������ԋ߂��I�u�W�F�N�g����肷��B����Ɍ����_���擾���Ă���

	//��ԋ߂��I�u�W�F�N�g�̌����_�ɉ����đ��Ǘ͂��v�Z����i�w���W���v���X�Ȃ玩���͍��ɁA�}�C�i�X�Ȃ玩���͉E�ɁA�������߂���Η͂����߂Ɍv�Z����j
}

void WallAvoidance_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

void Interpose_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

static math::CBasicVector3 GetHidingPosition_Sphere( const math::CBasicSphere &obstacle , const math::CBasicVector3 &targetPos )
{
	// �G�[�W�F���g���I��������Q���̃o�E���f�B���O���a����ǂꂭ�炢�����ɂ��邩���v�Z����
	const dectype distanceFromBoundary = 8.0f;

	dectype distAway = obstacle.GetRadius() + distanceFromBoundary;

	//�ڕW����I�u�W�F�N�g�ւ̌������v�Z����
	math::CBasicVector3 toOb = ( obstacle.GetPos() - targetPos ).Normalize();

	//�����K�؂ȑ傫���ɃX�P�[�����O���A��Q���̌��݂̈ʒu�ɉ����ĉB���ꏊ�𓾂�
	return ( toOb * distAway ) + obstacle.GetPos();
}

void Hide_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CollisionDataPtrList &obstacleList , CAIMoveObject &obj )
{
	dectype distToClosest = FLT_MAX;
	math::CBasicVector3 bestHidingSpot;

	for ( u32 i = 0 ; i < obstacleList.GetNum() ; i++ )
	{
		//�􉽌`����擾����
		const math::CGeometryBase *pGeometory = obstacleList[ i ]->GetGeometry();

		//�Փ˔��菈��
		math::CBasicVector3 hideSpot;
		switch( pGeometory->GetGeometoryType() )
		{
			//���̏ꍇ�̉B��鏈��
		case math::eGeometoryType_sphere:

				hideSpot = GetHidingPosition_Sphere( *( const math::CBasicSphere * )pGeometory , targetPos );
				break;
		default:
			continue;
		}

		//������Ԃō�Ƃ��A�G�[�W�F���g�ɍł��߂��B��ꏊ��������
		dectype dist = math::DistanceSquared( hideSpot , obj.m_Position );

		if ( dist < distToClosest )
		{
			distToClosest = dist;
			bestHidingSpot = hideSpot;
		}
	}

	//�K�؂ȏ�Q����������Ȃ��ꍇ�͖ڕW���瓦���s��������
	if ( distToClosest == FLT_MAX )
	{
		Flee_Move( fps , targetPos , obj );
	}
	else
	{
		Arrive_Move( fps , bestHidingSpot , obj );
	}
}

void FollowPath_Move( const fps::CFpsController &fps , CAIMoveObject &obj )
{
	//�\���Ɍ��݂̖ڕW�ɋ߂Â����玟�̖ڕW�ɐݒ肷��i�������׌y���̂��ߕ������m�̔�r�j
	if ( math::DistanceSquared( obj.m_Path.GetCurrentPoint() , obj.m_Position ) < obj.m_WaypointSeekDistSq )
	{
		obj.m_Path.SetNextPoint();
	}

	//�Ō�̃p�X�������ꍇ�͂����ɓ�������
	if ( obj.m_Path.IsFinalPoint() && obj.m_Path.GetPathEndType() == eAIPathEndType_End )
	{
		Arrive_Move( fps , obj.m_Path.GetCurrentPoint() , obj );
	}
	else
	{
		Seek_Move( fps , obj.m_Path.GetCurrentPoint() , obj );
	}
}

void OffsetPursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

dectype TurnAroundTime( const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype coefficient )
{
	//�ڕW�ɑ΂��Đ��K�������x�N�g�����v�Z����
	math::CBasicVector3 toTarget = ( targetPos - obj.m_Position ).Normalize();

	//�����̌����ƃx�N�g���̓��ς����
	dectype dot = obj.m_Direction.Dot( toTarget );

	return ( dot - 1.0f ) * -coefficient;
}

}

} } }