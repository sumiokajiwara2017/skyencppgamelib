#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

// OBJ�t�@�C�����̃O���[�v���
#define OBJ_GROUP_NAME_LEN_MAX ( 80 )
#define OBJ_GROUP_MTL_LEN_MAX  ( 80 )
#define OBJ_GROUP_NUM_MAX      ( 256 )
struct OBJ_GROUP
{
	skyString  name[ OBJ_GROUP_NAME_LEN_MAX ];  // �O���[�v��
	skyString  mtl[ OBJ_GROUP_MTL_LEN_MAX ];    // �}�e���A����
	s32        startIndex;                        // �O���[�v�̊J�n�C���f�b�N�X
	s32        countIndex;                        // �O���[�v�Ɋ܂܂��C���f�b�N�X��
};

static void LoadObj_( const hash::CHashStringPtr &path ,
	                  math::CVector3< f32 > *vertex , vrtxtype &vertexCount ,
	                  math::CVector2< f32 > *uv , 
					  math::CVector3< f32 > *normal ,
					  vrtxtype              *index , vrtxtype &indexCount ,
					  OBJ_GROUP           *mltGroup , vrtxtype &groupCnt ,
					  skyString             *mtlFileName
					  )
{
	//���O�Ƀ��[�h���ăX�g���[���œn��
	stream::IStreamPtr spStream = CFileUtil_::LoadFile( path );
	spStream->vOpen();

	static  skyString buf[ 1024 ];
    static	skyString op[ 4 ][ 256 ];
	vrtxtype vertexWorkCnt = 0;
	math::CVector3< f32 > *vertexWork     = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );

	vrtxtype uvWorkCnt = 0;
	math::CVector2< f32 > *uvWork         = NEW_ARRAY__( math::CVector2< f32 > , VERTEX_NUM_MAX );

	vrtxtype normalWorkCnt = 0;
	math::CVector3< f32 > *normalWork     = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );

	vrtxtype indexWorkCnt = 0;
	static vrtxtype indexWork[ VERTEX_NUM_MAX ][ 3 ];

//--------------------------obj�t�@�C�����[�h����---------------------------------��

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE	

	//BOM�̕��i�߂�
	spStream->vSetSeek( BOM_SIZE );

#endif

	s32 n;
	//�X�g���[���f�[�^����s���ǂ�
	while( spStream->vReadLine( buf , sizeof( buf ) ) )
	{
		// ���W
		n = SkyScanf( buf , _T( "v %f %f %f" ) , &vertexWork[ vertexWorkCnt ].X, &vertexWork[ vertexWorkCnt ].Y, &vertexWork[ vertexWorkCnt ].Z );
		if ( n > 0 )
		{ 
			++vertexWorkCnt; 
            SKY_ASSERT_MSG( vertexWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// �e�N�X�`�����W
		n = SkyScanf( buf , _T( "vt %f %f" ) , &uvWork[ uvWorkCnt ].X, &uvWork[ uvWorkCnt ].Y );
		if ( n > 0 )
		{ 
			++uvWorkCnt; 
            SKY_ASSERT_MSG( uvWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// �@���x�N�g��
		n = SkyScanf( buf , _T( "vn %f %f %f" ) , &normalWork[ normalWorkCnt ].X, &normalWork[ normalWorkCnt ].Y, &normalWork[ normalWorkCnt ].Z );
		if ( n > 0 )
		{ 
			++normalWorkCnt; 
            SKY_ASSERT_MSG( normalWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// �O���[�v��
		n = SkyScanf( buf , _T( "g %s" ) ,  mltGroup[ groupCnt ].name, sizeof( mltGroup[ groupCnt ].name ) / sizeof( skyString ) );
		if ( n > 0 )
		{//���̃O���[�v�����������Ă���
			mltGroup[ groupCnt ].mtl[ 0 ]  = NULL;
			mltGroup[ groupCnt ].startIndex  = indexWorkCnt;
			mltGroup[ groupCnt ].countIndex  = 0;
			++groupCnt;
            SKY_ASSERT_MSG( groupCnt <= OBJ_GROUP_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// �}�e���A����
		n = SkyScanf( buf , _T( "usemtl %s" ) ,  mltGroup[ groupCnt - 1 ].mtl, sizeof( mltGroup[ groupCnt - 1 ].mtl ) / sizeof( skyString ) );
		if ( n > 0 ) continue;

		// �}�e���A���E�t�@�C����
		n = SkyScanf( buf , _T( "mtllib %s" ) , mtlFileName , _MAX_PATH / sizeof( skyString ) );
		if ( n > 0 ) continue;

		// ��( �O�p�`�܂��͎l�p�`�Ɖ��� )
		n = SkyScanf( buf , _T( "f %s %s %s %s" ) ,
			 op[ 0 ] , sizeof( op[ 0 ] )  / sizeof( skyString ) , op[ 1 ] , sizeof( op[ 1 ] ) / sizeof( skyString ) ,
			 op[ 2 ] , sizeof( op[ 2 ] ) / sizeof( skyString ) , op[ 3 ] , sizeof( op[ 3 ] ) / sizeof( skyString ) );
		if ( n == 0 ) continue;
        SKY_ASSERT_MSG( !( n < 3 ) , _T( "Mesh reading failed.\n" ) ); //�v���~�e�B�u�͂R�ȉ��͋��e���Ȃ��B

		s32 f[ 4 ][ 3 ];
		for ( vrtxtype i = 0 ; i < n ; ++i )
		{
			s32 iv = 0 , it = 0 , in = 0;
			if ( SkyScanf( op[ i ], _T( "%i/%i/%i" ) , &iv , &it , &in ) == 1 )
				 SkyScanf( op[ i ], _T( "%i//%i" ) , &iv , &in );
			f[ i ][ 0 ] = ( iv < 0 ) ? vertexWorkCnt - iv + 1 : iv;
			f[ i ][ 1 ] = ( it < 0 ) ? uvWorkCnt     - it + 1 : it;
			f[ i ][ 2 ] = ( in < 0 ) ? normalWorkCnt - in + 1 : in;
		}

		for ( s32 count = 0 ; count < n - 2 ; ++count )
		{
            SKY_ASSERT_MSG( !( indexWorkCnt + 3 > VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //�v���~�e�B�u�͂R�ȉ��͋��e���Ȃ��B

			math::CVector3< f32 > vec = vertexWork[ f[ 0 ][ 0 ] - 1 ]  - vertexWork[ f[ count + 1 ][ 0 ] - 1 ];
			if ( vec.Length() < 0.00001f)
			{
				continue;
			}
			vec = vertexWork[ f[ 0 ][ 0 ] - 1 ]  - vertexWork[ f[ count + 2 ][ 0 ] - 1 ];
			if ( vec.Length() < 0.00001f )
			{
				continue;
			}
			vec = vertexWork[ f[ count + 1 ][ 0 ] - 1 ]  - vertexWork[ f[ count + 2 ][ 0 ] - 1 ];
			if ( vec.Length() < 0.00001f )
			{
				continue;
			}

			for ( s32 h = 0; h < 3; ++h )
			{
				indexWork[ indexWorkCnt ][ h ]     = ( vrtxtype )f[ 0 ][ h ]; 
				indexWork[ indexWorkCnt + 1 ][ h ] = ( vrtxtype )f[ count + 1 ][ h ]; 
				indexWork[ indexWorkCnt + 2 ][ h ] = ( vrtxtype )f[ count + 2 ][ h ]; 
			}
			indexWorkCnt += 3;
			mltGroup[ groupCnt - 1 ].countIndex += 3;
		}
	}

    // �ߐڒ��_����������i�œK���j
	for ( vrtxtype i = 0; i < vertexWorkCnt; ++i )
	{
		for ( vrtxtype h = i + 1; h < vertexWorkCnt; ++h )
		{
			math::CVector3< f32 > vec = vertexWork[ i ] - vertexWork[ h ];
			if ( vec.Length() >= 0.0001f )
			{
				continue;
			}

			for ( vrtxtype j = 0 ; j < indexWorkCnt ; ++j )
			{
				if ( indexWork[ j ][ 0 ] == h + 1 )
				{
					 indexWork[ j ][ 0 ] = i + 1;
				}
			}
		}
	}

	// �C���f�b�N�X�E�o�b�t�@��g�ݗ��Ă�
	vrtxtype indexWork2[ VERTEX_NUM_MAX ][ 3 ];
	vrtxtype j;
	vertexCount = 0;
	for ( vrtxtype i = 0; i< indexWorkCnt; ++i )
	{
		for ( j = 0; j < vertexCount; ++j )
		{
			if (indexWork2[ j ][ 0 ] == indexWork[ i ][ 0 ] && indexWork2[ j ][ 1 ] == indexWork[ i ][ 1 ] && indexWork2[ j ][ 2 ] == indexWork[ i ][ 2 ] )
			{
				break;
			}
		}

        SKY_ASSERT_MSG( !( j >= VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //�v���~�e�B�u�͂R�ȉ��͋��e���Ȃ��B

		if (j >= vertexCount)
		{
			indexWork2[ j ][ 0 ] = indexWork[ i ][ 0 ];
			indexWork2[ j ][ 1 ] = indexWork[ i ][ 1 ];
			indexWork2[ j ][ 2 ] = indexWork[ i ][ 2 ];
			++vertexCount;
		}

		index[ indexCount ] = j; ++indexCount;

        SKY_ASSERT_MSG( !( indexCount >= VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //�v���~�e�B�u�͂R�ȉ��͋��e���Ȃ��B
	}

	// ���_�o�b�t�@��g�ݗ��Ă�
	for ( vrtxtype i = 0 ; i < vertexCount ; ++i )
	{
		--indexWork2[ i ][ 0 ]; --indexWork2[ i ][ 1 ]; --indexWork2[ i ][ 2 ];
	}
	for ( vrtxtype i = 0 ; i < vertexCount ; ++i )
	{
		math::CVector3< f32 > *pP = &vertex[ i ]; 
		math::CVector2< f32 > *pT = &uv[ i ]; 
		math::CVector3< f32 > *pN = &normal[ i ]; 

		// ���W
		if ( ( indexWork2[ i ][ 0 ] < 0 ) || ( indexWork2[ i ][ 0 ] >= vertexWorkCnt ) )
		{
			 pP->X = pP->Y = pP->Z = 0.0f;
		}
		else
		{
			*pP = vertexWork[ indexWork2[ i ][ 0 ] ];
		}

		// �e�N�X�`�����W
		if ( ( indexWork2[ i ][ 1 ] < 0 ) || ( indexWork2[ i ][ 1 ] >= uvWorkCnt ) )
		{
			 pT->X = pT->Y = 0.0f;
		}
		else
		{
			*pT = uvWork[ indexWork2 [ i ][ 1 ] ];
		}

		// �@���x�N�g��
		if ((indexWork2[i][2] < 0) || (indexWork2[i][2] >= normalWorkCnt))
		{
			 pN->X = pN->Y = pN->Z = 1.0f;
		}
		else
		{
			*pN = normalWork[ indexWork2[ i ][ 2 ] ];
		}
	}

#if OFF_

	//���_�o�b�t�@���o�͂���
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f %f \n" ) , vertex[ i ].X , vertex[ i ].Y , vertex[ i ].Z );
	}

	//�@���o�b�t�@���o�͂���
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f %f \n" ) , normal[ i ].X , normal[ i ].Y , normal[ i ].Z );
	}

	//�t�u���o�͂���
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f \n" ) , uv[ i ].X , uv[ i ].Y );
	}

	//�C���f�b�N�X�o�b�t�@���o�͂���
	for ( vrtxtype i = 0; i< indexCount; ++i )
	{
		SKY_PRINTF( _T( "%d \n" ) , index[ i ] );
	}

#endif

	DEL_ARRAY vertexWork;
	DEL_ARRAY uvWork;
	DEL_ARRAY normalWork;

	spStream->vClose();

//--------------------------obj�t�@�C�����[�h����---------------------------------��

}

// �}�e���A���E�f�[�^
#define OBJ_MTL_TEXTURE_NAME_LEN ( 80 )
#define OBJ_MTL_MAX_NUM          ( 256 )
struct OBJ_MTL
{
	skyString name[ OBJ_MTL_TEXTURE_NAME_LEN ];     // �}�e���A����
	f32 Kd[ 3 ];                             // �f�B�t���[�Y�F�B�l�́u0�v�`�u1�v�B
	f32 Ks[ 3 ];                             // �X�y�L�����F�B�l�́u0�v�`�u1�v�B
	f32 Ka[ 3 ];                             // �A���r�G���g(��)�F�B�l�́u0�v�`�u1�v�B
	f32 d;                                   // �����x�B�l�́u0�v(����)�`�u1�v(�s����)�B
	f32 Ns;                                  // ����B�l�́A�u0�v�`�B 
	f32 Ni;                                  // ���ܗ��B�l�́u1�v�ȏ�B�u1�v�͋��܂Ȃ��B
	skyString map_Kd[ OBJ_MTL_TEXTURE_NAME_LEN ];   // �f�B�t���[�Y�E�}�b�v�B��ʓI�ȃe�N�X�`���B
	skyString map_Ks[ OBJ_MTL_TEXTURE_NAME_LEN ];   // �X�y�L�����E�}�b�v�B
	skyString map_Ka[ OBJ_MTL_TEXTURE_NAME_LEN ];   // ���}�b�v�B
	skyString map_Bump[ OBJ_MTL_TEXTURE_NAME_LEN ]; // �o���v�E�}�b�v�B
	skyString map_D[ OBJ_MTL_TEXTURE_NAME_LEN ];    // �����}�b�v�B
	skyString refl[ OBJ_MTL_TEXTURE_NAME_LEN ];     // ���˃}�b�v�B
};

static void LoadMTL_( const hash::CHashStringPtr &path , OBJ_MTL *mtl , s32 &mtlCount )
{
	mtlCount = -1;

	//���O�Ƀ��[�h���ăX�g���[���œn��
	stream::IStreamPtr spStream = CFileUtil_::LoadFile( path );
	spStream->vOpen();

	static  skyString buf[ 1024 ];
    static	skyString op[ 4 ][ 256 ];

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE	

	//BOM�̕��i�߂�
	spStream->vSetSeek( BOM_SIZE );

#endif

	s32 n;
	//�X�g���[���f�[�^����s���ǂ�
	while( spStream->vReadLine( buf , sizeof( buf ) ) )
	{
		// �}�e���A����
		n = SkyScanf( buf , _T( "newmtl %s" ) ,  mtl[ mtlCount + 1 ].name, sizeof( mtl[ mtlCount + 1 ].name ) );
		if ( n > 0 )
		{
			++mtlCount;
            SKY_ASSERT_MSG( !( mtlCount >= OBJ_MTL_MAX_NUM ) , _T( "Mesh reading failed.\n" ) ); //�v���~�e�B�u�͂R�ȉ��͋��e���Ȃ��B

			for (s32 i = 0 ; i < 3 ; ++i )
			{
				mtl[ mtlCount ].Kd[ i ] = 1.0f;
				mtl[ mtlCount ].Ks[ i ] = 1.0f;
				mtl[ mtlCount ].Ka[ i ] = 1.0f;
			}
			mtl[ mtlCount ].d             = 1.0f;
			mtl[ mtlCount ].Ns            = 0.0f;
			mtl[ mtlCount ].Ni            = 1.0f;
			mtl[ mtlCount ].map_Kd[ 0 ]   = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].map_Kd[ 0 ] , sizeof( mtl[ mtlCount ].map_Kd ) );
			mtl[ mtlCount ].map_Ks[ 0 ]   = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].map_Ks[ 0 ] , sizeof( mtl[ mtlCount ].map_Ks ) );
			mtl[ mtlCount ].map_Ka[ 0 ]   = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].map_Ka[ 0 ] , sizeof( mtl[ mtlCount ].map_Ka ) );
			mtl[ mtlCount ].map_Bump[ 0 ] = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].map_Bump[ 0 ] , sizeof( mtl[ mtlCount ].map_Bump ) );
			mtl[ mtlCount ].map_D[ 0 ]    = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].map_D[ 0 ] , sizeof( mtl[ mtlCount ].map_D ) );
			mtl[ mtlCount ].refl[ 0 ]     = NULL;
			CMemoryUtil_::ClearZero( &mtl[ mtlCount ].refl[ 0 ] , sizeof( mtl[ mtlCount ].refl ) );
			continue;
		}
		if ( mtlCount < 0 )	continue;
		// �f�B�t���[�Y�F
		n = SkyScanf( buf, _T( "Kd %f %f %f" ) , &mtl[ mtlCount ].Kd[ 0 ] , &mtl[ mtlCount ].Kd[ 1 ], &mtl[ mtlCount ].Kd[ 2 ] );
		if ( n > 0 ) continue;
		// �X�y�L�����F
		n = SkyScanf( buf, _T( "Ks %f %f %f" ) , &mtl[ mtlCount ].Ks[ 0 ] , &mtl[ mtlCount ].Ks[ 1 ], &mtl[ mtlCount ].Ks[ 2 ] );
		if ( n > 0 ) continue;
		// �A���r�G���g�F
		n = SkyScanf( buf, _T( "Ka %f %f %f" ) , &mtl[ mtlCount ].Ka[ 0 ] , &mtl[ mtlCount ].Ka[ 1 ], &mtl[ mtlCount ].Ka[ 2 ] );
		if ( n > 0 ) continue;
		// �����x
		n = SkyScanf( buf, _T( "d %f" ) , &mtl[ mtlCount ].d );
		if ( n > 0 ) continue;
		// ����
		n = SkyScanf( buf, _T( "Ns %f" ) , &mtl[ mtlCount ].Ns );
		if ( n > 0 ) continue;
		// ���ܗ�
		n = SkyScanf( buf, _T( "Ni %f" ) , &mtl[ mtlCount ].Ni );
		if ( n > 0 ) continue;
		// �f�B�t���[�Y�E�}�b�v
		n = SkyScanf( buf, _T( "map_Kd %s" ) , mtl[ mtlCount ].map_Kd, sizeof( mtl[ mtlCount ].map_Kd ) / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// �o���v�E�}�b�v
		n = SkyScanf( buf, _T( "map_Bump %s" ) , mtl[ mtlCount ].map_Bump, sizeof( mtl[ mtlCount ].map_Bump )  / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// �X�y�L�����E�}�b�v
		n = SkyScanf( buf, _T( "map_Ks %s" ) , mtl[ mtlCount ].map_Ks, sizeof( mtl[ mtlCount ].map_Ks ) / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// ���}�b�v
		n = SkyScanf( buf, _T( "map_Ka %s" ) , mtl[ mtlCount ].map_Ka, sizeof( mtl[ mtlCount ].map_Ka ) / sizeof( skyString ) );
		if ( n > 0 ) continue;
	}
	++mtlCount;

#if OFF_

	for( s32 i = 0 ; i < mtlCount ; i++ )
	{
		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].name );
		SKY_PRINTF( _T( "%f %f %f \n" ) , mtl[ i ].Kd[ 0 ] , mtl[ i ].Kd[ 1 ] , mtl[ i ].Kd[ 2 ] );
		SKY_PRINTF( _T( "%f %f %f \n" ) , mtl[ i ].Ks[ 0 ] , mtl[ i ].Ks[ 1 ] , mtl[ i ].Ks[ 2 ] );
		SKY_PRINTF( _T( "%f %f %f \n" ) , mtl[ i ].Ka[ 0 ] , mtl[ i ].Ka[ 1 ] , mtl[ i ].Ka[ 2 ] );
		SKY_PRINTF( _T( "%f \n" ) , mtl[ i ].d );
		SKY_PRINTF( _T( "%f \n" ) , mtl[ i ].Ns );
		SKY_PRINTF( _T( "%f \n" ) , mtl[ i ].Ni );
		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].map_Kd );
//		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].map_Ks );
//		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].map_Ka );
//		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].map_Bump );
//		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].map_D );
//		SKY_PRINTF( _T( "%s \n" ) , mtl[ i ].refl );
	}

#endif

	spStream->vClose();
}

static CTexturePtr LoadTexture_( const skyString *path )
{
	if ( CStringUtil_::Len( path ) == 0 )
	{
		return CTexturePtr();
	}

	CTexturePtr spResult;

	//���[�g�p�X���������猋������
	skyString texturePath[ _MAX_PATH ];
	if ( CGraphicFilePathManager_::GetTextureFilePathRoot().IsEmpty() == skyFalse )
	{
		CGraphicFilePathManager_::ConcatTextureFilePathRoot( texturePath , _MAX_PATH , path );
	}
	else
	{
		CStringUtil_::Copy( texturePath , _MAX_PATH , path );
	}

	//���łɃf�[�^�x�[�X�ɂ������炻����g�p����
	if ( CDataBase_::IsCreatedTexture( CHS( texturePath ) ) )
	{
		spResult = CDataBase_::GetTexture( CHS( texturePath ) );
	}
	else
	{
		spResult = CTextureCreateName_( CHS( texturePath ) );
		CDataBase_::AddTexture( spResult ); //�f�[�^�x�[�X�Ɋi�[
	}

	return spResult;
}

CMeshContenaPtr CWavefrontOBJLoader::Load( const hash::CHashStringPtr &path , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex )
{
	CMeshContenaPtr spResult;

	//�t�@�C����DataBase�ɂ�������N���[�����ďI��
	if ( CDataBase_::IsCreatedMesh( path ) )
	{
		return CDataBase_::GetMesh( path );
	}
	else
	{
		spResult = CMeshContenaCreate_();
	}

	//���������
	vrtxtype vertexCount = 0;
	math::CVector3< f32 > *vertex = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );
	math::CVector2< f32 > *uv     = NEW_ARRAY__( math::CVector2< f32 > , VERTEX_NUM_MAX );
	math::CVector3< f32 > *normal = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );
	vrtxtype indexCount = 0;
	vrtxtype *index               = NEW_ARRAY__( vrtxtype , VERTEX_NUM_MAX );
	vrtxtype groupCnt = 0;
	OBJ_GROUP *mltGroup             = NEW_ARRAY__( OBJ_GROUP , OBJ_GROUP_NUM_MAX );
	skyString mtlFileName[ _MAX_PATH ];

	//���[�g�p�X���������猋������
	skyString objPath[ _MAX_PATH ];
	if ( CGraphicFilePathManager_::GetObjFilePathRoot().IsEmpty() == skyFalse )
	{
		CGraphicFilePathManager_::ConcatObjFilePathRoot( objPath , _MAX_PATH , path->GetString() );
	}
	else
	{
		CStringUtil_::Copy( objPath , _MAX_PATH , path->GetString() );
	}

	//�I�u�W�F�t�@�C���̃��[�h
    LoadObj_( CHS( objPath ) ,
	          vertex , vertexCount ,
	          uv , normal ,
			  index , indexCount ,
			  mltGroup , groupCnt ,
			  mtlFileName );


	//���_�o�b�t�@�̍쐬
	spVertex->Name.SetName( CHS( path ) );
	spVertex->Initialize( vertexCount );

	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		spVertex->GetPositionStream()->WritePosition( i , math::CBasicVector3( vertex[ i ].X , vertex[ i ].Y , vertex[ i ].Z ) ); //�V�X�e���ƊO���f�[�^�̌^���Ⴄ�͈̂Ӗ�������B
	}

#if OFF_

	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "v { %f , %f , %f } , \n" ) , vertex[ i ].X , vertex[ i ].Y , vertex[ i ].Z );
	}
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "n { %f , %f , %f } , \n" ) , normal[ i ].X , normal[ i ].Y , normal[ i ].Z );
	}

#endif

	//�C���f�b�N�X�o�b�t�@�̍쐬
	spIndex->Name.SetName( CHS( path ) );
	spIndex->Initialize( indexCount );

	for( vrtxtype i = 0 ; i < indexCount ; i++ )
	{
		spIndex->SetIndex( i , index[ i ] );
	}

#if OFF_

	SKY_PRINTF( _T( "{" ) );
	for( vrtxtype i = 0 ; i < indexCount ; i++ )
	{
		SKY_PRINTF( _T( "%d," ) , index[ i ] );
	}
	SKY_PRINTF( _T( "}\n" ) );

#endif

//---------------------------------------------------------------------------------

	//���b�V�����쐬����
	spResult->AttachMesh( CMeshCreate_() );
	spResult->GetMesh()->AttachMaterialContena( CMaterialContenaCreate_() );
	spResult->GetMesh()->AttachVertexBuffer( spVertex );
	spResult->GetMesh()->AttachIndexBuffer( spIndex );

	s32 mtlCount = 0;
	OBJ_MTL *mtl = NEW_ARRAY__( OBJ_MTL , OBJ_MTL_MAX_NUM );

	//�}�e���A���t�@�C���̃��[�h
	skyString mtlPath[ _MAX_PATH ];
	if ( CGraphicFilePathManager_::GetMtlFilePathRoot().IsEmpty() == skyFalse )
	{
		CGraphicFilePathManager_::ConcatMtlFilePathRoot( mtlPath , _MAX_PATH , mtlFileName );
	}
	else
	{
		CStringUtil_::Copy( mtlPath , _MAX_PATH , mtlFileName );
	}
	LoadMTL_( CHS( mtlPath ) , mtl , mtlCount );

	CTexturePtr pTexture;
	CMaterialPtrHash hash( 7 );
	for ( s32 i = 0 ; i < mtlCount ; i++ )
	{
		CMaterialPtr mat = CMaterialCreateName_( CHS( mtl[ i ].name ) );

		// �f�B�t���[�Y�F�B�l�́u0�v�`�u1�v�B
		mat->Diffuse.r = mtl[ i ].Kd[ 0 ];
		mat->Diffuse.g = mtl[ i ].Kd[ 1 ];
		mat->Diffuse.b = mtl[ i ].Kd[ 2 ];
		mat->Diffuse.a = 1.0f;

		// �X�y�L�����F�B�l�́u0�v�`�u1�v�B
		mat->Specular.r = mtl[ i ].Ks[ 0 ];
		mat->Specular.g = mtl[ i ].Ks[ 1 ];
		mat->Specular.b = mtl[ i ].Ks[ 2 ];
		mat->Specular.a = 1.0f;

		// �A���r�G���g(��)�F�B�l�́u0�v�`�u1�v�B
		mat->Ambient.r = mtl[ i ].Ka[ 0 ];
		mat->Ambient.g = mtl[ i ].Ka[ 1 ];
		mat->Ambient.b = mtl[ i ].Ka[ 2 ];
		mat->Ambient.a = 1.0f;

		// �����x�B�l�́u0�v(����)�`�u1�v(�s����)�B
		mat->Alpha = mtl[ i ].d;

		// ����B�l�́A�u0�v�`�B 
		mat->Shininess = mtl[ i ].Ns;

		// ���ܗ��B�l�́u1�v�ȏ�B�u1�v�͋��܂Ȃ��B
		mat->Refractive = mtl[ i ].Ni;

		// �f�B�t���[�Y�E�}�b�v
		mat->AttachDiffuseMap( LoadTexture_( mtl[ i ].map_Kd ) );

		// �X�y�L�����E�}�b�v
		mat->AttachSpecularMap( LoadTexture_( mtl[ i ].map_Ks ) );

		// ���}�b�v
		mat->AttachAmbientMap( LoadTexture_( mtl[ i ].map_Ka ) );

		// �o���v�E�}�b�v
		mat->AttachBumpMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// �����}�b�v
		mat->AttachTransparenceMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// ���˃}�b�v
		mat->AttachRefractiveMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// �f�B�t���[�Y�E�}�b�v�B��ʓI�ȃe�N�X�`���B
		mat->AttachDiffuseMap( LoadTexture_( mtl[ i ].map_Kd ) );
		mat->AttachSpecularMap( LoadTexture_( mtl[ i ].map_Ks ) );
		mat->AttachAmbientMap( LoadTexture_( mtl[ i ].map_Ka ) );
		mat->AttachBumpMap( LoadTexture_( mtl[ i ].map_Bump ) );
		mat->AttachTransparenceMap( LoadTexture_( mtl[ i ].map_D ) );
		mat->AttachRefractiveMap( LoadTexture_( mtl[ i ].refl ) );

		//���O���\�z�i�t�@�C���p�X#�}�e���A�����Fhogehoge/hog.mtl#name �j
		skyString name[ _MAX_PATH ];
		CStringUtil_::Copy( name , _MAX_PATH , mtlPath );
		CStringUtil_::Cat( name , _T( "#" ) );
		CStringUtil_::Cat( name , mtl[ i ].name );
		mat->Name.SetName( CHS( name ) );
		CDataBase_::AddMaterial( mat ); //�f�[�^�x�[�X�Ɋi�[
		hash.Set( mat->Name.GetName() , mat );
	}

//---------------------------------------------------------------------------------

	//�}�e���A���O���[�v�̍쐬
	for( s32 i = 0 ; i < groupCnt ; ++i )
	{
		//���O���\�z�i�t�@�C���p�X#�}�e���A�����Fhogehoge/hog.mtl#name �j
		skyString name[ _MAX_PATH ];
		CStringUtil_::Copy( name , _MAX_PATH , mtlPath );
		CStringUtil_::Cat( name , _T( "#" ) );
		CStringUtil_::Cat( name , mltGroup[ i ].mtl );

		CMaterialPtr mat = hash.Get( CHS( name ) );
        SKY_ASSERT_MSG( !mat.IsEmpty() , _T( "Material maiking failed. \n" ) );

		spResult->GetMesh()->GetMaterialContena()->AddMaterial( mat , ( vrtxtype )mltGroup[ i ].startIndex , ( vrtxtype )mltGroup[ i ].countIndex );
	}

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------

	//��ڂ̃}�e���A���ɖ@����UV��ݒ�
	spVertex->CreateNormalStream( spVertex->GetVertexNum() );
	spVertex->CreateUvStream( spVertex->GetVertexNum() );
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		spVertex->GetNormalStream()->WriteNormal( i , math::CBasicVector3( normal[ i ].X , normal[ i ].Y , normal[ i ].Z ) ); //�V�X�e���ƊO���f�[�^�̌^���Ⴄ�͈̂Ӗ�������B
		spVertex->GetUvStream()->WriteUv( i , math::CBasicVector2( uv[ i ].X , uv[ i ].Y ) );                         //�V�X�e���ƊO���f�[�^�̌^���Ⴄ�͈̂Ӗ�������B
	}

	//�Q�ڈȍ~�̃}�e���A���ɂ�1�ڂƓ����@����ݒ肷��
	if ( groupCnt > 1 )
	{
		for( s32 i = 1 ; i < groupCnt ; ++i )
		{
			spResult->GetMesh( i )->AttachStream( eVertexBufferStreamType_Normal , spVertex->GetNormalStream() );
			spResult->GetMesh( i )->AttachStream( eVertexBufferStreamType_Uv     , spVertex->GetUvStream() );
		}
	}

//---------------------------------------------------------------------------------

	spResult->Name.SetName( CHS( objPath ) );
	CDataBase_::AddMeshContena( spResult );

	DEL_ARRAY vertex;
	DEL_ARRAY uv;
	DEL_ARRAY normal;
	DEL_ARRAY index;
	DEL_ARRAY mtl;
	DEL_ARRAY mltGroup;

	return spResult;
}

} } }
