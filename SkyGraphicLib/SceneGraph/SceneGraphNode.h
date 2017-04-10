#pragma once

namespace sky { namespace lib { namespace graphic {

namespace scenegraph {

const u32 NODE_STACK_DEFAULT_SIZE = 32;

}
/// <summary>
/// �V�[���O���t�X�}�[�g�|�C���^�^
/// </summary>
class CSceneGraphNode;
typedef SmartPointer< CSceneGraphNode >			CSceneGraphNodePtr;			//�X�}�[�g�|�C���^�^��`
typedef CList< u32 , CSceneGraphNodePtr >		CSceneGraphNodePtrList;		//�X�}�[�g�|�C���^���X�g�^
typedef CHash< CSceneGraphNodePtr >				CSceneGraphNodePtrHash;     //Hash��`
typedef CStack< CSceneGraphNodePtr >			CSceneGraphNodePtrStack;    //Stack��`

class CModel;
typedef SmartPointer< CModel >		CModelPtr;			//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CModelPtr >	CModelPtrList;		//���X�g��`
typedef CHash< CModelPtr >			CModelPtrHash;		//Hash��`
typedef CHashMT< CModelPtr >		CModelPtrHashMT;	//Hash��`

/// <summary>
/// �V�[���O���t�̃����_�����O�p�����[�^�[
/// </summary>
struct CRenderingParam
{
	/// <summary>
	/// �s��X�^�b�N
	/// </summary>
	math::CMatrix4x3Stack	       MatrixStack;

	/// <summary>
	/// �������f�[�^���X�g
	/// </summary>
	CAlphaRenderingCommandPtrList  AlphaDataList;

	/// <summary>
	/// �A���t�@�f�[�^���X�g
	/// </summary>
	CSceneGraphNodePtr             Window;

	/// <summary>
	/// �����_�����O�^�[�Q�b�g
	/// </summary>
	CSceneGraphNodePtrStack        RenderTargetStack;

	/// <summary>
	/// �[�x�X�e���V���o�b�t�@
	/// </summary>
	CSceneGraphNodePtrStack	       DepthStencilBufferStack;

	/// <summary>
	/// �r���[�|�[�g
	/// </summary>
	CSceneGraphNodePtrStack	       ViewPortStack;

	/// <summary>
	/// �V�U�����O��`
	/// </summary>
	CSceneGraphNodePtrStack        ScissorRectStack;

	/// <summary>
	/// �J����
	/// </summary>
	CSceneGraphNodePtrStack	       CameraStack;

	/// <summary>
	/// �����_�[�X�e�[�g
	/// </summary>
	CSceneGraphNodePtrStack	       RenderStateStack;

	/// <summary>
	/// �V�F�[�_�[
	/// </summary>
	CSceneGraphNodePtrStack	       ShaderStack;

	/// <summary>
	/// �G�L�X�g���f�[�^
	/// </summary>
	CSceneGraphNodePtrStack	       ExtraDataStack;

	/// <summary>
	/// ���C�g
	/// </summary>
	CSceneGraphNodePtrStack	       LightStack;

	/// <summary>
	/// �c���[�̐[��
	/// </summary>
	skyBool                        Indent;

	/// <summary>
	/// �X�V����
	/// </summary>
	fps::CFpsController            Fps;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �f�o�b�O�v�����g
	/// </summary>
	skyBool                        IsNodeTreePrint;

#endif

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderingParam() : 
	CameraStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ScissorRectStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ViewPortStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	DepthStencilBufferStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	RenderTargetStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	RenderStateStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ShaderStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ExtraDataStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	LightStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
#ifdef SW_SKYLIB_DEBUG_ON
	IsNodeTreePrint( skyFalse ) ,
#endif
	Indent( 0 )
	{}

	/// <summary>
	/// �������I�u�W�F�N�g�̕`��
	/// </summary>
	void AlphaDataRendering();
};

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �V�[���O���t�̃m�[�h�B
/// �V�[���O���t�̗v�f�ƂȂ�N���X�͂��ׂĂ��̃N���X���p������B
/// </summary>
class CSceneGraphNode : public CEffector
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CSceneGraphNode(){}

	/// <summary>
	/// ����ID
	/// </summary>
	s32   ID;

	/// <summary>
	/// �D�揇�ʂ̐ݒ�i���ۂ̓W���C���g�̂y�l�������_�����O�G���W�����ɃR���o�[�g���Đݒ肵�Ă邾���j
	/// </summary>
	void                            SetPrio( dectype prio );

	/// <summary>
	/// �`��
	/// </summary>
	virtual void vRendering          ( CRenderingParam &param )              { SKY_UNUSED_ARG( param ); };

	/// <summary>
	/// �q�m�[�h�̕`��i�ċA�j
	/// </summary>
	void         RenderingChild      ( CRenderingParam &param );

	/// <summary>
	/// �`��㏈��
	/// </summary>
	virtual void vPostRendering       ( CRenderingParam &param )              { SKY_UNUSED_ARG( param ); };

	/// <summary>
	/// �A�v���P�[�V�����J�����O�ݒ�
	/// </summary>
	void         SetAppCulling        ( skyBool isCulling )                  { m_IsCulling = isCulling; }
	skyBool      IsAppCulling         ()                                     { return m_IsCulling; }

	/// <summary>
	/// �[�x�o�b�t�@�N���A�ݒ�
	/// </summary>
	void         SetDepthBufferClear  ( skyBool isDepthBufferClear )          { m_IsDepthBufferClear = isDepthBufferClear; }
	skyBool      IsDepthBufferClear   ()                                      { return m_IsDepthBufferClear; }

	/// <summary>
	/// �X�e���V���o�b�t�@�N���A�ݒ�
	/// </summary>
	void         SetStencilBufferClear( skyBool isDepthBufferClear )          { m_IsStencilBufferClear = isDepthBufferClear; }
	skyBool      IsStencilBufferClear ()                                      { return m_IsStencilBufferClear; }

	/// <summary>
	/// �������ݒ�i�������ŏo�������X�v���C�g�⃂�f���̏ꍇ�A���̃t���O���n�m�ɂ��邱�Ƃŕ`�揇��ύX���Đ������`��ł���j
	/// </summary>
	void         SetAlpha             ( skyBool isAlpha )                     { m_IsAlpha = isAlpha; }
	skyBool      IsAlpha              ()                                      { return m_IsAlpha; }

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �`�掞�f�o�b�O�v�����g�����邩
	/// </summary>
	void                        SetNodeTreePrint        ( skyBool isPrint )                     { m_IsNodeTreePrintOn = isPrint; }
	skyBool                     IsNodeTreePrint         ()                                      { return m_IsNodeTreePrintOn; }

#endif

	/// <summary>
	/// �e�̍s������b�N����i���f���Ȃ��悤�Ɂj����ݒ�
	/// </summary>
	void                        SetLockJoint     ( skyBool isLockedJoint )                        { m_IsLockedJoint = isLockedJoint; }

	/// <summary>
	/// �e�̍s����v�Z����ہA��]�̓��b�N����i���f���Ȃ��悤�Ɂj����ݒ�
	/// </summary>
	void                        SetLockParentRotAndScale     ( skyBool isLockedRot )            { m_IsLockedRotAndScale = isLockedRot; }

	/// <summary>
	/// �W���C���g�g�p
	/// </summary>
	void SetJointUse( skyBool isUse = skyTrue );

	/// <summary>
	/// �W���C���g�̎g�p����
	/// </summary>
	skyBool IsJointUse();

	/// <summary>
	/// �t�̃��X�g��Ԃ�
	/// </summary>
	CSceneGraphNodePtrList &LeafList();

	/// <summary>
	/// �t�̊֘A�t��
	/// </summary>
	CSceneGraphNodePtr AttachLeaf     ( u32 key, const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// �t�̊֘A�t��
	/// </summary>
	CSceneGraphNodePtr AttachLeaf     ( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// �t�̊֘A�t��
	/// </summary>
	CSceneGraphNodePtr AttachLeafFront( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// �t�̊֘A�t������
	/// </summary>
	void               DetachLeaf     ( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// �t�̊֘A�t������
	/// </summary>
	void               DetachAllLeaf  ();

	/// <summary>
	/// �}�̊֘A�t��
	/// </summary>
	void               AttachBranch    ( const CSceneGraphNodePtr &spBranch );

	/// <summary>
	/// �}�̊֘A�t������
	/// </summary>
	void               DetachBranch    ();
	void               DetachBranch    ( const CSceneGraphNodePtr &spBranch );

	/// <summary>
	/// �}�̎擾
	/// </summary>
	const CSceneGraphNodePtr &GetBranchRoot();

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
	virtual const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	virtual const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// ����������i�e�m�[�h�ŃI�[�o�[���C�h����j
	/// </summary>
	virtual serialize::CSerializeInterfacePtr 	Clone();
	virtual serialize::CSerializeInterfacePtr 	CloneSharing();
	serialize::CSerializeInterfacePtr 	        CloneTraverse();
	serialize::CSerializeInterfacePtr 	        CloneSharingTraverse();
	static CSceneGraphNodePtr                   CloneTraverseImple( const CSceneGraphNodePtr &spNode , skyBool isSharing = skyFalse );

	/// <summary>
	/// ���s�\��Ԃɂ���
	/// </summary>
	virtual void Activate(){ ACTIVATE_COMMON(); };
	void ActivateTraverse();
	void ActivateTraverseImple( const CSceneGraphNodePtr &spNode );

	/// <summary>
	/// ���s�\��Ԃ���������
	/// </summary>
	virtual void Deactivate(){ DEACTIVATE_COMMON(); };

//---Static

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static CSceneGraphNodePtr	   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSceneGraphNodePtr spNode( NEW__( CSceneGraphNode , CSceneGraphNode() ) );
		if ( name.IsEmpty() == skyFalse ) spNode->Name.SetName( name );
		return spNode;
	}

//---Debug

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �f�o�b�O�v�����g�i�V�[���O���t�̃c���[�\�����������ꍇ�͂��������j
	/// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

	/// <summary>
	/// �g���o�[�X�f�o�b�O�v�����g
	/// </summary>
	void NodeTreePrintTraverse( u32 indent = 0 );
	static void NodeTreePrintTraverseImple( CSceneGraphNode *pNode , u32 indent , eRelationKind eKind );

	/// <summary>
	/// ���v�����g�i�f�o�b�O�v�����g���ڍׂȏ����o��,�C���f���g�͂��Ȃ��B�j
	/// </summary>
	virtual void InfoPrint();

	/// <summary>
	/// �g���o�[�X���v�����g
	/// </summary>
	void InfoPrintTraverse();
	static void InfoPrintTraverseImple( CSceneGraphNode *pNode );

#endif

protected:

//---Member

	/// <summary>
	/// �J�����O�t���O
	/// </summary>
	skyBool             m_IsCulling;

	/// <summary>
	/// �[�x�o�b�t�@�N���A�t���O
	/// </summary>
	skyBool             m_IsDepthBufferClear;

	/// <summary>
	/// �X�e���V���o�b�t�@�N���A�t���O
	/// </summary>
	skyBool             m_IsStencilBufferClear;

	/// <summary>
	/// �������t���O
	/// �}�e���A����Alpha�l�Ŕ������̔��肵�Ă��悢���A�摜��Alpha�l�𗘗p����ꍇ�A
	/// �}�e���A����Alpha�l�Ƃ͊֌W�Ȃ����ߏ����Ȃ̂Ŗ����I�Ƀt���O�𗧂Ă�K�v������Ɣ��f�B
	/// </summary>
	skyBool             m_IsAlpha;

	/// <summary>
	/// �e�̍s����͔��f���Ȃ��t���O
	/// </summary>
	skyBool             m_IsLockedJoint;

	/// <summary>
	/// �e�̉�]���͔��f���Ȃ��t���O
	/// </summary>
	skyBool             m_IsLockedRotAndScale;

	/// <summary>
	/// �W���C���g�g�p�t���O
	/// </summary>
	skyBool             m_IsJointUse;

//---Member

	/// <summary>
	/// �W���C���g�L���b�V��
	/// </summary>
	math::CJointPtr           m_spJoint;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// �f�o�b�O�v�����gOn
	/// </summary>
	skyBool             m_IsNodeTreePrintOn;

#endif

	/// <summary>
	/// �t���X�g
	/// </summary>
	CSceneGraphNodePtrList	m_LeafList;

	/// <summary>
	/// �}
	/// </summary>
	CSceneGraphNodePtr 	    m_Branch;

	/// <summary>
	/// SetUp�t���O(�Z�b�g�A�b�v���K�v�ȃm�[�h�Ŏg�p����j
	/// </summary>
	skyBool m_IsSetUp;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CSceneGraphNode( const CSceneGraphNodePtr &spParent = CSceneGraphNodePtr() );
};

#define CSceneGraphNodeCreate_()            sky::lib::graphic::CSceneGraphNode::Create()
#define CSceneGraphNodeCreateName_( name )	sky::lib::graphic::CSceneGraphNode::Create( name )

} } }
