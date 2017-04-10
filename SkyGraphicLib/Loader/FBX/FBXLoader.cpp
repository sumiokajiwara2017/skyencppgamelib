#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

//#define SW_SKYLIB_USE_FBX

#ifdef SW_SKYLIB_USE_FBX

///参考サイト
///http://maverickproj.web.fc2.com/FBXSDK_02.html


// ワーニングがうっとうしい
#pragma warning(disable: 4100)
#pragma warning(disable: 4512)

//-----------FBX SDK関連-------------▽

#include "fbxsdk.h"

#pragma comment ( lib, "wininet.lib" )
#pragma comment ( lib, "advapi32.lib" )

#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment ( lib, "../../../../../thirdpartylib/fbx/lib/vs2010/x86/fbxsdk-2012.1-mtd.lib" )
#else
#pragma comment ( lib, "../../../../../thirdpartylib/fbx/lib/vs2010/x86/fbxsdk-2012.1-mt.lib" )
#endif

//-----------FBX SDK関連-------------△

static KFbxSdkManager* s_pFBXSdkManager = skyNull;

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(s_pFBXSdkManager->GetIOSettings()))
#endif

#endif

namespace sky { namespace lib { namespace graphic {

#ifdef SW_SKYLIB_USE_FBX

static void FBXSceneLoadTraverse_( KFbxNode* pNode );
static void FBXSceneLoadNode_( KFbxNode* pNode );
static void LoadMesh( KFbxMesh* pMesh , const CMeshContenaPtr& spMesh );

#endif

void CFBXLoader::Initialize()
{

#ifdef SW_SKYLIB_USE_FBX

	if ( s_pFBXSdkManager != skyNull ) return;

	// FBX SDK マネージャーオブジェクトのインスタンス化
	s_pFBXSdkManager = KFbxSdkManager::Create();
	if( s_pFBXSdkManager == NULL )
	{
		SKY_PANIC_MSG( _T( "manager created failed. \n" ) );
		return;
	}

    // 各種設定などを行うためのオブジェクトを作成
    KFbxIOSettings* ios = NULL;
    // IOSettings オブジェクトを作成。IOSROOTは階層的なプロパティ名に使用される定義らしいがよくわからん。
    ios = KFbxIOSettings::Create( s_pFBXSdkManager , IOSROOT );
    s_pFBXSdkManager->SetIOSettings( ios );

  // プラグインのディレクトリパスを設定。機能拡張に使用する？

    // 実行ファイルが置いてあるディレクトリ
    KString lPath = KFbxGetApplicationDirectory();

    // OS によって拡張子が異なるので指定する
#if defined(KARCH_ENV_WIN)
    KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
    KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
    KString lExtension = "so";
#endif

    s_pFBXSdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());
#endif
}

void  CFBXLoader::Dispose()
{
#ifdef SW_SKYLIB_USE_FBX

if( s_pFBXSdkManager ) s_pFBXSdkManager->Destroy();
   s_pFBXSdkManager = skyNull;
#endif
}

void CFBXLoader::LoadFile( const hash::CHashStringPtr &path )
{
#ifdef SW_SKYLIB_USE_FBX

	//シーンの作成
    KFbxScene* pFBXScene = KFbxScene::Create( s_pFBXSdkManager , "" );

   // FBX SDK のバージョン
   int lSDKMajor , lSDKMinor , lSDKRevision;

   // FBXファイルのバージョン
   int lFileMajor , lFileMinor , lFileRevision;

   // インポーター
   KFbxImporter* pImporter = skyNull;

   // FBX SDK のバージョン取得
   KFbxSdkManager::GetFileFormatVersion( lSDKMajor , lSDKMinor , lSDKRevision );

   // FBX ファイルを読み込むためのインポーターを作成
   pImporter = KFbxImporter::Create( s_pFBXSdkManager , "" );

//-- FBXファイルへの絶対パス内に日本語文字が存在する場合インポーターの初期化に失敗するので、絶対パスを取得してFBX SDK内で正しく読めるように変換する

   TCHAR FullName[512];
   char FileName[512];

    // FBXファイルがあるディレクトリのフルパスを取得
	if( ::GetFullPathName( path->GetString() , MAX_PATH , FullName, NULL ) == 0 ) return; //失敗

    // FBX SDK 内部の文字列は char型配列 の Unicode っぽいので変換する
#ifdef _UNICODE
    // Unicode 文字コードを第一引数で指定した文字コードに変換する
    ::WideCharToMultiByte( CP_UTF8, 0, FullName, -1, FileName, (int)((wcslen(FullName)+1) * 2), NULL, NULL );
#else
    WCHAR str[512];
    // 第一引数で指定した文字コードを Unicode 文字コードに変換する( CP_ACP は日本語WindowdではシフトJISコード )
    ::MultiByteToWideChar( CP_ACP, 0, FullName, -1, str, (int)((strlen(FullName) + 1) * sizeof(WCHAR)) );
    // Unicode 文字コードを第一引数で指定した文字コードに変換する
    ::WideCharToMultiByte( CP_UTF8, 0, str, -1, FileName, (int)((wcslen(str)+1) * 2), NULL, NULL );
#endif

//---△

	// インポーターを使用してファイルロード
   bool result = pImporter->Initialize(
                                    FileName,                          // char型配列のUnicode形式で格納される FBX ファイルパス
                                    -1,                                // ファイルフォーマットは通常指定しない。指定しない場合、拡張子によってフォーマットを決定する。
                                    s_pFBXSdkManager->GetIOSettings()  // NULL の場合デフォルトの設定を使用するらしいが、デフォルトの設定ってなに？
                                    );

    if( result == false )
    {
		SKY_PANIC_MSG( _T( "file loading invalid. \n" ) );
		return;
    }

    // 読み込んだFBXファイルのバージョンを取得
    pImporter->GetFileVersion( lFileMajor, lFileMinor, lFileRevision );

    // FBX SDK がサポートするファイルバージョンと一致しないバージョンのファイルをロードした場合はエラーにする
    if( lSDKMajor != lFileMajor || lSDKMinor != lFileMinor || lSDKRevision != lFileRevision )
    {
// 		SKY_PANIC_MSG( _T( "The version of the file is different. \n" ) );
// 		return;
    }

    // インポート対象とする要素を指定してると思うが、IMP_FBX_TEXTUREをfalseにしてもテクスチャーを普通にロードできる。意味不明。
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_MATERIAL ,        true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_TEXTURE ,         true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_LINK ,            true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_SHAPE ,           true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_GOBO ,            true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_ANIMATION ,       true );
    s_pFBXSdkManager->GetIOSettings()->SetBoolProp( IMP_FBX_GLOBAL_SETTINGS , true );

    result = pImporter->Import( pFBXScene );
    if( result == false )
    {
		// パスワードが掛っていた場合
		if( pImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
		{
			SKY_PRINTF( "Please input fbx file password." );

			char password[ 1024 ] = { '\0' };

			scanf("%s", password);
			KString lString(password);
        
			IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
			IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

			result = pImporter->Import( pFBXScene);

			if( result == false && pImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR )
			{
 				SKY_PANIC_MSG( _T( "The password is wrong. \n" ) );
 				return;
			}
			else
			{
 				SKY_PANIC_MSG( _T( "Scene Creating faild. \n" ) );
 				return;
			}
		}
		else
		{
 			SKY_PANIC_MSG( _T( "Scene Creating faild. \n" ) );
 			return;
		}
    }

//------------FBXシーンの解析--------------▽

	KFbxNode* pNode = pFBXScene->GetRootNode();
	if( pNode )
	{
		FBXSceneLoadTraverse_( pNode );
	}

//------------FBXシーンの解析--------------△


   // インポーターオブジェクトを開放
   if( pImporter ) pImporter->Destroy();

	//シーンの削除
	if( pFBXScene ) pFBXScene->Destroy();
}

static void FBXSceneLoadTraverse_( KFbxNode* pNode )
{
	// メッシュ数ごとのループ処理
	int lChildCount = pNode->GetChildCount();

	for( int i = 0 ; i < lChildCount; i++ )
	{
		KFbxNode* pChildNode = pNode->GetChild( i );

		FBXSceneLoadNode_( pChildNode );
		FBXSceneLoadTraverse_( pChildNode );
	}
}

static void FBXSceneLoadNode_( KFbxNode* pNode )
{
	KFbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	if( pNodeAttribute )
	{
		switch ( pNodeAttribute->GetAttributeType() )
		{
			// メッシュ
			case KFbxNodeAttribute::eMESH:
			{
				KFbxMesh* pFBXMesh = ( KFbxMesh * )pNodeAttribute;
				CMeshContenaPtr spMesh = CMeshContenaCreate_();
				LoadMesh( pFBXMesh , spMesh );
				break;
			}
			// カメラ
			case KFbxNodeAttribute::eCAMERA:
			{
				break;
			}
			// ジョイント
			case KFbxNodeAttribute::eSKELETON:
			{
				break;
			}
			// マーカー
			case KFbxNodeAttribute::eMARKER:
			{
				break;
			}
			// バウンディングボックス
			case KFbxNodeAttribute::eBOUNDARY:
			{
				break;
			}
		}
	}
}

static void LoadMesh( KFbxMesh* pFBXMesh , const CMeshContenaPtr& spMesh )
{
	SKY_ASSERT_MSG( pFBXMesh->GetLayerCount() == 1 , _T( "LayerCount is 1 only." ) ); //レイヤーは一枚しか想定してません。

	//頂点数、UV数、ポリゴン数をカウント
	DWORD vertexCount = pFBXMesh->GetControlPointsCount();
	DWORD uvCount     = pFBXMesh->GetTextureUVCount(); //なんでこの関数constついてないんだろう？？
	DWORD faceCount   = pFBXMesh->GetPolygonCount();

	//頂点バッファ、マテリアルの作成
	CVertexBufferPtr spVertexBuffer = CVertexBufferCreate_();
	CMaterialPtr     spMaterial     = CMaterialCreate_();
	CIndexBufferPtr  spIndexBuffer  = CIndexBufferCreate_();
	CMeshPtr    spTriangle     = CMeshCreate_();

	if ( vertexCount < uvCount )
	{
		spVertexBuffer->Initialize( ( vrtxtype )uvCount );
		spVertexBuffer->CreateNormalStream( ( vrtxtype )uvCount ); //頂点数分作る
		spVertexBuffer->CreateUvStream( ( vrtxtype )uvCount );
	}
	else
	{
		spVertexBuffer->Initialize( ( vrtxtype )vertexCount );
		spVertexBuffer->CreateNormalStream( ( vrtxtype )vertexCount ); //頂点数分作る
		spVertexBuffer->CreateUvStream( ( vrtxtype )vertexCount );
	}

	//ポリゴン数×ポリゴンの頂点数 ＝ インデックス数
	spIndexBuffer->Initialize( ( vrtxtype )( faceCount * 3 ) ); //※ポリゴンは三角形ポリゴンしか想定していません。

	//マテリアルを格納
	spTriangle->AttachMaterial( spMaterial );

	//インデックスバッファを格納
	spTriangle->AttachIndexBuffer( spIndexBuffer );

	//頂点バッファを格納
	spTriangle->AttachVertexBuffer( spVertexBuffer );

	DWORD iCount = 0;
	for( DWORD k = 0 ; k < faceCount ; k++ )
	{
		if( vertexCount < uvCount ) 
		{//UVインデックスを使用
			spIndexBuffer->SetIndex( ( vrtxtype )iCount     , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 1 , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 2 , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
		}
		else
		{//頂点インデックスを使用
			spIndexBuffer->SetIndex( ( vrtxtype )iCount     , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 0 ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 1 , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 1 ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 2 , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 2 ) );
		}
		iCount += 3;
	}

	KFbxVector4 Normal;
	for( DWORD i = 0 ; i < faceCount ; i++ )
	{
		SKY_ASSERT_MSG( pFBXMesh->GetPolygonSize( i ) == 3 , "PolygonSize is 3 only." ); //※ポリゴンは三角形ポリゴンしか想定していません。

		//※ポリゴン数が3であることを前提の処理
		int iIndex0 = 0;
		int iIndex1 = 0;
		int iIndex2 = 0;	

		int iStartIndex = pFBXMesh->GetPolygonVertexIndex( i );
		int* piIndex    = pFBXMesh->GetPolygonVertices(); //（頂点インデックス）読み込み
		iIndex0 = piIndex[ iStartIndex ];
		iIndex1 = piIndex[ iStartIndex+1 ];
		iIndex2 = piIndex[ iStartIndex+2 ];

		if( vertexCount < uvCount )//UV数のほうが多い場合は本来の（頂点ベース）インデックスを利用しない。UVインデックスを基準にする
		{
			iIndex0 = pFBXMesh->GetTextureUVIndex( i , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			iIndex1 = pFBXMesh->GetTextureUVIndex( i , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			iIndex2 = pFBXMesh->GetTextureUVIndex( i , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES );
		}

		//頂点
		int index = pFBXMesh->GetPolygonVertex( i , 0 ); //ポリゴン単位の頂点取得
		KFbxVector4 Coord = pFBXMesh->GetControlPointAt( index );
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex0 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		index = pFBXMesh->GetPolygonVertex( i , 1 ); //ポリゴン単位の頂点取得
		Coord = pFBXMesh->GetControlPointAt( index );	
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex1 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		index = pFBXMesh->GetPolygonVertex( i , 2 ); //ポリゴン単位の頂点取得
		Coord = pFBXMesh->GetControlPointAt( index );	
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex2 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		//法線		
		pFBXMesh->GetPolygonVertexNormal( i , 0 , Normal ); //ポリゴン単位の法線取得
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex0 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		pFBXMesh->GetPolygonVertexNormal( i , 1 , Normal ); //ポリゴン単位の法線取得
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex1 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		pFBXMesh->GetPolygonVertexNormal( i , 2 , Normal ); //ポリゴン単位の法線取得
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex2 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		//テクスチャー座標の設定（パターン１）
		KFbxLayerElementUV* uv= pFBXMesh->GetLayer( 0 )->GetUVs(); //※レイヤーが1枚であること前提の処理
		if( uvCount && uv->GetMappingMode() == KFbxLayerElement::eBY_POLYGON_VERTEX )
		{//この取得の仕方は、UVマッピングモードがeBY_POLYGON_VERTEXの時じゃないとできない
			
			KFbxLayerElementUV* pUV = pFBXMesh->GetLayer( 0 )->GetUVs();

			int UVindex = pFBXMesh->GetTextureUVIndex( i , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			KFbxVector2 v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex0 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y座標反転。

			UVindex = pFBXMesh->GetTextureUVIndex( i , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex1 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y座標反転。

			UVindex = pFBXMesh->GetTextureUVIndex( i , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex2 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y座標反転。
		}	
	}

	//テクスチャー座標の設定（パターン２）
	KFbxLayerElementUV* uv = pFBXMesh->GetLayer( 0 )->GetUVs(); //※レイヤーが1枚であること前提の処理
	if( uvCount && uv->GetMappingMode() == KFbxLayerElement::eBY_CONTROL_POINT)
	{
		KFbxLayerElementUV* pUV = pFBXMesh->GetLayer( 0 )->GetUVs();
		for( DWORD k = 0 ; k < uvCount ; k++ )
		{
			KFbxVector2 v2;
			v2 = pUV->GetDirectArray().GetAt( k );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )k , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y座標反転。
		}
	}

	//マテリアル読み込み
	KFbxNode* pMeshNode = pFBXMesh->GetNode(); //なんで一回ノードに戻すんだろう？？
	DWORD materialCount = pMeshNode->GetMaterialCount();

	for( DWORD i = 0 ; i < materialCount ; i++ )
	{
		if ( i > 0 )
		{
			CMaterialPtr spBeforeMaterial = spMaterial;
			spMaterial = CMaterialCreate_();                                    //新しいマテリアルを作成する
			spVertexBuffer->AttachNormalStream( spBeforeMaterial->GetNormalStream() ); //法線頂点ストリームはマテリアルで共有
			spVertexBuffer->AttachUvStream( spBeforeMaterial->GetUvStream() );         //ＵＶ頂点ストリームはマテリアルで共有
		}

		//フォンモデルを想定
		KFbxSurfaceMaterial* pMaterial= pMeshNode->GetMaterial( i );

		if ( pMaterial )
		{
			if( pMaterial->GetClassId().Is( KFbxSurfaceLambert::ClassId ) )
			{//ランバート反射モデル
				KFbxSurfaceLambert * pLambert = ( KFbxSurfaceLambert * )pMaterial;

				//環境光
				KFbxPropertyDouble3 ambient = pLambert->GetAmbientColor();
				spMaterial->Ambient.r = ( f32 )ambient.Get()[ 0 ];
				spMaterial->Ambient.g = ( f32 )ambient.Get()[ 1 ];
				spMaterial->Ambient.b = ( f32 )ambient.Get()[ 2 ];
				spMaterial->Ambient.a = 1.0f;

				//拡散反射光
				KFbxPropertyDouble3 diffuse = pLambert->GetDiffuseColor();
				spMaterial->Diffuse.r = ( f32 )diffuse.Get()[ 0 ];
				spMaterial->Diffuse.g = ( f32 )diffuse.Get()[ 1 ];
				spMaterial->Diffuse.b = ( f32 )diffuse.Get()[ 2 ];
				spMaterial->Diffuse.a = 1.0f;

				//鏡面反射光
				KFbxPropertyDouble3 specular = pLambert->GetDiffuseColor();
				spMaterial->Specular.r = ( f32 )specular.Get()[ 0 ];
				spMaterial->Specular.g = ( f32 )specular.Get()[ 1 ];
				spMaterial->Specular.b = ( f32 )specular.Get()[ 2 ];
				spMaterial->Specular.a = 1.0f;
			}
			else if( pMaterial->GetClassId().Is( KFbxSurfacePhong::ClassId ) )
			{//フォン反射モデル
				KFbxSurfacePhong * pPhong=( KFbxSurfacePhong * )pMaterial;

				//環境光
				KFbxPropertyDouble3 ambient = pPhong->GetAmbientColor();
				spMaterial->Ambient.r = ( f32 )ambient.Get()[ 0 ];
				spMaterial->Ambient.g = ( f32 )ambient.Get()[ 1 ];
				spMaterial->Ambient.b = ( f32 )ambient.Get()[ 2 ];
				spMaterial->Ambient.a = 1.0f;

				//拡散反射光
				KFbxPropertyDouble3 diffuse = pPhong->GetDiffuseColor();
				spMaterial->Diffuse.r = ( f32 )diffuse.Get()[ 0 ];
				spMaterial->Diffuse.g = ( f32 )diffuse.Get()[ 1 ];
				spMaterial->Diffuse.b = ( f32 )diffuse.Get()[ 2 ];
				spMaterial->Diffuse.a = 1.0f;

				//鏡面反射光
				KFbxPropertyDouble3 specular = pPhong->GetDiffuseColor();
				spMaterial->Specular.r = ( f32 )specular.Get()[ 0 ];
				spMaterial->Specular.g = ( f32 )specular.Get()[ 1 ];
				spMaterial->Specular.b = ( f32 )specular.Get()[ 2 ];
				spMaterial->Specular.a = 1.0f;

				//光沢
				spMaterial->Shininess = ( f32 )pPhong->GetShininess().Get();

				//反射率
				spMaterial->Refractive = ( f32 )pPhong->GetReflectionFactor().Get();
			}

			{//ディフューズテクスチャ
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sDiffuse ); //Diffuseテクスチャ取得

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //レイヤードテクスチャはサポート外

				//テクスチャ数取得
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //複数テクスチャはサポート外（これは意外とあるかも）

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //データベースに格納
					spMaterial->AttachDiffuseMap( spTexture );
				}
			}

			{//法線マップテクスチャ
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sNormalMap ); //法線マップテクスチャ取得

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //レイヤードテクスチャはサポート外

				//テクスチャ数取得
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //複数テクスチャはサポート外（これは意外とあるかも）

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //データベースに格納
					spMaterial->AttachNomalMap( spTexture );
				}
			}

			{//バンプマップテクスチャ
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sBump ); //法線マップテクスチャ取得

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //レイヤードテクスチャはサポート外

				//テクスチャ数取得
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //複数テクスチャはサポート外（これは意外とあるかも）

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //データベースに格納
					spMaterial->AttachBumpMap( spTexture );
				}
			}
		}

		//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる		
		vrtxtype materialIndexCount = 0;
		vrtxtype materialIndexStart = 0;
		skyBool isFast = skyTrue;
		for( DWORD k = 0 ; k < faceCount ; k++ )
		{
			KFbxLayerElementMaterial* mat = pFBXMesh->GetLayer(0)->GetMaterials();//レイヤーが1枚だけを想定
			int matId=mat->GetIndexArray().GetAt( k );
			if( matId == i )
			{
				if ( isFast == skyTrue )
				{
					materialIndexStart = i;
					isFast = skyFalse;
				}
				materialIndexCount += 3; //※ポリゴンは三角形ポリゴンしか想定していません。
			}
		}

		//マテリアルグループを作成する
		spMesh->CreateMesh( hash::CHashStringPtr() , spMaterial , materialIndexStart , ( vrtxtype )materialIndexCount );
	}
#endif
}

/*
//昔の処理

// 四角形ポリゴンを三角ポリゴンに変換する際に使用するインデックス
// 分割方法は適当
static u32 s_IndexArray[ 6 ] = { 0, 1, 2, 0, 2, 3 };

static void LoadMesh( KFbxMesh* pFBXMesh , const CMeshContenaPtr& spMesh )
{
    // 頂点バッファの配列数を取得
    int vertexCount = pFBXMesh->GetControlPointsCount();

    // ポリゴン数を取得
    int polygonCount = pFBXMesh->GetPolygonCount();

    // インデックスバッファの要素数を取得
    UINT indexBufferCount = 0;
    for( int i=0; i<polygonCount; i++ )
    {
        // 1ポリゴン内の頂点数を取得
        int polygonSize = pFBXMesh->GetPolygonSize( i );

        switch( polygonSize )
        {
            case 3:  // 三角ポリゴン
                indexBufferCount += 3;
                break;
            case 4:  // 四角ポリゴン
				// 四角ポリゴンの場合三角ポリゴンに変換する
				indexBufferCount += 6;
                break;
			default:
				// ５角形以上のパターンがあるけど、対応しない
				SKY_PANIC_MSG( _T( "over 4 polygon size no support." ) );
                break;
        }
    }

	SKY_ASSERT_MSG( USHRT_MAX >= indexBufferCount , _T( "vertex num is over." ) );

    // 頂点バッファ、インデックスバッファの作成
	CVertexBufferPtr spVertexBuffer = CVertexBufferCreate_();
	CIndexBufferPtr  spIndexBuffer  = CIndexBufferCreate_();
	spVertexBuffer->Initialize( ( vrtxtype )vertexCount );
	spIndexBuffer->Initialize( ( vrtxtype )indexBufferCount );
	spMesh->AttachVertexBuffer( spVertexBuffer );
	spVertexBuffer->AttachIndexBuffer( spIndexBuffer );

	//頂点の設定
    for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		KFbxVector4 fbxVec4 = pFBXMesh->GetControlPointAt( i );
		spVertexBuffer->GetPositionStream()->WritePosition( i , math::CBasicVector3( fbxVec4.GetAt( 0 ) , fbxVec4.GetAt( 1 ) , fbxVec4.GetAt( 2 ) ) );
	}

	//インデックスバッファの設定
	vrtxtype indexCount = 0;
	for( vrtxtype i = 0 ; i < polygonCount ; i++ )
	{
        // 1ポリゴン内の頂点数を取得
        int polygonSize = pFBXMesh->GetPolygonSize( i );

        switch( polygonSize )
        {
            case 3:  // 三角ポリゴン
				for( int j = 0 ; j < 3 ; j++ )
				{
					spIndexBuffer->SetIndex( indexCount , ( vrtxtype )pFBXMesh->GetPolygonVertex( i , j ) );
					indexCount++;
				}
                break;
            case 4:  // 四角ポリゴン
				for( int j = 0 ; j < 6 ; j++ )
				{
					spIndexBuffer->SetIndex( indexCount , ( vrtxtype )pFBXMesh->GetPolygonVertex( i , s_IndexArray[ j ] ) );
					indexCount++;
				}
                break;
        }
	}
	// レイヤー数を取得(レイヤー＝インデックス数が最大のメッシュグループとしてロードする。結果同じポリゴンを違うマテリアル、テクスチャ情報で何度も描画することになる。）
	int layerCount = pFBXMesh->GetLayerCount();
	for( vrtxtype r = 0 ; r < layerCount ; r++ )
	{
		CMaterialPtr spMaterial = CMaterialCreate_();

		const KFbxLayerElementNormal* leNormal = pFBXMesh->GetLayer( r )->GetNormals();
		if( leNormal )
		{    
			switch( leNormal->GetMappingMode() )
			{
			// ポリゴン単位で頂点情報がセットされている
			case KFbxLayerElement::eBY_POLYGON_VERTEX:
				{
					// 頂点ごとに対応する法線ベクトルを探す
					spMaterial->CreateNormalStream( ( vrtxtype )vertexCount ); //頂点数分作る
					for( s32 i = 0 ; i < vertexCount ; i++ )
					{
						math::CBasicVector3 Vec3;
						Vec3.Zero();
						UINT NormalIndex = 0;

						// 法線ベクトルの重複チェック用のバッファ
						math::CBasicVector3Vector NormalTempArray;

						// ポリゴンごとのループ
						for( s32 j = 0 ; j < polygonCount ; j++ )
						{
							// 1ポリゴン内の頂点数を取得
							s32 lPolygonSize = pFBXMesh->GetPolygonSize(j);
							for( s32 k = 0 ; k < lPolygonSize ; k++ )
							{
								// インデックスが同じなので処理対象
								if( i == pFBXMesh->GetPolygonVertex( j, k ) )
								{
									// 法線ベクトル取得
									KFbxVector4 lVec4 = leNormal->GetDirectArray().GetAt( NormalIndex );

									skyBool Find = skyFalse;
									for( s32 l = 0 ; l < ( s32 )NormalTempArray.GetNum(); l++ )
									{
										// すでに加算処理すみの法線ベクトルかチェックする
										if( NormalTempArray[ l ].X == lVec4.GetAt( 0 ) &&
											NormalTempArray[ l ].Y == lVec4.GetAt( 1 ) &&
											NormalTempArray[ l ].Z == lVec4.GetAt( 2 ) )
										{
											Find = skyTrue;
											break;
										}
									}
									// 法線ベクトルが完全に一致する頂点を再度加算処理することがあるので、
									// 重複して加算しないようにする
									if( Find == skyFalse )
									{
										NormalTempArray.Add( math::CBasicVector3( lVec4.GetAt( 0 ), lVec4.GetAt( 1 ), lVec4.GetAt( 2 ) ) );

										Vec3 = math::CBasicVector3( 
										Vec3.X + lVec4.GetAt( 0 ),
										Vec3.Y + lVec4.GetAt( 1 ),
										Vec3.Z + lVec4.GetAt( 2 ) );
									}
								}

								NormalIndex++;
							}
						}

						// 単位ベクトルの計算
						Vec3.Normalize();

						// 法線ベクトルをセット
						spMaterial->GetNormalStream()->WriteNormal( ( vrtxtype )i , Vec3 );
					}
				}
				break;
			// 頂点単位で頂点情報がセットされている
			case KFbxLayerElement::eBY_CONTROL_POINT:
				{
					if( leNormal->GetReferenceMode() == KFbxLayerElement::eDIRECT )
					{
						spMaterial->CreateNormalStream( ( vrtxtype )vertexCount ); //頂点数分作る
						for( s32 i = 0; i < vertexCount ; i++ )
						{
							KFbxVector4 lVec4 = leNormal->GetDirectArray().GetAt(i);

							// 法線ベクトルをセット
							spMaterial->GetNormalStream()->WriteNormal(
								( vrtxtype )i , vertex::CVertexFmt_Normal( ( f32 )lVec4.GetAt( 0 ) , ( f32 )lVec4.GetAt( 1 ) , ( f32 )lVec4.GetAt( 2 ) ) );
						}
					}
					// サンプルにここの処理は実装されていない。
					else
					{
						SKY_PANIC_MSG( _T( "this pattern no support." ) );
					}
					break;
				}
				break;

			default:
				SKY_PANIC_MSG( _T( "this pattern no support." ) );
				break;
			}

		}

		const KFbxLayerElementUV* leUV = pFBXMesh->GetLayer( r )->GetUVs();
		if( leUV )
		{ 
			switch( leUV->GetMappingMode() )
			{
			// ポリゴン単位で頂点情報がセットされている
			case KFbxLayerElement::eBY_POLYGON_VERTEX:

				switch( leUV->GetReferenceMode() )
				{
					// このパターンは確認できず
					case KFbxLayerElement::eDIRECT:
					SKY_PANIC_MSG( _T( "this pattern no support." ) );
					break;

					// インデックス参照で法線ベクトルが格納されている
					case KFbxLayerElement::eINDEX_TO_DIRECT:
					{
						// 頂点バッファのループ
						spMaterial->CreateUvStream( ( vrtxtype )vertexCount );
						for( s32 i = 0 ; i < vertexCount ; i++ )
						{
							u32 UVIndex = 0;
							// ポリゴンごとのループ
							for( s32 j = 0 ; j < polygonCount ; j++ )
							{
								// ポリゴン数を取得
								s32 lPolygonSize = pFBXMesh->GetPolygonSize(j);

								// １ポリゴン内の頂点ごとのループ
								for( s32 k = 0 ; k < lPolygonSize ; k++ )
								{
									// インデックスが同じなので処理対象
									if( i == pFBXMesh->GetPolygonVertex( j, k ) )
									{
										// インデックスバッファからインデックスを取得する
										int lUVIndex = leUV->GetIndexArray().GetAt( UVIndex );

										// 取得したインデックスから UV を取得する
										KFbxVector2 lVec2 = leUV->GetDirectArray().GetAt( lUVIndex );

										// UV値セット
										spMaterial->GetUvStream()->WriteUv( ( vrtxtype )i , math::CBasicVector2( lVec2.GetAt( 0 ) , lVec2.GetAt( 1 ) ) );
									}
									UVIndex++;
								}
							}
						}
					}
				}
	            break;

				// 3DS MAX の場合、法線を頂点ごとに分割して出力するとここが処理される
				case KFbxLayerElement::eBY_CONTROL_POINT:
					switch ( leUV->GetReferenceMode() )
					{
						// インデックスバッファを使用せず、直接法線ベクトルが格納されている
						case KFbxLayerElement::eDIRECT:
							spMaterial->CreateUvStream( ( vrtxtype )vertexCount );
							for( vrtxtype i = 0 ; i < vertexCount ; i++ )
							{
								KFbxVector2 lVec2 = leUV->GetDirectArray().GetAt(i);

								// UV値セット
								spMaterial->GetUvStream()->WriteUv( i , math::CBasicVector2( lVec2.GetAt( 0 ) , lVec2.GetAt( 1 ) ) );
							}
						break;

						// 確認できず
						case KFbxLayerElement::eINDEX_TO_DIRECT:
							SKY_PANIC_MSG( _T( "this pattern no support." ) );
							break;

						default:
							SKY_PANIC_MSG( _T( "this pattern no support." ) );
							break;
					}
				break;

			default:
				SKY_PANIC_MSG( _T( "this pattern no support." ) );
				break;
            }
		}
		spMesh->CreateMesh( hash::CHashStringPtr() , spMaterial , 0 , ( vrtxtype )indexCount );
	}

	//データベースに格納
	const char *name = pFBXMesh->GetName();
	if ( CStringUtil_::Len( name ) > 0 )
	{
		spMesh->Name.SetName( CHS( ( skyString * )pFBXMesh->GetName() ) );
	}
	CDataBase_::AddMeshContena( spMesh );
}

*/

} } }