#pragma once

namespace sky { namespace lib { namespace graphic {

class   CPostEffectManager;
typedef SmartPointer< CPostEffectManager > CPostEffectManagerPtr;			//�X�}�[�g�|�C���^�^��`

/// <summary>
/// �|�X�g�G�t�F�N�g�Ǘ�
/// �|�X�g�G�t�F�N�g�̓X�^�b�N�Ǘ��ŁA�|�X�g�G�t�F�N�g�p�̃V�F�[�_�[���X�^�b�N�̉�����`�悷��
/// �y���Ӂz�|�X�g�G�t�F�N�g��L���ɂ���V�[���́A�K���e�N�X�`�������_�[�^�[�Q�b�g���g�p���Ă��������B
///         ���̃N���X��CScene�N���X��ʂ��Ă̂ݐ����ł��܂��BCScene�@�\�̈ꕔ�Ƃ��ē��삵�܂��B
/// </summary>
class CPostEffectManager : public base::SkyRefObject
{
	friend class CScene;

    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CPostEffectManager(){};

	/// <summary>
	/// �o�b�t�@�p�����_�����O�^�[�Q�b�g
	/// </summary>
	void SetBufferRenderTarget( const CRenderTargetPtr &spBufferRenderTarget );

	/// <summary>
	/// �T���v���[�̐ݒ�i�����Ă������j
	/// </summary>
	void SetSampler( const CSamplerData &sampler );

	/// <summary>
	/// �|�X�g�G�t�F�N�g��ς�
	/// </summary>
	void PushPostEffect( const CPostEffectObjectPtr &spPostEffect );

	/// <summary>
	/// �|�X�g�G�t�F�N�g�����
	/// </summary>
	void PopPostEffect();

	/// <summary>
	/// �|�X�g�G�t�F�N�g��L���^����
	/// </summary>
	void SetEnablePostEffect( u32 stackNo , skyBool isEnable );

	/// <summary>
	/// �|�X�g�G�t�F�N�g�����s
	/// �����͏o�̓����_�����O�^�[�Q�b�g�B
	/// </summary>
	void Rendering( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spDepthStencilBuffer );

//---Serialize

	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	skyBool LoadBinary( const stream::IStreamPtr& stream );

private:

	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�X�^�b�N�i�^�̓��X�g�����X�^�b�N�̂悤�Ɏg�p����j
	/// </summary>
	CPostEffectObjectPtrList m_PostEffectStack;

	/// <summary>
	/// �\���p�X�v���C�g
	/// </summary>
	CSceneGraphNodePtr m_BufferSizeSprite; //�t�Q�Ƃ����邽�ߌ^�͂�����CSpritePtr�ł͂Ȃ�

	/// <summary>
	/// �\���p�J����
	/// </summary>
	CameraPtr m_spCamera;

	/// <summary>
	/// �T���v���[�f�[�^
	/// �T���v���̓|�X�g�G�t�F�N�g�����������邽�߂̓��̓e�N�X�`���f�[�^�B
	/// ���̓����_�����O�^�[�Q�b�g�����ł����ꍇ�͎g�p���Ȃ��B
	/// </summary>
	CSamplerData       m_SamplerData;

	/// <summary>
	/// �o�b�t�@�p�����_�����O�^�[�Q�b�g
	/// </summary>
	CRenderTargetPtr   m_spBufferRenderTarget;

	/// <summary>
	/// �o�b�t�@�R�s�[�p�|�X�g�G�t�F�N�g
	/// </summary>
	CPostEffectTextureMappingPtr m_spBufferCopyPostEffect;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPostEffectManager();

	/// <summary>
	/// �`��p�X�v���C�g�̍쐬
	/// </summary>
	void CreateSprite(  dectype width , dectype height );

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CPostEffectManagerPtr Create()
	{
		CPostEffectManagerPtr spPostEffectManager( NEW__( CPostEffectManager , CPostEffectManager() ) );
		return spPostEffectManager;
	}
};

} } }

#define CPostEffectManagerCreate_()	 sky::lib::graphic::CPostEffectManager::Create()
