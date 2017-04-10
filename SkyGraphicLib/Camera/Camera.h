#pragma once

namespace sky { namespace lib { namespace graphic {

class Camera;
typedef SmartPointer< Camera >	CameraPtr;					//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �J�����f�[�^�\����
/// </summary>
struct CameraData
{
	/// <summary>
	/// �J�����̍��W
	/// </summary>
	math::CBasicVector3      m_Eye;

	/// <summary>
	/// �J�����̒����_
	/// </summary>
	math::CBasicVector3      m_Tag;

	/// <summary>
	/// �����x�N�g��
	/// </summary>
	math::CBasicVector3      m_ViewVec;

	/// <summary>
	/// �������`�x�N�g��
	/// </summary>
	math::CBasicVector3      m_UpVec;

	/// <summary>
	/// �E������`�x�N�g��
	/// </summary>
	math::CBasicVector3      m_RightVec;

	/// <summary>
	/// ���̐ς̋ߕ���
	/// </summary>
    dectype                  m_Near;

	/// <summary>
	/// ������
	/// </summary>
    dectype                  m_Far;

	/// <summary>
	/// ����p
	/// </summary>
	dectype                  m_Fov;

	/// <summary>
	/// ���ˉe�g��k���l
	/// </summary>
	dectype                  m_Zoom;

	/// <summary>
	/// �A�X�y�N�g��
	/// </summary>
	dectype                  m_Aspect;

	/// <summary>
	/// �J�����Ɣ�ʑ̂̋���
	/// </summary>
	dectype                  m_Distance;

	/// <summary>
	/// ��]�p�x�i�J�����̎�ʂɂ���ĈӖ����ς���Ă���j
	/// </summary>
	math::CBasicAngleVector3 m_Angle;

	/// <summary>
	/// �X���p�x�i���ˉe���[�h�ł͈Ӗ��͖����j
	/// </summary>
	math::CBasicAngleVector3 m_InclinationAngle;

	/// <summary>
	/// �J�����̕`���`
	/// </summary>
	math::CBasicSize         m_ViewSize;
};

/// <summary>
/// �J�����N���X
/// </summary>
class Camera : public CSceneGraphNode //�ŏI�I�ɂ͂�����
{
    SkyTypeDefRTTI;
	friend class CameraDebugDispNode;
	friend class CameraManager;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera(){};

	/// <summary>
	/// �J�����f�[�^�̎擾
	/// </summary>
	inline CameraData &GetData(){ return m_Data; }

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// �V�[���O���t�m�[�h�Ƃ��Ă̓���㏈��
	/// </summary>
	void vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// �J�����Ǘ��̍X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// �������̐ݒ�
	/// </summary>
	void SetInitInfo( const math::CBasicVector3 &eye , 
		              const math::CBasicVector3 &tag , 
					  const math::CBasicVector3 &upVec , 
					  const math::CBasicVector3 &rightVec , 
					  dectype fov , 
					  dectype nearValue , 
					  dectype farValue , 
					  dectype width , 
					  dectype height , 
					  const math::CBasicAngleVector3 &angle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 ) , 
					  const math::CBasicAngleVector3 &inclinationangle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 ) , 
					  dectype distance = 10.0f , 
					  dectype zoom = 1.0f );

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// ��ʃT�C�Y�̐ݒ�
	/// </summary>
	void SetDisplaySize( dectype width , dectype height );

	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	void SetDistance( dectype distance );

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	void SetAngle( const math::CBasicAngleVector3 &angle );
	const math::CBasicAngleVector3 &GetAngle();

	/// <summary>
	/// �J�����̈ʒu�̐ݒ�^�擾
	/// </summary>
	void SetEye  ( const math::CBasicVector3 &eye );

	/// <summary>
	/// �Ώۈʒu�̐ݒ�^�擾
	/// </summary>
	void SetTag( const math::CBasicVector3 &tag );

	/// <summary>
	/// �r���[���̍X�V�i�擾��GetViewMatrix()�Łj
	/// </summary>
	void UpdateViewInfo( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &upVec );

	/// <summary>
	/// �������ˏ��̍X�V�i�擾��GetProjectionMatrix()�Łj
	/// </summary>
	void UpdateProjectionInfo( dectype fov , dectype width , dectype height , dectype nearValue , dectype farValue );
	void UpdateProjectionInfo_Fov( dectype fov );
	void UpdateProjectionInfo_Size( dectype width , dectype height );
	void UpdateProjectionInfo_NearFar( dectype nearValue , dectype farValue );

	/// <summary>
	/// ���ˉe���̍X�V�i�擾��GetOrthoMatrix()�Łj
	/// </summary>
	void UpdateOrthoInfo( dectype width , dectype height , dectype nearValue , dectype farValue , dectype zoom = 1.0f );
	void UpdateOrthoInfo_Size( dectype width , dectype height );
	void UpdateOrthoInfo_NearFar( dectype nearValue , dectype farValue );
	void UpdateOrthoInfo_Zoom( dectype zoom );

	/// <summary>
	/// ���[���h�}�g���b�N�X�̎擾
	/// </summary>
	const math::CMatrix4x3 &GetWorldMatrix();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	const math::CMatrix4x3 &GetViewMatrix();

	/// <summary>
	/// �v���W�F�N�V�����s��̎擾
	/// </summary>
	const math::CMatrix4x3 &GetProjectionMatrix();

	/// <summary>
	/// �r���[�v���W�F�N�V�����s��̎擾
	/// </summary>
	const math::CMatrix4x3 &GetViewProjection();

	/// <summary>
	/// �J�����ɐ��ʂ������r���{�[�h��]�}�g���b�N�X���v�Z
	/// </summary>
	const math::CRotationMatrix &ComputeBillboardRotationMatrix();

	/// <summary>
	/// ���ˉe�s��n�m�^�n�e�e
	/// </summary>
	void SetOrtho( skyBool isOrthoOn );

	/// <summary>
	/// ���ˉe�s�񔻒�
	/// </summary>
	skyBool IsOrthoOn();

	/// <summary>
	/// ���ˉe�s��̎擾
	/// </summary>
	const math::CMatrix4x3 &GetOrthoMatrix();

	/// <summary>
	/// �J�����ϊ��}�g���N�X���쐬
	/// ���ˉe���[�h�Ȃ�GetOrthoMatrix()�̒l��Ԃ�
	/// ����ȊO�Ȃ�GetViewMatrix()�̒l��Ԃ�
	/// </summary>
	const math::CMatrix4x3 &GetCameraMatrix();

	/// <summary>
	/// �J�����̌X���p�x��ݒ肷��
	/// </summary>
	void SetInitInclinationAngle( const math::CBasicAngleVector3 &angle );

	/// <summary>
	/// ���㌴�_�t���O�i���ˉe�̂ݗL���B���ꂪ�n�m�̂Ƃ��͊e�X�v���C�g�����㌴�_���W�ƂȂ�悤�Ɉʒu��ϊ����s���j
	/// </summary>
	void SetLeftTopCenter( skyBool isLeftTopCenterOn );

	/// <summary>
	/// ���㌴�_�t���O�`�F�b�N
	/// </summary>
	skyBool IsLeftTopCenter();

	/// <summary>
	/// �f�t�H���g���͂n�m�^�n�e�e
	/// </summary>
	void SetDefaultInput( skyBool isOn = skyTrue );

	/// <summary>
	/// �f�t�H���g���͂n�m�^�n�e�e
	/// </summary>
	math::CBasicVector3 ComputeDistancePos( dectype distance );

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CameraPtr Create( eCameraKind ekind = eCameraKind_Satellite , const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CameraPtr spResult = CameraPtr( NEW__( Camera , Camera( ekind ) ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

protected:

	/// <summary>
	/// �J�����f�[�^
	/// </summary>
	 CameraData              m_Data;

	/// <summary>
	/// �J���������f�[�^
	/// </summary>
	 CameraData              m_InitData;

	/// <summary>
	/// ���ˉe�t���O
	/// </summary>
	skyBool                  m_IsOrthoOn;

	/// <summary>
	/// �W�����͎�t�t���O
	/// </summary>
	skyBool                  m_IsDefaultInputOn;

	/// <summary>
	/// �}�E�X���{�^�������t���O
	/// </summary>
	skyBool                  m_IsMouseLBtnClick;

	/// <summary>
	/// �}�E�X�E�{�^�������t���O
	/// </summary>
	skyBool                  m_IsMouseRBtnClick;

	/// <summary>
	/// �}�E�X�{�^���������W
	/// </summary>
	math::CBasicVector2      m_MouseClickPos;

	/// <summary>
	/// �J�����g�t���O�n�m
	/// </summary>
	skyBool                  m_IsCurrentOn;

	/// <summary>
	/// �J�������
	/// </summary>
	eCameraKind              m_eKind;

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	math::CMatrix4x3         m_WorldMatrix;

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	math::CMatrix4x3         m_ViewMatrix;

	/// <summary>
	/// �v���W�F�N�V�����s��̎擾
	/// </summary>
	math::CMatrix4x3         m_ProjectionMatrix;

	/// <summary>
	/// �r���[�v���W�F�N�V�����s��̎擾
	/// </summary>
	math::CMatrix4x3         m_ViewProjectionMatrix;

	/// <summary>
	/// ���ˉe�s��̎擾
	/// </summary>
	math::CMatrix4x3         m_OrthoMatrix;

	/// <summary>
	/// �r���{�[�h�s��
	/// </summary>
	math::CRotationMatrix    m_BillBoardRotationMatrix;

	/// <summary>
	/// �����N���X
	/// </summary>
	CFrustum                 m_Frustum;

	/// <summary>
	/// ���㌴�_�t���O
	/// </summary>
	skyBool                  m_IsLeftTopCenterFlg;

	/// <summary>
	/// �}�E�X�C�x���g�f���Q�[�g�i�}�E�X�C�x���g�n���h���ɓo�^����j
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// �L�[�{�[�h�C�x���g�f���Q�[�g�i�L�[�{�[�h�C�x���g�n���h���ɓo�^����j
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBoradEventDelegate;
	void KeyBoradEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera( eCameraKind ekind );

	/// <summary>
	/// ���݂̃J�����ɂ���
	/// </summary>
	void SetCurrentOn( skyBool isCurrentOn );

//---Serialize
			
	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// ����������
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );
};

} } }

#define CameraCreate_( ekind )            sky::lib::graphic::Camera::Create( ekind )
#define CameraCreateName_( ekind , name ) sky::lib::graphic::Camera::Create( ekind , name )
