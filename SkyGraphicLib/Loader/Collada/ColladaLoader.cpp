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

//name����������ΕԂ��A�Ȃ����id������Ԃ�
#define GET_TAG_NAME( pTag ) ( pTag->getName() == skyNull ) ? pTag->getId() : pTag->getName()
#define GET_TAG_ID( pTag )   ( pTag->getId() == skyNull ) ? pTag->getName() : pTag->getId()

namespace sky { namespace lib { namespace graphic {

//�e�N�X�`���̃��[�h
static void LoadTexture_( domCOLLADA *pRoot )
{
    // �e�N�X�`�����������[�v
	domLibrary_images_Array &images = pRoot->getLibrary_images_array();
	for( u32 i = 0 ; i < images.getCount() ; i++ )
	{
		//image�^�O�̎擾
		domImage_Array &imageArray = images.get( i )->getImage_array();
		for ( u32 j = 0 ; j < imageArray.getCount() ; j++ )
		{
			domImageRef pImage = imageArray.get( j );
   
			if( pImage != skyNull )
			{
				 // �t�@�C���p�X�̎擾
				skyMString pathBuffer[ PATH_LEN_MAX ];
				CTexturePtr spTexture = CTextureCreateName_( 
					CHS( CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , pImage->getInit_from()->getValue().getOriginalURI() ) ) );

				// �������擾
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
	//�T�[�t�F�[�X��T��
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

	//�C���[�W��T��
	//�A���Ńe�N�X�`���t�@�C���p�X��T��
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
			texturePath = dynamic_cast< domImage * >( image )->getInit_from()->getValue().getOriginalURI(); //�^�O�ɏ����ꂽ�܂܂̒l���Ƃ��

			CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , texturePath );

			//���������̂ŏI���
			break;
		}
	}

	//�p�X����CTexturePtr�������Ċ֘A�t����
	return CDataBase::GetTexture( CHS( pathBuffer ) );
}

//�}�e���A���̃��[�h
//�}�e���A�����g�p������x�͂̃p�X����肷��ɂ́A�}�e���A�����T���v���[���T�[�t�F�X���C���[�W���t�@�C���p�X�̏��ԂŒH��K�v������B
//�}�e���A���̑O�Ƀe�N�X�`�������[�h���Ă����B
static void LoadMaterial_( domCOLLADA *pRoot )
{
	domLibrary_materials_Array &materials = pRoot->getLibrary_materials_array();

    // material�̖��������[�v
	for( u32 i = 0 ; i < materials.getCount() ; i++ )
	{
		//material�^�O�̎擾
		domMaterial_Array &materialArray = materials.get( i )->getMaterial_array();

		for ( u32 j = 0 ; j < materialArray.getCount() ; j++ )
		{
			domMaterialRef &pMaterial = materialArray.get( j );

			//name
			CMaterialPtr spMaterial = CMaterialCreateName_( CHS( GET_TAG_ID( pMaterial ) ) );

			domInstance_effectRef einst = pMaterial->getInstance_effect();
			domEffectRef effect = daeSafeCast<domEffect>( pMaterial->getInstance_effect()->getUrl().getElement() );

			const domFx_profile_abstract_Array &profiles = effect->getFx_profile_abstract_array();
			SKY_ASSERT( profiles.getCount() == 1 ); //�P�ӊO�͑z��O
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
						//�A���r�G���g�F�̎擾
						domCommon_color_or_texture_typeRef ambient = rPhone->getAmbient();
						if( ambient && ambient->getColor() )
						{
							domFx_color_common &color = ambient->getColor()->getValue();
							spMaterial->Ambient.r = ( colortype )color[ 0 ];
							spMaterial->Ambient.g = ( colortype )color[ 1 ];
							spMaterial->Ambient.b = ( colortype )color[ 2 ];
							spMaterial->Ambient.a = ( colortype )color[ 3 ];
						}
						//�X�y�L�����F�̎擾
						domCommon_color_or_texture_typeRef specular = rPhone->getSpecular();
						if( specular && specular->getColor() )
						{
							domFx_color_common &color = specular->getColor()->getValue();
							spMaterial->Specular.r = ( colortype )color[ 0 ];
							spMaterial->Specular.g = ( colortype )color[ 1 ];
							spMaterial->Specular.b = ( colortype )color[ 2 ];
							spMaterial->Specular.a = ( colortype )color[ 3 ];
						}
						//�f�B�t���[�Y�F�̎擾
						domCommon_color_or_texture_typeRef diffuse = rPhone->getDiffuse();
						if( diffuse && diffuse->getColor() )
						{
							domFx_color_common &color = diffuse->getColor()->getValue();
							spMaterial->Diffuse.r = ( colortype )color[ 0 ];
							spMaterial->Diffuse.g = ( colortype )color[ 1 ];
							spMaterial->Diffuse.b = ( colortype )color[ 2 ];
							spMaterial->Diffuse.a = ( colortype )color[ 3 ];
						}
						//�G�~�b�V�����̎擾
						domCommon_color_or_texture_typeRef emission = rPhone->getEmission();
						if( emission && emission->getColor() )
						{
							domFx_color_common &color = emission->getColor()->getValue();
							spMaterial->Emissive.r = ( colortype )color[ 0 ];
							spMaterial->Emissive.g = ( colortype )color[ 1 ];
							spMaterial->Emissive.b = ( colortype )color[ 2 ];
							spMaterial->Emissive.a = ( colortype )color[ 3 ];
						}
						//���x
						domCommon_float_or_param_typeRef shininess = rPhone->getShininess();
						if(shininess && shininess->getFloat())
						{
							domFloat fval = shininess->getFloat()->getValue();
							spMaterial->Shininess = ( f32 )fval;
						}
						//���˓x
						domCommon_float_or_param_typeRef reflectivity = rPhone->getReflectivity();
						if(reflectivity && reflectivity->getFloat())
						{
							domFloat fval = reflectivity->getFloat()->getValue();
							spMaterial->Refractive = ( f32 )fval;
						}
						//�����x
						domCommon_float_or_param_typeRef transparency = rPhone->getTransparency();
						if(transparency && transparency->getFloat())
						{
							domFloat fval = transparency->getFloat()->getValue();
							spMaterial->Alpha = ( f32 )fval;
						}
	//----textrue
						//�f�B�t���[�Y�e�N�X�`���̎擾
						if( diffuse && diffuse->getTexture() )
						{
							spMaterial->AttachDiffuseMap( GetTexturePtr_( diffuse->getTexture()->getTexture() , pProfile ) );
						}
						//�A���r�G���g�e�N�X�`���̎擾
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

	//�p�X����CMaterialPtr�������Ċ֘A�t����

	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreate_() );
	spMesh->GetMaterialContena()->AddMaterial( CDataBase::GetMaterial( CHS( effectName.c_str() ) ) , 0 , 0 );
	return spMesh;
}

//FloatArray��C�ӂ̃o�b�t�@�ɃR�s�[����
static void CopyFloatArray_( const CVertexBufferStreamPtr &destBuffer , const domListOfFloats &srcArray , const domListOfUInts &indexArray , vrtxtype formatOffset , vrtxtype indexOffset , vrtxtype vertexFormatNum , vrtxtype posIndexOffset )
{
	vrtxtype posIndex = 0;
	vrtxtype index = 0;
	for( vrtxtype i = 0 ; i < indexArray.getCount() / vertexFormatNum ; i++ )
	{
		posIndex = ( vrtxtype )indexArray.get( i * vertexFormatNum + posIndexOffset );
		index    = ( vrtxtype )indexArray.get( i * vertexFormatNum + indexOffset );

		//�l���i�[
		for( vrtxtype j = 0 ; j < formatOffset ; j++ )
		{
			destBuffer->WriteCpuMemory( posIndex * formatOffset + j , ( f32 )srcArray.get( index * formatOffset + j ) );
		}
	}
}

//Triangele�̐�����Mesh�����
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

		//�e��Z�}���e�B�N�X�̃I�t�Z�b�g�𓾂�
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

		//�C���f�b�N�X�̎擾
		domListOfUInts &indexArray = pTriangles->getP()->getValue();

		//���_���W����������o�b�t�@�쐬
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
		//���W���x�Ƃy�����ւ���
		spVertex->GetPositionStream()->SwapPositionYAndZ();
#endif

		//�@�����W����������o�b�t�@���쐬
		if( normalOffset != SHRT_MAX )
		{
			const domListOfFloats& normalArray = pCurrentMesh->getSource_array()[ normalOffset ]->getFloat_array()->getValue();
			spVertex->CreateNormalStream( spVertex->GetVertexNum() );
			CopyFloatArray_( spVertex->GetNormalStream() , normalArray , indexArray , 3 , normalOffset , vertexFormatNum , posOffset );
		}

		//�e�N�X�`�����W����������o�b�t�@���쐬
		if( texcoordOffset != SHRT_MAX )
		{
			const domListOfFloats& textcoordArray = pCurrentMesh->getSource_array()[ texcoordOffset ]->getFloat_array()->getValue();
			spVertex->CreateUvStream( spVertex->GetVertexNum() );
			CopyFloatArray_( spVertex->GetUvStream() , textcoordArray , indexArray , 2 , texcoordOffset , vertexFormatNum , posOffset );
		}

		//�C���f�b�N�X�o�b�t�@�̍쐬�i���_�f�[�^�̃C���f�b�N�X���g�p����j
		spIndex->Initialize( ( vrtxtype )( indexArray.getCount() / vertexFormatNum ) );
		for( vrtxtype j = 0 ; j < spIndex->GetIndexNum() ; j++ )
		{
			spIndex->SetIndex( j , ( vrtxtype )indexArray.get( j * vertexFormatNum ) );
		}

		//�C���f�b�N�X�o�b�t�@���i�[
		spMesh->AttachIndexBuffer( spIndex );

		//���_�o�b�t�@���i�[
		spMesh->AttachVertexBuffer( spVertex );

		//�􉽌`������쐬
		spMeshContena->GetMesh()->GetMaterialContena()->SetDrawIndex( 0 , 0 , spIndex->GetIndexNum() );

		//���b�V�������f���Ɋ֘A�t����
		spModel->AttachMeshContena( spMeshContena );

		//���b�V�����f�[�^�x�[�X�ɓ����
		CDataBase_::AddMeshContena( spMeshContena );
	}
}

// �I�t�Z�b�g�̍ő�l��Ԃ�
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

// �O�p�`��(Polylist)
void ConvertPolylistToTriangles( domMesh *thisMesh )
{
	int polylistElemntCount = ( s32 )( thisMesh->getPolylist_array().getCount() );
	for(int currentPolylist=0;currentPolylist<polylistElemntCount;currentPolylist++)
	{
		domPolylist* thisPolylist = thisMesh->getPolylist_array().get(0);

		// 4�p�`��3�p�`��
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

// �O�p�`��(Polygons)
void ConvertPolygonsToTriangles( domMesh *thisMesh )
{
	int polygonsElementCount = ( int )( thisMesh->getPolygons_array().getCount() );
	for( int currentPolygons=0 ; currentPolygons< polygonsElementCount ; currentPolygons++ )
	{
		domPolygons* thisPolygons = thisMesh->getPolygons_array().get(0);

		// 3�p�`��
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
	//�W�I���g��������
	domInstance_geometry_Array &geometry_array = node->getInstance_geometry_array();
	for( u32 i = 0 ; i < geometry_array.getCount() ; i++ )
	{
		LoadGeometory_( node , geometry_array.get( i ) , spSceneGraph );
	}

	//�J����������
	domInstance_camera_Array &camera_array = node->getInstance_camera_array();
	for( u32 i = 0 ; i < camera_array.getCount() ; i++ )
	{
		LoadCamera_( camera_array.get( i ) );
	}

	//���C�g������
	domInstance_light_Array &light_array = node->getInstance_light_array();
	for( u32 i = 0 ; i < light_array.getCount() ; i++ )
	{
		LoadLight_( light_array.get( i ) );
	}
}

static void LoadGeometory_( const domNodeRef &node , const domInstance_geometryRef &geometory , const CSceneGraphNodePtr &spParentModel ) //�ċA
{
	domGeometryRef geometory_ = daeSafeCast< domGeometry >( geometory->getUrl().getElement() );

	//���f���쐬
	CModelPtr spModel = CModelCreateName_( CHS( node->getId() ) );

	//���b�V�������o��
	domMesh* pCurrentMesh = geometory_->getMesh();

	if( pCurrentMesh != NULL )
	{
		// Polylist�̏ꍇTriangleTag�ɕϊ�����
		ConvertPolylistToTriangles( pCurrentMesh );

		// Polygons�̏ꍇTriangleTag�ɕϊ�����
		ConvertPolygonsToTriangles( pCurrentMesh );

		// Triangles�̏ꍇ
		GetMeshContenaFromTriangles_( pCurrentMesh , spModel );
	}

	//�f�t�H���g�Ńe�N�X�`���}�b�s���O�̃V�F�[�_�[��t���Ă���
	spModel->AttachShader( CDirectionalLightShaderObjectCreate_() );

	//���f�����f�[�^�x�[�X�ɓ����
	CDataBase_::AddModel( spModel );

	//�����e��������e�q�֌W������
	if ( spParentModel.IsEmpty() == skyFalse )
	{
		spParentModel->AttachChild( spModel );
	}

	//�W���C���g�쐬
	spModel->SetJointUse();

	//�g�k�̏����l��ݒ�
	spModel->SetScale_X( ( dectype )node->getScale_array().get( 0 )->getValue().get( 0 ) );
	spModel->SetScale_Y( ( dectype )node->getScale_array().get( 0 )->getValue().get( 1 ) );
	spModel->SetScale_Z( ( dectype )node->getScale_array().get( 0 )->getValue().get( 2 ) );

	u32 sort_array[ 3 ] = { 2 , 1 , 0 };

	//��]
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
				SKY_PANIC(); //�����͂��肦�Ȃ�
			}
		}
    }

	//�ʒu�̏����l��ݒ�
 	spModel->SetPosition_X( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 0 ) );
	spModel->SetPosition_Y( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 1 ) );
	spModel->SetPosition_Z( ( dectype )node->getTranslate_array().get( 0 )->getValue().get( 2 ) );

	//�q��{��
	domNode_Array &children = node->getNode_array();
	for( u32 i = 0 ; i < children.getCount() ; i++ )
	{
		LoadNode_( children.get( i ) , spModel );
	}
}

static void LoadCamera_( domInstance_cameraRef &geometory ) //�ċA
{

}

static void LoadLight_( domInstance_lightRef &geometory ) //�ċA
{

}

static void LoadScenes_( domCOLLADA *pRoot )
{
	domLibrary_visual_scenes_Array &scenes = pRoot->getLibrary_visual_scenes_array();

	//�V�[���̐���������
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
	//�f�B���N�g���p�X�w�肪�������ꍇpath�ƍ�������
	skyString pathBuffer[ PATH_LEN_MAX ];
	CGraphicFilePathManager::ConcatColladaFilePathRoot( pathBuffer , PATH_LEN_MAX , path->GetString() );

	//wchar��char�ɕϊ�
	skyMString *cPath = CStringUtil_::CreateMString( pathBuffer );

	//���[�_�[���쐬
	DAE* pDae = NEW DAE;

	//���[�h
	s32 result = pDae->load( cPath );

	//���[�h���ʊm�F
	if( result != DAE_OK )
	{
		SKY_PANIC_MSG( "this path is invalid[ %s ] \n" , cPath );
	}

	//���[�g���擾
	domCOLLADA *pRoot = pDae->getRoot( cPath );

	//�e�N�X�`���v�f�̓ǂݍ��݁i�}�e���A���̑O�ɂ��Ȃ炸���[�h���Ă����j
	LoadTexture_( pRoot );

	//�}�e���A���v�f�̓ǂݍ���
	LoadMaterial_( pRoot );

	//�V�[���̓ǂݍ���
	LoadScenes_( pRoot );

	pDae->cleanup();
	DAE::cleanup();

	//���[�_�[��j��
	SAFE_DELETE( pDae );

	//char�̃p�X������
	CStringUtil_::DeleteMString( cPath );
}

} } }
