#pragma once

namespace sky { namespace lib { namespace graphic {

class CWindowSystemManager;

typedef SmartPointer< CWindowSystemManager >   CWindowSystemManagerPtr;		//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// Window�Ǘ��N���X
/// </summary>
class CWindowSystemManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWindowSystemManager(){};

	/// <summary>
	/// Window�m�[�h�̍쐬
	/// </summary>
	CWindowSystemNodePtr CreateWindowNode();

	/// <summary>
	/// Window�m�[�h�̔j��
	/// </summary>
	void DestroyWindowNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// �V�[���O���t���[�g�擾�i������Q�c�J�����V�[���̃V�[���O���t�ɃA�^�b�`����j
	/// </summary>
	CSceneGraphNodePtr GetSceneGraphRoot();

	/// <summary>
	/// Window�m�[�h�̃��[�g��o�^
	/// </summary>
	void AttachWindowGroupRootNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// Window�m�[�h�O���[�v�̃��[�g��o�^����
	/// </summary>
	void DetachWindowGroupRootNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// Window�m�[�h�O���[�v�̃g�O���I��
	/// </summary>
	void ToggleSelectWindowGroup();

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

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CWindowSystemManager >;
	static SingletonPtr< CWindowSystemManager > SingletonMember;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CWindowSystemManager();

private:

	/// <summary>
	/// Window�m�[�h�O���[�v�̃��[�g���X�g
	/// </summary>
	CWindowSystemNodePtrList m_WindowGroupRootNodeList;

	/// <summary>
	/// ���ݑI�𒆂̃m�[�h�̎Q��
	/// </summary>
	CWindowSystemNodePtr m_SelectedNode;

	/// <summary>
	/// �V�[���O���t�m�[�h
	/// </summary>
	CSceneGraphNodePtr m_SceneGraphAttachNode;
};

} } }