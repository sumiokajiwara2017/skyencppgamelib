#pragma once

namespace sky { namespace lib { namespace framework {

static const dectype AI_MOVE_OBJECT_DEFAULT_MASS = 0.1f;
static const dectype AI_MOVE_OBJECT_DEFAULT_MAXSPEED = 8.0f;
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_POS( 0 , 0 , 0 );
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_DIR( 0 , 0 , 1 );
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_VEL( 0 , 0 , 0 );
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_RADIUS = 3.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_DISTANCE = 1.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_JITTER = 5.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WAYPOINT_SEEK_DIST = 0.5 * 0.5f; //����
	
/// <summary>
/// �`�h����N���X
/// �����I�u�W�F�N�g�Ɣ��Ɏ��Ă��邪�A�ړ��ɂ����鏈�����Ⴄ���ߕ����č쐬���Ă���B
/// </summary>
class CAIMoveObject : base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CAIMoveObject(){}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAIMoveObject() : 
		m_Mass( AI_MOVE_OBJECT_DEFAULT_MASS ) , 
		m_MaxSpeed( AI_MOVE_OBJECT_DEFAULT_MAXSPEED ) ,
		m_Position( AI_MOVE_OBJECT_DEFAULT_POS ) ,
		m_Direction( AI_MOVE_OBJECT_DEFAULT_DIR ) ,
		m_Velocity( AI_MOVE_OBJECT_DEFAULT_VEL ) ,
		m_WanderTarget( AI_MOVE_OBJECT_DEFAULT_POS ) ,
		m_WanderRadius( AI_MOVE_OBJECT_DEFAULT_WANDER_RADIUS ) ,
		m_WanderDistance( AI_MOVE_OBJECT_DEFAULT_WANDER_DISTANCE ) ,
		m_WanderJitter( AI_MOVE_OBJECT_DEFAULT_WANDER_JITTER ) ,
	    m_WaypointSeekDistSq( AI_MOVE_OBJECT_DEFAULT_WAYPOINT_SEEK_DIST )
	{}

	/// <summary>
	/// ���݂̑��x�i�ړ������{�ړ������j���������o���A���̕����ɉ�]���邽�߂̉�]�}�g���b�N�X���擾����
	/// </summary>
	math::CQuaternion GetDirectionQuaternion();

	/// <summary>
	/// �͂Ǝ��Ԃ������đ��x�ƈʒu���X�V����
	/// </summary>
	void Update( const fps::CFpsController &fps , const math::CBasicVector3 &force );

	/// <summary>
	/// ���ʁi���̒l���傫���ƑS�Ă̂`�h����œ������ɖ��ɂȂ�܂��B���܂�ɓ������o�^�o�^���������ꍇ�͂�����グ��Ƃ悢�j
	/// </summary>
	dectype m_Mass;

	/// <summary>
	/// �ő呬�x�i���x��������𔲂����X�J���l�ŕ\���j
	/// </summary>
	dectype m_MaxSpeed;

	/// <summary>
	/// �ʒu�i���݂̈ʒu�j
	/// </summary>
	math::CBasicVector3 m_Position;

	/// <summary>
	/// �����i���݂̌����j
	/// </summary>
	math::CBasicVector3 m_Direction;

	/// <summary>
	/// ���x�i�i�ދ����ƕ������x�N�g���ŕ\���j
	/// </summary>
	math::CBasicVector3 m_Velocity;

//----�p�j�s���Ɏg�p

	/// <summary>
	/// �p�j�~�i���̒l���傫���Ɠ��������G�ɂȂ�܂��j
	/// </summary>
	dectype m_WanderRadius;

	/// <summary>
	/// �p�j�~�̒��S�܂ł̋����i���̒l���傫���Ɠ����������Ȃ�܂��B�������ő�X�s�[�h���z���邱�Ƃ͂Ȃ��ł��j
	/// </summary>
	dectype m_WanderDistance;

	/// <summary>
	/// �����_���ψʂ̍ő�ʂŁA�ڕW�ɖ��b�ŉ��Z�����i���̒l���傫���Ɠ��������G�ɂȂ�܂��j
	/// </summary>
	dectype m_WanderJitter;

	/// <summary>
	/// �p�j�~�̒��S�_�i�����l��m_Position�Ɠ����j
	/// </summary>
	math::CBasicVector3 m_WanderTarget;

//----�o�H�Ǐ]�s���Ɏg�p

	/// <summary>
	/// �ڕW�n�_�ɂǂꂾ���߂Â����玟�̖ڕW�n�_�ɍs�����̋����i�����j�B���̒l����������Ώ������قǃp�X�ɐ��m�ɐi��
	/// </summary>
	dectype m_WaypointSeekDistSq;

	/// <summary>
	/// �o�H���
	/// </summary>
	CAIMovePath m_Path; //�J�����g�̃C���f�b�N�X���ێ����Ă��邽�ߊe�C���X�^���X���ɂ����Ă����ق�������
};

} } }