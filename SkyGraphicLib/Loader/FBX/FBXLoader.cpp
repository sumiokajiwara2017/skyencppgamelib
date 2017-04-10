#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

//#define SW_SKYLIB_USE_FBX

#ifdef SW_SKYLIB_USE_FBX

///�Q�l�T�C�g
///http://maverickproj.web.fc2.com/FBXSDK_02.html


// ���[�j���O�������Ƃ�����
#pragma warning(disable: 4100)
#pragma warning(disable: 4512)

//-----------FBX SDK�֘A-------------��

#include "fbxsdk.h"

#pragma comment ( lib, "wininet.lib" )
#pragma comment ( lib, "advapi32.lib" )

#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment ( lib, "../../../../../thirdpartylib/fbx/lib/vs2010/x86/fbxsdk-2012.1-mtd.lib" )
#else
#pragma comment ( lib, "../../../../../thirdpartylib/fbx/lib/vs2010/x86/fbxsdk-2012.1-mt.lib" )
#endif

//-----------FBX SDK�֘A-------------��

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

	// FBX SDK �}�l�[�W���[�I�u�W�F�N�g�̃C���X�^���X��
	s_pFBXSdkManager = KFbxSdkManager::Create();
	if( s_pFBXSdkManager == NULL )
	{
		SKY_PANIC_MSG( _T( "manager created failed. \n" ) );
		return;
	}

    // �e��ݒ�Ȃǂ��s�����߂̃I�u�W�F�N�g���쐬
    KFbxIOSettings* ios = NULL;
    // IOSettings �I�u�W�F�N�g���쐬�BIOSROOT�͊K�w�I�ȃv���p�e�B���Ɏg�p������`�炵�����悭�킩���B
    ios = KFbxIOSettings::Create( s_pFBXSdkManager , IOSROOT );
    s_pFBXSdkManager->SetIOSettings( ios );

  // �v���O�C���̃f�B���N�g���p�X��ݒ�B�@�\�g���Ɏg�p����H

    // ���s�t�@�C�����u���Ă���f�B���N�g��
    KString lPath = KFbxGetApplicationDirectory();

    // OS �ɂ���Ċg���q���قȂ�̂Ŏw�肷��
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

	//�V�[���̍쐬
    KFbxScene* pFBXScene = KFbxScene::Create( s_pFBXSdkManager , "" );

   // FBX SDK �̃o�[�W����
   int lSDKMajor , lSDKMinor , lSDKRevision;

   // FBX�t�@�C���̃o�[�W����
   int lFileMajor , lFileMinor , lFileRevision;

   // �C���|�[�^�[
   KFbxImporter* pImporter = skyNull;

   // FBX SDK �̃o�[�W�����擾
   KFbxSdkManager::GetFileFormatVersion( lSDKMajor , lSDKMinor , lSDKRevision );

   // FBX �t�@�C����ǂݍ��ނ��߂̃C���|�[�^�[���쐬
   pImporter = KFbxImporter::Create( s_pFBXSdkManager , "" );

//-- FBX�t�@�C���ւ̐�΃p�X���ɓ��{�ꕶ�������݂���ꍇ�C���|�[�^�[�̏������Ɏ��s����̂ŁA��΃p�X���擾����FBX SDK���Ő������ǂ߂�悤�ɕϊ�����

   TCHAR FullName[512];
   char FileName[512];

    // FBX�t�@�C��������f�B���N�g���̃t���p�X���擾
	if( ::GetFullPathName( path->GetString() , MAX_PATH , FullName, NULL ) == 0 ) return; //���s

    // FBX SDK �����̕������ char�^�z�� �� Unicode ���ۂ��̂ŕϊ�����
#ifdef _UNICODE
    // Unicode �����R�[�h��������Ŏw�肵�������R�[�h�ɕϊ�����
    ::WideCharToMultiByte( CP_UTF8, 0, FullName, -1, FileName, (int)((wcslen(FullName)+1) * 2), NULL, NULL );
#else
    WCHAR str[512];
    // �������Ŏw�肵�������R�[�h�� Unicode �����R�[�h�ɕϊ�����( CP_ACP �͓��{��Windowd�ł̓V�t�gJIS�R�[�h )
    ::MultiByteToWideChar( CP_ACP, 0, FullName, -1, str, (int)((strlen(FullName) + 1) * sizeof(WCHAR)) );
    // Unicode �����R�[�h��������Ŏw�肵�������R�[�h�ɕϊ�����
    ::WideCharToMultiByte( CP_UTF8, 0, str, -1, FileName, (int)((wcslen(str)+1) * 2), NULL, NULL );
#endif

//---��

	// �C���|�[�^�[���g�p���ăt�@�C�����[�h
   bool result = pImporter->Initialize(
                                    FileName,                          // char�^�z���Unicode�`���Ŋi�[����� FBX �t�@�C���p�X
                                    -1,                                // �t�@�C���t�H�[�}�b�g�͒ʏ�w�肵�Ȃ��B�w�肵�Ȃ��ꍇ�A�g���q�ɂ���ăt�H�[�}�b�g�����肷��B
                                    s_pFBXSdkManager->GetIOSettings()  // NULL �̏ꍇ�f�t�H���g�̐ݒ���g�p����炵�����A�f�t�H���g�̐ݒ���ĂȂɁH
                                    );

    if( result == false )
    {
		SKY_PANIC_MSG( _T( "file loading invalid. \n" ) );
		return;
    }

    // �ǂݍ���FBX�t�@�C���̃o�[�W�������擾
    pImporter->GetFileVersion( lFileMajor, lFileMinor, lFileRevision );

    // FBX SDK ���T�|�[�g����t�@�C���o�[�W�����ƈ�v���Ȃ��o�[�W�����̃t�@�C�������[�h�����ꍇ�̓G���[�ɂ���
    if( lSDKMajor != lFileMajor || lSDKMinor != lFileMinor || lSDKRevision != lFileRevision )
    {
// 		SKY_PANIC_MSG( _T( "The version of the file is different. \n" ) );
// 		return;
    }

    // �C���|�[�g�ΏۂƂ���v�f���w�肵�Ă�Ǝv�����AIMP_FBX_TEXTURE��false�ɂ��Ă��e�N�X�`���[�𕁒ʂɃ��[�h�ł���B�Ӗ��s���B
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
		// �p�X���[�h���|���Ă����ꍇ
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

//------------FBX�V�[���̉��--------------��

	KFbxNode* pNode = pFBXScene->GetRootNode();
	if( pNode )
	{
		FBXSceneLoadTraverse_( pNode );
	}

//------------FBX�V�[���̉��--------------��


   // �C���|�[�^�[�I�u�W�F�N�g���J��
   if( pImporter ) pImporter->Destroy();

	//�V�[���̍폜
	if( pFBXScene ) pFBXScene->Destroy();
}

static void FBXSceneLoadTraverse_( KFbxNode* pNode )
{
	// ���b�V�������Ƃ̃��[�v����
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
			// ���b�V��
			case KFbxNodeAttribute::eMESH:
			{
				KFbxMesh* pFBXMesh = ( KFbxMesh * )pNodeAttribute;
				CMeshContenaPtr spMesh = CMeshContenaCreate_();
				LoadMesh( pFBXMesh , spMesh );
				break;
			}
			// �J����
			case KFbxNodeAttribute::eCAMERA:
			{
				break;
			}
			// �W���C���g
			case KFbxNodeAttribute::eSKELETON:
			{
				break;
			}
			// �}�[�J�[
			case KFbxNodeAttribute::eMARKER:
			{
				break;
			}
			// �o�E���f�B���O�{�b�N�X
			case KFbxNodeAttribute::eBOUNDARY:
			{
				break;
			}
		}
	}
}

static void LoadMesh( KFbxMesh* pFBXMesh , const CMeshContenaPtr& spMesh )
{
	SKY_ASSERT_MSG( pFBXMesh->GetLayerCount() == 1 , _T( "LayerCount is 1 only." ) ); //���C���[�͈ꖇ�����z�肵�Ă܂���B

	//���_���AUV���A�|���S�������J�E���g
	DWORD vertexCount = pFBXMesh->GetControlPointsCount();
	DWORD uvCount     = pFBXMesh->GetTextureUVCount(); //�Ȃ�ł��̊֐�const���ĂȂ��񂾂낤�H�H
	DWORD faceCount   = pFBXMesh->GetPolygonCount();

	//���_�o�b�t�@�A�}�e���A���̍쐬
	CVertexBufferPtr spVertexBuffer = CVertexBufferCreate_();
	CMaterialPtr     spMaterial     = CMaterialCreate_();
	CIndexBufferPtr  spIndexBuffer  = CIndexBufferCreate_();
	CMeshPtr    spTriangle     = CMeshCreate_();

	if ( vertexCount < uvCount )
	{
		spVertexBuffer->Initialize( ( vrtxtype )uvCount );
		spVertexBuffer->CreateNormalStream( ( vrtxtype )uvCount ); //���_�������
		spVertexBuffer->CreateUvStream( ( vrtxtype )uvCount );
	}
	else
	{
		spVertexBuffer->Initialize( ( vrtxtype )vertexCount );
		spVertexBuffer->CreateNormalStream( ( vrtxtype )vertexCount ); //���_�������
		spVertexBuffer->CreateUvStream( ( vrtxtype )vertexCount );
	}

	//�|���S�����~�|���S���̒��_�� �� �C���f�b�N�X��
	spIndexBuffer->Initialize( ( vrtxtype )( faceCount * 3 ) ); //���|���S���͎O�p�`�|���S�������z�肵�Ă��܂���B

	//�}�e���A�����i�[
	spTriangle->AttachMaterial( spMaterial );

	//�C���f�b�N�X�o�b�t�@���i�[
	spTriangle->AttachIndexBuffer( spIndexBuffer );

	//���_�o�b�t�@���i�[
	spTriangle->AttachVertexBuffer( spVertexBuffer );

	DWORD iCount = 0;
	for( DWORD k = 0 ; k < faceCount ; k++ )
	{
		if( vertexCount < uvCount ) 
		{//UV�C���f�b�N�X���g�p
			spIndexBuffer->SetIndex( ( vrtxtype )iCount     , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 1 , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 2 , ( vrtxtype )pFBXMesh->GetTextureUVIndex( k , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES ) );
		}
		else
		{//���_�C���f�b�N�X���g�p
			spIndexBuffer->SetIndex( ( vrtxtype )iCount     , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 0 ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 1 , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 1 ) );
			spIndexBuffer->SetIndex( ( vrtxtype )iCount + 2 , ( vrtxtype )pFBXMesh->GetPolygonVertex( k , 2 ) );
		}
		iCount += 3;
	}

	KFbxVector4 Normal;
	for( DWORD i = 0 ; i < faceCount ; i++ )
	{
		SKY_ASSERT_MSG( pFBXMesh->GetPolygonSize( i ) == 3 , "PolygonSize is 3 only." ); //���|���S���͎O�p�`�|���S�������z�肵�Ă��܂���B

		//���|���S������3�ł��邱�Ƃ�O��̏���
		int iIndex0 = 0;
		int iIndex1 = 0;
		int iIndex2 = 0;	

		int iStartIndex = pFBXMesh->GetPolygonVertexIndex( i );
		int* piIndex    = pFBXMesh->GetPolygonVertices(); //�i���_�C���f�b�N�X�j�ǂݍ���
		iIndex0 = piIndex[ iStartIndex ];
		iIndex1 = piIndex[ iStartIndex+1 ];
		iIndex2 = piIndex[ iStartIndex+2 ];

		if( vertexCount < uvCount )//UV���̂ق��������ꍇ�͖{���́i���_�x�[�X�j�C���f�b�N�X�𗘗p���Ȃ��BUV�C���f�b�N�X����ɂ���
		{
			iIndex0 = pFBXMesh->GetTextureUVIndex( i , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			iIndex1 = pFBXMesh->GetTextureUVIndex( i , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			iIndex2 = pFBXMesh->GetTextureUVIndex( i , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES );
		}

		//���_
		int index = pFBXMesh->GetPolygonVertex( i , 0 ); //�|���S���P�ʂ̒��_�擾
		KFbxVector4 Coord = pFBXMesh->GetControlPointAt( index );
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex0 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		index = pFBXMesh->GetPolygonVertex( i , 1 ); //�|���S���P�ʂ̒��_�擾
		Coord = pFBXMesh->GetControlPointAt( index );	
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex1 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		index = pFBXMesh->GetPolygonVertex( i , 2 ); //�|���S���P�ʂ̒��_�擾
		Coord = pFBXMesh->GetControlPointAt( index );	
		spVertexBuffer->GetPositionStream()->WritePosition( ( vrtxtype )iIndex2 , math::CBasicVector3( ( vrtxtype )Coord.GetAt( 0 ) , ( vrtxtype )Coord.GetAt( 1 ) , ( vrtxtype )Coord.GetAt( 2 ) ) );

		//�@��		
		pFBXMesh->GetPolygonVertexNormal( i , 0 , Normal ); //�|���S���P�ʂ̖@���擾
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex0 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		pFBXMesh->GetPolygonVertexNormal( i , 1 , Normal ); //�|���S���P�ʂ̖@���擾
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex1 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		pFBXMesh->GetPolygonVertexNormal( i , 2 , Normal ); //�|���S���P�ʂ̖@���擾
		spVertexBuffer->GetNormalStream()->WriteNormal( ( vrtxtype )iIndex2 , math::CBasicVector3( Normal[ 0 ] , Normal[ 1 ] , Normal[ 2 ] ) );

		//�e�N�X�`���[���W�̐ݒ�i�p�^�[���P�j
		KFbxLayerElementUV* uv= pFBXMesh->GetLayer( 0 )->GetUVs(); //�����C���[��1���ł��邱�ƑO��̏���
		if( uvCount && uv->GetMappingMode() == KFbxLayerElement::eBY_POLYGON_VERTEX )
		{//���̎擾�̎d���́AUV�}�b�s���O���[�h��eBY_POLYGON_VERTEX�̎�����Ȃ��Ƃł��Ȃ�
			
			KFbxLayerElementUV* pUV = pFBXMesh->GetLayer( 0 )->GetUVs();

			int UVindex = pFBXMesh->GetTextureUVIndex( i , 0 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			KFbxVector2 v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex0 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y���W���]�B

			UVindex = pFBXMesh->GetTextureUVIndex( i , 1 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex1 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y���W���]�B

			UVindex = pFBXMesh->GetTextureUVIndex( i , 2 , KFbxLayerElement::eDIFFUSE_TEXTURES );
			v2 = pUV->GetDirectArray().GetAt( UVindex );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )iIndex2 , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y���W���]�B
		}	
	}

	//�e�N�X�`���[���W�̐ݒ�i�p�^�[���Q�j
	KFbxLayerElementUV* uv = pFBXMesh->GetLayer( 0 )->GetUVs(); //�����C���[��1���ł��邱�ƑO��̏���
	if( uvCount && uv->GetMappingMode() == KFbxLayerElement::eBY_CONTROL_POINT)
	{
		KFbxLayerElementUV* pUV = pFBXMesh->GetLayer( 0 )->GetUVs();
		for( DWORD k = 0 ; k < uvCount ; k++ )
		{
			KFbxVector2 v2;
			v2 = pUV->GetDirectArray().GetAt( k );
			spVertexBuffer->GetUvStream()->WriteUv( ( vrtxtype )k , math::CBasicVector2( v2.GetAt( 0 ) , 1.0f - v2.GetAt( 1 ) ) ); //Y���W���]�B
		}
	}

	//�}�e���A���ǂݍ���
	KFbxNode* pMeshNode = pFBXMesh->GetNode(); //�Ȃ�ň��m�[�h�ɖ߂��񂾂낤�H�H
	DWORD materialCount = pMeshNode->GetMaterialCount();

	for( DWORD i = 0 ; i < materialCount ; i++ )
	{
		if ( i > 0 )
		{
			CMaterialPtr spBeforeMaterial = spMaterial;
			spMaterial = CMaterialCreate_();                                    //�V�����}�e���A�����쐬����
			spVertexBuffer->AttachNormalStream( spBeforeMaterial->GetNormalStream() ); //�@�����_�X�g���[���̓}�e���A���ŋ��L
			spVertexBuffer->AttachUvStream( spBeforeMaterial->GetUvStream() );         //�t�u���_�X�g���[���̓}�e���A���ŋ��L
		}

		//�t�H�����f����z��
		KFbxSurfaceMaterial* pMaterial= pMeshNode->GetMaterial( i );

		if ( pMaterial )
		{
			if( pMaterial->GetClassId().Is( KFbxSurfaceLambert::ClassId ) )
			{//�����o�[�g���˃��f��
				KFbxSurfaceLambert * pLambert = ( KFbxSurfaceLambert * )pMaterial;

				//����
				KFbxPropertyDouble3 ambient = pLambert->GetAmbientColor();
				spMaterial->Ambient.r = ( f32 )ambient.Get()[ 0 ];
				spMaterial->Ambient.g = ( f32 )ambient.Get()[ 1 ];
				spMaterial->Ambient.b = ( f32 )ambient.Get()[ 2 ];
				spMaterial->Ambient.a = 1.0f;

				//�g�U���ˌ�
				KFbxPropertyDouble3 diffuse = pLambert->GetDiffuseColor();
				spMaterial->Diffuse.r = ( f32 )diffuse.Get()[ 0 ];
				spMaterial->Diffuse.g = ( f32 )diffuse.Get()[ 1 ];
				spMaterial->Diffuse.b = ( f32 )diffuse.Get()[ 2 ];
				spMaterial->Diffuse.a = 1.0f;

				//���ʔ��ˌ�
				KFbxPropertyDouble3 specular = pLambert->GetDiffuseColor();
				spMaterial->Specular.r = ( f32 )specular.Get()[ 0 ];
				spMaterial->Specular.g = ( f32 )specular.Get()[ 1 ];
				spMaterial->Specular.b = ( f32 )specular.Get()[ 2 ];
				spMaterial->Specular.a = 1.0f;
			}
			else if( pMaterial->GetClassId().Is( KFbxSurfacePhong::ClassId ) )
			{//�t�H�����˃��f��
				KFbxSurfacePhong * pPhong=( KFbxSurfacePhong * )pMaterial;

				//����
				KFbxPropertyDouble3 ambient = pPhong->GetAmbientColor();
				spMaterial->Ambient.r = ( f32 )ambient.Get()[ 0 ];
				spMaterial->Ambient.g = ( f32 )ambient.Get()[ 1 ];
				spMaterial->Ambient.b = ( f32 )ambient.Get()[ 2 ];
				spMaterial->Ambient.a = 1.0f;

				//�g�U���ˌ�
				KFbxPropertyDouble3 diffuse = pPhong->GetDiffuseColor();
				spMaterial->Diffuse.r = ( f32 )diffuse.Get()[ 0 ];
				spMaterial->Diffuse.g = ( f32 )diffuse.Get()[ 1 ];
				spMaterial->Diffuse.b = ( f32 )diffuse.Get()[ 2 ];
				spMaterial->Diffuse.a = 1.0f;

				//���ʔ��ˌ�
				KFbxPropertyDouble3 specular = pPhong->GetDiffuseColor();
				spMaterial->Specular.r = ( f32 )specular.Get()[ 0 ];
				spMaterial->Specular.g = ( f32 )specular.Get()[ 1 ];
				spMaterial->Specular.b = ( f32 )specular.Get()[ 2 ];
				spMaterial->Specular.a = 1.0f;

				//����
				spMaterial->Shininess = ( f32 )pPhong->GetShininess().Get();

				//���˗�
				spMaterial->Refractive = ( f32 )pPhong->GetReflectionFactor().Get();
			}

			{//�f�B�t���[�Y�e�N�X�`��
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sDiffuse ); //Diffuse�e�N�X�`���擾

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //���C���[�h�e�N�X�`���̓T�|�[�g�O

				//�e�N�X�`�����擾
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //�����e�N�X�`���̓T�|�[�g�O�i����͈ӊO�Ƃ��邩���j

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //�f�[�^�x�[�X�Ɋi�[
					spMaterial->AttachDiffuseMap( spTexture );
				}
			}

			{//�@���}�b�v�e�N�X�`��
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sNormalMap ); //�@���}�b�v�e�N�X�`���擾

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //���C���[�h�e�N�X�`���̓T�|�[�g�O

				//�e�N�X�`�����擾
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //�����e�N�X�`���̓T�|�[�g�O�i����͈ӊO�Ƃ��邩���j

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //�f�[�^�x�[�X�Ɋi�[
					spMaterial->AttachNomalMap( spTexture );
				}
			}

			{//�o���v�}�b�v�e�N�X�`��
				KFbxProperty lProperty;
				lProperty = pMaterial->FindProperty( KFbxSurfaceMaterial::sBump ); //�@���}�b�v�e�N�X�`���擾

				SKY_ASSERT_MSG( lProperty.GetSrcObjectCount( KFbxLayeredTexture::ClassId ) == 0 , "Rayerd Texture no support." ); //���C���[�h�e�N�X�`���̓T�|�[�g�O

				//�e�N�X�`�����擾
				int texNum = lProperty.GetSrcObjectCount( KFbxTexture::ClassId );

				SKY_ASSERT_MSG( texNum <= 1 , "Multi Texture no support." ); //�����e�N�X�`���̓T�|�[�g�O�i����͈ӊO�Ƃ��邩���j

				for ( int i = 0 ; i < texNum ; i++ )
				{
					KFbxTexture* texture = KFbxCast< KFbxTexture >( lProperty.GetSrcObject( KFbxTexture::ClassId , i ) );

					skyString *pPath = CStringUtil_::CreateString( texture->GetName() );
					CTexturePtr spTexture = CTextureCreateName_( CHS( pPath ) );
					CStringUtil_::DeleteString( pPath );
					CDataBase_::AddTexture( spTexture ); //�f�[�^�x�[�X�Ɋi�[
					spMaterial->AttachBumpMap( spTexture );
				}
			}
		}

		//���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�@����ɃC���f�b�N�X�̌������ׂ�		
		vrtxtype materialIndexCount = 0;
		vrtxtype materialIndexStart = 0;
		skyBool isFast = skyTrue;
		for( DWORD k = 0 ; k < faceCount ; k++ )
		{
			KFbxLayerElementMaterial* mat = pFBXMesh->GetLayer(0)->GetMaterials();//���C���[��1��������z��
			int matId=mat->GetIndexArray().GetAt( k );
			if( matId == i )
			{
				if ( isFast == skyTrue )
				{
					materialIndexStart = i;
					isFast = skyFalse;
				}
				materialIndexCount += 3; //���|���S���͎O�p�`�|���S�������z�肵�Ă��܂���B
			}
		}

		//�}�e���A���O���[�v���쐬����
		spMesh->CreateMesh( hash::CHashStringPtr() , spMaterial , materialIndexStart , ( vrtxtype )materialIndexCount );
	}
#endif
}

/*
//�̂̏���

// �l�p�`�|���S�����O�p�|���S���ɕϊ�����ۂɎg�p����C���f�b�N�X
// �������@�͓K��
static u32 s_IndexArray[ 6 ] = { 0, 1, 2, 0, 2, 3 };

static void LoadMesh( KFbxMesh* pFBXMesh , const CMeshContenaPtr& spMesh )
{
    // ���_�o�b�t�@�̔z�񐔂��擾
    int vertexCount = pFBXMesh->GetControlPointsCount();

    // �|���S�������擾
    int polygonCount = pFBXMesh->GetPolygonCount();

    // �C���f�b�N�X�o�b�t�@�̗v�f�����擾
    UINT indexBufferCount = 0;
    for( int i=0; i<polygonCount; i++ )
    {
        // 1�|���S�����̒��_�����擾
        int polygonSize = pFBXMesh->GetPolygonSize( i );

        switch( polygonSize )
        {
            case 3:  // �O�p�|���S��
                indexBufferCount += 3;
                break;
            case 4:  // �l�p�|���S��
				// �l�p�|���S���̏ꍇ�O�p�|���S���ɕϊ�����
				indexBufferCount += 6;
                break;
			default:
				// �T�p�`�ȏ�̃p�^�[�������邯�ǁA�Ή����Ȃ�
				SKY_PANIC_MSG( _T( "over 4 polygon size no support." ) );
                break;
        }
    }

	SKY_ASSERT_MSG( USHRT_MAX >= indexBufferCount , _T( "vertex num is over." ) );

    // ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̍쐬
	CVertexBufferPtr spVertexBuffer = CVertexBufferCreate_();
	CIndexBufferPtr  spIndexBuffer  = CIndexBufferCreate_();
	spVertexBuffer->Initialize( ( vrtxtype )vertexCount );
	spIndexBuffer->Initialize( ( vrtxtype )indexBufferCount );
	spMesh->AttachVertexBuffer( spVertexBuffer );
	spVertexBuffer->AttachIndexBuffer( spIndexBuffer );

	//���_�̐ݒ�
    for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		KFbxVector4 fbxVec4 = pFBXMesh->GetControlPointAt( i );
		spVertexBuffer->GetPositionStream()->WritePosition( i , math::CBasicVector3( fbxVec4.GetAt( 0 ) , fbxVec4.GetAt( 1 ) , fbxVec4.GetAt( 2 ) ) );
	}

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	vrtxtype indexCount = 0;
	for( vrtxtype i = 0 ; i < polygonCount ; i++ )
	{
        // 1�|���S�����̒��_�����擾
        int polygonSize = pFBXMesh->GetPolygonSize( i );

        switch( polygonSize )
        {
            case 3:  // �O�p�|���S��
				for( int j = 0 ; j < 3 ; j++ )
				{
					spIndexBuffer->SetIndex( indexCount , ( vrtxtype )pFBXMesh->GetPolygonVertex( i , j ) );
					indexCount++;
				}
                break;
            case 4:  // �l�p�|���S��
				for( int j = 0 ; j < 6 ; j++ )
				{
					spIndexBuffer->SetIndex( indexCount , ( vrtxtype )pFBXMesh->GetPolygonVertex( i , s_IndexArray[ j ] ) );
					indexCount++;
				}
                break;
        }
	}
	// ���C���[�����擾(���C���[���C���f�b�N�X�����ő�̃��b�V���O���[�v�Ƃ��ă��[�h����B���ʓ����|���S�����Ⴄ�}�e���A���A�e�N�X�`�����ŉ��x���`�悷�邱�ƂɂȂ�B�j
	int layerCount = pFBXMesh->GetLayerCount();
	for( vrtxtype r = 0 ; r < layerCount ; r++ )
	{
		CMaterialPtr spMaterial = CMaterialCreate_();

		const KFbxLayerElementNormal* leNormal = pFBXMesh->GetLayer( r )->GetNormals();
		if( leNormal )
		{    
			switch( leNormal->GetMappingMode() )
			{
			// �|���S���P�ʂŒ��_��񂪃Z�b�g����Ă���
			case KFbxLayerElement::eBY_POLYGON_VERTEX:
				{
					// ���_���ƂɑΉ�����@���x�N�g����T��
					spMaterial->CreateNormalStream( ( vrtxtype )vertexCount ); //���_�������
					for( s32 i = 0 ; i < vertexCount ; i++ )
					{
						math::CBasicVector3 Vec3;
						Vec3.Zero();
						UINT NormalIndex = 0;

						// �@���x�N�g���̏d���`�F�b�N�p�̃o�b�t�@
						math::CBasicVector3Vector NormalTempArray;

						// �|���S�����Ƃ̃��[�v
						for( s32 j = 0 ; j < polygonCount ; j++ )
						{
							// 1�|���S�����̒��_�����擾
							s32 lPolygonSize = pFBXMesh->GetPolygonSize(j);
							for( s32 k = 0 ; k < lPolygonSize ; k++ )
							{
								// �C���f�b�N�X�������Ȃ̂ŏ����Ώ�
								if( i == pFBXMesh->GetPolygonVertex( j, k ) )
								{
									// �@���x�N�g���擾
									KFbxVector4 lVec4 = leNormal->GetDirectArray().GetAt( NormalIndex );

									skyBool Find = skyFalse;
									for( s32 l = 0 ; l < ( s32 )NormalTempArray.GetNum(); l++ )
									{
										// ���łɉ��Z�������݂̖@���x�N�g�����`�F�b�N����
										if( NormalTempArray[ l ].X == lVec4.GetAt( 0 ) &&
											NormalTempArray[ l ].Y == lVec4.GetAt( 1 ) &&
											NormalTempArray[ l ].Z == lVec4.GetAt( 2 ) )
										{
											Find = skyTrue;
											break;
										}
									}
									// �@���x�N�g�������S�Ɉ�v���钸�_���ēx���Z�������邱�Ƃ�����̂ŁA
									// �d�����ĉ��Z���Ȃ��悤�ɂ���
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

						// �P�ʃx�N�g���̌v�Z
						Vec3.Normalize();

						// �@���x�N�g�����Z�b�g
						spMaterial->GetNormalStream()->WriteNormal( ( vrtxtype )i , Vec3 );
					}
				}
				break;
			// ���_�P�ʂŒ��_��񂪃Z�b�g����Ă���
			case KFbxLayerElement::eBY_CONTROL_POINT:
				{
					if( leNormal->GetReferenceMode() == KFbxLayerElement::eDIRECT )
					{
						spMaterial->CreateNormalStream( ( vrtxtype )vertexCount ); //���_�������
						for( s32 i = 0; i < vertexCount ; i++ )
						{
							KFbxVector4 lVec4 = leNormal->GetDirectArray().GetAt(i);

							// �@���x�N�g�����Z�b�g
							spMaterial->GetNormalStream()->WriteNormal(
								( vrtxtype )i , vertex::CVertexFmt_Normal( ( f32 )lVec4.GetAt( 0 ) , ( f32 )lVec4.GetAt( 1 ) , ( f32 )lVec4.GetAt( 2 ) ) );
						}
					}
					// �T���v���ɂ����̏����͎�������Ă��Ȃ��B
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
			// �|���S���P�ʂŒ��_��񂪃Z�b�g����Ă���
			case KFbxLayerElement::eBY_POLYGON_VERTEX:

				switch( leUV->GetReferenceMode() )
				{
					// ���̃p�^�[���͊m�F�ł���
					case KFbxLayerElement::eDIRECT:
					SKY_PANIC_MSG( _T( "this pattern no support." ) );
					break;

					// �C���f�b�N�X�Q�ƂŖ@���x�N�g�����i�[����Ă���
					case KFbxLayerElement::eINDEX_TO_DIRECT:
					{
						// ���_�o�b�t�@�̃��[�v
						spMaterial->CreateUvStream( ( vrtxtype )vertexCount );
						for( s32 i = 0 ; i < vertexCount ; i++ )
						{
							u32 UVIndex = 0;
							// �|���S�����Ƃ̃��[�v
							for( s32 j = 0 ; j < polygonCount ; j++ )
							{
								// �|���S�������擾
								s32 lPolygonSize = pFBXMesh->GetPolygonSize(j);

								// �P�|���S�����̒��_���Ƃ̃��[�v
								for( s32 k = 0 ; k < lPolygonSize ; k++ )
								{
									// �C���f�b�N�X�������Ȃ̂ŏ����Ώ�
									if( i == pFBXMesh->GetPolygonVertex( j, k ) )
									{
										// �C���f�b�N�X�o�b�t�@����C���f�b�N�X���擾����
										int lUVIndex = leUV->GetIndexArray().GetAt( UVIndex );

										// �擾�����C���f�b�N�X���� UV ���擾����
										KFbxVector2 lVec2 = leUV->GetDirectArray().GetAt( lUVIndex );

										// UV�l�Z�b�g
										spMaterial->GetUvStream()->WriteUv( ( vrtxtype )i , math::CBasicVector2( lVec2.GetAt( 0 ) , lVec2.GetAt( 1 ) ) );
									}
									UVIndex++;
								}
							}
						}
					}
				}
	            break;

				// 3DS MAX �̏ꍇ�A�@���𒸓_���Ƃɕ������ďo�͂���Ƃ��������������
				case KFbxLayerElement::eBY_CONTROL_POINT:
					switch ( leUV->GetReferenceMode() )
					{
						// �C���f�b�N�X�o�b�t�@���g�p�����A���ږ@���x�N�g�����i�[����Ă���
						case KFbxLayerElement::eDIRECT:
							spMaterial->CreateUvStream( ( vrtxtype )vertexCount );
							for( vrtxtype i = 0 ; i < vertexCount ; i++ )
							{
								KFbxVector2 lVec2 = leUV->GetDirectArray().GetAt(i);

								// UV�l�Z�b�g
								spMaterial->GetUvStream()->WriteUv( i , math::CBasicVector2( lVec2.GetAt( 0 ) , lVec2.GetAt( 1 ) ) );
							}
						break;

						// �m�F�ł���
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

	//�f�[�^�x�[�X�Ɋi�[
	const char *name = pFBXMesh->GetName();
	if ( CStringUtil_::Len( name ) > 0 )
	{
		spMesh->Name.SetName( CHS( ( skyString * )pFBXMesh->GetName() ) );
	}
	CDataBase_::AddMeshContena( spMesh );
}

*/

} } }