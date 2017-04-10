#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �����_�����O�X�e�b�v����
/// </summary>
class CRenderStepManager : public base::SkyRefObject
{

//�F�B
friend class CRenderStep; //�F�B�Ȃ̂ŁA�����̃V�[���O���t�̎Q�ƂƂ������B

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderStepManager(){};

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CRenderStepManager >;
    static SingletonPtr< CRenderStepManager > SingletonMember;

	/// <summary>
	/// �X�e�b�v�̓o�^
	/// </summary>
	void AttachStep( RenderStep_Prio prio , CRenderStepPtr &spStep );

	/// <summary>
	/// �X�e�b�v�̎擾
	/// </summary>
	CRenderStepPtr Get( const hash::CHashStringPtr &spHash );

//---------------�V�[��-------------------------��

	/// <summary>
	/// �V�[���̒ǉ�
	/// </summary>
	void AttachScene      ( dectype prio , const CScenePtr &spScene );
	void AttachFrontScene ( const CScenePtr &spScene );
	void AttachTailScene  ( const CScenePtr &spScene );

	/// <summary>
	/// �V�[���̍폜
	/// </summary>
	void DetachScene( const CScenePtr &spScene );

	/// <summary>
	/// �V�[�����X�g�̎擾
	/// </summary>
	CScenePtrList &GetSceneList();

	/// <summary>
	/// �V�[���̑S�j���i�񓯊��̏ꍇ�A�`��̏I���ɂ�����ĂсA�`��p�ɃN���[�����ꂽ�V�[���O���t��S�j������j
	/// </summary>
	void DeleteAllScene();

	/// <summary>
	/// �����_�����O
	/// </summary>
	void Rendering( const fps::CFpsController &fps );

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CRenderStepManagerPtr Create()
	{
		
		CRenderStepManagerPtr spRenderStepManager( NEW__( CRenderStepManager , CRenderStepManager() ) );
		return spRenderStepManager;
	}

private:

	/// <summary>
	/// �X�e�b�v���X�g
	/// </summary>
	CRenderStepPtrList			m_kStepList;

	/// <summary>
	/// �X�e�b�vHash
	/// </summary>
	CRenderStepPtrHash			m_kStepHash;

	/// <summary>
	/// �V�[��List
	/// </summary>
	CScenePtrList				m_kSceneList;

	/// <summary>
	/// ���O�ł̃V�[���O���t�̎擾
	/// </summary>
	skyBool GetSceneGraph_( const hash::CHashStringPtr &spHash , CSceneGraphNodePtr &spNode );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderStepManager(): m_kStepHash(C_S32(render_SCENEGRAPH_HASH_SIZE) ){};

};

} } }

//�C���X�^���X�A�N�Z�X������
#define CRenderStepManagerCreate_()	sky::lib::graphic::CRenderStepManager::SingletonMember.CreateInstance()
#define CRenderStepManager_			sky::lib::graphic::CRenderStepManager::SingletonMember.GetInstance()
#define CRenderStepManagerDelete_()	sky::lib::graphic::CRenderStepManager::SingletonMember.DeleteInstance()
