#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// �p�X�I���^�C�v
/// </summary>
enum eAIPathEndType
{
	eAIPathEndType_End = 0    , //�I��
	eAIPathEndType_Repetition , //�ŏ��̍��W����J�n
	eAIPathEndType_Back       , //�Ō�̍��W����J�n
};

/// <summary>
/// �`�h���H�郋�[�g�p�X
/// </summary>
class CAIMovePath : base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CAIMovePath();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAIMovePath();

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	CAIMovePath( const CAIMovePath& src );

	/// <summary>
	/// ������Z�q
	/// </summary>
	CAIMovePath& operator =( const CAIMovePath& src );

	/// <summary>
	/// ���[�O���b�h�O���t�ƍŒZ�p�X�C���f�b�N�X���X�g���烋�[�g�p�X�𐶐�����
	/// </summary>
	void AddGraph( const math::CEuclidGraph &graph , math::GraphIndexList &list );

	/// <summary>
	/// �p�X��ǉ�
	/// </summary>
	void AddPath( const math::CBasicVector3 &pos );

	/// <summary>
	/// �J�����g�̖ڕW�n�_���擾
	/// </summary>
	math::CBasicVector3 GetCurrentPoint();

	/// <summary>
	/// �J�����g�̖ڕW�n�_�����̖ڕW�n�_�ɐݒ�
	/// </summary>
	void SetNextPoint();

	/// <summary>
	/// �Ō�̖ڕW�n�_�̏ꍇ��skyTrue��Ԃ�
	/// </summary>
	skyBool IsFinalPoint();

	/// <summary>
	/// �I��������ݒ�^�擾
	/// </summary>
	void           SetPathEndType( eAIPathEndType ePathEnd );
	eAIPathEndType GetPathEndType() const;

	/// <summary>
	/// �`��̂��߂̃��f����Ԃ�
	/// </summary>
	graphic::CModelPtr CreateModel( const graphic::CColor &color );

private:

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum eState
	{
		eState_Play = 0 ,
		eState_Play_Reverse ,
	} m_eState;

	/// <summary>
	/// ���݂̃J�����g���W�ԍ�
	/// </summary>
	u32 m_CurrentPointNo;

	/// <summary>
	/// ���݂̃J�����g���W�ԍ�
	/// </summary>
	math::CBasicVector3Vector m_PathVector;

	/// <summary>
	/// �p�X�I��������
	/// </summary>
	eAIPathEndType m_ePathEnd;
};

} } }
