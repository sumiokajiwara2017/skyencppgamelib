#pragma once

namespace sky { namespace lib { namespace graphic {

namespace database {

	static const u32 DATABASE_HASH_SIZE = 256;
}

/// <summary>
/// ���[�h�f�[�^�̃L���b�V���f�[�^�x�[�X
/// ���[�_�[�̃��[�h���ʂ͑S�Ă����Ɋi�[����܂��B
/// �y��z
/// graphic::CXFileLoader::LoadFile( CHS( _T( "tiny.x" ) ) );              //���[�h����B
/// graphic::CModelPtr spModel = graphic::CDataBase::GetModelIndex( 0 );   //���ʂ����B
/// spModel->ActivateTraverse();                                           //���p�\�ɂ���
/// CDataBase_::Clear();                                                   //�j��������B�j�����������̃��[�h������ƁA���[�h���ʂ�������܂��B���O�Ŏ擾�ł��܂����A���O���d�����Ȃ����Ƃ��O��ł��B
/// </summary>
class CDataBase : public base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDataBase(){};

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose();

	/// <summary>
	/// �N���A�i�f�[�^�x�[�X���N���A�����Ƃ��Ă��O���ɃX�}�[�g�|�C���^�Ƃ��ĕێ����Ă���l�͏����Ȃ��j
	/// </summary>
	static void    Clear();

#define DATABASE_INF_FUNC( name , ptrname ) \
	static skyBool  IsCreated##name   ( const hash::CHashStringPtr &spName ); \
	static ptrname  Get##name         ( const hash::CHashStringPtr &spName , skyBool isClone = skyFalse , skyBool isTraverseClone = skyTrue , skyBool isSharingClone = skyTrue ); \
	static void     Add##name         ( ptrname sp##name ); \
	static u32      Get##name##Num    (); \
	static ptrname  Get##name##Index  ( u32 index , skyBool isClone = skyFalse , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse ); 

//�����������������y�V�[���O���t�v�f���ǉ����ꂽ�珈���������ɒǉ�����z��������������������
	DATABASE_INF_FUNC( MeshContena , CMeshContenaPtr )
	DATABASE_INF_FUNC( Mesh , CMeshPtr )
	DATABASE_INF_FUNC( Material , CMaterialPtr )
	DATABASE_INF_FUNC( MaterialDrawData , CMaterialDrawDataPtr )
	DATABASE_INF_FUNC( MaterialContena , CMaterialContenaPtr )
	DATABASE_INF_FUNC( Texture , CTexturePtr )
	DATABASE_INF_FUNC( Sprite , CSpritePtr )
	DATABASE_INF_FUNC( Model , CModelPtr )
	DATABASE_INF_FUNC( Window , CWindowPtr )
	DATABASE_INF_FUNC( ScreenObject , CScreenObjectPtr )
	DATABASE_INF_FUNC( RenderTarget , CRenderTargetPtr )
	DATABASE_INF_FUNC( DepthStencilBuffer , CDepthStencilBufferPtr )
	DATABASE_INF_FUNC( ViewPort , CViewPortPtr )
	DATABASE_INF_FUNC( ScissorRect , CScissorRectPtr )
	DATABASE_INF_FUNC( ExtraData , CExtraDataPtr )
	DATABASE_INF_FUNC( Scene , CScenePtr )
	DATABASE_INF_FUNC( Font , CFontPtr )
	DATABASE_INF_FUNC( FontSimple , CFontSimplePtr )
	DATABASE_INF_FUNC( MaterialDiffuseColorShaderObject , CMaterialDiffuseColorShaderObjectPtr )
	DATABASE_INF_FUNC( TextureMappingShaderObject , CTextureMappingShaderObjectPtr )
	DATABASE_INF_FUNC( VertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObjectPtr )
	DATABASE_INF_FUNC( PointLightShaderObject , CPointLightShaderObjectPtr )
	DATABASE_INF_FUNC( SceneGraphNode , CSceneGraphNodePtr )
	DATABASE_INF_FUNC( RenderState , CRenderStatePtr )
	DATABASE_INF_FUNC( IndexBuffer , CIndexBufferPtr )
	DATABASE_INF_FUNC( VertexBuffer , CVertexBufferPtr )
	DATABASE_INF_FUNC( VertexBufferStream , CVertexBufferStreamPtr )
	DATABASE_INF_FUNC( NilNode , CNilNodePtr )
	DATABASE_INF_FUNC( CameraManager , CameraManagerPtr )
//���������������y�V�[���O���t�v�f���ǉ����ꂽ�珈���������ɒǉ�����z����������������������

//���������������y�V�[���O���t�v�f�ȊO���ǉ����ꂽ�珈���������ɒǉ�����z����������������������
	DATABASE_INF_FUNC( MotionData , math::CMotionDataPtr )
	DATABASE_INF_FUNC( MotionDataContena , math::CMotionDataContenaPtr )
//���������������y�V�[���O���t�v�f�ȊO���ǉ����ꂽ�珈���������ɒǉ�����z����������������������

private:

	/// <summary>
	/// �e��Create�֐��̋��ʎ���
	/// </summary>
	static CSceneGraphNodePtr CreateName( const hash::CHashStringPtr &spName , CSceneGraphNodePtrList &hash , skyBool isClone , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse );
	static CSceneGraphNodePtr CreateIndex( u32 index , CSceneGraphNodePtrList &hash , skyBool isClone , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse );
};

#define CDataBaseInitialize_()	sky::lib::graphic::CDataBase::Initialize()
#define CDataBase_				sky::lib::graphic::CDataBase
#define CDataBaseDispose_()		sky::lib::graphic::CDataBase::Dispose()

} } }
