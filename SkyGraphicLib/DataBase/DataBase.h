#pragma once

namespace sky { namespace lib { namespace graphic {

namespace database {

	static const u32 DATABASE_HASH_SIZE = 256;
}

/// <summary>
/// ロードデータのキャッシュデータベース
/// ローダーのロード結果は全てここに格納されます。
/// 【例】
/// graphic::CXFileLoader::LoadFile( CHS( _T( "tiny.x" ) ) );              //ロードする。
/// graphic::CModelPtr spModel = graphic::CDataBase::GetModelIndex( 0 );   //結果を取る。
/// spModel->ActivateTraverse();                                           //利用可能にする
/// CDataBase_::Clear();                                                   //破棄をする。破棄をせず次のロードをすると、ロード結果が混ざります。名前で取得できますが、名前が重複しないことが前提です。
/// </summary>
class CDataBase : public base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDataBase(){};

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// クリア（データベースをクリアしたとしても外部にスマートポインタとして保持している値は消えない）
	/// </summary>
	static void    Clear();

#define DATABASE_INF_FUNC( name , ptrname ) \
	static skyBool  IsCreated##name   ( const hash::CHashStringPtr &spName ); \
	static ptrname  Get##name         ( const hash::CHashStringPtr &spName , skyBool isClone = skyFalse , skyBool isTraverseClone = skyTrue , skyBool isSharingClone = skyTrue ); \
	static void     Add##name         ( ptrname sp##name ); \
	static u32      Get##name##Num    (); \
	static ptrname  Get##name##Index  ( u32 index , skyBool isClone = skyFalse , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse ); 

//▽▽▽▽▽▽▽▽【シーングラフ要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
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
//△△△△△△△【シーングラフ要素が追加されたら処理をここに追加する】△△△△△△△△△△△

//▽▽▽▽▽▽▽【シーングラフ要素以外が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽▽
	DATABASE_INF_FUNC( MotionData , math::CMotionDataPtr )
	DATABASE_INF_FUNC( MotionDataContena , math::CMotionDataContenaPtr )
//△△△△△△△【シーングラフ要素以外が追加されたら処理をここに追加する】△△△△△△△△△△△

private:

	/// <summary>
	/// 各種Create関数の共通実装
	/// </summary>
	static CSceneGraphNodePtr CreateName( const hash::CHashStringPtr &spName , CSceneGraphNodePtrList &hash , skyBool isClone , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse );
	static CSceneGraphNodePtr CreateIndex( u32 index , CSceneGraphNodePtrList &hash , skyBool isClone , skyBool isTraverseClone = skyFalse , skyBool isSharingClone = skyFalse );
};

#define CDataBaseInitialize_()	sky::lib::graphic::CDataBase::Initialize()
#define CDataBase_				sky::lib::graphic::CDataBase
#define CDataBaseDispose_()		sky::lib::graphic::CDataBase::Dispose()

} } }
