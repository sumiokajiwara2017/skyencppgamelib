#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// チャンク
/// </summary>
//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
static const u32 CHUNK_ROOT               = 0x00;
static const u32 CHUNK_BRANCH_START       = 0x01;
static const u32 CHUNK_BRANCH_END         = 0x02;
static const u32 CHUNK_LEAF_START         = 0x03;
static const u32 CHUNK_LEAF_END           = 0x04;
static const u32 CHUNK_CHILD_START        = 0x05;
static const u32 CHUNK_CHILD_END          = 0x06;
static const u32 CHUNK_WINDOW             = 0x07;
static const u32 CHUNK_SCENEGRAPHNODE     = 0x08;
static const u32 CHUNK_SCREENOBJECT       = 0x09;
static const u32 CHUNK_RENDERTARGET       = 0x0A;
static const u32 CHUNK_VIEWPORT           = 0x0B;
static const u32 CHUNK_SCISSORRECT        = 0x0C;
static const u32 CHUNK_EXTRADATA          = 0x0D;
static const u32 CHUNK_SCENE              = 0x14;
static const u32 CHUNK_DEPTHSTENCILBUFFER = 0x15;
static const u32 CHUNK_SPRITE             = 0x16;
static const u32 CHUNK_MODEL              = 0x17;
static const u32 CHUNK_MATERIAL           = 0x18;
static const u32 CHUNK_FONT               = 0x19;
static const u32 CHUNK_FONTSIMPLE         = 0x1A;
static const u32 CHUNK_TEXTURE            = 0x1B;
static const u32 CHUNK_MATERIALDIFFUSECOLORSHADEROBJECT = 0x1C;
static const u32 CHUNK_TEXTUREMAPPINGSHADEROBJECT       = 0x1D;
static const u32 CHUNK_VERTEXDIFFUSECOLORSHADEROBJECT   = 0x1E;
static const u32 CHUNK_POINTLIGHTSHADEROBJECT           = 0x1F;
static const u32 CHUNK_MESH                = 0x20;
static const u32 CHUNK_MESHGROUP           = 0x21;
static const u32 CHUNK_RENDERSTATE         = 0x22;
static const u32 CHUNK_INDEXBUFFER         = 0x23;
static const u32 CHUNK_VERTEXBUFFER        = 0x24;
static const u32 CHUNK_VERTEXBUFFERSTREAM  = 0x25;
static const u32 CHUNK_NILNODE             = 0x27;
static const u32 CHUNK_CAMERAMANAGER       = 0x28;
static const u32 CHUNK_SKINMESHANIMATION   = 0x29;
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△

static void LoadSceneGraphNode( const CSceneGraphNodePtr& spParentNode , const stream::IStreamPtr& stream , eRelationKind eKind );
static void LoadSceneGraphNodeImple( const CSceneGraphNodePtr& spNode , const CSceneGraphNodePtr& spParentNode , const stream::IStreamPtr& stream , eRelationKind eKind );
static void SaveSceneGraphNode( const CSceneGraphNodePtr& spNode , const stream::IStreamPtr& stream );

void CSkyGraphicBinaryLoader::LoadFile( const hash::CHashStringPtr &path )
{
	//ファイルを読む
	stream::IStreamPtr spSrcStream = CFileUtil_::LoadFile( path );

	//ストリームを開く
	spSrcStream->vOpen();

	//バイナリチェック
	if ( CMemoryUtil_::Cmp( spSrcStream->vGetData() , binary::g_cFileCode , sizeof( binary::g_cFileCode ) ) )
	{//バイナリの場合バイナリ解析する

		//ファイルヘッダーを読む
		binary::FileHeader fileheader;
		spSrcStream->vReadStream( ( u8 * )&fileheader , sizeof( binary::FileHeader ) );

		if ( CMemoryUtil_::Cmp( spSrcStream->vGetSeekData() , ( u8 * )&CHUNK_ROOT , sizeof( CHUNK_ROOT ) ) )
		{
			spSrcStream->vAddSeek( sizeof( CHUNK_ROOT ) );
			while( fileheader.nFileSize > spSrcStream->vGetSeek() )
			{
				//シーングラフ系の解析
				LoadSceneGraphNode( CSceneGraphNodePtr() , spSrcStream , eRelationKind_Root );
			}
		}
	}
	else
	{
		SKY_PANIC_MSG( _T( "This file is not skygraphicbinary. %s" ) , path->GetString() );
	}

	//ストリームを閉じる
	spSrcStream->vClose();
}

static void LoadSceneGraphNode( const CSceneGraphNodePtr& spParentNode , const stream::IStreamPtr& stream , eRelationKind eKind )
{

#define LOAD_CREATE_FUNC( classname , name , ptrname , simbol ) \
	if ( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&simbol , sizeof( simbol ) ) ) \
	{ \
		stream->vAddSeek( sizeof( simbol ) ); \
		ptrname sp##name = classname##CreateName_( hash::CHashStringPtr() ); \
		LoadSceneGraphNodeImple( sp##name , spParentNode , stream , eKind ); \
		CDataBase_::Add##name( sp##name ); \
	} else

//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
	LOAD_CREATE_FUNC( CWindow , Window , CWindowPtr , CHUNK_WINDOW )
	LOAD_CREATE_FUNC( CScreenObject , ScreenObject , CScreenObjectPtr , CHUNK_SCREENOBJECT )
	LOAD_CREATE_FUNC( CRenderTarget , RenderTarget , CRenderTargetPtr , CHUNK_RENDERTARGET )
	LOAD_CREATE_FUNC( CDepthStencilBuffer , DepthStencilBuffer , CDepthStencilBufferPtr , CHUNK_DEPTHSTENCILBUFFER )
	LOAD_CREATE_FUNC( CViewPort , ViewPort , CViewPortPtr , CHUNK_VIEWPORT )
	LOAD_CREATE_FUNC( CScissorRect , ScissorRect , CScissorRectPtr , CHUNK_SCISSORRECT )
	LOAD_CREATE_FUNC( CExtraData , ExtraData , CExtraDataPtr , CHUNK_EXTRADATA )
	LOAD_CREATE_FUNC( CScene , Scene , CScenePtr , CHUNK_SCENE )
	LOAD_CREATE_FUNC( CSprite , Sprite , CSpritePtr , CHUNK_SPRITE )
	LOAD_CREATE_FUNC( CModel , Model , CModelPtr , CHUNK_MODEL )
	LOAD_CREATE_FUNC( CMaterial , Material , CMaterialPtr , CHUNK_MATERIAL )
	LOAD_CREATE_FUNC( CFont , Font , CFontPtr , CHUNK_FONT )
	LOAD_CREATE_FUNC( CFontSimple , FontSimple , CFontSimplePtr , CHUNK_FONTSIMPLE )
	LOAD_CREATE_FUNC( CTexture , Texture , CTexturePtr , CHUNK_TEXTURE )
	LOAD_CREATE_FUNC( CMaterialDiffuseColorShaderObject , MaterialDiffuseColorShaderObject , CMaterialDiffuseColorShaderObjectPtr , CHUNK_MATERIALDIFFUSECOLORSHADEROBJECT )
	LOAD_CREATE_FUNC( CTextureMappingShaderObject , TextureMappingShaderObject , CTextureMappingShaderObjectPtr , CHUNK_TEXTUREMAPPINGSHADEROBJECT )
	LOAD_CREATE_FUNC( CVertexDiffuseColorShaderObject , VertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObjectPtr , CHUNK_VERTEXDIFFUSECOLORSHADEROBJECT )
	LOAD_CREATE_FUNC( CPointLightShaderObject , PointLightShaderObject , CPointLightShaderObjectPtr , CHUNK_POINTLIGHTSHADEROBJECT )
	LOAD_CREATE_FUNC( CMeshContena , Mesh , CMeshContenaPtr , CHUNK_MESH )
	LOAD_CREATE_FUNC( CMesh , Mesh , CMeshPtr , CHUNK_MESHGROUP )
	LOAD_CREATE_FUNC( CRenderState , RenderState , CRenderStatePtr , CHUNK_RENDERSTATE )
	LOAD_CREATE_FUNC( CIndexBuffer , IndexBuffer , CIndexBufferPtr , CHUNK_INDEXBUFFER )
	LOAD_CREATE_FUNC( CVertexBuffer , VertexBuffer , CVertexBufferPtr , CHUNK_VERTEXBUFFER )
	LOAD_CREATE_FUNC( CVertexBufferStream , VertexBufferStream , CVertexBufferStreamPtr , CHUNK_VERTEXBUFFERSTREAM )
	LOAD_CREATE_FUNC( CNilNode , NilNode , CNilNodePtr , CHUNK_NILNODE )
	LOAD_CREATE_FUNC( CameraManager , CameraManager , CameraManagerPtr , CHUNK_CAMERAMANAGER )
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△
	LOAD_CREATE_FUNC( CSceneGraphNode , SceneGraphNode , CSceneGraphNodePtr , CHUNK_SCENEGRAPHNODE ) //こいつは絶対最後
	if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_BRANCH_END , sizeof( CHUNK_BRANCH_END ) ) )
	{
		//返す
	}
	else if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_LEAF_END , sizeof( CHUNK_LEAF_END ) ) )
	{
		//返す
	}
	else if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_CHILD_END , sizeof( CHUNK_CHILD_END ) ) )
	{
		//返す
	}
	else
	{
		SKY_PANIC_MSG( _T( "This chunk is unsupport. %d  \n" ) , *( u32 * )stream->vGetSeekData() ); //想定外のチャンクが見つかりました。
	}
}

static void LoadSceneGraphNodeImple( const CSceneGraphNodePtr& spNode , const CSceneGraphNodePtr& spParentNode , const stream::IStreamPtr& stream , eRelationKind eKind )
{
	//自分自身を作る
	spNode->LoadBinary( stream );

	//親があったら親子関係を結ぶ
	if ( spParentNode.IsEmpty() == skyFalse )
	{
		if( eKind == eRelationKind_Branch )
		{
			spParentNode->AttachBranch( spNode );
		}
		else if( eKind == eRelationKind_Leaf )
		{
			spParentNode->AttachLeaf( spNode );
		}
		else if( eKind == eRelationKind_Child )
		{
			spParentNode->AttachChild( spNode );
		}
	}

	//枝がいるか？
	if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_BRANCH_START , sizeof( CHUNK_BRANCH_START ) ) )
	{
		stream->vAddSeek( sizeof( CHUNK_BRANCH_START ) );
		LoadSceneGraphNode( spNode , stream , eRelationKind_Branch );
		if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_BRANCH_END , sizeof( CHUNK_BRANCH_END ) ) )
		{
			stream->vAddSeek( sizeof( CHUNK_BRANCH_END ) );
		}
		else
		{
			SKY_PANIC();
		}
	}

	//葉がいるか？
	if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_LEAF_START , sizeof( CHUNK_LEAF_START ) ) )
	{
		stream->vAddSeek( sizeof( CHUNK_LEAF_START ) );
		while( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_LEAF_END , sizeof( CHUNK_LEAF_END ) ) == skyFalse )
		{
			LoadSceneGraphNode( spNode , stream , eRelationKind_Leaf );
		}
		stream->vAddSeek( sizeof( CHUNK_LEAF_END ) );
	}

	//子がいるか？
	if( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_CHILD_START , sizeof( CHUNK_CHILD_START ) ) )
	{
		stream->vAddSeek( sizeof( CHUNK_CHILD_START ) );
		while( CMemoryUtil_::Cmp( stream->vGetSeekData() , ( u8 * )&CHUNK_CHILD_END , sizeof( CHUNK_CHILD_END ) ) == skyFalse )
		{
			LoadSceneGraphNode( spNode , stream , eRelationKind_Child );
		}
		stream->vAddSeek( sizeof( CHUNK_CHILD_END ) );
	}
}

#define PRE_SAVE_FILE() \
	file::CFileStreamPtr stream( NEW__( file::CFileStream , file::CFileStream( path->GetString() , 8 , skyTrue , skyFalse , skyTrue ) ) ); \
	binary::FileHeader fileheader; \
	CMemoryUtil_::Copy( fileheader.cCode , binary::g_cFileCode , sizeof( binary::g_cFileCode ) ); \
	fileheader.bIsBigEndian = endian::EndianUtil::GetEndian(); \
	fileheader.nVersion = binary::VERSION_NO; \
	stream->vMarkWrite( binary::FILE_HEADER_MARK , sizeof( binary::FileHeader ) ); \
	stream->vWriteStream( CHUNK_ROOT ); 

#define POST_SAVE_FILE() \
	fileheader.nFileSize = stream->vGetSize(); \
	stream->vMarkReplace( binary::FILE_HEADER_MARK , ( u8 *)&fileheader , sizeof( binary::FileHeader ) ); \
	CFileUtil_::SaveFile( stream ); 

void CSkyGraphicBinaryLoader::SaveFile( const hash::CHashStringPtr &path , const CSceneGraphNodePtr &spSceneGraphNode )
{
	PRE_SAVE_FILE();
	SaveSceneGraphNode( spSceneGraphNode , stream );
	POST_SAVE_FILE();
}

static void SaveSceneGraphNode( const CSceneGraphNodePtr& spNode , const stream::IStreamPtr& stream )
{

#define LOAD_JUDG_FUNC( classname , simbol ) \
	if( SkyIsKindOf( classname , spNode.GetPtrNoConst() ) ) \
	{ \
		stream->vWriteStream( simbol ); \
	} else

//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
	LOAD_JUDG_FUNC( CWindow , CHUNK_WINDOW )
	LOAD_JUDG_FUNC( CScreenObject , CHUNK_SCREENOBJECT )
	LOAD_JUDG_FUNC( CRenderTarget , CHUNK_RENDERTARGET )
	LOAD_JUDG_FUNC( CDepthStencilBuffer , CHUNK_DEPTHSTENCILBUFFER )
	LOAD_JUDG_FUNC( CViewPort , CHUNK_VIEWPORT )
	LOAD_JUDG_FUNC( CScissorRect , CHUNK_SCISSORRECT )
	LOAD_JUDG_FUNC( CExtraData , CHUNK_EXTRADATA )
	LOAD_JUDG_FUNC( CSprite , CHUNK_SPRITE )
	LOAD_JUDG_FUNC( CModel , CHUNK_MODEL )
	LOAD_JUDG_FUNC( CScene , CHUNK_SCENE )
	LOAD_JUDG_FUNC( CFont , CHUNK_FONT )
	LOAD_JUDG_FUNC( CFontSimple , CHUNK_FONTSIMPLE )
	LOAD_JUDG_FUNC( CMaterial , CHUNK_MATERIAL )
	LOAD_JUDG_FUNC( CTexture , CHUNK_TEXTURE )
	LOAD_JUDG_FUNC( CMaterialDiffuseColorShaderObject , CHUNK_MATERIALDIFFUSECOLORSHADEROBJECT )
	LOAD_JUDG_FUNC( CTextureMappingShaderObject , CHUNK_TEXTUREMAPPINGSHADEROBJECT )
	LOAD_JUDG_FUNC( CVertexDiffuseColorShaderObject , CHUNK_VERTEXDIFFUSECOLORSHADEROBJECT )
	LOAD_JUDG_FUNC( CPointLightShaderObject , CHUNK_POINTLIGHTSHADEROBJECT )
	LOAD_JUDG_FUNC( CMeshContena , CHUNK_MESH )
	LOAD_JUDG_FUNC( CMesh , CHUNK_MESHGROUP )
	LOAD_JUDG_FUNC( CRenderState , CHUNK_RENDERSTATE )
	LOAD_JUDG_FUNC( CIndexBuffer , CHUNK_INDEXBUFFER )
	LOAD_JUDG_FUNC( CVertexBuffer , CHUNK_VERTEXBUFFER )
	LOAD_JUDG_FUNC( CVertexBufferStream , CHUNK_VERTEXBUFFERSTREAM )
	LOAD_JUDG_FUNC( CNilNode , CHUNK_NILNODE )
	LOAD_JUDG_FUNC( CameraManager , CHUNK_CAMERAMANAGER )
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△
	LOAD_JUDG_FUNC( CSceneGraphNode , CHUNK_SCENEGRAPHNODE ) //こいつは絶対最後
	{
		SKY_PANIC_MSG( _T( "This type is no support." ) );
	}

	//自分の情報をバイナリに書きこむ
	spNode->SaveBinary( stream );

	//枝の情報もバイナリに書きこむ
	if ( spNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		stream->vWriteStream( CHUNK_BRANCH_START );
		SaveSceneGraphNode( spNode->GetBranchRoot() , stream );
		stream->vWriteStream( CHUNK_BRANCH_END );
	}

	//葉の情報もバイナリに書きこむ
	if ( spNode->LeafList().GetNum() > 0 )
	{
		stream->vWriteStream( CHUNK_LEAF_START );
		FOREACH( CSceneGraphNodePtrList , it , spNode->LeafList() )
		{
			//再帰
			SaveSceneGraphNode( ( *it ) , stream );
		}
		stream->vWriteStream( CHUNK_LEAF_END );
	}

	//子の情報もバイナリに書き込む
	if ( spNode->ChildList().GetNum() > 0 )
	{
		stream->vWriteStream( CHUNK_CHILD_START );
		FOREACH( math::CJointPtrList , it , spNode->ChildList() )
		{
			//再帰
			SaveSceneGraphNode( ( *it ) , stream );
		}
		stream->vWriteStream( CHUNK_CHILD_END );
	}
}

} } }