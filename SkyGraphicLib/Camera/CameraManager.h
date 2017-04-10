#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �J�����X�}�[�g�|�C���^�^
/// </summary>
class CameraManager;
typedef SmartPointer< CameraManager > CameraManagerPtr;	//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �J�����Ǘ��N���X
/// �C���X�^���X�̓r���[�|�[�g�̐���������̂ŁA�V���O���g���ł͂Ȃ��B
/// </summary>
class CameraManager : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CameraManager(){};

	/// <summary>
	/// �J������o�^
	/// </summary>
	void ResisterCamera( const CameraPtr &spCamera );

	/// <summary>
	/// �J������o�^����
	/// </summary>
	void UnResisterCamera( const CameraPtr &spCamera );

	/// <summary>
	/// �J������؂�ւ�
	/// </summary>
	void NextCameraRequest( updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );

	/// <summary>
	/// �J������؂�ւ�������i��Ԓ��j
	/// </summary>
	skyBool IsInterpolate();

	/// <summary>
	/// ���݂̃J�����̐ݒ�
	/// </summary>
	void SetCurrentCameraRequest( const CameraPtr &spCamera , updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );
	void SetCurrentCameraRequest( const hash::CHashStringPtr &name , updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );

	/// <summary>
	/// ���݂̃J�����̎擾
	/// </summary>
	CameraPtr GetCurrentCamera();

	/// <summary>
	/// �J�����Ǘ��̍X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �L�[�{�[�h�C�x���g�f���Q�[�g�i�L�[�{�[�h�C�x���g�n���h���ɓo�^����j
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBordEventDelegate;
	void KeyBordEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// �f�t�H���g�̓��͑���n�m�^�n�e�e
	/// </summary>
	void SetDefaultInputOn( skyBool isDefaultInputOn ){ m_IsDefaultInputOn = isDefaultInputOn; }

//---Serialize

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CameraManagerPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CameraManagerPtr spResult = CameraManagerPtr( NEW__( CameraManager , CameraManager() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

private:

	/// <summary>
	/// ��ԗv���f�[�^
	/// </summary>
	struct CInterpRequestData
	{
		/// <summary>
		/// ��Ԓ��ړ���J�������_
		/// </summary>
		math::CBasicVector3 m_FromEye;
		math::CBasicVector3 m_ToEye;

		/// <summary>
		/// ��Ԓ��ړ���J���������_
		/// </summary>
		math::CBasicVector3 m_FromTag;
		math::CBasicVector3 m_ToTag;

		/// <summary>
		/// ��Ԏ���
		/// </summary>
		updateTime m_Time;

		/// <summary>
		/// ��Ԍo�ߎ���
		/// </summary>
		updateTime m_ElapsedTime;

		/// <summary>
		/// ��ԃ^�C�v
		/// </summary>
		eInterpolateType m_eInterpType;
	};
	CFifo< CInterpRequestData > m_InterpQueue;

	/// <summary>
	/// �f�t�H���g����ON
	/// </summary>
	skyBool	m_IsDefaultInputOn;

//-----------State Function-----------------��

	/// <summary>
	/// �X�e�[�g���
	/// </summary>
	enum eState
	{
		eState_Start = 0 ,
		eState_Wait ,
		eState_Interpolate ,
		eState_End ,
	};

	/// <summary>
	/// �X�e�[�g�}�V���@�\��t��
	/// </summary>
	state::CFunctionStateManager< CameraManager , eState > m_FunctionState;

//-----StateFunction-----��

	skyBool State_Start( const fps::CFpsController &fps );
	skyBool State_Wait( const fps::CFpsController &fps );
	skyBool State_Interpolate( const fps::CFpsController &fps );
	skyBool State_End( const fps::CFpsController &fps );

//-----StateFunction-----��

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CameraManager();
};

} } }

#define CameraManagerCreate_()               sky::lib::graphic::CameraManager::Create()
#define CameraManagerCreateName_( name )     sky::lib::graphic::CameraManager::Create( name )
