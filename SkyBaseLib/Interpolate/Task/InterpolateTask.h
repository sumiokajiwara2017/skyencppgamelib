#pragma once

namespace sky { namespace lib { namespace interp {

/// <summary>
/// �Đ��I���^�C�v
/// </summary>
enum ePlayEndType
{
	ePlayEndType_End = 0    , //�Đ��I��
	ePlayEndType_Repetition , //������Đ��J�n�i���������Ă������j
	ePlayEndType_Back       , //Start��End�����ւ��čĂэĐ��J�n�i���������Ă������j
};

class CInterpolateTaskPlayer;

//�e���v���[�gTypedef�S
class CInterpolateTaskBase;
typedef CInterpolateTaskBase*                   CInterpolateTaskBasePtr;		//�|�C���^��`
typedef CList< u32 , CInterpolateTaskBasePtr >	CInterpolateTaskBasePtrList;	//���X�g��`

/// <summary>
/// ��ԃ^�X�N��{�N���X
/// �t�@���N�V�����J�[�u������ĕ�Ԃ���������_��ŊȒP�ȕ�Ԏ��s��񋟂���B
/// ���̂΂��̏�̔��f�Ŏ��݂ɍs�����ς���I�u�W�F�N�g���A���܂����^�̖�����ԓ��������
/// �̂ɍœK�B
/// </summary>
class CInterpolateTaskBase : public base::SkyRefObject
{

public:

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void *Update( updateTime time ) = 0;

	/// <summary>
	/// �Đ��I������
	/// </summary>
	virtual skyBool IsEnd() = 0;
};

/// <summary>
/// ��ԃ^�X�N�N���X
/// </summary>
template < class T >
class CInterpolateTask : public CInterpolateTaskBase
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CInterpolateTask(){};

	/// <summary>
	/// �J�n�i�v���C���[�ɓo�^�����j
	/// </summary>
	inline void Start( T startValue , T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType = ePlayEndType_End , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f );
	inline void Start( T endValue , updateTime time , CInterpolateTaskPlayer *pPlayer , ePlayEndType endType = ePlayEndType_End , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f );//���݂̒l����X�^�[�g����

	/// <summary>
	/// �I��(�v���C���[����o�^���������j
	/// </summary>
	inline void End();

	/// <summary>
	/// �I���ʒu�̒ǉ��i�L���[�ɒ��߂��āA���݂̕�Ԃ��I�������l���玟�̒l�ւ̕�Ԃ��n�܂�j
	/// </summary>
	inline void RequestEndValue( T endValue , updateTime time , updateTime blendTime = 0.0f , eInterpolateType eInterpType = eInterpolateType_Linear , updateTime speed = 1.0f , ePlayEndType endType = ePlayEndType_End );
	inline void RequestEndValue( T endValue );
	inline void RequestEndValue( T endValue , updateTime time );

	/// <summary>
	/// ���N�G�X�g�̑S�P��
	/// </summary>
	inline void CancelRequest();

	/// <summary>
	/// ���ݍĐ����̏I���ʒu�̕ύX�i��ԓr���̏ꍇ�͌��݂̒l����ύX���ꂽ�l�֕�Ԃ��J�n�����j
	/// </summary>
	inline void ChangeEndValue( T endValue );

	/// <summary>
	/// ���ݍĐ����̕�ԕ��@�̕ύX
	/// </summary>
	inline void ChangeInterpolateType ( eInterpolateType eInterpType );

	/// <summary>
	/// ���ݍĐ����̃X�s�[�h�̕ύX
	/// </summary>
	inline void ChangeSpeed ( updateTime speed );

	/// <summary>
	/// ���ݍĐ����̏I���^�C�v��ύX
	/// </summary>
	inline void ChangeEndType( ePlayEndType endType );

	/// <summary>
	/// �X�V�i�������Ԃ�i�߂�j
	/// </summary>
	virtual void *Update( updateTime time );

	/// <summary>
	/// �w��̎��Ԃ̒l���擾
	/// </summary>
	inline T GetValue( updateTime time );

	/// <summary>
	/// �I������
	/// </summary>
	virtual skyBool IsEnd();

	/// <summary>
	/// �V�l�Z�o���C�x���g�n���h��
	/// </summary>
	delegate::CDelegateHandler  m_GetValueEventHndl;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CInterpolateTask();

private:

	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum eState_
	{
		eState_Wait_ = 0 ,
		eState_Play_ ,
	};

	/// <summary>
	/// ���s���
	/// </summary>
	struct CExecData_
	{
		/// <summary>
		/// �X�e�[�g
		/// </summary>
		 eState_          m_eState;

		/// <summary>
		/// �J�n�ʒu
		/// </summary>
		 T                m_StartValue;

		/// <summary>
		/// �I���ʒu
		/// </summary>
		 T                m_EndValue;

		/// <summary>
		/// ����
		/// </summary>
		 updateTime       m_Time;

		/// <summary>
		/// ��ԃ^�C�v
		/// </summary>
		 eInterpolateType m_eInterpType;

		/// <summary>
		/// �X�s�[�h
		/// </summary>
		 updateTime       m_Speed;

		/// <summary>
		/// �I���^�C�v
		/// </summary>
		 ePlayEndType     m_EndType;

		/// <summary>
		/// �u�����h�t���O
		/// </summary>
		 updateTime       m_BlendTime;

		/// <summary>
		/// ���݂̎��ԁiUpdate���Ă΂��x�ɓ����ŉ��Z����Ă����B���̖ړI�l��Pop���ꂽ�烊�Z�b�g�����j
		/// </summary>
		updateTime m_NowTime;

		/// <summary>
		/// ���݂̎��Ԃ̒l
		/// </summary>
		T m_NowValue;
	};
	typedef CFifo< CExecData_ > CEndRequestDataFifo_;	//���X�g��`
	CEndRequestDataFifo_  m_EndRequestQueue;

	/// <summary>
	/// ���ݍĐ����f�[�^
	/// </summary>
	CExecData_ m_CurrentData;

	/// <summary>
	/// �u�����h�f�[�^
	/// </summary>
	CExecData_ m_BlendData;

	/// <summary>
	/// ���ݍĐ����V�[�P���X�̃E�F�C�g�l
	/// </summary>
	f32 m_CurrentWeights;

	/// <summary>
	/// �Đ��@�̎Q��
	/// </summary>
	CInterpolateTaskPlayer *m_pPlayer;
};

} } }

#include "InterpolateTask.inl"