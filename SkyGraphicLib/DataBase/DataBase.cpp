#include "StdAfx.h"
#include "SkyGraphicLib/DataBase.h"

#include "SkyGraphicLib/Material.h"
#include "SkyGraphicLib/Mesh.h"
#include "SkyGraphicLib/Texture.h"
#include "SkyGraphicLib/Sprite.h"
#include "SkyGraphicLib/Model.h"
#include "SkyGraphicLib/Screen.h"
#include "SkyGraphicLib/Nil.h"
#include "SkyGraphicLib/Font.h"


namespace sky { namespace lib { namespace graphic {

//コンテナをHashではなくListにしたのは、名前がない場合格納できないため。
//どのみち名前なしでは検索しづらいが、そこは運用時に名前を付けるか、インデックスにルールを付けるか、読み込むたびにDataBaseをクリアして検索しやすくするかすること。

//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
static CSceneGraphNodePtrList *s_pMeshContenaList               = skyNull;
static CSceneGraphNodePtrList *s_pMeshList               = skyNull;
static CSceneGraphNodePtrList *s_pMaterialList           = skyNull;
static CSceneGraphNodePtrList *s_pMaterialDrawDataList  = skyNull;
static CSceneGraphNodePtrList *s_pMaterialContenaList    = skyNull;
static CSceneGraphNodePtrList *s_pTextureList            = skyNull;
static CSceneGraphNodePtrList *s_pSpriteList             = skyNull;
static CSceneGraphNodePtrList *s_pModelList              = skyNull;
static CSceneGraphNodePtrList *s_pScreenObjectList       = skyNull;
static CSceneGraphNodePtrList *s_pWindowList             = skyNull;
static CSceneGraphNodePtrList *s_pRenderTargetList       = skyNull;
static CSceneGraphNodePtrList *s_pDepthStencilBufferList = skyNull;
static CSceneGraphNodePtrList *s_pViewPortList           = skyNull;
static CSceneGraphNodePtrList *s_pScissorRectList        = skyNull;
static CSceneGraphNodePtrList *s_pExtraDataList          = skyNull;
static CSceneGraphNodePtrList *s_pEntity2DList           = skyNull;
static CSceneGraphNodePtrList *s_pEntity3DList           = skyNull;
static CSceneGraphNodePtrList *s_pSceneGraphNodeList     = skyNull;
static CSceneGraphNodePtrList *s_pSceneList              = skyNull;
static CSceneGraphNodePtrList *s_pFontList               = skyNull;
static CSceneGraphNodePtrList *s_pFontSimpleList         = skyNull;
static CSceneGraphNodePtrList *s_pMaterialDiffuseColorShaderObjectList = skyNull;
static CSceneGraphNodePtrList *s_pTextureMappingShaderObjectList       = skyNull;
static CSceneGraphNodePtrList *s_pVertexDiffuseColorShaderObjectList   = skyNull;
static CSceneGraphNodePtrList *s_pPointLightShaderObjectList   = skyNull;
static CSceneGraphNodePtrList *s_pRenderStateList   = skyNull;
static CSceneGraphNodePtrList *s_pIndexBufferList   = skyNull;
static CSceneGraphNodePtrList *s_pVertexBufferList   = skyNull;
static CSceneGraphNodePtrList *s_pVertexBufferStreamList   = skyNull;
static CSceneGraphNodePtrList *s_pNilNodeList   = skyNull;
static CSceneGraphNodePtrList *s_pCameraManagerList   = skyNull;
static math::CMotionDataPtrList *s_pMotionDataList   = skyNull;
static math::CMotionDataContenaPtrList *s_pMotionDataContenaList   = skyNull;
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△

void CDataBase::Initialize()
{
//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
	s_pMeshContenaList           = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
	s_pMeshList      = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
	s_pMaterialList       = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
	s_pMaterialDrawDataList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
	s_pMaterialContenaList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
	s_pTextureList        = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pSpriteList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pModelList          = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pScreenObjectList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pWindowList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pRenderTargetList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pDepthStencilBufferList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pViewPortList       = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pScissorRectList    = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pExtraDataList      = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pEntity2DList       = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pEntity3DList       = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pSceneGraphNodeList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pSceneList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pFontList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pFontSimpleList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pMaterialDiffuseColorShaderObjectList = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pTextureMappingShaderObjectList       = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pVertexDiffuseColorShaderObjectList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pPointLightShaderObjectList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pRenderStateList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pIndexBufferList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pVertexBufferList         = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pVertexBufferStreamList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pNilNodeList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pCameraManagerList   = NEW__( CSceneGraphNodePtrList , CSceneGraphNodePtrList() );
    s_pMotionDataList   = NEW__( math::CMotionDataPtrList , math::CMotionDataPtrList() );
    s_pMotionDataContenaList   = NEW__( math::CMotionDataContenaPtrList , math::CMotionDataContenaPtrList() );
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△
}

void CDataBase::Dispose()
{
//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
	DEL s_pMeshContenaList;
	DEL s_pMeshList;
	DEL s_pMaterialList;
	DEL s_pMaterialDrawDataList;
	DEL s_pMaterialContenaList;
	DEL s_pTextureList;
	DEL s_pSpriteList;
	DEL s_pModelList;
	DEL s_pScreenObjectList;
	DEL s_pWindowList;
	DEL s_pRenderTargetList;
	DEL s_pDepthStencilBufferList;
	DEL s_pViewPortList;
	DEL s_pScissorRectList;
	DEL s_pExtraDataList;
	DEL s_pEntity2DList;
	DEL s_pEntity3DList;
	DEL s_pSceneGraphNodeList;
	DEL s_pSceneList;
	DEL s_pFontList;
	DEL s_pFontSimpleList;
	DEL s_pMaterialDiffuseColorShaderObjectList;
	DEL s_pTextureMappingShaderObjectList;
	DEL s_pVertexDiffuseColorShaderObjectList;
	DEL s_pPointLightShaderObjectList;
	DEL s_pRenderStateList;
	DEL s_pIndexBufferList;
	DEL s_pVertexBufferList;
	DEL s_pVertexBufferStreamList;
	DEL s_pNilNodeList;
	DEL s_pCameraManagerList;
	DEL s_pMotionDataList;
	DEL s_pMotionDataContenaList;
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△
}

void CDataBase::Clear()
{
//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
	s_pMeshContenaList->DeleteAll();
	s_pMeshList->DeleteAll();
	s_pMaterialList->DeleteAll();
	s_pMaterialDrawDataList->DeleteAll();
	s_pMaterialContenaList->DeleteAll();
	s_pTextureList->DeleteAll();
    s_pSpriteList->DeleteAll();
    s_pModelList->DeleteAll();
    s_pScreenObjectList->DeleteAll();
    s_pWindowList->DeleteAll();
    s_pRenderTargetList->DeleteAll();
    s_pDepthStencilBufferList->DeleteAll();
    s_pViewPortList->DeleteAll();
	s_pScissorRectList->DeleteAll();
	s_pExtraDataList->DeleteAll();
	s_pEntity2DList->DeleteAll();
	s_pEntity3DList->DeleteAll();
    s_pSceneGraphNodeList->DeleteAll();
    s_pSceneList->DeleteAll();
    s_pFontList->DeleteAll();
    s_pFontSimpleList->DeleteAll();
    s_pMaterialDiffuseColorShaderObjectList->DeleteAll();
    s_pTextureMappingShaderObjectList->DeleteAll();
    s_pVertexDiffuseColorShaderObjectList->DeleteAll();
    s_pPointLightShaderObjectList->DeleteAll();
    s_pRenderStateList->DeleteAll();
    s_pIndexBufferList->DeleteAll();
    s_pVertexBufferList->DeleteAll();
    s_pVertexBufferStreamList->DeleteAll();
    s_pNilNodeList->DeleteAll();
    s_pCameraManagerList->DeleteAll();
    s_pMotionDataList->DeleteAll();
    s_pMotionDataContenaList->DeleteAll();
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△
}

static skyBool GetNameFromList( const hash::CHashStringPtr &spName , CSceneGraphNodePtrList &list , CSceneGraphNodePtr &dest )
{
	skyBool result = skyFalse;

	FOREACH( CSceneGraphNodePtrList , it , list )
	{
		if ( ( *it )->Name == spName )
		{
			dest = ( *it );
			result = skyTrue;
			break;
		}
	}
	return result;
}

CSceneGraphNodePtr CDataBase::CreateName( const hash::CHashStringPtr &spName , CSceneGraphNodePtrList &list , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone )
{
	CSceneGraphNodePtr spResult;
	if ( GetNameFromList( spName , list , spResult ) )
	{
		if ( isClone == skyTrue )
		{
			if ( isTraverseClone )
			{
				if ( isSharingClone == skyFalse )
				{
					spResult = spResult->CloneTraverse();
				}
				else
				{
					spResult = spResult->CloneSharingTraverse();
				}
			}
			else
			{
				if ( isSharingClone == skyFalse )
				{
					spResult = spResult->Clone();
				}
				else
				{
					spResult = spResult->CloneSharing();
				}
			}
		}
	}
	else
	{
		SKY_PANIC_MSG( _T( "%s none \n" ) , spName->GetString() );
	}
	return spResult;
}

CSceneGraphNodePtr CDataBase::CreateIndex( u32 index , CSceneGraphNodePtrList &list , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone )
{
	CSceneGraphNodePtr spResult;
	if ( index < list.GetNum() )
	{
		spResult = list[ index ];
		if ( isClone == skyTrue )
		{
			if ( isTraverseClone )
			{
				if ( isSharingClone == skyFalse )
				{
					spResult = spResult->CloneTraverse();
				}
				else
				{
					spResult = spResult->CloneSharingTraverse();
				}
			}
			else
			{
				if ( isSharingClone == skyFalse )
				{
					spResult = spResult->Clone();
				}
				else
				{
					spResult = spResult->CloneSharing();
				}
			}
		}
	}
	else
	{
		SKY_PANIC_MSG( _T( "index over [ %d ] list num [ %d ]  \n" ) , index , list.GetNum() );
	}
	return spResult;
}

#define DATABASE_IMPLE_FUNC( name , ptrname ) \
skyBool  CDataBase::IsCreated##name ( const hash::CHashStringPtr &spName ) \
{ \
	CSceneGraphNodePtr spResult; \
	return GetNameFromList( spName , *s_p##name##List , spResult ); \
} \
ptrname CDataBase::Get##name ( const hash::CHashStringPtr &spName , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone ) \
{ \
	return CreateName( spName , *s_p##name##List , isClone , isTraverseClone , isSharingClone ); \
} \
void CDataBase::Add##name ( ptrname sp##name ) \
{ \
	s_p##name##List->AddTail( sp##name ); \
} \
u32 CDataBase::Get##name##Num () \
{ \
	return s_p##name##List->GetNum(); \
} \
ptrname CDataBase::Get##name##Index  ( u32 index , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone ) \
{ \
	return CreateIndex( index , *s_p##name##List , isClone , isTraverseClone , isSharingClone ); \
}
//▽▽▽▽▽▽▽▽【要素が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽
DATABASE_IMPLE_FUNC( MeshContena , CMeshContenaPtr )
DATABASE_IMPLE_FUNC( Mesh , CMeshPtr )
DATABASE_IMPLE_FUNC( Material , CMaterialPtr )
DATABASE_IMPLE_FUNC( MaterialDrawData , CMaterialDrawDataPtr )
DATABASE_IMPLE_FUNC( MaterialContena , CMaterialContenaPtr )
DATABASE_IMPLE_FUNC( Texture , CTexturePtr )
DATABASE_IMPLE_FUNC( Sprite , CSpritePtr )
DATABASE_IMPLE_FUNC( Model , CModelPtr )
DATABASE_IMPLE_FUNC( Window , CWindowPtr )
DATABASE_IMPLE_FUNC( ScreenObject , CScreenObjectPtr )
DATABASE_IMPLE_FUNC( RenderTarget , CRenderTargetPtr )
DATABASE_IMPLE_FUNC( DepthStencilBuffer , CDepthStencilBufferPtr )
DATABASE_IMPLE_FUNC( ViewPort , CViewPortPtr )
DATABASE_IMPLE_FUNC( ScissorRect , CScissorRectPtr )
DATABASE_IMPLE_FUNC( ExtraData , CExtraDataPtr )
DATABASE_IMPLE_FUNC( Scene , CScenePtr )
DATABASE_IMPLE_FUNC( Font , CFontPtr )
DATABASE_IMPLE_FUNC( FontSimple , CFontSimplePtr )
DATABASE_IMPLE_FUNC( MaterialDiffuseColorShaderObject , CMaterialDiffuseColorShaderObjectPtr )
DATABASE_IMPLE_FUNC( TextureMappingShaderObject , CTextureMappingShaderObjectPtr )
DATABASE_IMPLE_FUNC( VertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObjectPtr )
DATABASE_IMPLE_FUNC( PointLightShaderObject , CPointLightShaderObjectPtr )
DATABASE_IMPLE_FUNC( SceneGraphNode , CSceneGraphNodePtr )
DATABASE_IMPLE_FUNC( RenderState , CRenderStatePtr )
DATABASE_IMPLE_FUNC( IndexBuffer , CIndexBufferPtr )
DATABASE_IMPLE_FUNC( VertexBuffer , CVertexBufferPtr )
DATABASE_IMPLE_FUNC( VertexBufferStream , CVertexBufferStreamPtr )
DATABASE_IMPLE_FUNC( NilNode , CNilNodePtr )
DATABASE_IMPLE_FUNC( CameraManager , CameraManagerPtr )
//△△△△△△△【要素が追加されたら処理をここに追加する】△△△△△△△△△△△

#define DATABASE_IMPLE_FUNC_2( name , ptrname , listType ) \
skyBool  CDataBase::IsCreated##name ( const hash::CHashStringPtr &spName ) \
{ \
	skyBool result = skyFalse; \
	FOREACH( listType , it , *s_p##name##List ) \
    { \
		if ( ( *it )->Name == spName ) \
		{ \
			result = skyTrue; \
			break; \
		} \
    } \
	return result; \
} \
ptrname CDataBase::Get##name ( const hash::CHashStringPtr &spName , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone ) \
{ \
	SKY_UNUSED_ARG( isClone ); \
	SKY_UNUSED_ARG( isTraverseClone ); \
	SKY_UNUSED_ARG( isSharingClone ); \
	ptrname result; \
	FOREACH( listType , it , *s_p##name##List ) \
    { \
		if ( ( *it )->Name == spName ) \
		{ \
			result = ( *it ); \
			break; \
		} \
    } \
	return result; \
} \
void CDataBase::Add##name ( ptrname sp##name ) \
{ \
	s_p##name##List->AddTail( sp##name ); \
} \
u32 CDataBase::Get##name##Num () \
{ \
	return s_p##name##List->GetNum(); \
} \
ptrname CDataBase::Get##name##Index  ( u32 index , skyBool isClone , skyBool isTraverseClone , skyBool isSharingClone ) \
{ \
	SKY_UNUSED_ARG( isClone ); \
	SKY_UNUSED_ARG( isTraverseClone ); \
	SKY_UNUSED_ARG( isSharingClone ); \
	return s_p##name##List->GetIdx( index ); \
}

//▽▽▽▽▽▽▽【シーングラフ要素以外が追加されたら処理をここに追加する】▽▽▽▽▽▽▽▽▽▽▽
DATABASE_IMPLE_FUNC_2( MotionData        , math::CMotionDataPtr        , math::CMotionDataPtrList )
DATABASE_IMPLE_FUNC_2( MotionDataContena , math::CMotionDataContenaPtr , math::CMotionDataContenaPtrList )
//△△△△△△△【シーングラフ要素以外が追加されたら処理をここに追加する】△△△△△△△△△△△

} } }
