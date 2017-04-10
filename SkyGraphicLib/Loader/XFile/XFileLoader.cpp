#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyBaseLib/Token.h"
#include "SkyGraphicLib/DataBase.h"

namespace sky { namespace lib { namespace graphic {

char *s_TagTable[] = 
{
	"template" ,
	"VertexDuplicationIndices" ,
	"XSkinMeshHeader" ,
	"Frame" ,
	"Mesh" ,
	"MeshMaterialList" ,
	"MeshNormals" ,
	"MeshTextureCoords" ,
	"MeshVertexColors" ,
	"SkinWeights" ,
	"AnimationSet" ,
	"Animation" ,
};

#define TAG_TBL_NUM ( array_sizeof( s_TagTable ) )

struct CFace_
{
    vrtxtype m_MaterialIndex;
    vrtxtype m_StartIndex;
    vrtxtype m_EndIndex;
};
typedef CVector< CFace_ > CFaceVector_;

struct CMaterial_
{
	CMaterialPtr m_spMaterial;
};
typedef CVector< CMaterial_ > CMaterialVector_;

struct CWeight_
{
    CName Name;
	CVectorPrimitive< f32 >      m_Weights;
	CVectorPrimitive< vrtxtype > m_Indices;
	math::CMatrix4x3             m_Matrix;

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CWeight_& operator =( const CWeight_& src )
	{

		Name             = src.Name;
		m_Matrix         = src.m_Matrix;

		//Weights�̃R�s�[
		m_Weights.Clear();
		for( u32 i = 0 ; i < src.m_Weights.GetNum() ; i++ )
		{
			m_Weights.Add( src.m_Weights[ i ] );
		}

		//Indices�̃R�s�[
		m_Indices.Clear();
		for( u32 i = 0 ; i < src.m_Indices.GetNum() ; i++ )
		{
			m_Indices.Add( src.m_Indices[ i ] );
		}

		return *this;
	}
};

struct CNormal_
{
	dectype x;
	dectype y;
	dectype z;
};
typedef CVector< CNormal_ > CNormalVector_;

struct CFrame_;
typedef CList< u32 , CFrame_ * > CFramePtrList;
typedef CStack< CFrame_ * >      CFramePtrStack;
struct CFrame_
{
	//�c���[�\��
	CFrame_*                m_ParentFrame;
	CFramePtrList           m_ChildFrame;

	//�R�A�f�[�^
	hash::CHashStringPtr    m_Name;
	math::CMatrix4x3        m_Matrix;

	//Mesh��}�e���A����Model�Ƃ��ĕۑ�
	CMeshPtr                m_spMesh;

	//�ŏI�I�ɍ����\�z�ɂ��g�p�����
	CVector< CWeight_ >     m_WeightData; 

	//���ݎg���Ă���܂��񂪈ꉞ�ǂ�ł܂��B
	CVectorPrimitive< u16 > m_FaceData;
	CVector< CFace_ > m_MeshSubSetData; 
};

//�A�j���[�V�����֘A
struct CAnimationKey_
{
	u32 m_Time;
	s32 m_Type;

	union
	{
		f32 m_Rot[ 4 ];
		f32 m_Sc[ 3 ];
		f32 m_Trans[ 3 ];
		f32 m_Matrix[ 16 ];
	};
	CAnimationKey_()
	{
		m_Type = -1;
		m_Time = 0;
	}
	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationKey_& operator =( const CAnimationKey_& src )
	{
		m_Time        = src.m_Time;
		m_Type        = src.m_Type;

		for( u32 i = 0 ; i < 16 ; i++ )
		{
			m_Matrix[ i ] = src.m_Matrix[ i ];
		}

		return *this;
	}
};
typedef CVector< CAnimationKey_ >                 CAnimationKeyFrameArray_;
typedef CList< u32 , CAnimationKeyFrameArray_ * > CAnimationKeyFrameArrayPtrList_;

//���[�h
struct CLoadCacheData_
{
	//X�t�@�C�����g�[�N���P�ʂœǂݍ���
	token::CTokenizerM               m_Token;

	//���f��
	CModelPtr                        m_spModel;

	//�X�P���g��
	math::CSkeletonPtr               m_spSkeleton;

	//�t���[�������[�h���ɐς�ł���
	CFrame_*                         m_RootFrame;
	math::CJointPtr                  m_spPoseJointRoot;
	CFramePtrStack                   m_FrameStack;
	CFramePtrList                    m_FrameList; //��ň�C�ɏ������߂̃��X�g

	//���݃��[�h���̃��b�V�����
	CMeshPtr                         m_CurrentMesh;

	//���݃��[�h���A�j���[�V�������i������m_spMotionDataContena�Ɋi�[�j
	f32                              m_CurrentAnimationMaxTime;
	math::CAnimationDataPallet             m_CurrentAnimationPallet;

	//���[�h�����A�j���[�V�������Q
	math::CMotionDataContenaPtr m_spMotionDataContena;

	//�ǂݎ̂ď��
	CVectorPrimitive< u16 >          m_FaceData;
	CVector< CFace_ >          m_MeshSubSetData; 

	CLoadCacheData_() : m_Token( 64 , 64 , 64 ) , m_FrameStack( 128 ) , m_RootFrame( skyNull ){}
};

//�v���g�^�C�v
static void CheckNode( CLoadCacheData_ &cache );

//�S������ɂ��Ȃ��m�[�h
static void SkipNode( CLoadCacheData_ &cache )
{
	// '{'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
//      SKY_PRINTF( "Skip Token : %s", cache.m_Token.GetChar() );
		if ( strchr( cache.m_Token.GetChar(), '{' ) )
			break;
	}

	// �u���b�N��
	int count = 1;

	//�@�I�[�܂��̓u���b�N�����I���܂Ń��[�v
	while( !cache.m_Token.IsEnd() && count > 0 )
	{
		//�@�g�[�N�����擾
		cache.m_Token.Next();
//      SKY_PRINTF( "Skip Token : %s", cache.m_Token.GetChar() );

		//�@�u���b�N�J�n
		if ( strchr( cache.m_Token.GetChar(), '{' ) )
		{
			count++;
		}
		//�@�u���b�N�I��
		else if ( strchr( cache.m_Token.GetChar(), '}' ) )
		{
			count--;
		}		
	}

	//�@�I�[�̏ꍇ������̂Ń`�F�b�N
	if ( count > 0 )
	{
		SKY_PRINTF( "Error : �J�b�R����v���Ă��܂���\n" );
	}
}

static void NextToken( CLoadCacheData_ &cache )
{
	skyBool isTag = skyFalse;
	for ( u32 i = 0 ; i < TAG_TBL_NUM ; i++ )
	{
		if ( cache.m_Token.Check( s_TagTable[ i ] ) == skyTrue )
		{
			isTag = skyTrue;
			break;
		}
	}

	if ( isTag == skyFalse )
	{
		cache.m_Token.Next();
	}
}

static void ParseFrame( CLoadCacheData_ &cache )
{
	CFrame_ *pFrame = NEW CFrame_();
	cache.m_FrameList.AddTail( pFrame );

	//�ŏ��̂������߂����[�g
	if ( cache.m_RootFrame == skyNull )
	{
		cache.m_RootFrame = pFrame;
	}
	else
	{
		//��̎q�ɂȂ�
		if ( cache.m_FrameStack.GetNum() > 0 ) 
		{
			cache.m_FrameStack.GetTop()->m_ChildFrame.AddTail( pFrame );
		}
	}

	//�X�^�b�N�ɏ悹��
	cache.m_FrameStack.Push( pFrame );

    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
        //�@�K���ȃt���[���l�[�����擾
		char temp[255];
		std::sprintf( temp, "Joint%03d" , cache.m_FrameList.GetNum() );
		pFrame->m_Name = CHS( temp );
    }
    else
    {
    	//�@�t���[�������擾
		pFrame->m_Name = CHS( cache.m_Token.GetChar() );
    
        // '{'���o��܂ŃX�L�b�v
    	while( !cache.m_Token.IsEnd() )
    	{
    		cache.m_Token.Next();
    		if ( cache.m_Token.Check( "{"  ) )
    			break;
    	}
    }

	//�@���̃g�[�N����"FrameTransformMatrix"�ł��邱�Ƃ��m�F
	cache.m_Token.Next();
	if( cache.m_Token.Check( "FrameTransformMatrix" ) )
	{
		// '{'���o��܂ŃX�L�b�v
		while( !cache.m_Token.IsEnd() )
		{
			cache.m_Token.Next();
			if ( cache.m_Token.Check( "{" ) )
				break;
		}

		// �s����擾
		dectype matrix[ 16 ] = { 0 };
		for( s32 i = 0 ; i < 16 ; i++ )
		{
			matrix[ i ] = cache.m_Token.GetNextFloat();
//			SKY_PRINTF( "FrameTransformMatrix[%d] : %f" , i , matrix[ i ] );
		}

		math::CMatrix4x3 mat( matrix );

		//��̎q�ɂȂ�
		pFrame->m_Matrix = mat;

		// '}'���o��܂ŃX�L�b�v
		while( !cache.m_Token.IsEnd() )
		{		
			cache.m_Token.Next();
			if ( cache.m_Token.Check( "}" ) )
			{
				cache.m_Token.Next();
				break;
			}
		}		
	}
	//�@�����̂킩����̓X�L�b�v
	else
	{
		SkipNode( cache );
	}

//-----�q��{��---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//���̃m�[�h�͏I�����
		if ( cache.m_Token.Check( "Frame" ) ||
			 cache.m_Token.Check( "Mesh" ) )
		{
    		CheckNode( cache );
		}
		else if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		NextToken( cache );
	}

//-----�q��{��---->

	//�X�^�b�N������
	cache.m_FrameStack.Pop();
}

static void ParseMesh( CLoadCacheData_ &cache )
{
	CVertexBufferPtr spVertex = CVertexBuffer::Create();
	CIndexBufferPtr  spIndex  = CIndexBufferCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreate_() );

	cache.m_Token.Next();
	if ( !cache.m_Token.Check( "{" ) )
	{
		//���O���Z�b�g
		skyMString *name = cache.m_Token.GetChar();
//		SKY_PRINTF( _T( "Mesh Name : %s" ), name );
		spMesh->Name.SetName( CHS( name ) );

		//'{'���o��܂ŃX�L�b�v
		while( !cache.m_Token.IsEnd() )
		{
			cache.m_Token.Next();
			if ( strchr( cache.m_Token.GetChar(), '{' ) )
				break;
		}
	}
	else
	{
		//�K���Ȗ��O���Z�b�g
		spMesh->Name.SetName( CHS( "XFile Mesh" ) );
	}

	//�@���_�����擾
	vrtxtype vertices_size = 0;
	vertices_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Vertices : %d", vertices_size );

	//���_�o�b�t�@��������
	spVertex->Initialize( vertices_size );

	for( vrtxtype i = 0 ; i < vertices_size ; i++ )
	{
		vertex::CVertexFmt_Pos pos;
		pos.x = cache.m_Token.GetNextFloat();
		pos.y = cache.m_Token.GetNextFloat();
		pos.z = cache.m_Token.GetNextFloat();

		spVertex->GetPositionStream()->WritePosition( i , pos );

// 		SKY_PRINTF( "Vertex[ %d ] : %f,  %f, %f \n", i, pos.x , pos.y , pos.z );
	}

	//�@�C���f�b�N�X�����擾
	vrtxtype indices_size = 0;
	indices_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Vertex Indices : %d", indices_size );

	CVectorPrimitive< u16 >      faceData;
	CVectorPrimitive< vrtxtype > indexValues( indices_size );
	for( vrtxtype i = 0 ; i < indices_size ; i++ )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "3" ) )
		{
			cache.m_FaceData.Add( 3 );

			vrtxtype idx[ 3 ] = { 0 };
			idx[ 0 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 1 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 2 ] = ( vrtxtype )cache.m_Token.GetNextInt();

			//�@�C���f�b�N�X���i�[
			for( vrtxtype j = 0 ; j < 3 ; ++j )
			{
				SKY_ASSERT( idx[ j ] < vertices_size );
//				SKY_PRINTF( "Vertex Index[%d] : %d" , i , idx[ j ] );
				indexValues.Add( idx[ j ] );
			}
		}
		else if ( cache.m_Token.Check( "4" ) )
		{
			cache.m_FaceData.Add( 4 );

			//�@�l�p�`���O�p�`2�����ɕϊ�
			vrtxtype idx[ 6 ] = { 0 };
			idx[ 0 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 1 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 2 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 3 ] = ( vrtxtype )idx[ 2 ];
			idx[ 4 ] = ( vrtxtype )cache.m_Token.GetNextInt();
			idx[ 5 ] = ( vrtxtype )idx[ 0 ];

			for( vrtxtype j = 0 ; j < 6 ; j++ )
			{
				SKY_ASSERT( idx[ j ] < vertices_size );
//				SKY_PRINTF( "Vertex Index[%d] : %d" , i , idx[ j ] );
				indexValues.Add( idx[ j ] );
			}
		}
	}

	spIndex->Initialize( ( vrtxtype )indexValues.GetNum() );

	for( vrtxtype i = 0 ; i < indexValues.GetNum() ; i++ )
	{
		SKY_ASSERT( indexValues[ i ] < vertices_size );
		spIndex->SetIndex( i , indexValues[ i ] );
	}

	//�C���f�b�N�X�o�b�t�@���i�[
	spMesh->AttachIndexBuffer( spIndex );

	//���_�o�b�t�@���i�[
	spMesh->AttachVertexBuffer( spVertex );

	if ( cache.m_FrameStack.GetNum() > 0 ) 
	{
		//�ŏ�ʂ̃t���[���ɓo�^
		cache.m_FrameStack.GetTop()->m_spMesh = spMesh;
	}
	else
	{
		//�t���[���Ǘ�����Ă��Ȃ�Mesh�Ȃ̂Œ���Model�ɓo�^���Ă��܂�
		cache.m_spModel->GetMeshContena()->AttachMesh( spMesh );
	}

	//���݂̃��b�V���ɓo�^
	cache.m_CurrentMesh = spMesh;

//-----�q��{��---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//���̃m�[�h�͏I�����
		if ( cache.m_Token.CheckNowAndNext( "MeshMaterialList" ) ||
             cache.m_Token.CheckNowAndNext( "MeshNormals" ) ||
			 cache.m_Token.CheckNowAndNext( "MeshTextureCoords" ) ||
			 cache.m_Token.CheckNowAndNext( "MeshVertexColors" ) ||
			 cache.m_Token.CheckNowAndNext( "VertexDuplicationIndices" ) ||
			 cache.m_Token.CheckNowAndNext( "XSkinMeshHeader" ) ||
			 cache.m_Token.CheckNowAndNext( "SkinWeights" ) )
		{
    		CheckNode( cache );
		}
		else if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		NextToken( cache );
	}

//-----�q��{��---->
}

static void ParseMeshNormals( CLoadCacheData_ &cache )
{
	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();
	CIndexBufferPtr spIndex = cache.m_CurrentMesh->GetIndexBuffer();

	//�@'{'���o��܂ŃX�L�b�v
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//�@�@���x�N�g�������擾
	vrtxtype normal_size = 0;
	normal_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Normals : %d", normal_size );

	//�@�@���x�N�g�����擾
	spVertex->CreateNormalStream( spVertex->GetVertexNum() );
	CVectorPrimitive< vertex::CVertexFmt_Normal > normals;
	for( vrtxtype i = 0 ; i < normal_size ; i++ )
	{
		vertex::CVertexFmt_Normal val;
		val.nx = cache.m_Token.GetNextFloat();
		val.ny = cache.m_Token.GetNextFloat();
		val.nz = cache.m_Token.GetNextFloat();
		normals.Add( val );

//		SKY_PRINTF( "Normal[%d] : %f,  %f, %f", i, normals[i].nx, normals[i].ny, normals[i].nz );
	}

	//�@�@���C���f�b�N�X�����擾
	vrtxtype indices_size = 0;
	indices_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Normal Indices : %d", indices_size );

	//�@�@���C���f�b�N�X���擾
	CVectorPrimitive< vrtxtype > indices;
	for( vrtxtype i = 0 ; i < indices_size ; i++ )
	{
		cache.m_Token.Next();

		if ( cache.m_Token.Check( "3" ) )
		{
			s32 idx[ 3 ] = { -1 };
			idx[ 0 ] = cache.m_Token.GetNextInt();
			idx[ 1 ] = cache.m_Token.GetNextInt();
			idx[ 2 ] = cache.m_Token.GetNextInt();

			//�@�C���f�b�N�X���i�[
			for( vrtxtype j = 0 ; j < 3 ; j++ )
			{
				SKY_ASSERT( idx[ j ] < spVertex->GetVertexNum() );
//				SKY_PRINTF( "Normal Index[%d] : %d", i, idx[ j ] );
				indices.Add( ( vrtxtype )idx[ j ] );
			}
		}
		else if ( cache.m_Token.Check( "4" ) )
		{
			//�@�l�p�`���O�p�`2�����ɕϊ�
			s32 idx[ 6 ] = { -1 };
			idx[ 0 ] = cache.m_Token.GetNextInt();
			idx[ 1 ] = cache.m_Token.GetNextInt();
			idx[ 2 ] = cache.m_Token.GetNextInt();
			idx[ 3 ] = idx[ 2 ];
			idx[ 4 ] = cache.m_Token.GetNextInt();
			idx[ 5 ] = idx[ 0 ];

			for( vrtxtype j = 0 ; j < 6 ; j++ )
			{
				SKY_ASSERT( idx[ j ] < spVertex->GetVertexNum() );
//				SKY_PRINTF( "Normal Index[%d] : %d", i, idx[ j ] );
				indices.Add( ( vrtxtype )idx[ j ] );
			}
		}
	}

	SKY_ASSERT( spIndex->GetIndexNum() == indices.GetNum() ); //��Έ�v����͂�

	//���_���Ń��[�v���Ċe���_�ɐ������@�������Ă���
	for( vrtxtype i = 0 ; i < indices.GetNum() ; i++ )
	{
		vrtxtype normalIndex = indices[ i ];
		vrtxtype vertexIndex = spIndex->Index( i );
		spVertex->GetNormalStream()->WriteNormal( vertexIndex , normals[ normalIndex ] );
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->
}

static void ParseMeshTextureCoords( CLoadCacheData_ &cache )
{
	//�@'{'���o��܂ŃX�L�b�v
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//�@�e�N�X�`�����W�����擾
	vrtxtype texcoord_size = 0;
	texcoord_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of TexCoords : %d", texcoord_size );

	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();
	spVertex->CreateUvStream( spVertex->GetVertexNum() );

	SKY_ASSERT( spVertex->GetVertexNum() == texcoord_size );

	//�@�e�N�X�`�����W���擾
	for( vrtxtype i = 0 ; i < spVertex->GetVertexNum() ; i++ )
	{
		vertex::CVertexFmt_UV uv;
		uv.tu = cache.m_Token.GetNextFloat();
		uv.tv = cache.m_Token.GetNextFloat();

		spVertex->GetUvStream()->WriteUv( i , uv );
//		SKY_PRINTF( "TexCoord[%d] : %f, %f", i , uv.tu , uv.tv );
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->
}

static void ParseMeshVertexColors( CLoadCacheData_ &cache )
{
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//�@�J���[�����擾
	vrtxtype color_size = 0;
	color_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Vertex Colors: %d" , color_size );

	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();
	spVertex->CreateColorStream( spVertex->GetVertexNum() );

	SKY_ASSERT( spVertex->GetVertexNum() == color_size );

	CVectorPrimitive< vrtxtype > indices;
	CVector< vertex::CVertexFmt_Color > colors;
	for( vrtxtype i = 0 ; i < spVertex->GetVertexNum() ; i++ )
	{
		indices.Add( ( vrtxtype )cache.m_Token.GetNextInt() );
		vertex::CVertexFmt_Color color;
		color.color[ 0 ] = cache.m_Token.GetNextFloat();
		color.color[ 1 ] = cache.m_Token.GetNextFloat();
		color.color[ 2 ] = cache.m_Token.GetNextFloat();
		color.color[ 3 ] = cache.m_Token.GetNextFloat();
		colors.Add( color );
	}

	for( vrtxtype i = 0 ; i < spVertex->GetVertexNum() ; i++ )
	{
		spVertex->GetColorStream()->WriteColor( i , colors[ indices[ i ] ] );
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->
}

static void ParseMeshMaterialList( CLoadCacheData_ &cache )
{
	// '{'���o��܂ŃX�L�b�v
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	// �}�e���A�������擾
	vrtxtype material_size = 0;
	material_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Materials : %d", material_size );

	// �}�e���A���C���f�b�N�X�����擾
    vrtxtype indices_size = 0;
    indices_size = ( vrtxtype )cache.m_Token.GetNextInt();

	vrtxtype prev_index = 0;
	vrtxtype face_index = 0;
	vrtxtype vertex_index = 0;
	vrtxtype face_count = 0;
	vrtxtype startIndex = 0;

	CFaceVector_ faceVector;
	u32 max_index = 0;
    for( vrtxtype i = 0 ; i < indices_size ; i++ )
    {
		//�@�}�e���A���C���f�b�N�X�擾
        vrtxtype index  = ( vrtxtype )cache.m_Token.GetNextInt();
//      SKY_PRINTF( "Material Index[%d] : %d", i, index );

		//�@�������ʂ̃C���f�b�N�X���Z�o
		face_index++;
		if ( cache.m_FaceData[ i ] == 4 )
		{
			face_index++;
		}

		//�@�ȑO�̃}�e���A���C���f�b�N�X�ƈقȂ�ꍇ
		if ( prev_index != index )
		{
			//�@�t�F�C�X����ǉ�
			CFace_ face;
			face.m_MaterialIndex = prev_index;
			face.m_StartIndex    = startIndex;
			face.m_EndIndex      = vertex_index;
			faceVector.Add( face );

			//��ԑ傫�ȃC���f�b�N�X���G���[�`�F�b�N�p�ɂƂ��Ă���
			max_index = ( max_index < index ) ? index : max_index;

    		//�X�V
			prev_index = index;

			//���̕`��̊J�n�C���f�b�N�X
			startIndex = vertex_index;

			//�ʐ���������
			face_count = 1;
			if ( cache.m_FaceData[ i ] == 4 )
			{
				face_count++;
			}
		}
		else
		{
			//�ʐ����J�E���g
			face_count++;
			if ( cache.m_FaceData[ i ] == 4 )
			{
				face_count++;
			}
		}

		if ( cache.m_FaceData[ i ] == 4 )
		{//�l�p�`�ʂ̏ꍇ�́{�R�C���f�b�N�X�������Ă�
			vertex_index += 6;
		}
		else
		{//�R�p�`���_�̏ꍇ�̓C���f�b�N�X���R
			vertex_index += cache.m_FaceData[ i ];
		}
    }

	//�Ō�̃t�F�[�X�����i�[
	CFace_ face;
	face.m_MaterialIndex = prev_index;
	face.m_StartIndex    = startIndex;
	face.m_EndIndex      = vertex_index;
	faceVector.Add( face );

	SKY_ASSERT( max_index < material_size );

	CMaterialVector_ materialArray;
	for( vrtxtype i = 0 ; i < material_size ; i++ )
	{
		CMaterial_ mat;
		mat.m_spMaterial = CMaterialCreate_();

		//�@"Material"���o��܂ŃX�L�b�v
		while( !cache.m_Token.IsEnd() )
		{
			if ( cache.m_Token.Check( "Material" ) )
				break;

			cache.m_Token.Next();
		}

		//�@�}�e���A�����擾
		cache.m_Token.Next();
		if ( !cache.m_Token.Check( "{" ) )
		{
			//�@���O���Z�b�g
			skyMString *name = cache.m_Token.GetNextChar();
//			SKY_PRINTF( "Material Name : %s", name );
			mat.m_spMaterial->Name.SetName( CHS( name ) );

			//�@'{'���o��܂ŃX�L�b�v
			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "{" ) )
					break;
			}
		}
		else
		{
			//�@�K���Ȗ��O���Z�b�g
			mat.m_spMaterial->Name.SetName( CHS( "XFile Material" ) );
		}

		// Diffuse�擾
		mat.m_spMaterial->Diffuse.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.a = cache.m_Token.GetNextFloat();
//		SKY_PRINTF( "Diffuse : %f, %f, %f, %f" , spMaterial->Diffuse.r , spMaterial->Diffuse.g , spMaterial->Diffuse.b , spMaterial->Diffuse.a );

		// Power�擾
		mat.m_spMaterial->Power = cache.m_Token.GetNextFloat();
//		SKY_PRINTF( "Power : %f" , spMaterial->Power );

		// Specular�擾
		mat.m_spMaterial->Specular.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.a = 1.0f;
//		SKY_PRINTF( "Specular : %f, %f, %f, %f" , spMaterial->Diffuse.r , spMaterial->Diffuse.g , spMaterial->Diffuse.b , spMaterial->Diffuse.a );

		// Emissive�擾
		mat.m_spMaterial->Emissive.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.a = 1.0f;
//		SKY_PRINTF( "Specular : %f, %f, %f, %f" , spMaterial->Emissive.r , spMaterial->Emissive.g , spMaterial->Emissive.b , spMaterial->Emissive.a );

		// ���̃g�[�N�����擾���`�F�b�N����
		skyMString pathBuffer[ PATH_LEN_MAX ];
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "}" ) )
		{
			//�}�b�v�n�͂Ȃ��悤���B
		}
		else if ( cache.m_Token.Check( "TextureFilename" ) )
		{
			// '{'���o��܂ŃX�L�b�v
			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "{" ) )
					break;
			}

			//�@�e�N�X�`���t�@�C�����擾
			skyMString *texture_filename = cache.m_Token.GetNextChar();
//			SKY_PRINTF( "Texture File Name : %s", texture_filename );

			CGraphicFilePathManager::ConcatTextureFilePathRoot( pathBuffer , PATH_LEN_MAX , texture_filename );
			CTexturePtr spTexture = CTextureCreateName_( CHS( pathBuffer ) );
			mat.m_spMaterial->AttachDiffuseMap( spTexture );
			spTexture->Load();

			// '}'���o��܂ŃX�L�b�v
			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "}" ) )
					break;
			}			
		}

		materialArray.Add( mat );
	}

	for ( u32 i = 0 ; i < faceVector.GetNum() ; i++ )
	{
		cache.m_CurrentMesh->GetMaterialContena()->AddMaterial( materialArray[ faceVector[ i ].m_MaterialIndex ].m_spMaterial , faceVector[ i ].m_StartIndex , faceVector[ i ].m_EndIndex );
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v(Material�̕��j
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

	cache.m_Token.Next();

	// '}'���o��܂ŃX�L�b�v(MaterialList�̕��j
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

	NextToken( cache );

//-----�q�͖����̂Ŏ������g����������I��点��---->

}

static void ParseSkinWeights( CLoadCacheData_ &cache )
{
	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();

	//�@'{'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

    CWeight_ weight;

	//�@�t���[�������擾����
	weight.Name.SetName( CHS( cache.m_Token.GetNextChar() ) );
//	SKY_PRINTF( "Skin Weight Frame Name : %s", weight.Name );

	//�@�E�F�C�g�����擾����
	s32 weight_size = 0;
	weight_size = cache.m_Token.GetNextInt();
	weight.m_Weights.Assign( weight_size , 0 );
	weight.m_Indices.Assign( weight_size , 0 );
//	SKY_PRINTF( "Number Of Skin Weights : %d", weight_size );

	//�@���_�ԍ����擾����
	for( s32 i = 0 ; i < weight_size ; i++ )
	{
		vrtxtype index = static_cast< vrtxtype >( cache.m_Token.GetNextInt() );
		SKY_ASSERT( index < spVertex->GetVertexNum() );
//		SKY_PRINTF( "Skin Index[%d] : %d", i, index );
        weight.m_Indices[ i ] = index;
	}

	//�@�E�F�C�g���擾����
	for( s32 i = 0 ; i < weight_size ; i++ )
	{
		f32 weightVal = cache.m_Token.GetNextFloat();
		SKY_ASSERT( weightVal <= 1.0f );
//		SKY_PRINTF( "Skin Weight[%d] : %f", i, weight );
        weight.m_Weights[ i ] = weightVal;
	}

	//�@�I�t�Z�b�g�s����擾����
	for( s32 i = 0 ; i < 4 ; i++ )
	{
		for( s32 j = 0 ; j < 4 ; j++ )
		{
			f32 value = cache.m_Token.GetNextFloat();
//			SKY_PRINTF( "Offset Matrix[%d] : %f" , i , value );
			weight.m_Matrix.m[ i ][ j ] = value;
		}
	}

	if ( cache.m_FrameStack.GetNum() > 0 ) 
	{
		cache.m_FrameStack.GetTop()->m_WeightData.Add( weight );
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->

}

static void ParseAnimationSet( CLoadCacheData_ &cache )
{
	//�܂��R���e�i���쐬����ĂȂ���
	if ( cache.m_spMotionDataContena.IsEmpty() == skyTrue )
	{
		//�A�j���[�V�����f�[�^�R���e�i�̍쐬
		cache.m_spMotionDataContena = CMotionDataContenaCreate_();
	}

	math::CMotionDataPtr animation = CMotionDataCreate_();

	if ( cache.m_spMotionDataContena->GetAnimationNum() > 0 )
    {
        u32 clip_index = cache.m_spMotionDataContena->GetAnimationNum() - 1;
		cache.m_spMotionDataContena->GetAnimation( clip_index )->SetAnimationPallet( cache.m_CurrentAnimationPallet );
		cache.m_spMotionDataContena->GetAnimation( clip_index )->SetMaxTime( cache.m_CurrentAnimationMaxTime );
		cache.m_CurrentAnimationPallet.Clear();
        cache.m_CurrentAnimationMaxTime = 0.0f;
    }

    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
        //�@�K���ɃA�j���[�V�����Z�b�g����t����
        char temp[255];
		std::sprintf( temp, "clip_%03d" , cache.m_spMotionDataContena->GetAnimationNum() );
		animation->Name.SetName( CHS( temp ) );
//		SKY_PRINTF( "AnimationSet Name : %s", temp );
    }
    else
    {
		//�@�A�j���[�V�����Z�b�g�����擾
		hash::CHashStringPtr name = CHS( cache.m_Token.GetChar() );
//      SKY_PRINTF( "AnimationSet Name : %s", name );
		animation->Name.SetName( name );
        while( !cache.m_Token.IsEnd() )
        {
            cache.m_Token.Next();
            if ( cache.m_Token.Check( "{" ) )
                break;
        }
    }

	//�@�N���b�v��ǉ�
	cache.m_spMotionDataContena->AddAnimation( animation );

//-----�q��{��---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//���̃m�[�h�͏I�����
		if ( cache.m_Token.Check( "Animation" ) ) 
		{
    		CheckNode( cache );
		}
		else if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q��{��---->
}

static void ParseAnimation( CLoadCacheData_ &cache )
{
	math::CAnimationData animation;

	//�@�A�j���[�V��������ǂݔ�΂�
    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
        char temp[255];
		std::sprintf( temp, "animation_%03d", cache.m_spMotionDataContena->GetAnimationNum() );
		animation.Name.SetName( CHS( temp ) );
//		SKY_PRINTF( "AnimationSet Name : %s", temp );
    }
    else
    {
		//�@�A�j���[�V���������擾
		hash::CHashStringPtr name = CHS( cache.m_Token.GetChar() );
		animation.Name.SetName( CHS( cache.m_Token.GetChar() ) );
//      SKY_PRINTF( "Animation Name : %s", name );

        while( !cache.m_Token.IsEnd() )
        {
            cache.m_Token.Next();
            if ( cache.m_Token.Check( "{" ) )
                break;
        }
    }

	//�@�t���[�������ŏ��ɂ���p�^�[���̏ꍇ
    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
		//�@�t���[�������擾
		animation.m_JointName = CHS( cache.m_Token.GetNextChar() );
//      SKY_PRINTF( "Animation Frame Name : %s", frame_name );

		//�����ʂ܂œǂݔ�΂�
        while( !cache.m_Token.IsEnd() )
        {
            cache.m_Token.Next();
            if ( cache.m_Token.Check( "}" ) )
			{
			    cache.m_Token.Next();
                break;
			}
        }
    }

    if ( cache.m_Token.Check( "AnimationKey" ) )
    {
		u32 i = 0;
		CAnimationKeyFrameArrayPtrList_ animKeysList;
		while( cache.m_Token.Check( "AnimationKey" ) )
		{
			CAnimationKeyFrameArray_ *pAnimKeys = NEW CAnimationKeyFrameArray_();
			i++;

//======================================================================================

			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "{" ) )
					break;
			}

			//�@�^�C�v���擾
			s32 type = cache.m_Token.GetNextInt();
	//      SKY_PRINTF( "Type : %d", type );

			//�@�L�[�T�C�Y���擾
			s32 key_size = cache.m_Token.GetNextInt();

			//�L�[�T�C�Y��2�ȏ�
			SKY_ASSERT( key_size >= 2 );

	//      SKY_PRINTF( "Key Size : %d", key_size );
			pAnimKeys->Reset( key_size );

			//�@�L�[�T�C�Y�����[�v
			for ( s32 i = 0 ; i < key_size ; i++ )
			{
				//�@���Ԃƃp�����[�^�T�C�Y���擾
				u32 time = static_cast< u32 >( cache.m_Token.GetNextInt() );
				int param_size = cache.m_Token.GetNextInt();
	//          SKY_PRINTF( "Time : %d, ParamSize : %d", time, param_size );

				//�@���Ԃ�ݒ�
				( *pAnimKeys )[ i ].m_Time = time;

				CVectorPrimitive< f32 > params;
				params.Assign( param_size , 0.0f );

				//�@�p�����[�^�T�C�Y�����[�v
				for( s32 j = 0 ; j < param_size ; j++ )
				{
					//�@�p�����[�^�擾
					f32 param = cache.m_Token.GetNextFloat();
	//              SKY_PRINTF( "Param[%d][%d] : %f", i, j, param );
					params[ j ] = param;
				}

				//�@�^�C�v�ɂ�菈���𕪊�
				( *pAnimKeys )[ i ].m_Type = type;
				switch( type )
				{
					//�@��]
					case 0:
						{
							SKY_ASSERT( param_size == 4 );

							( *pAnimKeys )[ i ].m_Rot[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Rot[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Rot[ 2 ] = params[ 2 ];
							( *pAnimKeys )[ i ].m_Rot[ 3 ] = params[ 3 ];
						}
						break;

					//�@�g��k��
					case 1:
						{
							SKY_ASSERT( param_size == 3 );

							( *pAnimKeys )[ i ].m_Sc[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Sc[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Sc[ 2 ] = params[ 2 ];
						}
						break;

					//�@���s�ړ�
					case 2:
						{
							SKY_ASSERT( param_size == 3 );

							( *pAnimKeys )[ i ].m_Trans[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Trans[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Trans[ 2 ] = params[ 2 ];
						}
						break;

					//�@������
					case 3:
						SKY_PANIC();
						break;

					//�@�s��
					case 4:
						{
							SKY_ASSERT( param_size == 16 );

							( *pAnimKeys )[ i ].m_Matrix[ 0 ]  = params[ 0 ];
							( *pAnimKeys )[ i ].m_Matrix[ 1 ]  = params[ 1 ];
							( *pAnimKeys )[ i ].m_Matrix[ 2 ]  = params[ 2 ];
							( *pAnimKeys )[ i ].m_Matrix[ 3 ]  = params[ 3 ];
							( *pAnimKeys )[ i ].m_Matrix[ 4 ]  = params[ 4 ];
							( *pAnimKeys )[ i ].m_Matrix[ 5 ]  = params[ 5 ];
							( *pAnimKeys )[ i ].m_Matrix[ 6 ]  = params[ 6 ];
							( *pAnimKeys )[ i ].m_Matrix[ 7 ]  = params[ 7 ];
							( *pAnimKeys )[ i ].m_Matrix[ 8 ]  = params[ 8 ];
							( *pAnimKeys )[ i ].m_Matrix[ 9 ]  = params[ 9 ];
							( *pAnimKeys )[ i ].m_Matrix[ 10 ] = params[ 10 ];
							( *pAnimKeys )[ i ].m_Matrix[ 11 ] = params[ 11 ];
							( *pAnimKeys )[ i ].m_Matrix[ 12 ] = params[ 12 ];
							( *pAnimKeys )[ i ].m_Matrix[ 13 ] = params[ 13 ];
							( *pAnimKeys )[ i ].m_Matrix[ 14 ] = params[ 14 ];
							( *pAnimKeys )[ i ].m_Matrix[ 15 ] = params[ 15 ];
						}
						break;
				}
			}

			//�@�p�����Ԃ�ݒ�
			SKY_ASSERT( key_size >= 1 );
			f32 duration = static_cast< f32 >( ( *pAnimKeys )[ key_size -1 ].m_Time );
			cache.m_CurrentAnimationMaxTime = TemplateUtil::Max< f32 >( duration , cache.m_CurrentAnimationMaxTime );

			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "}" ) )
					break;
			}

			//�@�t���[�������Ō�ɂ���p�^�[���̏ꍇ
			cache.m_Token.Next();

			//�i�[
			animKeysList.AddTail( pAnimKeys );
		}

		//�ő�L�[���擾
		FOREACH( CAnimationKeyFrameArrayPtrList_ , it , animKeysList )
		{
			CAnimationKeyFrameArray_ *pArray = ( *it );

			//�s�񂾂�����B
			if ( ( *pArray )[ 0 ].m_Type == 4 )
			{
				math::CAnimationKeyFrameMatrixArray animKeysResult;
				animKeysResult.Reset( ( *it )->GetNum() );
				for ( u32 i = 0 ; i < ( *it )->GetNum() ; i++ )
				{
					animKeysResult[ i ].m_Time   = ( *pArray )[ i ].m_Time;
					animKeysResult[ i ].m_Matrix = ( *pArray )[ i ].m_Matrix;
				}
				animation.SetKeyFrameMatrixArray( animKeysResult );
			}
			//�@��]
			else if ( ( *pArray )[ 0 ].m_Type == 0 )
			{
				math::CAnimationKeyFrameRotArray animKeysResult;
				animKeysResult.Reset( ( *it )->GetNum() );
				for ( u32 i = 0 ; i < ( *it )->GetNum() ; i++ )
				{
					animKeysResult[ i ].m_Time = ( *pArray )[ i ].m_Time;
					animKeysResult[ i ].m_Rot  = ( *pArray )[ i ].m_Rot;
				}
				animation.SetKeyFrameRotArray( animKeysResult );
			}
			//�@�g��k��
			else if ( ( *pArray )[ 0 ].m_Type == 1 )
			{
				math::CAnimationKeyFrameScaleArray animKeysResult;
				animKeysResult.Reset( ( *it )->GetNum() );
				for ( u32 i = 0 ; i < ( *it )->GetNum() ; i++ )
				{
					animKeysResult[ i ].m_Time  = ( *pArray )[ i ].m_Time;
					animKeysResult[ i ].m_Scale = ( *pArray )[ i ].m_Sc;
				}
				animation.SetKeyFrameScaleArray( animKeysResult );
			}
			//�@�ړ�
			else if ( ( *pArray )[ 0 ].m_Type == 2 )
			{
				math::CAnimationKeyFrameTransArray animKeysResult;
				animKeysResult.Reset( ( *it )->GetNum() );
				for ( u32 i = 0 ; i < ( *it )->GetNum() ; i++ )
				{
					animKeysResult[ i ].m_Time  = ( *pArray )[ i ].m_Time;
					animKeysResult[ i ].m_Trans = ( *pArray )[ i ].m_Trans;
				}
				animation.SetKeyFrameTransArray( animKeysResult );
			}
		}

		//�ǂݍ��ݗp�f�[�^�͏���
		FOREACH( CAnimationKeyFrameArrayPtrList_ , it , animKeysList )
		{
			DEL ( *it );
		}

		//�t���[��������ɂ���p�^�[��
        if ( cache.m_Token.Check( "{" ) )
        {
			//�W���C���g���擾
			animation.m_JointName = CHS( cache.m_Token.GetNextChar() );

//          SKY_PRINTF( "Animation Frame Name : %s", frame_name );

			cache.m_CurrentAnimationPallet.Add( animation );

            while( !cache.m_Token.IsEnd() )
            {
                cache.m_Token.Next();
                if ( cache.m_Token.Check( "}" ) )
                    break;
            }
        }
    }

	cache.m_CurrentAnimationPallet.Add( animation );

//-----�q�͖����̂Ŏ������g����������I��点��---->

	// '}'���o��܂ŃX�L�b�v
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----�q�͖����̂Ŏ������g����������I��点��---->
}

static void CheckNode( CLoadCacheData_ &cache )
{
	if ( cache.m_Token.Check( "template" ) ||
	     cache.m_Token.Check( "VertexDuplicationIndices" ) ||
		 cache.m_Token.Check( "XSkinMeshHeader" ) )
    {
        SkipNode( cache );
    }
    else if ( cache.m_Token.Check( "Frame" ) )
    {
	    ParseFrame( cache );
    }
    else if ( cache.m_Token.Check( "Mesh" ) )
    {
        ParseMesh( cache );
    }
    else if ( cache.m_Token.Check( "MeshMaterialList" ) )
    {
        ParseMeshMaterialList( cache );
    }
    else if ( cache.m_Token.Check( "MeshNormals" ) )
    {
        ParseMeshNormals( cache );
    }
    else if ( cache.m_Token.Check( "MeshTextureCoords" ) )
    {
        ParseMeshTextureCoords( cache );
    }
	else if ( cache.m_Token.Check( "MeshVertexColors" ) )
	{
		ParseMeshVertexColors( cache );
	}
    else if ( cache.m_Token.Check( "SkinWeights" ) )
    {
        ParseSkinWeights( cache );
    }
    else if ( cache.m_Token.Check( "AnimationSet" ) )
    {
        ParseAnimationSet( cache );
    }
    else if ( cache.m_Token.Check( "Animation" ) )
    {
        ParseAnimation( cache );
    }
}

static void SetSkinDataFromCache( CLoadCacheData_ &cache , CFrame_ *pFrame )
{
	CVertexBufferPtr spVertex = pFrame->m_spMesh->GetVertexBuffer();

    //�@�t���[��������
	for( u32 i = 0 ; i < pFrame->m_WeightData.GetNum() ; ++i )
    {
			//�@���O����t���[���ԍ�������
		s32 joint_index = cache.m_spSkeleton->m_spJointPallet->GetJoint( pFrame->m_WeightData[ i ].Name.GetName() )->Index;

		SKY_ASSERT( joint_index >= -1 );
		SKY_ASSERT( joint_index < 65535 ); //100�N��ɂ͂��������f�[�^�����邩��

        //�@���������ꍇ
	    if ( joint_index > -1 )
	    {
			vrtxtype num_vertices = pFrame->m_spMesh->GetVertexBuffer()->GetVertexNum();

			if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream( hash::CHashStringPtr() , skyFalse ).IsEmpty() == skyTrue ||
				 pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream( hash::CHashStringPtr() , skyFalse ).IsEmpty() == skyTrue )
			{
				//�o�b�t�@���쐬
				pFrame->m_spMesh->GetVertexBuffer()->CreateIndexStream( num_vertices );
				pFrame->m_spMesh->GetVertexBuffer()->CreateWeightsStream( num_vertices );

				//�o�b�t�@��������
				pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->WriteIndexAll( vertex::CVertexFmt_Index( 0 ) );
				pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->WriteWeightsAll( vertex::CVertexFmt_Weights( 0.0f ) );
			}

		    //�@�E�F�C�g�������[�v
			for( u32 j = 0 ; j < pFrame->m_WeightData[ i ].m_Weights.GetNum() ; ++j )
		    {
			    //�@���_�ԍ�
				vrtxtype vertex_index = pFrame->m_WeightData[ i ].m_Indices[ j ];
				f32 weight            = pFrame->m_WeightData[ i ].m_Weights[ j ];

				SKY_ASSERT( vertex_index < spVertex->GetVertexNum() );
				SKY_ASSERT( weight <= 1.0f );

			    //�@index 0 ���ݒ�
				if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 0 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 0 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 0 ] = weight;
			    }
			    //�@index 1 ���ݒ�
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 1 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 1 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 1 ] = weight;
			    }
			    //�@index 2 ���ݒ�
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 2 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 2 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 2 ] = weight;
			    }
			    //�@index 3 ���ݒ�
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 3 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 3 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 3 ] = weight;
			    }
		    }

		    //�{�[���I�t�Z�b�g�s��i���[�J�����W�ɂ��钸�_���{�[����Ԃ̍��W�ɕϊ�����s��j��ݒ�
			cache.m_spSkeleton->m_spJointPallet->GetJoint( joint_index )->SetOffsetMatrix( pFrame->m_WeightData[ i ].m_Matrix );

#ifdef SW_SKYLIB_DEBUG_ON
//		SKY_PRINTF( _T( "=====< OffsetMatrix[ %s ][ %d ] >===== \n" ) , cache.m_spSkeleton->m_spJointPallet->GetJoint( joint_index )->Name.GetString() , joint_index );
//		pFrame->m_WeightData[ i ].m_Matrix.DebugPrint();
#endif
	    }
    }
}

static void SetJointDataFromFrame( const math::CJointPtr &spJoint , CFrame_ *pFrame )
{
	spJoint->Name.SetName( pFrame->m_Name );
	spJoint->SetInitialTransform( pFrame->m_Matrix ); //�����p���s��i���f�����ŏ��ɂƂ��Ă�|�[�Y��\���j
}

static void TraverseFrameTree( CLoadCacheData_ &cache )
{
//---- �p���W���C���g�c���[���쐬
	CFramePtrStack   stack( 1024 );
	math::CJointPtrStack jointStack( 1024 );

	cache.m_spPoseJointRoot = CJointCreate_();

	SetJointDataFromFrame( cache.m_spPoseJointRoot , cache.m_RootFrame );

	jointStack.Push( cache.m_spPoseJointRoot );
	stack.Push( cache.m_RootFrame );

	while( stack.GetNum() > 0 )
	{
		math::CJointPtr spCurrentJoint;
		CFrame_ *pCurrentFrame;
		jointStack.Pop( spCurrentJoint );
		stack.Pop( pCurrentFrame );

		//�q�������炷�ׂăX�^�b�N�ɐς�ŁA���Ń��[�v
		//�X�^�b�N�ł̃c���[�\���̉�͂́A���[�̎q���珈�����s�����Ƃ�����
		if ( pCurrentFrame->m_ChildFrame.GetNum() > 0 )
		{
			FOREACH( CFramePtrList , it , pCurrentFrame->m_ChildFrame )
			{
				math::CJointPtr spJoint = CJointCreate_();

				SetJointDataFromFrame( spJoint , *it );

				spCurrentJoint->AttachChild( spJoint );

				jointStack.Push( spJoint );
				stack.Push( *it );
			}
			continue;
		}
	}

	//�|�[�Y�W���C���g�c���[���A�^�b�`
	if( cache.m_spPoseJointRoot->ChildList().GetNum() > 0 )
	{
//---- �W���C���g�p���b�g�A���_����Weight , Index�̍쐬

		FOREACH( CFramePtrList , it , cache.m_FrameList )
		{
			if ( ( *it )->m_WeightData.GetNum() > 0 )
			{//���̃t���[���ɂ̓W���C���g�f�[�^�����݂���

				//���b�V�����ƂɃ|�[�Y�͓����X�P���g�������i���ۃA�j���[�V�����f�[�^�������������̃��b�V�������邱�Ƃ͖����Ǝv�����j
				cache.m_spSkeleton = CSkeletonCreate_( cache.m_spPoseJointRoot );

				//�W���C���g�A�W���C���g�p���b�g���쐬����
				SetSkinDataFromCache( cache , *it );

				//�􉽌`������쐬
				cache.m_spModel->GetMeshContena()->AttachMesh( ( *it )->m_spMesh );

				//���b�V���ɃX�P���g�����Ԃ牺����
				( *it )->m_spMesh->AttachSkeleton( cache.m_spSkeleton );
			}
		}
	}
}

void CXFileLoader::LoadFile( const hash::CHashStringPtr &path )
{
	//�f�B���N�g���p�X�w�肪�������ꍇpath�ƍ�������
	skyString pathBuffer[ PATH_LEN_MAX ];
	CGraphicFilePathManager::ConcatXFilePathRoot( pathBuffer , PATH_LEN_MAX , path->GetString() );

	stream::IStreamPtr spStream = CFileUtil_::LoadFile( CHS( pathBuffer ) );
	spStream->vOpen();

	CLoadCacheData_ cache;

	cache.m_Token.SetBuffer( ( skyMString * )spStream->vGetData() );
	cache.m_Token.SetSeparator( " \t\r\n,;\"" );
	cache.m_Token.SetCutOff( "{}" );

	//���f����ǂݍ���
	cache.m_spModel = CModelCreate_();

	//���b�V���R���e�i�̍쐬
	CMeshContenaPtr spMeshContena = CMeshContenaCreate_();

	//���b�V�������f���Ɋ֘A�t����
	cache.m_spModel->AttachMeshContena( spMeshContena );

	//�f�t�H���g�Ńe�N�X�`���}�b�s���O�̃V�F�[�_�[��t���Ă���
	cache.m_spModel->AttachShader( CDirectionalLightShaderObjectCreate_() );

	//�@�t�@�C���I�[�܂Ń��[�v
	while( !cache.m_Token.IsEnd() )
	{
		NextToken( cache );
//      SKY_PRINTF( "Check Token : %s", cache.m_Token.GetChar() );

        CheckNode( cache );
	}

	if ( cache.m_FrameList.GetNum() > 0 )
	{
		//�ǂݍ��݌��ʂ���͂���
		TraverseFrameTree( cache );

		if ( cache.m_spMotionDataContena->GetAnimationNum() > 0 )
		{
			u32 clip_index = cache.m_spMotionDataContena->GetAnimationNum() - 1;
			cache.m_spMotionDataContena->GetAnimation( clip_index )->SetAnimationPallet( cache.m_CurrentAnimationPallet );
			cache.m_spMotionDataContena->GetAnimation( clip_index )->SetMaxTime( cache.m_CurrentAnimationMaxTime );

			CDataBase_::AddMotionDataContena( cache.m_spMotionDataContena );
		}
	}

	//���b�V������ł�����ΗL���ȃ��f���Ƃ݂Ȃ�
	if ( cache.m_spModel->GetMeshContena()->GetMeshNum() > 0 )
	{
		//�T�C�Y�������O�v�Z
		cache.m_spModel->ComputeSizeData();

		//���f�����f�[�^�x�[�X�ɓ����
		CDataBase_::AddModel( cache.m_spModel );
	}

	//��͂����t���[����S�ď���
	FOREACH( CFramePtrList , it ,  cache.m_FrameList )
	{
		DEL *it;
	}

	spStream->vClose();

#ifdef _DEBUG
		CheckMemBreak();
#endif

}

} } }