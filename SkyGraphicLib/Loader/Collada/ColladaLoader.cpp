#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"

#define NO_BOOST

#define BLENDER_TO_DIREXTX

#include <dae.h>
#include <dae/daeTinyXMLPlugin.h>
#include <1.4/dom/domCOLLADA.h>
#include <1.4/dom/domProfile_COMMON.h>
#include <1.4/dom/domFx_profile_abstract.h>

//name属性があれば返し、なければid属性を返す
#define GET_TAG_NAME( pTag ) ( pTag->getName() == skyNull ) ? pTag->getId() : pTag->getName()
#define GET_TAG_ID( pTag )   ( pTag->getId() == skyNull ) ? pTag->getName() : pTag->getId()

namespace sky { namespace lib { namespace graphic {

//テクスチャのロード
static void LoadTexture_( domCOLLADA *pRoot )
{
    // テクスチャ枚数分ループ
	domLibrary_images_Array &images = pRoot->getLibrary_images_array();
	for( u32 i = 0 ; i < images.getCount() ; i++ )
	{
		//imageタグの取得
		domImage_Array &imageArray = images.get( i )->getImage_array();
		for ( u32 j = 0 ; j < imageArray.getCount() ; j++ )
		{
			domImageRef pImage = imageArray.get( j );
   
			if( pImage != skyNull )
			{
				 // ファイルパスの取得
				skyMString pathBuffer[ PATH_LEN_MAX ];
				CTexturePtr spTexture = CTextureCreateName_( 
					CHS( CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , pImage->getInit_from()->getValue().getOriginalURI() ) ) );

				// 属性を取得
				if ( pImage->getAttribute( "id" ).c_str() != skyNull )
				{
				}
				if ( pImage->getAttribute( "format" ).c_str() != skyNull )
				{
				}
				if ( pImage->getAttribute( "height" ).c_str() != skyNull )
				{
					spTexture->GetInfo().Height = ( texture::eTexture_height )pImage->getHeight();
				}
				if ( pImage->getAttribute( "width" ).c_str() != skyNull )
				{
					spTexture->GetInfo().Width = ( texture::eTexture_with )pImage->getWidth();
				}
				if ( pImage->getAttribute( "depth" ).c_str() != skyNull )
				{
				}
				if ( pImage->getAttribute( "name" ).c_str() != skyNull )
				{
				}

				CDataBase_::AddTexture( spTexture );
			 }
		}
    }
}

static CTexturePtr GetTexturePtr_( const std::string &samplerName , domProfile_COMMON* pProfile )
{
	//サーフェースを探す
	std::string surfaceName;
	domCommon_newparam_type_Array& newParamArray = pProfile->getNewparam_array();
	for( u32 i = 0 ; i < newParamArray.getCount(); i++ )
	{
		if ( newParamArray[ i ]->getSampler2D() != skyNull && samplerName == newParamArray[ i ]->getSid() )
		{
			surfaceName = newParamArray[ i ]->getSampler2D()->getSource()->getValue();
			break;
		}
	}

	//イメージを探す
	//連続でテクスチャファイルパスを探す
	const skyMString* texturePath;
	skyMString pathBuffer[ PATH_LEN_MAX ];
	for( u32 i = 0 ; i < newParamArray.getCount(); i++ )
	{
		if ( newParamArray[ i ]->getSurface() != skyNull && surfaceName == newParamArray[ i ]->getSid() )
		{
			SKY_ASSERT( newParamArray[ i ]->getSurface()->getType() == FX_SURFACE_TYPE_ENUM_2D ) ;
			SKY_ASSERT( newParamArray[ i ]->getSurface()->getFx_surface_init_common()->getInit_from_array().getCount() == 1 ) ;

			daeElement* image = newParamArray[ i ]->getSurface()->getFx_surface_init_common()->getInit_from_array()[ 0 ]->getValue().getElement();
			SKY_ASSERT( image->getElementType() == COLLADA_TYPE::IMAGE );
			texturePath = dynamic_cast< domImage * >( image )->getInit_from()->getValue().getOriginalURI(); //タグに書かれたままの値がとれる

			CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , texturePath );

			//見つかったので終わり
			break;
		}
	}

	//パスからCTexturePtrを見つけて関連付ける
	return CDataBase::GetTexture( CHS( pathBuffer ) );
}

//マテリアルのロード
//マテリアルが使用する手空xはのパスを特定するには、マテリアル→サンプラー→サーフェス→イメージ→ファイルパスの順番で辿る必要がある。
//マテリアルの前にテクスチャをロードしておく。
static void LoadMaterial_( domCOLLADA *pRoot )
{
	domLibrary_materials_Array &materials = pRoot->getLibrary_materials_array();

    // materialの枚数分ループ
	for( u32 i = 0 ; i < materials.getCount() ; i++ )
	{
		//materialタグの取得
		domMaterial_Array &materialArray = materials.get( i )->getMaterial_array();

		for ( u32 j = 0 ; j < materialArray.getCount() ; j++ )
		{
			domMaterialRef &pMaterial = materialArray.get( j );

			//name
			CMaterialPtr spMaterial = CMaterialCreateName_( CHS( GET_TAG_ID( pMaterial ) ) );

			domInstance_effectRef einst = pMaterial->getInstance_effect();
			domEffectRef effect = daeSafeCast<domEffect>( pMaterial->getInstance_effect()->getUrl().getElement() );

			const domFx_profile_abstract_Array &profiles = effect->getFx_profile_abstract_array();
			SKY_ASSERT( profiles.getCount() == 1 ); //１個意外は想定外
			for ( size_t j = 0 ; j < profiles.getCount() ; ++j )
			{
				domProfile_COMMON *pProfile = daeSafeCast<domProfile_COMMON>( profiles[ j ] );

				if( pProfile != skyNull )
				{
					const domProfile_COMMON::domTechniqueRef rTech            = pProfile->getTechnique();
					const domProfile_COMMON::domTechnique::domPhongRef rPhone = rTech->getPhong();

					if ( rPhone != skyNull )
					{
	//----param
						//アンビエント色の取得
						domCommon_color_or_texture_typeRef ambient = rPhone->getAmbient();
						if( ambient && ambient->getColor() )
						{
							domFx_color_common &color = ambient->getColor()->getValue();
							spMaterial->Ambient.r = ( colortype )color[ 0 ];
							spMaterial->Ambient.g = ( colortype )color[ 1 ];
							spMaterial->Ambient.b = ( colortype )color[ 2 ];
							spMaterial->Ambient.a = ( colortype )color[ 3 ];
						}
						//スペキュラ色の取得
						domCommon_color_or_texture_typeRef specular = rPhone->getSpecular();
						if( specular && specular->getColor() )
						{
							domFx_color_common &color = specular->getColor()->getValue();
							spMaterial->Specular.r = ( colortype )color[ 0 ];
							spMaterial->Specular.g = ( colortype )color[ 1 ];
							spMaterial->Specular.b = ( colortype )color[ 2 ];
							spMaterial->Specular.a = ( colortype )color[ 3 ];
						}
						//ディフューズ色の取得
						domCommon_color_or_texture_typeRef diffuse = rPhone->getDiffuse();
						if( diffuse && diffuse->getColor() )
						{
							domFx_color_common &color = diffuse->getColor()->getValue();
							spMaterial->Diffuse.r = ( colortype )color[ 0 ];
							spMaterial->Diffuse.g = ( colortype )color[ 1 ];
							spMaterial->Diffuse.b = ( colortype )color[ 2 ];
							spMaterial->Diffuse.a = ( colortype )color[ 3 ];
						}
						//エミッションの取得
						domCommon_color_or_texture_typeRef emission = rPhone->getEmission();
						if( emission && emission->getColor() )
						{
							domFx_color_common &color = emission->getColor()->getValue();
							spMaterial->Emissive.r = ( colortype )color[ 0 ];
							spMaterial->Emissive.g = ( colortype )color[ 1 ];
							spMaterial->Emissive.b = ( colortype )color[ 2 ];
							spMaterial->Emissive.a = ( colortype )color[ 3 ];
						}
						//光度
						domCommon_float_or_param_typeRef shininess = rPhone->getShininess();
						if(shininess && shininess->getFloat())
						{
							domFloat fval = shininess->getFloat()->getValue();
							spMaterial->Shininess = ( f32 )fval;
						}
						//反射度
						domCommon_float_or_param_typeRef reflectivity = rPhone->getReflectivity();
						if(reflectivity && reflectivity->getFloat())
						{
							domFloat fval = reflectivity->getFloat()->getValue();
							spMaterial->Refractive = ( f32 )fval;
						}
						//透明度
						domCommon_float_or_param_typeRef transparency = rPhone->getTransparency();
						if(transparency && transparency->getFloat())
						{
							domFloat fval = transparency->getFloat()->getValue();
							spMaterial->Alpha = ( f32 )fval;
						}
	//----textrue
						//ディフューズテクスチャの取得
						if( diffuse && diffuse->getTexture() )
						{
							spMaterial->AttachDiffuseMap( GetTexturePtr_( diffuse->getTexture()->getTexture() , pProfile ) );
						}
						//アンビエントテクスチャの取得
						if( ambient && ambient->getTexture() )
						{
							spMaterial->AttachDiffuseMap( GetTexturePtr_( ambient->getTexture()->getTexture() , pProfile ) );
						}
					}
				}
			}
	        CDataBase_::AddMaterial( spMaterial );
		}
	}
}

static CMeshPtr GetTriangles_( const std::string &materialName , domTriangles* pTriangles )
{
	s32 materialCount = static_cast<int>( pTriangles->getDAE()->getDatabase()->getElementCount(NULL, "instance_material", NULL) );
	std::string effectName;
    for( s32 i = 0 ; i < materialCount ; ++i )
	{
		domInstance_material *pMaterial = NULL;
		pTriangles->getDAE()->getDatabase()->getElement((daeElement**)&pMaterial, i , NULL,  "instance_material" );
		if ( materialName == pMaterial->getSymbol() )
		{
			effectName = pMaterial->getTarget().fragment();
			break;
		}
	}

	//パスからCMaterialPtrを見つけて関連付ける

	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreate_() );
	spMesh->GetMaterialContena()->AddMaterial( CDataBase::GetMaterial( CHS( effectName.c_str() ) ) , 0 , 0 );
	return spMesh;
}

//FloatArrayを任意のバッファにコピーする
static void CopyFloatArray_( const CVertexBufferStreamPtr &destBuffer , const domListOfFloats &srcArray , const domListOfUInts &indexArray , vrtxtype formatOffset , vrtxtype indexOffset , vrtxtype vertexFormatNum , vrtxtype posIndexOffset )
{
	vrtxtype posIndex = 0;
	vrtxtype index = 0;
	for( vrtxtype i = 0 ; i < indexArray.getCount() / vertexFormatNum ; i++ )
	{
		posIndex = ( vrtxtype )indexArray.get( i * vertexFormatNum + posIndexOffset );
		index    = ( vrtxtype )indexArray.get( i * vertexFormatNum + indexOffset );

		//値を格納
		for( vrtxtype j = 0 ; j < formatOffset ; j++ )
		{
			destBuffer->WriteCpuMemory( posIndex * formatOffset + j , ( f32 )srcArray.get( index * formatOffset + j ) );
		}
	}
}

//Triangeleの数だけMeshを作る
static void GetMeshContenaFromTriangles_( domMesh* pCurrentMesh , const CModelPtr &spModel )
{
	domTriangles_Array& pArray = pCurrentMesh->getTriangles_array();

	s32 triangleCount = static_cast< s32 >( pArray.getCount() );

	for( s32 i = 0; i < triangleCount; ++i)
	{
		CMeshContenaPtr  spMeshContena   = CMeshContenaCreate_();
		CVertexBufferPtr spVertex = CVertexBuffer::Create();
		CIndexBufferPtr  spIndex  = CIndexBufferCreate_();

		domTriangles* pTriangles = pArray.get( i );

		vrtxtype posOffset = SHRT_MAX , texcoordOffset = SHRT_MAX , normalOffset = SHRT_MAX;

		CMeshPtr spMesh = GetTriangles_( pTriangles->getMaterial() , pTriangles );
		spMeshContena->AttachMesh( spMesh );

		//各種セマンティクスのオフセットを得る
		vrtxtype vertexFormatNum = ( vrtxtype )pTriangles->getInput_array().getCount();
		for( u32 j = 0 ; j < vertexFormatNum ; j++ )
		{
			if( strcmpi( pTriangles->getInput_array()[ j ]->getSemantic() , "VERTEX" ) ==0 )
			{
				posOffset = ( vrtxtype )pTriangles->getInput_array()[ j ]->getOffset();
			}

			if( strcmpi( pTriangles->getInput_array() [ j ]->getSemantic() , "NORMAL" ) == 0 )
			{
				normalOffset = ( vrtxtype )pTriangles->getInput_array()[ j ]->getOffset();
			}

			if( strcmpi( pTriangles->getInput_array()[ j ]->getSemantic() , "TEXCOORD" ) ==0 )
			{
				texcoordOffset  = ( vrtxtype )pTriangles->getInput_array()[ j ]->getOffset();
			}
		}

		//インデックスの取得
		domListOfUInts &indexArray = pTriangles->getP()->getValue();

		//頂点座標があったらバッファ作成
		if ( posOffset != SHRT_MAX )
		{
			const domListOfFloats& vertexArray = pCurrentMesh->getSource_array()[ posOffset ]->getFloat_array()->getValue();
			spVertex->Initialize( ( vrtxtype )vertexArray.getCount() / 3 );
			CopyFloatArray_( spVertex->GetStream( eVertexBufferStreamType_Position ) , vertexArray , indexArray , 3 , posOffset , vertexFormatNum , posOffset );
		}
		else
		{
			SKY_PANIC();
		}

#ifdef BLENDER_TO_DIREXTX
		//座標をＹとＺを入れ替える
		spVertex->GetPositionStream()->SwapPositionYAndZ();
#endif

		//法線座標があったらバッファを作成
		if( normalOffset != SHRT_MAX )
		{
			const domListOfFloats& normalArray = pCurrentMesh->getSource_array()[ normalOffset ]->getFloat_array()->getValue();
			spVertex->CreateNormalStream( spVertex->GetVertexNum() );
			CopyFloatArray_( spVertex->GetNormalStream() , normalArray , indexArray , 3 , normalOffset , vertexFormatNum , posOffset );
		}

		//テクスチャ座標があったらバッファを作成
		if( texcoordOffset != SHRT_MAX )
		{
			const domListOfFloats& textcoordArray = pCurrentMesh->getSource_array()[ texcoordOffset ]->getFloat_array()->getValue();
			spVertex->CreateUvStream( spVertex->GetVertexNum() );
			CopyFloatArray_( spVertex->GetUvStream() , textcoordArray , indexArray , 2 , texcoordOffset , vertexFormatNum , posOffset );
		}

		//インデックスバッファの作成（頂点データのインデックスを使用する）
		spIndex->Initialize( ( vrtxtype )( indexArray.getCount() / vertexFormatNum ) );
		for( vrtxtype j = 0 ; j < spIndex->GetIndexNum() ; j++ )
		{
			spIndex->SetIndex( j , ( vrtxtype )indexArray.get( j * vertexFormatNum ) );
		}

		//インデックスバッファを格納
		spMesh->AttachIndexBuffer( spIndex );

		//頂点バッファを格納
		spMesh->AttachVertexBuffer( spVertex );

		//幾何形状情報を作成
		spMeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spIndex->GetIndexNum() );

		//メッシュをモデルに関連付ける
		spModel->AttachMeshContena( spMeshContena );

		//メッシュをデータベースに入れる
		CDataBase_::AddMeshContena( spMeshContena );
	}
}

// オフセットの最大値を返す
u32 getMaxOffset( domInputLocalOffset_Array &input_array ) 
{
	u32 maxOffset = 0;
	for ( u32 i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (u32)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

// 三角形化(Polylist)
void ConvertPolylistToTriangles( domMesh *thisMesh )
{
	int polylistElemntCount = ( s32 )( thisMesh->getPolylist_array().getCount() );
	for(int currentPolylist=0;currentPolylist<polylistElemntCount;currentPolylist++)
	{
		domPolylist* thisPolylist = thisMesh->getPolylist_array().get(0);

		// 4角形を3角形化
		domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
		u32 triangles = 0;
		thisTriangles->setMaterial(thisPolylist->getMaterial());
		domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

		for( s32 i = 0 ; i < ( s32 )( thisPolylist->getInput_array().getCount() ) ; i++ )
		{
			thisTriangles->placeElement( thisPolylist->getInput_array()[ i ]->clone() );
		}

		// Get the number of inputs and primitives for the polygons array
		int numberOfInputs = ( s32 )getMaxOffset(thisPolylist->getInput_array()) + 1;
		int numberOfPrimitives = ( s32 )(thisPolylist->getVcount()->getValue().getCount());

		u32 offset = 0;

		for( s32 j = 0 ; j < numberOfPrimitives ; j++ )
		{	
			s32 triangleCount = ( s32 )thisPolylist->getVcount()->getValue()[ j ] - 2;
			// Write out the primitives as triangles, just fan using the first element as the base
			s32 idx = numberOfInputs;
			for( s32 k = 0 ; k < triangleCount ; k++ )
			{
				// First vertex
				for( s32 l = 0 ; l < numberOfInputs; l++ )
				{
					s32 a = ( s32 )thisPolylist->getP()->getValue()[ offset + l ];
					f32 b;
					memcpy( &b , &a , sizeof( f32 ) );
					p_triangles->getValue().append( thisPolylist->getP()->getValue()[ offset + l ] );
				}

				// Second vertex
				for( s32 l = 0 ; l < numberOfInputs ; l++ )
				{
					p_triangles->getValue().append( thisPolylist->getP()->getValue()[ offset + idx + l ] );
				}

				// Third vertex
				idx += numberOfInputs;
				for( s32 l = 0 ; l < numberOfInputs ; l++ )
				{
					p_triangles->getValue().append( thisPolylist->getP()->getValue()[ offset + idx + l ] );
				}
				triangles++;
			}
			offset += ( u32 )thisPolylist->getVcount()->getValue()[ j ] * numberOfInputs;
		}

		thisTriangles->setCount( triangles );
	}
}

// 三角形化(Polygons)
void ConvertPolygonsToTriangles( domMesh *thisMesh )
{
	int polygonsElementCount = ( int )( thisMesh->getPolygons_array().getCount() );
	for( int currentPolygons=0 ; currentPolygons< polygonsElementCount ; currentPolygons++ )
	{
		domPolygons* thisPolygons = thisMesh->getPolygons_array().get(0);

		// 3角形化
		domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
		thisTriangles->setCount( 0 );
		thisTriangles->setMaterial(thisPolygons->getMaterial());
		domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

		// Give the new <triangles> the same <input> and <parameters> as the old <polygons>
		for( s32 i = 0 ; i < ( s32 )( thisPolygons->getInput_array().getCount() ); i++ )
		{
			thisTriangles->placeElement( thisPolygons->getInput_array()[i]->clone() );
		}
		// Get the number of inputs and primitives for the polygons array
		int numberOfInputs = (int)getMaxOffset(thisPolygons->getInput_array()) +1;
		int numberOfPrimitives = (int)(thisPolygons->getP_array().getCount());

		// Triangulate all the primitives, this generates all the triangles in a single <p> element
		for( s32 j = 0 ; j < numberOfPrimitives ; j++)
		{
			// Check the polygons for consistancy (some exported files have had the wrong number of indices)
			domP * thisPrimitive = thisPolygons->getP_array()[j];
			int elementCount = (int)(thisPrimitive->getValue().getCount());
			if((elementCount%numberOfInputs) != 0)
			{
				continue;
			}
			else
			{
				int triangleCount = (elementCount/numberOfInputs)-2;
				// Write out the primitives as triangles, just fan using the first element as the base
				int idx = numberOfInputs;
				for(int k = 0; k < triangleCount; k++)
				{
					// First vertex
					for(int l = 0; l < numberOfInputs; l++)
					{
						p_triangles->getValue().append(thisPrimitive->getValue()[l]);
					}
					// Second vertex
					for(int l = 0; l < numberOfInputs; l++)
					{
						p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
					}
					// Third vertex
					idx += numberOfInputs;
					for(int l = 0; l < numberOfInputs; l++)
					{
						p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
					}
					thisTriangles->setCount(thisTriangles->getCount()+1);
				}
			}
		}
	}
}

static void LoadGeometory_( const domNodeRef &node , const domInstance_geometryRef &geometory , const CSceneGraphNodePtr &spParentModel );
static void LoadCamera_( domInstance_cameraRef &geometory );
static void LoadLight_( domInstance_lightRef &geometory );

static void LoadNode_( const domNodeRef &node , const CSceneGraphNodePtr &spSceneGraph )
{
	//ジオメトリだった
	domInstance_geometry_Array &geometry_array = node->getInstance_geometry_array();
	for( u32 i = 0 ; i < geometry_array.getCount() ; i++ )
	{
		LoadGeometory_( node , geometry_array.get( i ) , spSceneGraph );
	}

	//カメラだった
	domInstance_camera_Array &camera_array = node->getInstance_camera_array();
	for( u32 i = 0 ; i < camera_array.getCount() ; i++ )
	{
		LoadCamera_( camera_array.get( i ) );
	}

	//ライトだった
	domInstance_light_Array &light_array = node->getInstance_light_array();
	for( u32 i = 0 ; i < light_array.getCount() ; i++ )
	{
		LoadLight_( light_array.get( i ) );
	}
}

static void LoadGeometory_( const domNodeRef &node , const domInstance_geometryRef &geometory , const CSceneGraphNodePtr &spParentModel ) //再帰
{
	domGeometryRef geometory_ = daeSafeCast< domGeometry >( geometory->getUrl().getElement() );

	//モデル作成
	CModelPtr spModel = CModelCreateName_( CHS( node->getId() ) );

	//メッシュを取り出す
	domMesh* pCurrentMesh = geometory_->getMesh();

	if( pCurrentMesh != NULL )
	{
		// Polylistの場合TriangleTagに変換する
		ConvertPolylistToTriangles( pCurrentMesh );

		// Polygonsの場合TriangleTagに変換する
		ConvertPolygonsToTriangles( pCurrentMesh );

		// Trianglesの場合
		GetMeshContenaFromTriangles_( pCurrentMesh , spModel );
	}

	//デフォルトでテクスチャマッピングのシェーダーを付けておく
	spModel->AttachShader( CDirectionalLightShaderObjectCreate_() );

	//モデルをデータベースに入れる
	CDataBase_::AddModel( spModel );

	//もし親が居たら親子関係を結ぶ
	if ( spParentModel.IsEmpty() == skyFalse )
	{
		spParentModel->AttachChild( spModel );
	}

	//ジョイント作成
	spModel->SetJointUse();

	//拡縮の初期値を設定
	spModel->SetScale_X( ( dectype )node->getScale_array().get( 0 )->getValue().get( 0 ) );
	spModel->SetScale_Y( ( dectype )node->getScale_array().get( 0 )->getValue().get( 1 ) );
	spModel->SetScale_Z( ( dectype )node->getScale_array().get( 0 )->getValue().get( 2 ) );

	u32 sort_array[ 3 ] = { 2 , 1 , 0 };

	//回転
    for ( u32 i = 0 ; i< node->getRotate_array().getCount() ; i++ ) 
    {
        daeSmartRef<domRotate> rot = node->getRotate_array().get( sort_array[ i ] );
        domFloat4& r = rot->getValue();

		if ( r[3] != 0.0f )
		{
			math::CBasicVector3 axis;

			axis.Set( ( dectype )r[ 0 ] , ( dectype )r[ 1 ] , ( dectype )r[ 2 ] );

			if ( axis == math::CBasicVector3( 1 , 0 , 0 ) )
			{
				spModel->MulRotation_X( math::CMathUtil::DegToRad( ( dectype )r[ 3 ] ) );
			}
			else if ( axis == math::CBasicVector3( 0 , 1 , 0 ) )
			{
				spModel->MulRotation_Y( math::CMathUtil::DegToRad( ( dectype )r[ 3 ] ) );
			}
			else if ( axis == math::CBasicVector3( 0 , 0 , 1 ) )
			{
				spModel->MulRotation_Z( math::CMathUtil::DegToRad( ( dectype )r[ 3 ] ) );
			}
			else
			{
				SKY_PANIC(); //ここはありえない
			}
		}
    }

	//位置の初期値を設定
 	spModel->SetPosition_X( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 0 ) );
	spModel->SetPosition_Y( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 1 ) );
	spModel->SetPosition_Z( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 2 ) );

	//子を捜す
	domNode_Array &children = node->getNode_array();
	for( u32 i = 0 ; i < children.getCount() ; i++ )
	{
		LoadNode_( children.get( i ) , spModel );
	}
}

static void LoadCamera_( domInstance_cameraRef &geometory ) //再帰
{

}

static void LoadLight_( domInstance_lightRef &geometory ) //再帰
{

}

static void LoadScenes_( domCOLLADA *pRoot )
{
	domLibrary_visual_scenes_Array &scenes = pRoot->getLibrary_visual_scenes_array();

	//シーンの数だけ処理
	for ( u32 i = 0 ; i < scenes.getCount() ; i++ )
	{
		domLibrary_visual_scenesRef pCurrentScenes = scenes.get( i );
		domVisual_scene_Array &visualArray = pCurrentScenes->getVisual_scene_array();
		for( u32 j = 0 ; j < visualArray.getCount() ; j++ )
		{
			domNode_Array &nodeArray = visualArray[ j ]->getNode_array();

			for( u32 k = 0 ; k < nodeArray.getCount() ; k++ )
			{
				LoadNode_( nodeArray[ k ] , CSceneGraphNodePtr() );
			}
		}
	}
}

void ColladaLoader::LoadFile( const hash::CHashStringPtr &path )
{
	//ディレクトリパス指定があった場合pathと合成する
	skyString pathBuffer[ PATH_LEN_MAX ];
	CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , path->GetString() );

	//wcharをcharに変換
	skyMString *cPath = CStringUtil_::CreateMString( pathBuffer );

	//ローダーを作成
	DAE* pDae = NEW DAE;

	//ロード
	s32 result = pDae->load( cPath );

	//ロード結果確認
	if( result != DAE_OK )
	{
		SKY_PANIC_MSG( "this path is invalid[ %s ] \n" , cPath );
	}

	//ルートを取得
	domCOLLADA *pRoot = pDae->getRoot( cPath );

	//テクスチャ要素の読み込み（マテリアルの前にかならずロードしておく）
	LoadTexture_( pRoot );

	//マテリアル要素の読み込み
	LoadMaterial_( pRoot );

	//シーンの読み込み
	LoadScenes_( pRoot );

	pDae->cleanup();
	DAE::cleanup();

	//ローダーを破棄
	SAFE_DELETE( pDae );

	//charのパスを消す
	CStringUtil_::DeleteMString( cPath );
}

} } }
