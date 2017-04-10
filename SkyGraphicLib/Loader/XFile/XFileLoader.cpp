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
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CWeight_& operator =( const CWeight_& src )
	{

		Name             = src.Name;
		m_Matrix         = src.m_Matrix;

		//Weightsのコピー
		m_Weights.Clear();
		for( u32 i = 0 ; i < src.m_Weights.GetNum() ; i++ )
		{
			m_Weights.Add( src.m_Weights[ i ] );
		}

		//Indicesのコピー
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
	//ツリー構造
	CFrame_*                m_ParentFrame;
	CFramePtrList           m_ChildFrame;

	//コアデータ
	hash::CHashStringPtr    m_Name;
	math::CMatrix4x3        m_Matrix;

	//MeshやマテリアルはModelとして保存
	CMeshPtr                m_spMesh;

	//最終的に骨情報構築にも使用される
	CVector< CWeight_ >     m_WeightData; 

	//現在使われておりませんが一応読んでます。
	CVectorPrimitive< u16 > m_FaceData;
	CVector< CFace_ > m_MeshSubSetData; 
};

//アニメーション関連
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
	/// 代入演算子オーバーライド
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

//ロード
struct CLoadCacheData_
{
	//Xファイルをトークン単位で読み込む
	token::CTokenizerM               m_Token;

	//モデル
	CModelPtr                        m_spModel;

	//スケルトン
	math::CSkeletonPtr               m_spSkeleton;

	//フレームをロード順に積んでいく
	CFrame_*                         m_RootFrame;
	math::CJointPtr                  m_spPoseJointRoot;
	CFramePtrStack                   m_FrameStack;
	CFramePtrList                    m_FrameList; //後で一気に消すためのリスト

	//現在ロード中のメッシュ情報
	CMeshPtr                         m_CurrentMesh;

	//現在ロード中アニメーション情報（完了後m_spMotionDataContenaに格納）
	f32                              m_CurrentAnimationMaxTime;
	math::CAnimationDataPallet             m_CurrentAnimationPallet;

	//ロードしたアニメーション情報群
	math::CMotionDataContenaPtr m_spMotionDataContena;

	//読み捨て情報
	CVectorPrimitive< u16 >          m_FaceData;
	CVector< CFace_ >          m_MeshSubSetData; 

	CLoadCacheData_() : m_Token( 64 , 64 , 64 ) , m_FrameStack( 128 ) , m_RootFrame( skyNull ){}
};

//プロトタイプ
static void CheckNode( CLoadCacheData_ &cache );

//全く相手にしないノード
static void SkipNode( CLoadCacheData_ &cache )
{
	// '{'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
//      SKY_PRINTF( "Skip Token : %s", cache.m_Token.GetChar() );
		if ( strchr( cache.m_Token.GetChar(), '{' ) )
			break;
	}

	// ブロック数
	int count = 1;

	//　終端またはブロックが閉じ終わるまでループ
	while( !cache.m_Token.IsEnd() && count > 0 )
	{
		//　トークンを取得
		cache.m_Token.Next();
//      SKY_PRINTF( "Skip Token : %s", cache.m_Token.GetChar() );

		//　ブロック開始
		if ( strchr( cache.m_Token.GetChar(), '{' ) )
		{
			count++;
		}
		//　ブロック終了
		else if ( strchr( cache.m_Token.GetChar(), '}' ) )
		{
			count--;
		}		
	}

	//　終端の場合もあるのでチェック
	if ( count > 0 )
	{
		SKY_PRINTF( "Error : カッコが一致していません\n" );
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

	//最初のいっこめがルート
	if ( cache.m_RootFrame == skyNull )
	{
		cache.m_RootFrame = pFrame;
	}
	else
	{
		//上の子になる
		if ( cache.m_FrameStack.GetNum() > 0 ) 
		{
			cache.m_FrameStack.GetTop()->m_ChildFrame.AddTail( pFrame );
		}
	}

	//スタックに乗せる
	cache.m_FrameStack.Push( pFrame );

    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
        //　適当なフレームネームを取得
		char temp[255];
		std::sprintf( temp, "Joint%03d" , cache.m_FrameList.GetNum() );
		pFrame->m_Name = CHS( temp );
    }
    else
    {
    	//　フレーム名を取得
		pFrame->m_Name = CHS( cache.m_Token.GetChar() );
    
        // '{'が出るまでスキップ
    	while( !cache.m_Token.IsEnd() )
    	{
    		cache.m_Token.Next();
    		if ( cache.m_Token.Check( "{"  ) )
    			break;
    	}
    }

	//　次のトークンが"FrameTransformMatrix"であることを確認
	cache.m_Token.Next();
	if( cache.m_Token.Check( "FrameTransformMatrix" ) )
	{
		// '{'が出るまでスキップ
		while( !cache.m_Token.IsEnd() )
		{
			cache.m_Token.Next();
			if ( cache.m_Token.Check( "{" ) )
				break;
		}

		// 行列を取得
		dectype matrix[ 16 ] = { 0 };
		for( s32 i = 0 ; i < 16 ; i++ )
		{
			matrix[ i ] = cache.m_Token.GetNextFloat();
//			SKY_PRINTF( "FrameTransformMatrix[%d] : %f" , i , matrix[ i ] );
		}

		math::CMatrix4x3 mat( matrix );

		//上の子になる
		pFrame->m_Matrix = mat;

		// '}'が出るまでスキップ
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
	//　分けのわからんやつはスキップ
	else
	{
		SkipNode( cache );
	}

//-----子を捜す---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//このノードは終わった
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

//-----子を捜す---->

	//スタックから取る
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
		//名前をセット
		skyMString *name = cache.m_Token.GetChar();
//		SKY_PRINTF( _T( "Mesh Name : %s" ), name );
		spMesh->Name.SetName( CHS( name ) );

		//'{'が出るまでスキップ
		while( !cache.m_Token.IsEnd() )
		{
			cache.m_Token.Next();
			if ( strchr( cache.m_Token.GetChar(), '{' ) )
				break;
		}
	}
	else
	{
		//適当な名前をセット
		spMesh->Name.SetName( CHS( "XFile Mesh" ) );
	}

	//　頂点数を取得
	vrtxtype vertices_size = 0;
	vertices_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Vertices : %d", vertices_size );

	//頂点バッファを初期化
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

	//　インデックス数を取得
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

			//　インデックスを格納
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

			//　四角形→三角形2枚分に変換
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

	//インデックスバッファを格納
	spMesh->AttachIndexBuffer( spIndex );

	//頂点バッファを格納
	spMesh->AttachVertexBuffer( spVertex );

	if ( cache.m_FrameStack.GetNum() > 0 ) 
	{
		//最上位のフレームに登録
		cache.m_FrameStack.GetTop()->m_spMesh = spMesh;
	}
	else
	{
		//フレーム管理されていないMeshなので直接Modelに登録してしまう
		cache.m_spModel->GetMeshContena()->AttachMesh( spMesh );
	}

	//現在のメッシュに登録
	cache.m_CurrentMesh = spMesh;

//-----子を捜す---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//このノードは終わった
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

//-----子を捜す---->
}

static void ParseMeshNormals( CLoadCacheData_ &cache )
{
	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();
	CIndexBufferPtr spIndex = cache.m_CurrentMesh->GetIndexBuffer();

	//　'{'が出るまでスキップ
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//　法線ベクトル数を取得
	vrtxtype normal_size = 0;
	normal_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Normals : %d", normal_size );

	//　法線ベクトルを取得
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

	//　法線インデックス数を取得
	vrtxtype indices_size = 0;
	indices_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Normal Indices : %d", indices_size );

	//　法線インデックスを取得
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

			//　インデックスを格納
			for( vrtxtype j = 0 ; j < 3 ; j++ )
			{
				SKY_ASSERT( idx[ j ] < spVertex->GetVertexNum() );
//				SKY_PRINTF( "Normal Index[%d] : %d", i, idx[ j ] );
				indices.Add( ( vrtxtype )idx[ j ] );
			}
		}
		else if ( cache.m_Token.Check( "4" ) )
		{
			//　四角形→三角形2枚分に変換
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

	SKY_ASSERT( spIndex->GetIndexNum() == indices.GetNum() ); //絶対一致するはず

	//頂点数でループして各頂点に正しい法線をあてがう
	for( vrtxtype i = 0 ; i < indices.GetNum() ; i++ )
	{
		vrtxtype normalIndex = indices[ i ];
		vrtxtype vertexIndex = spIndex->Index( i );
		spVertex->GetNormalStream()->WriteNormal( vertexIndex , normals[ normalIndex ] );
	}

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----子は無いので自分自身をしっかり終わらせる---->
}

static void ParseMeshTextureCoords( CLoadCacheData_ &cache )
{
	//　'{'が出るまでスキップ
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//　テクスチャ座標数を取得
	vrtxtype texcoord_size = 0;
	texcoord_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of TexCoords : %d", texcoord_size );

	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();
	spVertex->CreateUvStream( spVertex->GetVertexNum() );

	SKY_ASSERT( spVertex->GetVertexNum() == texcoord_size );

	//　テクスチャ座標を取得
	for( vrtxtype i = 0 ; i < spVertex->GetVertexNum() ; i++ )
	{
		vertex::CVertexFmt_UV uv;
		uv.tu = cache.m_Token.GetNextFloat();
		uv.tv = cache.m_Token.GetNextFloat();

		spVertex->GetUvStream()->WriteUv( i , uv );
//		SKY_PRINTF( "TexCoord[%d] : %f, %f", i , uv.tu , uv.tv );
	}

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----子は無いので自分自身をしっかり終わらせる---->
}

static void ParseMeshVertexColors( CLoadCacheData_ &cache )
{
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	//　カラー数を取得
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

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----子は無いので自分自身をしっかり終わらせる---->
}

static void ParseMeshMaterialList( CLoadCacheData_ &cache )
{
	// '{'が出るまでスキップ
	while ( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

	// マテリアル数を取得
	vrtxtype material_size = 0;
	material_size = ( vrtxtype )cache.m_Token.GetNextInt();
//	SKY_PRINTF( "Number Of Materials : %d", material_size );

	// マテリアルインデックス数を取得
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
		//　マテリアルインデックス取得
        vrtxtype index  = ( vrtxtype )cache.m_Token.GetNextInt();
//      SKY_PRINTF( "Material Index[%d] : %d", i, index );

		//　正しい面のインデックスを算出
		face_index++;
		if ( cache.m_FaceData[ i ] == 4 )
		{
			face_index++;
		}

		//　以前のマテリアルインデックスと異なる場合
		if ( prev_index != index )
		{
			//　フェイス情報を追加
			CFace_ face;
			face.m_MaterialIndex = prev_index;
			face.m_StartIndex    = startIndex;
			face.m_EndIndex      = vertex_index;
			faceVector.Add( face );

			//一番大きなインデックスをエラーチェック用にとっておく
			max_index = ( max_index < index ) ? index : max_index;

    		//更新
			prev_index = index;

			//次の描画の開始インデックス
			startIndex = vertex_index;

			//面数を初期化
			face_count = 1;
			if ( cache.m_FaceData[ i ] == 4 )
			{
				face_count++;
			}
		}
		else
		{
			//面数をカウント
			face_count++;
			if ( cache.m_FaceData[ i ] == 4 )
			{
				face_count++;
			}
		}

		if ( cache.m_FaceData[ i ] == 4 )
		{//四角形面の場合は＋３インデックスが増えてる
			vertex_index += 6;
		}
		else
		{//３角形頂点の場合はインデックスも３
			vertex_index += cache.m_FaceData[ i ];
		}
    }

	//最後のフェース情報を格納
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

		//　"Material"が出るまでスキップ
		while( !cache.m_Token.IsEnd() )
		{
			if ( cache.m_Token.Check( "Material" ) )
				break;

			cache.m_Token.Next();
		}

		//　マテリアルを取得
		cache.m_Token.Next();
		if ( !cache.m_Token.Check( "{" ) )
		{
			//　名前をセット
			skyMString *name = cache.m_Token.GetNextChar();
//			SKY_PRINTF( "Material Name : %s", name );
			mat.m_spMaterial->Name.SetName( CHS( name ) );

			//　'{'が出るまでスキップ
			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "{" ) )
					break;
			}
		}
		else
		{
			//　適当な名前をセット
			mat.m_spMaterial->Name.SetName( CHS( "XFile Material" ) );
		}

		// Diffuse取得
		mat.m_spMaterial->Diffuse.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Diffuse.a = cache.m_Token.GetNextFloat();
//		SKY_PRINTF( "Diffuse : %f, %f, %f, %f" , spMaterial->Diffuse.r , spMaterial->Diffuse.g , spMaterial->Diffuse.b , spMaterial->Diffuse.a );

		// Power取得
		mat.m_spMaterial->Power = cache.m_Token.GetNextFloat();
//		SKY_PRINTF( "Power : %f" , spMaterial->Power );

		// Specular取得
		mat.m_spMaterial->Specular.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Specular.a = 1.0f;
//		SKY_PRINTF( "Specular : %f, %f, %f, %f" , spMaterial->Diffuse.r , spMaterial->Diffuse.g , spMaterial->Diffuse.b , spMaterial->Diffuse.a );

		// Emissive取得
		mat.m_spMaterial->Emissive.r = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.g = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.b = cache.m_Token.GetNextFloat();
		mat.m_spMaterial->Emissive.a = 1.0f;
//		SKY_PRINTF( "Specular : %f, %f, %f, %f" , spMaterial->Emissive.r , spMaterial->Emissive.g , spMaterial->Emissive.b , spMaterial->Emissive.a );

		// 次のトークンを取得しチェックする
		skyMString pathBuffer[ PATH_LEN_MAX ];
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "}" ) )
		{
			//マップ系はないようだ。
		}
		else if ( cache.m_Token.Check( "TextureFilename" ) )
		{
			// '{'が出るまでスキップ
			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "{" ) )
					break;
			}

			//　テクスチャファイル名取得
			skyMString *texture_filename = cache.m_Token.GetNextChar();
//			SKY_PRINTF( "Texture File Name : %s", texture_filename );

			CGraphicFilePathManager::ConcatTextureFilePathRoot( pathBuffer , PATH_LEN_MAX , texture_filename );
			CTexturePtr spTexture = CTextureCreateName_( CHS( pathBuffer ) );
			mat.m_spMaterial->AttachDiffuseMap( spTexture );
			spTexture->Load();

			// '}'が出るまでスキップ
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

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ(Materialの分）
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

	cache.m_Token.Next();

	// '}'が出るまでスキップ(MaterialListの分）
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

	NextToken( cache );

//-----子は無いので自分自身をしっかり終わらせる---->

}

static void ParseSkinWeights( CLoadCacheData_ &cache )
{
	CVertexBufferPtr spVertex = cache.m_CurrentMesh->GetVertexBuffer();

	//　'{'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		cache.m_Token.Next();
		if ( cache.m_Token.Check( "{" ) )
			break;
	}

    CWeight_ weight;

	//　フレーム名を取得する
	weight.Name.SetName( CHS( cache.m_Token.GetNextChar() ) );
//	SKY_PRINTF( "Skin Weight Frame Name : %s", weight.Name );

	//　ウェイト数を取得する
	s32 weight_size = 0;
	weight_size = cache.m_Token.GetNextInt();
	weight.m_Weights.Assign( weight_size , 0 );
	weight.m_Indices.Assign( weight_size , 0 );
//	SKY_PRINTF( "Number Of Skin Weights : %d", weight_size );

	//　頂点番号を取得する
	for( s32 i = 0 ; i < weight_size ; i++ )
	{
		vrtxtype index = static_cast< vrtxtype >( cache.m_Token.GetNextInt() );
		SKY_ASSERT( index < spVertex->GetVertexNum() );
//		SKY_PRINTF( "Skin Index[%d] : %d", i, index );
        weight.m_Indices[ i ] = index;
	}

	//　ウェイトを取得する
	for( s32 i = 0 ; i < weight_size ; i++ )
	{
		f32 weightVal = cache.m_Token.GetNextFloat();
		SKY_ASSERT( weightVal <= 1.0f );
//		SKY_PRINTF( "Skin Weight[%d] : %f", i, weight );
        weight.m_Weights[ i ] = weightVal;
	}

	//　オフセット行列を取得する
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

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----子は無いので自分自身をしっかり終わらせる---->

}

static void ParseAnimationSet( CLoadCacheData_ &cache )
{
	//まだコンテナが作成されてないか
	if ( cache.m_spMotionDataContena.IsEmpty() == skyTrue )
	{
		//アニメーションデータコンテナの作成
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
        //　適当にアニメーションセット名を付ける
        char temp[255];
		std::sprintf( temp, "clip_%03d" , cache.m_spMotionDataContena->GetAnimationNum() );
		animation->Name.SetName( CHS( temp ) );
//		SKY_PRINTF( "AnimationSet Name : %s", temp );
    }
    else
    {
		//　アニメーションセット名を取得
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

	//　クリップを追加
	cache.m_spMotionDataContena->AddAnimation( animation );

//-----子を捜す---->

	skyBool isLoop = skyTrue;
	while( isLoop )
	{
		//このノードは終わった
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

//-----子を捜す---->
}

static void ParseAnimation( CLoadCacheData_ &cache )
{
	math::CAnimationData animation;

	//　アニメーション名を読み飛ばす
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
		//　アニメーション名を取得
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

	//　フレーム名が最初にあるパターンの場合
    cache.m_Token.Next();
    if ( cache.m_Token.Check( "{" ) )
    {
		//　フレーム名を取得
		animation.m_JointName = CHS( cache.m_Token.GetNextChar() );
//      SKY_PRINTF( "Animation Frame Name : %s", frame_name );

		//閉じ括弧まで読み飛ばす
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

			//　タイプを取得
			s32 type = cache.m_Token.GetNextInt();
	//      SKY_PRINTF( "Type : %d", type );

			//　キーサイズを取得
			s32 key_size = cache.m_Token.GetNextInt();

			//キーサイズは2以上
			SKY_ASSERT( key_size >= 2 );

	//      SKY_PRINTF( "Key Size : %d", key_size );
			pAnimKeys->Reset( key_size );

			//　キーサイズ分ループ
			for ( s32 i = 0 ; i < key_size ; i++ )
			{
				//　時間とパラメータサイズを取得
				u32 time = static_cast< u32 >( cache.m_Token.GetNextInt() );
				int param_size = cache.m_Token.GetNextInt();
	//          SKY_PRINTF( "Time : %d, ParamSize : %d", time, param_size );

				//　時間を設定
				( *pAnimKeys )[ i ].m_Time = time;

				CVectorPrimitive< f32 > params;
				params.Assign( param_size , 0.0f );

				//　パラメータサイズ分ループ
				for( s32 j = 0 ; j < param_size ; j++ )
				{
					//　パラメータ取得
					f32 param = cache.m_Token.GetNextFloat();
	//              SKY_PRINTF( "Param[%d][%d] : %f", i, j, param );
					params[ j ] = param;
				}

				//　タイプにより処理を分岐
				( *pAnimKeys )[ i ].m_Type = type;
				switch( type )
				{
					//　回転
					case 0:
						{
							SKY_ASSERT( param_size == 4 );

							( *pAnimKeys )[ i ].m_Rot[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Rot[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Rot[ 2 ] = params[ 2 ];
							( *pAnimKeys )[ i ].m_Rot[ 3 ] = params[ 3 ];
						}
						break;

					//　拡大縮小
					case 1:
						{
							SKY_ASSERT( param_size == 3 );

							( *pAnimKeys )[ i ].m_Sc[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Sc[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Sc[ 2 ] = params[ 2 ];
						}
						break;

					//　平行移動
					case 2:
						{
							SKY_ASSERT( param_size == 3 );

							( *pAnimKeys )[ i ].m_Trans[ 0 ] = params[ 0 ];
							( *pAnimKeys )[ i ].m_Trans[ 1 ] = params[ 1 ];
							( *pAnimKeys )[ i ].m_Trans[ 2 ] = params[ 2 ];
						}
						break;

					//　何これ
					case 3:
						SKY_PANIC();
						break;

					//　行列
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

			//　継続時間を設定
			SKY_ASSERT( key_size >= 1 );
			f32 duration = static_cast< f32 >( ( *pAnimKeys )[ key_size -1 ].m_Time );
			cache.m_CurrentAnimationMaxTime = TemplateUtil::Max< f32 >( duration , cache.m_CurrentAnimationMaxTime );

			while( !cache.m_Token.IsEnd() )
			{
				cache.m_Token.Next();
				if ( cache.m_Token.Check( "}" ) )
					break;
			}

			//　フレーム名が最後にあるパターンの場合
			cache.m_Token.Next();

			//格納
			animKeysList.AddTail( pAnimKeys );
		}

		//最大キー数取得
		FOREACH( CAnimationKeyFrameArrayPtrList_ , it , animKeysList )
		{
			CAnimationKeyFrameArray_ *pArray = ( *it );

			//行列だったら。
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
			//　回転
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
			//　拡大縮小
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
			//　移動
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

		//読み込み用データは消す
		FOREACH( CAnimationKeyFrameArrayPtrList_ , it , animKeysList )
		{
			DEL ( *it );
		}

		//フレーム名が後にあるパターン
        if ( cache.m_Token.Check( "{" ) )
        {
			//ジョイント名取得
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

//-----子は無いので自分自身をしっかり終わらせる---->

	// '}'が出るまでスキップ
	while( !cache.m_Token.IsEnd() )
	{
		if ( cache.m_Token.Check( "}" ) )
		{
			break;
		}
		cache.m_Token.Next();
	}

//-----子は無いので自分自身をしっかり終わらせる---->
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

    //　フレームを検索
	for( u32 i = 0 ; i < pFrame->m_WeightData.GetNum() ; ++i )
    {
			//　名前からフレーム番号を検索
		s32 joint_index = cache.m_spSkeleton->m_spJointPallet->GetJoint( pFrame->m_WeightData[ i ].Name.GetName() )->Index;

		SKY_ASSERT( joint_index >= -1 );
		SKY_ASSERT( joint_index < 65535 ); //100年後にはこういうデータもあるかも

        //　発見した場合
	    if ( joint_index > -1 )
	    {
			vrtxtype num_vertices = pFrame->m_spMesh->GetVertexBuffer()->GetVertexNum();

			if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream( hash::CHashStringPtr() , skyFalse ).IsEmpty() == skyTrue ||
				 pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream( hash::CHashStringPtr() , skyFalse ).IsEmpty() == skyTrue )
			{
				//バッファを作成
				pFrame->m_spMesh->GetVertexBuffer()->CreateIndexStream( num_vertices );
				pFrame->m_spMesh->GetVertexBuffer()->CreateWeightsStream( num_vertices );

				//バッファを初期化
				pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->WriteIndexAll( vertex::CVertexFmt_Index( 0 ) );
				pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->WriteWeightsAll( vertex::CVertexFmt_Weights( 0.0f ) );
			}

		    //　ウェイト数分ループ
			for( u32 j = 0 ; j < pFrame->m_WeightData[ i ].m_Weights.GetNum() ; ++j )
		    {
			    //　頂点番号
				vrtxtype vertex_index = pFrame->m_WeightData[ i ].m_Indices[ j ];
				f32 weight            = pFrame->m_WeightData[ i ].m_Weights[ j ];

				SKY_ASSERT( vertex_index < spVertex->GetVertexNum() );
				SKY_ASSERT( weight <= 1.0f );

			    //　index 0 未設定
				if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 0 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 0 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 0 ] = weight;
			    }
			    //　index 1 未設定
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 1 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 1 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 1 ] = weight;
			    }
			    //　index 2 未設定
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 2 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 2 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 2 ] = weight;
			    }
			    //　index 3 未設定
			    else if ( pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 3 ] <= 0 )
			    {
					pFrame->m_spMesh->GetVertexBuffer()->GetIndexStream()->GetIndex( vertex_index )->index[ 3 ]       = ( f32 )joint_index;
					pFrame->m_spMesh->GetVertexBuffer()->GetWeightsStream()->GetWeights( vertex_index )->weights[ 3 ] = weight;
			    }
		    }

		    //ボーンオフセット行列（ローカル座標にある頂点をボーン空間の座標に変換する行列）を設定
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
	spJoint->SetInitialTransform( pFrame->m_Matrix ); //初期姿勢行列（モデルが最初にとってるポーズを表す）
}

static void TraverseFrameTree( CLoadCacheData_ &cache )
{
//---- 姿勢ジョイントツリーを作成
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

		//子が居たらすべてスタックに積んで、即最ループ
		//スタックでのツリー構造の解析は、末端の子から処理を行うことが原則
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

	//ポーズジョイントツリーをアタッチ
	if( cache.m_spPoseJointRoot->ChildList().GetNum() > 0 )
	{
//---- ジョイントパレット、頂点毎のWeight , Indexの作成

		FOREACH( CFramePtrList , it , cache.m_FrameList )
		{
			if ( ( *it )->m_WeightData.GetNum() > 0 )
			{//このフレームにはジョイントデータが存在する

				//メッシュごとにポーズは同じスケルトンを作る（実際アニメーションデータを持った複数のメッシュがくることは無いと思うが）
				cache.m_spSkeleton = CSkeletonCreate_( cache.m_spPoseJointRoot );

				//ジョイント、ジョイントパレットを作成する
				SetSkinDataFromCache( cache , *it );

				//幾何形状情報を作成
				cache.m_spModel->GetMeshContena()->AttachMesh( ( *it )->m_spMesh );

				//メッシュにスケルトンをぶら下げる
				( *it )->m_spMesh->AttachSkeleton( cache.m_spSkeleton );
			}
		}
	}
}

void CXFileLoader::LoadFile( const hash::CHashStringPtr &path )
{
	//ディレクトリパス指定があった場合pathと合成する
	skyString pathBuffer[ PATH_LEN_MAX ];
	CGraphicFilePathManager::ConcatXFilePathRoot( pathBuffer , PATH_LEN_MAX , path->GetString() );

	stream::IStreamPtr spStream = CFileUtil_::LoadFile( CHS( pathBuffer ) );
	spStream->vOpen();

	CLoadCacheData_ cache;

	cache.m_Token.SetBuffer( ( skyMString * )spStream->vGetData() );
	cache.m_Token.SetSeparator( " \t\r\n,;\"" );
	cache.m_Token.SetCutOff( "{}" );

	//モデル一個読み込み
	cache.m_spModel = CModelCreate_();

	//メッシュコンテナの作成
	CMeshContenaPtr spMeshContena = CMeshContenaCreate_();

	//メッシュをモデルに関連付ける
	cache.m_spModel->AttachMeshContena( spMeshContena );

	//デフォルトでテクスチャマッピングのシェーダーを付けておく
	cache.m_spModel->AttachShader( CDirectionalLightShaderObjectCreate_() );

	//　ファイル終端までループ
	while( !cache.m_Token.IsEnd() )
	{
		NextToken( cache );
//      SKY_PRINTF( "Check Token : %s", cache.m_Token.GetChar() );

        CheckNode( cache );
	}

	if ( cache.m_FrameList.GetNum() > 0 )
	{
		//読み込み結果を解析する
		TraverseFrameTree( cache );

		if ( cache.m_spMotionDataContena->GetAnimationNum() > 0 )
		{
			u32 clip_index = cache.m_spMotionDataContena->GetAnimationNum() - 1;
			cache.m_spMotionDataContena->GetAnimation( clip_index )->SetAnimationPallet( cache.m_CurrentAnimationPallet );
			cache.m_spMotionDataContena->GetAnimation( clip_index )->SetMaxTime( cache.m_CurrentAnimationMaxTime );

			CDataBase_::AddMotionDataContena( cache.m_spMotionDataContena );
		}
	}

	//メッシュが一個でもあれば有効なモデルとみなす
	if ( cache.m_spModel->GetMeshContena()->GetMeshNum() > 0 )
	{
		//サイズ情報を事前計算
		cache.m_spModel->ComputeSizeData();

		//モデルをデータベースに入れる
		CDataBase_::AddModel( cache.m_spModel );
	}

	//解析したフレームを全て消す
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