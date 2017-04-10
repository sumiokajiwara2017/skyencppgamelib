#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

#include "SkyGraphicLib/DataBase.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

// OBJファイル内のグループ情報
#define OBJ_GROUP_NAME_LEN_MAX ( 80 )
#define OBJ_GROUP_MTL_LEN_MAX  ( 80 )
#define OBJ_GROUP_NUM_MAX      ( 256 )
struct OBJ_GROUP
{
	skyString  name[ OBJ_GROUP_NAME_LEN_MAX ];  // グループ名
	skyString  mtl[ OBJ_GROUP_MTL_LEN_MAX ];    // マテリアル名
	s32        startIndex;                        // グループの開始インデックス
	s32        countIndex;                        // グループに含まれるインデックス数
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
	//事前にロードしてストリームで渡す
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

//--------------------------objファイルロード部分---------------------------------▽

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE	

	//BOMの分進める
	spStream->vSetSeek( BOM_SIZE );

#endif

	s32 n;
	//ストリームデータを一行ずつ読む
	while( spStream->vReadLine( buf , sizeof( buf ) ) )
	{
		// 座標
		n = SkyScanf( buf , _T( "v %f %f %f" ) , &vertexWork[ vertexWorkCnt ].X, &vertexWork[ vertexWorkCnt ].Y, &vertexWork[ vertexWorkCnt ].Z );
		if ( n > 0 )
		{ 
			++vertexWorkCnt; 
            SKY_ASSERT_MSG( vertexWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// テクスチャ座標
		n = SkyScanf( buf , _T( "vt %f %f" ) , &uvWork[ uvWorkCnt ].X, &uvWork[ uvWorkCnt ].Y );
		if ( n > 0 )
		{ 
			++uvWorkCnt; 
            SKY_ASSERT_MSG( uvWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// 法線ベクトル
		n = SkyScanf( buf , _T( "vn %f %f %f" ) , &normalWork[ normalWorkCnt ].X, &normalWork[ normalWorkCnt ].Y, &normalWork[ normalWorkCnt ].Z );
		if ( n > 0 )
		{ 
			++normalWorkCnt; 
            SKY_ASSERT_MSG( normalWorkCnt <= VERTEX_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// グループ名
		n = SkyScanf( buf , _T( "g %s" ) ,  mltGroup[ groupCnt ].name, sizeof( mltGroup[ groupCnt ].name ) / sizeof( skyString ) );
		if ( n > 0 )
		{//次のグループを初期化しておく
			mltGroup[ groupCnt ].mtl[ 0 ]  = NULL;
			mltGroup[ groupCnt ].startIndex  = indexWorkCnt;
			mltGroup[ groupCnt ].countIndex  = 0;
			++groupCnt;
            SKY_ASSERT_MSG( groupCnt <= OBJ_GROUP_NUM_MAX , _T( "Mesh reading failed.\n" ) );
			continue;
		}

		// マテリアル名
		n = SkyScanf( buf , _T( "usemtl %s" ) ,  mltGroup[ groupCnt - 1 ].mtl, sizeof( mltGroup[ groupCnt - 1 ].mtl ) / sizeof( skyString ) );
		if ( n > 0 ) continue;

		// マテリアル・ファイル名
		n = SkyScanf( buf , _T( "mtllib %s" ) , mtlFileName , _MAX_PATH / sizeof( skyString ) );
		if ( n > 0 ) continue;

		// 面( 三角形または四角形と仮定 )
		n = SkyScanf( buf , _T( "f %s %s %s %s" ) ,
			 op[ 0 ] , sizeof( op[ 0 ] )  / sizeof( skyString ) , op[ 1 ] , sizeof( op[ 1 ] ) / sizeof( skyString ) ,
			 op[ 2 ] , sizeof( op[ 2 ] ) / sizeof( skyString ) , op[ 3 ] , sizeof( op[ 3 ] ) / sizeof( skyString ) );
		if ( n == 0 ) continue;
        SKY_ASSERT_MSG( !( n < 3 ) , _T( "Mesh reading failed.\n" ) ); //プリミティブは３以下は許容しない。

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
            SKY_ASSERT_MSG( !( indexWorkCnt + 3 > VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //プリミティブは３以下は許容しない。

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

    // 近接頂点を結合する（最適化）
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

	// インデックス・バッファを組み立てる
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

        SKY_ASSERT_MSG( !( j >= VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //プリミティブは３以下は許容しない。

		if (j >= vertexCount)
		{
			indexWork2[ j ][ 0 ] = indexWork[ i ][ 0 ];
			indexWork2[ j ][ 1 ] = indexWork[ i ][ 1 ];
			indexWork2[ j ][ 2 ] = indexWork[ i ][ 2 ];
			++vertexCount;
		}

		index[ indexCount ] = j; ++indexCount;

        SKY_ASSERT_MSG( !( indexCount >= VERTEX_NUM_MAX ) , _T( "Mesh reading failed.\n" ) ); //プリミティブは３以下は許容しない。
	}

	// 頂点バッファを組み立てる
	for ( vrtxtype i = 0 ; i < vertexCount ; ++i )
	{
		--indexWork2[ i ][ 0 ]; --indexWork2[ i ][ 1 ]; --indexWork2[ i ][ 2 ];
	}
	for ( vrtxtype i = 0 ; i < vertexCount ; ++i )
	{
		math::CVector3< f32 > *pP = &vertex[ i ]; 
		math::CVector2< f32 > *pT = &uv[ i ]; 
		math::CVector3< f32 > *pN = &normal[ i ]; 

		// 座標
		if ( ( indexWork2[ i ][ 0 ] < 0 ) || ( indexWork2[ i ][ 0 ] >= vertexWorkCnt ) )
		{
			 pP->X = pP->Y = pP->Z = 0.0f;
		}
		else
		{
			*pP = vertexWork[ indexWork2[ i ][ 0 ] ];
		}

		// テクスチャ座標
		if ( ( indexWork2[ i ][ 1 ] < 0 ) || ( indexWork2[ i ][ 1 ] >= uvWorkCnt ) )
		{
			 pT->X = pT->Y = 0.0f;
		}
		else
		{
			*pT = uvWork[ indexWork2 [ i ][ 1 ] ];
		}

		// 法線ベクトル
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

	//頂点バッファを出力する
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f %f \n" ) , vertex[ i ].X , vertex[ i ].Y , vertex[ i ].Z );
	}

	//法線バッファを出力する
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f %f \n" ) , normal[ i ].X , normal[ i ].Y , normal[ i ].Z );
	}

	//ＵＶを出力する
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		SKY_PRINTF( _T( "%f %f \n" ) , uv[ i ].X , uv[ i ].Y );
	}

	//インデックスバッファを出力する
	for ( vrtxtype i = 0; i< indexCount; ++i )
	{
		SKY_PRINTF( _T( "%d \n" ) , index[ i ] );
	}

#endif

	DEL_ARRAY vertexWork;
	DEL_ARRAY uvWork;
	DEL_ARRAY normalWork;

	spStream->vClose();

//--------------------------objファイルロード部分---------------------------------△

}

// マテリアル・データ
#define OBJ_MTL_TEXTURE_NAME_LEN ( 80 )
#define OBJ_MTL_MAX_NUM          ( 256 )
struct OBJ_MTL
{
	skyString name[ OBJ_MTL_TEXTURE_NAME_LEN ];     // マテリアル名
	f32 Kd[ 3 ];                             // ディフューズ色。値は「0」～「1」。
	f32 Ks[ 3 ];                             // スペキュラ色。値は「0」～「1」。
	f32 Ka[ 3 ];                             // アンビエント(環境)色。値は「0」～「1」。
	f32 d;                                   // 透明度。値は「0」(透明)～「1」(不透明)。
	f32 Ns;                                  // 光沢。値は、「0」～。 
	f32 Ni;                                  // 屈折率。値は「1」以上。「1」は屈折なし。
	skyString map_Kd[ OBJ_MTL_TEXTURE_NAME_LEN ];   // ディフューズ・マップ。一般的なテクスチャ。
	skyString map_Ks[ OBJ_MTL_TEXTURE_NAME_LEN ];   // スペキュラ・マップ。
	skyString map_Ka[ OBJ_MTL_TEXTURE_NAME_LEN ];   // 環境マップ。
	skyString map_Bump[ OBJ_MTL_TEXTURE_NAME_LEN ]; // バンプ・マップ。
	skyString map_D[ OBJ_MTL_TEXTURE_NAME_LEN ];    // 透明マップ。
	skyString refl[ OBJ_MTL_TEXTURE_NAME_LEN ];     // 反射マップ。
};

static void LoadMTL_( const hash::CHashStringPtr &path , OBJ_MTL *mtl , s32 &mtlCount )
{
	mtlCount = -1;

	//事前にロードしてストリームで渡す
	stream::IStreamPtr spStream = CFileUtil_::LoadFile( path );
	spStream->vOpen();

	static  skyString buf[ 1024 ];
    static	skyString op[ 4 ][ 256 ];

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE	

	//BOMの分進める
	spStream->vSetSeek( BOM_SIZE );

#endif

	s32 n;
	//ストリームデータを一行ずつ読む
	while( spStream->vReadLine( buf , sizeof( buf ) ) )
	{
		// マテリアル名
		n = SkyScanf( buf , _T( "newmtl %s" ) ,  mtl[ mtlCount + 1 ].name, sizeof( mtl[ mtlCount + 1 ].name ) );
		if ( n > 0 )
		{
			++mtlCount;
            SKY_ASSERT_MSG( !( mtlCount >= OBJ_MTL_MAX_NUM ) , _T( "Mesh reading failed.\n" ) ); //プリミティブは３以下は許容しない。

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
		// ディフューズ色
		n = SkyScanf( buf, _T( "Kd %f %f %f" ) , &mtl[ mtlCount ].Kd[ 0 ] , &mtl[ mtlCount ].Kd[ 1 ], &mtl[ mtlCount ].Kd[ 2 ] );
		if ( n > 0 ) continue;
		// スペキュラ色
		n = SkyScanf( buf, _T( "Ks %f %f %f" ) , &mtl[ mtlCount ].Ks[ 0 ] , &mtl[ mtlCount ].Ks[ 1 ], &mtl[ mtlCount ].Ks[ 2 ] );
		if ( n > 0 ) continue;
		// アンビエント色
		n = SkyScanf( buf, _T( "Ka %f %f %f" ) , &mtl[ mtlCount ].Ka[ 0 ] , &mtl[ mtlCount ].Ka[ 1 ], &mtl[ mtlCount ].Ka[ 2 ] );
		if ( n > 0 ) continue;
		// 透明度
		n = SkyScanf( buf, _T( "d %f" ) , &mtl[ mtlCount ].d );
		if ( n > 0 ) continue;
		// 光沢
		n = SkyScanf( buf, _T( "Ns %f" ) , &mtl[ mtlCount ].Ns );
		if ( n > 0 ) continue;
		// 屈折率
		n = SkyScanf( buf, _T( "Ni %f" ) , &mtl[ mtlCount ].Ni );
		if ( n > 0 ) continue;
		// ディフューズ・マップ
		n = SkyScanf( buf, _T( "map_Kd %s" ) , mtl[ mtlCount ].map_Kd, sizeof( mtl[ mtlCount ].map_Kd ) / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// バンプ・マップ
		n = SkyScanf( buf, _T( "map_Bump %s" ) , mtl[ mtlCount ].map_Bump, sizeof( mtl[ mtlCount ].map_Bump )  / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// スペキュラ・マップ
		n = SkyScanf( buf, _T( "map_Ks %s" ) , mtl[ mtlCount ].map_Ks, sizeof( mtl[ mtlCount ].map_Ks ) / sizeof( skyString ) );
		if ( n > 0 ) continue;
		// 環境マップ
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

	//ルートパスがあったら結合する
	skyString texturePath[ _MAX_PATH ];
	if ( CGraphicFilePathManager_::GetTextureFilePathRoot().IsEmpty() == skyFalse )
	{
		CGraphicFilePathManager_::ConcatTextureFilePathRoot( texturePath , _MAX_PATH , path );
	}
	else
	{
		CStringUtil_::Copy( texturePath , _MAX_PATH , path );
	}

	//すでにデータベースにあったらそれを使用する
	if ( CDataBase_::IsCreatedTexture( CHS( texturePath ) ) )
	{
		spResult = CDataBase_::GetTexture( CHS( texturePath ) );
	}
	else
	{
		spResult = CTextureCreateName_( CHS( texturePath ) );
		CDataBase_::AddTexture( spResult ); //データベースに格納
	}

	return spResult;
}

CMeshContenaPtr CWavefrontOBJLoader::Load( const hash::CHashStringPtr &path , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex )
{
	CMeshContenaPtr spResult;

	//ファイルがDataBaseにあったらクローンして終了
	if ( CDataBase_::IsCreatedMesh( path ) )
	{
		return CDataBase_::GetMesh( path );
	}
	else
	{
		spResult = CMeshContenaCreate_();
	}

	//回収する情報
	vrtxtype vertexCount = 0;
	math::CVector3< f32 > *vertex = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );
	math::CVector2< f32 > *uv     = NEW_ARRAY__( math::CVector2< f32 > , VERTEX_NUM_MAX );
	math::CVector3< f32 > *normal = NEW_ARRAY__( math::CVector3< f32 > , VERTEX_NUM_MAX );
	vrtxtype indexCount = 0;
	vrtxtype *index               = NEW_ARRAY__( vrtxtype , VERTEX_NUM_MAX );
	vrtxtype groupCnt = 0;
	OBJ_GROUP *mltGroup             = NEW_ARRAY__( OBJ_GROUP , OBJ_GROUP_NUM_MAX );
	skyString mtlFileName[ _MAX_PATH ];

	//ルートパスがあったら結合する
	skyString objPath[ _MAX_PATH ];
	if ( CGraphicFilePathManager_::GetObjFilePathRoot().IsEmpty() == skyFalse )
	{
		CGraphicFilePathManager_::ConcatObjFilePathRoot( objPath , _MAX_PATH , path->GetString() );
	}
	else
	{
		CStringUtil_::Copy( objPath , _MAX_PATH , path->GetString() );
	}

	//オブジェファイルのロード
    LoadObj_( CHS( objPath ) ,
	          vertex , vertexCount ,
	          uv , normal ,
			  index , indexCount ,
			  mltGroup , groupCnt ,
			  mtlFileName );


	//頂点バッファの作成
	spVertex->Name.SetName( CHS( path ) );
	spVertex->Initialize( vertexCount );

	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		spVertex->GetPositionStream()->WritePosition( i , math::CBasicVector3( vertex[ i ].X , vertex[ i ].Y , vertex[ i ].Z ) ); //システムと外部データの型が違うのは意味がある。
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

	//インデックスバッファの作成
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

	//メッシュを作成する
	spResult->AttachMesh( CMeshCreate_() );
	spResult->GetMesh()->AttachMaterialContena( CMaterialContenaCreate_() );
	spResult->GetMesh()->AttachVertexBuffer( spVertex );
	spResult->GetMesh()->AttachIndexBuffer( spIndex );

	s32 mtlCount = 0;
	OBJ_MTL *mtl = NEW_ARRAY__( OBJ_MTL , OBJ_MTL_MAX_NUM );

	//マテリアルファイルのロード
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

		// ディフューズ色。値は「0」～「1」。
		mat->Diffuse.r = mtl[ i ].Kd[ 0 ];
		mat->Diffuse.g = mtl[ i ].Kd[ 1 ];
		mat->Diffuse.b = mtl[ i ].Kd[ 2 ];
		mat->Diffuse.a = 1.0f;

		// スペキュラ色。値は「0」～「1」。
		mat->Specular.r = mtl[ i ].Ks[ 0 ];
		mat->Specular.g = mtl[ i ].Ks[ 1 ];
		mat->Specular.b = mtl[ i ].Ks[ 2 ];
		mat->Specular.a = 1.0f;

		// アンビエント(環境)色。値は「0」～「1」。
		mat->Ambient.r = mtl[ i ].Ka[ 0 ];
		mat->Ambient.g = mtl[ i ].Ka[ 1 ];
		mat->Ambient.b = mtl[ i ].Ka[ 2 ];
		mat->Ambient.a = 1.0f;

		// 透明度。値は「0」(透明)～「1」(不透明)。
		mat->Alpha = mtl[ i ].d;

		// 光沢。値は、「0」～。 
		mat->Shininess = mtl[ i ].Ns;

		// 屈折率。値は「1」以上。「1」は屈折なし。
		mat->Refractive = mtl[ i ].Ni;

		// ディフューズ・マップ
		mat->AttachDiffuseMap( LoadTexture_( mtl[ i ].map_Kd ) );

		// スペキュラ・マップ
		mat->AttachSpecularMap( LoadTexture_( mtl[ i ].map_Ks ) );

		// 環境マップ
		mat->AttachAmbientMap( LoadTexture_( mtl[ i ].map_Ka ) );

		// バンプ・マップ
		mat->AttachBumpMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// 透明マップ
		mat->AttachTransparenceMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// 反射マップ
		mat->AttachRefractiveMap( LoadTexture_( mtl[ i ].map_Bump ) );

		// ディフューズ・マップ。一般的なテクスチャ。
		mat->AttachDiffuseMap( LoadTexture_( mtl[ i ].map_Kd ) );
		mat->AttachSpecularMap( LoadTexture_( mtl[ i ].map_Ks ) );
		mat->AttachAmbientMap( LoadTexture_( mtl[ i ].map_Ka ) );
		mat->AttachBumpMap( LoadTexture_( mtl[ i ].map_Bump ) );
		mat->AttachTransparenceMap( LoadTexture_( mtl[ i ].map_D ) );
		mat->AttachRefractiveMap( LoadTexture_( mtl[ i ].refl ) );

		//名前を構築（ファイルパス#マテリアル名：hogehoge/hog.mtl#name ）
		skyString name[ _MAX_PATH ];
		CStringUtil_::Copy( name , _MAX_PATH , mtlPath );
		CStringUtil_::Cat( name , _T( "#" ) );
		CStringUtil_::Cat( name , mtl[ i ].name );
		mat->Name.SetName( CHS( name ) );
		CDataBase_::AddMaterial( mat ); //データベースに格納
		hash.Set( mat->Name.GetName() , mat );
	}

//---------------------------------------------------------------------------------

	//マテリアルグループの作成
	for( s32 i = 0 ; i < groupCnt ; ++i )
	{
		//名前を構築（ファイルパス#マテリアル名：hogehoge/hog.mtl#name ）
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

	//一つ目のマテリアルに法線とUVを設定
	spVertex->CreateNormalStream( spVertex->GetVertexNum() );
	spVertex->CreateUvStream( spVertex->GetVertexNum() );
	for( vrtxtype i = 0 ; i < vertexCount ; i++ )
	{
		spVertex->GetNormalStream()->WriteNormal( i , math::CBasicVector3( normal[ i ].X , normal[ i ].Y , normal[ i ].Z ) ); //システムと外部データの型が違うのは意味がある。
		spVertex->GetUvStream()->WriteUv( i , math::CBasicVector2( uv[ i ].X , uv[ i ].Y ) );                         //システムと外部データの型が違うのは意味がある。
	}

	//２つ目以降のマテリアルには1つ目と同じ法線を設定する
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
