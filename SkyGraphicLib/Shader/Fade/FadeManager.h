#pragma once

namespace sky { namespace lib { namespace graphic {

static const f32  FADE_TIME = 100.0f;

class   CFadeManager;
typedef SmartPointer< CFadeManager > CFadeManagerPtr; //�X�}�[�g�|�C���^�^��`
typedef kfm::CFCurve< f32 >          CFadeFCurve;
typedef kfm::CFCurvePlayer< f32 >    CFadeFCurvePlayer;

/// <summary>
/// �t�F�[�h��Ԏ��
/// </summary>
enum eFadeStateKind
{
	eFadeStateKind_FadeIn = 0 ,
	eFadeStateKind_FadePlaying ,
	eFadeStateKind_FadeOut ,
};

/// <summary>
/// �t�F�[�h�Ǘ�
/// ��{�̔��t�F�[�h�A���t�F�[�h�A���̑��̐F�t�F�[�h�̂ݒ�
/// ���G�ȃC���[�W�t�F�[�h�̓X�v���C�g�ƃ}�e���A���A�j���[�V�����̑g�ݍ��킹��
/// �Ǝ��Ɏ������Ă��������B
/// </summary>
class CFadeManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFadeManager(){};

	/// <summary>
	/// �t�F�[�h�X�v���C�g���쐬
	/// </summary>
	void CreateFadeSprite( dectype w , dectype h );

	/// <summary>
	/// �t�F�[�h�X�s�[�h�̐ݒ�
	/// </summary>
	void SetFadeSpeed( f32 speed );

	/// <summary>
	/// �t�F�[�h�J���[�̐ݒ�
	/// </summary>
	void SetFadeColor( CColor color );

	/// <summary>
	/// �t�F�[�h�C������
	/// </summary>
	void ExecFadeIn();

	/// <summary>
	/// �t�F�[�h�A�E�g����
	/// </summary>
	void ExecFadeOut();

	/// <summary>
	/// �t�F�[�h�C���ςݏ�Ԃɂ���
	/// </summary>
	void SetFadeInEnd();

	/// <summary>
	/// �t�F�[�h�C���ςݏ�Ԃɂ���
	/// </summary>
	void SetFadeOutEnd();

	/// <summary>
	/// �t�F�[�h��Ԏ擾
	/// </summary>
	eFadeStateKind GetFadeState();

	/// <summary>
	/// �V�[���O���t�̃��[�g���擾
	/// �t�F�[�h�̓t�F�[�h��p�̃V�[���������������ā@�Q�c�쐬���ăJ������SetLeftTopCenter( skyFalse )�ɂ��āA�����Ńt�F�[�h��`�悷��B
	/// </summary>
	const CSceneGraphNodePtr &GetSceneGraphRoot();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update( const fps::CFpsController &fps );

//----Static

	/// <summary>
	/// �C���X�^���X����
	/// EventManager�̓V�X�e����1���݂��邪�A�C���X�^���X���쐬���ă��[�J���Ŏg�p���邱�Ƃ��ł���
	/// </summary>
    static CFadeManagerPtr Create();

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CFadeManager >;
	static SingletonPtr< CFadeManager > Singleton;

private:

	/// <summary>
	/// �T�C�Y�ύX���m�f���Q�[�g
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;
    void ReSize( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );

	/// <summary>
	/// �V�����l���v�Z
	/// </summary>
	void NewValueColFuncEvent ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args );

	/// <summary>
	/// �t�F�[�h�X�s�[�h
	/// </summary>
	f32 m_FadeSpeed;

	/// <summary>
	/// �t�F�[�h�C���t�@���N�V�����J�[�u
	/// </summary>
	CFadeFCurve  m_FadeInFCurve;

	/// <summary>
	/// �t�F�[�h�A�E�g�t�@���N�V�����J�[�u
	/// </summary>
	CFadeFCurve  m_FadeOutFCurve;

	/// <summary>
	/// �t�@���N�V�����J�[�u�Đ��@
	/// </summary>
	CFadeFCurvePlayer m_Player;

	/// <summary>
	/// �t�F�[�h�p�X�v���C�g
	/// </summary>
	CSceneGraphNodePtr m_spSprite;

	/// <summary>
	/// �V�[���O���t�̃��[�g
	/// </summary>
	CSceneGraphNodePtr m_spSceneGraphRoot;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CFadeManager();
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CFadeManagerCreate_()	sky::lib::graphic::CFadeManager::Singleton.CreateInstance()
#define CFadeManager_			sky::lib::graphic::CFadeManager::Singleton.GetInstance()
#define CFadeManagerDelete_()	sky::lib::graphic::CFadeManager::Singleton.DeleteInstance()
