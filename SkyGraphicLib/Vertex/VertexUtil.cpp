#include "StdAfx.h"
#include "SkyGraphicLib/Vertex.h"

#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

void CVertexUtil::CreatePoint( const math::CBasicVector3 &point , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	spVertexBuffer->Initialize( 1 , vertex::ePrimitiveType_Pointlist );
	spVertexBuffer->CreateColorStream( 1 );

	spVertexBuffer->GetPositionStream()->WritePosition( 0 , point );

	spVertexBuffer->GetColorStream()->WriteColor( 0 , color );

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();
	spIndexBuffer->Initialize( 1 );

	spIndexBuffer->SetIndex( 0 , 0 );

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector2Vector &vector , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() > 0 , _T( "num is over 0. \n" ) );

	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linestrip );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();
	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );

	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &start , const math::CBasicVector2 &end , const CColor &color )
{
	math::CBasicVector2Vector vector;
	vector.Add( start );
	vector.Add( end );

	CreateLine2D( spVertexBuffer , vector , spMesh , color );
}

void CVertexUtil::CreateSquareLine2D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const CColor &color )
{
	dectype oneX = width / 2;
	dectype oneY = height / 2;

	math::CBasicVector2Vector vector;
	vector.Add( math::CBasicVector2( -oneX ,  oneY ) );
	vector.Add( math::CBasicVector2(  oneX ,  oneY ) );
	vector.Add( math::CBasicVector2( -oneX , -oneY ) );
	vector.Add( math::CBasicVector2(  oneX , -oneY ) );

	CreateLine2D( spVertexBuffer , vector , spMesh , color );

	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 5 );

	vrtxtype idx = 0; 

	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 0 );

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CMeshPtr &spMesh , const CColor &color )
{
	//頂点は２で初期化
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linestrip );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const math::CBasicLine3List &lineList , const CMeshPtr &spMesh , const CColor &color )
{
	vrtxtype vertexNum = ( vrtxtype )lineList.GetNum() * 2;

	//頂点は２で初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < ( vrtxtype )lineList.GetNum() ;  i++ )
	{
		pos3 = lineList[ i ].m_Pos1;
		spVertexBuffer->GetPositionStream()->WritePosition( i * 2 , pos3 );
		pos3 = lineList[ i ].m_Pos2;
		spVertexBuffer->GetPositionStream()->WritePosition( i * 2 + 1 , pos3 );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( vertexNum );
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateLine3D( const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &start , const math::CBasicVector3 &end , const CColor &color )
{
	math::CBasicVector3Vector vector;
	vector.Add( start );
	vector.Add( end );
	CreateLine3D( spVertexBuffer , vector , spMesh , color );
}


struct Vertex
{
	dectype x , y , z , w;
	dectype u , v;
};

void CVertexUtil::CreateSphereTexture( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius , vrtxtype xnum , vrtxtype ynum , const CColor &color )
{
	vrtxtype vertexNum = ( xnum + 1 ) * ( ynum + 1 ); //頂点数
	vrtxtype indexNum  = 6 * xnum * ynum;             //インデックス数
	vrtxtype idx = 0; 

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	//頂点情報
	for ( vrtxtype j = 0 ; j <= ynum ; j++ )
	{
		 for ( vrtxtype i = 0 ; i <= xnum ; i++ )
		 {
			 Vertex vertex;

			 dectype theta = ( ( dectype ) i ) * 2 * math::util::KPI / xnum;
			 dectype phi   = ( ( dectype ) j ) * math::util::KPI / ynum - math::util::KPI / 2;
			 vertex.x  = radius * ( dectype )math::CMathUtil::Cos( phi ) * ( dectype )math::CMathUtil::Cos( theta );
			 vertex.z  = radius * ( dectype )math::CMathUtil::Cos( phi ) * ( dectype )math::CMathUtil::Sin( theta );
			 vertex.y  = radius * ( dectype )math::CMathUtil::Sin( phi );
			 vertex.w  = 1.0f;
			 vertex.u  = ( dectype )i / xnum;
			 vertex.v  = 1.0f - ( dectype )j / ynum;
			 spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
			 spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
			 idx++;
		 }
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスを１プリミティブインデックス数２、プリミティブ数１で初期化
	spIndexBuffer->Initialize( indexNum );

	idx = 0;

	//インデックス情報
	for ( vrtxtype j = 0 ; j < ynum ; j++ )
	{
		for ( vrtxtype i = 0 ; i < xnum ; i++)
		{
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 1 );
			spIndexBuffer->SetIndex( idx++ , ( j + 0 ) * ( xnum + 1 ) + i + 1 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 0 );
			spIndexBuffer->SetIndex( idx++ , ( j + 1 ) * ( xnum + 1 ) + i + 1 );
		}
	}

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype size , dectype uvrelnum , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		Vertex vertex;

		vertex.x  = ( i == 0 || i == 1 )? ( -size ) : ( +size );
		vertex.z  = ( i == 0 || i == 2 )? ( -size ) : ( +size );
		vertex.y  = 0.0f;
		vertex.w  = 1.0f;
		vertex.u  = ( i == 0 || i == 1 )? 0 : uvrelnum;
		vertex.v  = ( i == 0 || i == 2 )? 0 : uvrelnum;
		spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
		spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
		idx++;
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx   , 2 );

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateCylinderRevers( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype radius , dectype height , vrtxtype ypolnum , vrtxtype circlepolnum , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = ( ypolnum + 1 ) * ( circlepolnum + 1 ); //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

    // 頂点をセットアップ
    for ( vrtxtype i = 0; i <= circlepolnum; i++)
	{
		Vertex vertex;

        f32 theta = ( 2 * ( f32 )math::util::KPI * ( f32 ) i ) / ( f32 ) circlepolnum;
        for ( u32 j = 0; j <= ypolnum; j++)
		{
            vertex.x = radius * ( dectype )math::CMathUtil::Cos( theta );
            vertex.z = radius * ( dectype )math::CMathUtil::Sin( theta );
            vertex.y = height * ( ( dectype )j / ( dectype )ypolnum - 0.0f );
            vertex.w   = 1.0f;
            vertex.u = ( dectype )i / ( dectype )circlepolnum;
            vertex.v = 1.0f-( dectype )j / ( dectype )ypolnum;
			spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( vertex.x ,  vertex.y , vertex.z ) );
			spVertexBuffer->GetUvStream()->WriteUv( idx , math::CBasicVector2( vertex.u , vertex.v ) );
			idx++;
        }
    }

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスを１プリミティブインデックス数２、プリミティブ数１で初期化
	spIndexBuffer->Initialize( circlepolnum * ypolnum );

	idx = 0;

    for ( vrtxtype i = 0; i < circlepolnum; i++)
	{
        for ( vrtxtype j = 0; j < ypolnum; j++)
		{
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 0 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 1 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 0 ) * ( ypolnum + 1 ) );

			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 0 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 0 + ( i + 1 ) * ( ypolnum + 1 ) );
			spIndexBuffer->SetIndex( idx++ , j + 1 + ( i + 1 ) * ( ypolnum + 1 ) );
        }
    }

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateCube( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector3 &scale , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //頂点数

	//頂点は８で初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	dectype oneX = 0.5f * scale.X;
	dectype oneY = 0.5f * scale.Y;
	dectype oneZ = 0.5f * scale.Z;

	//頂点の座標を設定
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , -oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY ,  oneZ ) );
	spVertexBuffer->GetPositionStream()->WritePosition( idx   , math::CBasicVector3( -oneX , -oneY ,  oneZ ) );

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスを１プリミティブインデックス数３、プリミティブ数１２で初期化
	spIndexBuffer->Initialize( 36 );

	idx = 0; 

	//インデックスバッファを設定する
	spIndexBuffer->SetIndex( idx++ , 0 ); //1
	spIndexBuffer->SetIndex( idx++ , 1 ); //2
	spIndexBuffer->SetIndex( idx++ , 2 ); //3
	spIndexBuffer->SetIndex( idx++ , 0 ); //4
	spIndexBuffer->SetIndex( idx++ , 2 ); //5
	spIndexBuffer->SetIndex( idx++ , 3 ); //6
	spIndexBuffer->SetIndex( idx++ , 1 ); //7
	spIndexBuffer->SetIndex( idx++ , 5 ); //8
	spIndexBuffer->SetIndex( idx++ , 6 ); //9
	spIndexBuffer->SetIndex( idx++ , 1 ); //10
	spIndexBuffer->SetIndex( idx++ , 6 ); //11
	spIndexBuffer->SetIndex( idx++ , 2 ); //12
	spIndexBuffer->SetIndex( idx++ , 5 ); //13
	spIndexBuffer->SetIndex( idx++ , 4 ); //14
	spIndexBuffer->SetIndex( idx++ , 7 ); //15
	spIndexBuffer->SetIndex( idx++ , 5 ); //16
	spIndexBuffer->SetIndex( idx++ , 7 ); //17
	spIndexBuffer->SetIndex( idx++ , 6 ); //18
	spIndexBuffer->SetIndex( idx++ , 4 ); //19
	spIndexBuffer->SetIndex( idx++ , 0 ); //20
	spIndexBuffer->SetIndex( idx++ , 3 ); //21
	spIndexBuffer->SetIndex( idx++ , 4 ); //22
	spIndexBuffer->SetIndex( idx++ , 3 ); //23
	spIndexBuffer->SetIndex( idx++ , 7 ); //24
	spIndexBuffer->SetIndex( idx++ , 4 ); //25
	spIndexBuffer->SetIndex( idx++ , 5 ); //26
	spIndexBuffer->SetIndex( idx++ , 1 ); //27
	spIndexBuffer->SetIndex( idx++ , 4 ); //28
	spIndexBuffer->SetIndex( idx++ , 1 ); //29
	spIndexBuffer->SetIndex( idx++ , 0 ); //30
	spIndexBuffer->SetIndex( idx++ , 3 ); //31
	spIndexBuffer->SetIndex( idx++ , 2 ); //32
	spIndexBuffer->SetIndex( idx++ , 6 ); //33
	spIndexBuffer->SetIndex( idx++ , 3 ); //34
	spIndexBuffer->SetIndex( idx++ , 6 ); //35
	spIndexBuffer->SetIndex( idx   , 7 ); //36

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale , const CColor &color )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( vertexNum );
	spVertexBuffer->CreateUvStream( vertexNum );

	dectype oneX = 0.5f * scale.X;
	dectype oneY = 0.5f * scale.Y;

	//頂点の座標を設定
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , 0 ) ); //左上
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , 0 ) ); //右上
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , 0 ) ); //左下
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , 0 ) ); //右下

	//ＵＶ座標を設定
	idx = 0; 
#ifdef SKYLIB_RENDER_OPENGL4
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  1.0f ) ); //左上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  1.0f ) ); //右上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  0.0f ) ); //左下
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  0.0f ) ); //右下
#else
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  0.0f ) ); //左上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  0.0f ) ); //右上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 0.0f ,  1.0f ) ); //左下
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , math::CBasicVector2( 1.0f ,  1.0f ) ); //右下
#endif

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	//インデックスバッファを設定する
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSquare( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , dectype width , dectype height , const math::CBasicVector2 &uvLt , const math::CBasicVector2 &uvRt , const math::CBasicVector2 &uvLb , const math::CBasicVector2 &uvRb )
{
	vrtxtype idx = 0; 

	vrtxtype vertexNum = 4; //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateUvStream( vertexNum );

	dectype oneX = width / 2;
	dectype oneY = height / 2;

	//頂点の座標を設定
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX ,  oneY , 0 ) ); //左上
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX ,  oneY , 0 ) ); //右上
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( -oneX , -oneY , 0 ) ); //左下
	spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3(  oneX , -oneY , 0 ) ); //右下

	//ＵＶ座標を設定
	idx = 0; 
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvLt ); //左上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvRt ); //右上
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvLb ); //左下
	spVertexBuffer->GetUvStream()->WriteUv( idx++ , uvRb ); //右下

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 6 );

	idx = 0; 

	//インデックスバッファを設定する
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 3 );

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::CreateSphere( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , dectype sc , vrtxtype divNum, const CColor &color )
{
	SKY_UNUSED_ARG( spIndexBuffer );

	vrtxtype idx = 0; 
	vrtxtype vertexNum = divNum * 3 * 2; //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( vertexNum );

	dectype		fStep = math::util::KPI / divNum * 2.0f;
		
	// XY平面
	for ( vrtxtype i = 0; i < divNum; ++i )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep ), math::CMathUtil::Cos( i * fStep ), 0.0f ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep + fStep ), math::CMathUtil::Cos( i * fStep + fStep ), 0.0f ) * 0.5f * sc );
	}
	// XZ平面
	for ( vrtxtype i = 0; i < divNum; ++i ) {
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep ), 0.0f, math::CMathUtil::Cos( i * fStep ) ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( math::CMathUtil::Sin( i * fStep + fStep ), 0.0f, math::CMathUtil::Cos( i * fStep + fStep ) ) * 0.5f * sc );
	}
	// YZ平面
	for ( vrtxtype i = 0; i < divNum; ++i ) {
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( 0.0f, math::CMathUtil::Cos( i * fStep ), math::CMathUtil::Sin( i * fStep ) ) * 0.5f * sc );
		spVertexBuffer->GetPositionStream()->WritePosition( idx++ , math::CBasicVector3( 0.0f, math::CMathUtil::Cos( i * fStep + fStep ), math::CMathUtil::Sin( i * fStep + fStep ) ) * 0.5f * sc );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

void CVertexUtil::ComputeEntity3D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity3DPtr &entity3d )
{
	math::CMatrix4x3 trans , rot , sc , w;

	//拡大縮小
	sc.SetupScale( entity3d->GetSc() );

	//回転
	entity3d->GetRot().ToMatrixYXZ( rot );

	//移動
	trans.SetupTranslation( entity3d->GetPos() );

	//行列を合成する
	w = sc * rot * trans;

	//頂点とかけ合わせる
	spVertexBuffer->ComputePosMatrix( w );
}

void CVertexUtil::ComputeEntity2D( const CVertexBufferPtr &spVertexBuffer , const math::CEntity2DPtr &entity )
{
	ComputeEntity3D( spVertexBuffer , entity->Create3D() );
}

void CVertexUtil::CreateGrid_XZ( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	math::CBasicVector3Vector vector;

	dectype startIndex = -( dectype )large;

	//縦線の頂点を設定
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex + i , 0 , large ) );
		vector.Add( math::CBasicVector3( startIndex + i , 0 , -( dectype )large ) );
	}

	//横線の頂点を設定
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex , 0 , ( dectype )large - i ) );
		vector.Add( math::CBasicVector3( large , 0 , ( dectype )large - i ) );
	}

	//頂点は２で初期化
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateGrid_YX( dectype large , const CVertexBufferPtr &spVertexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	math::CBasicVector3Vector vector;

	dectype startIndex = -( dectype )large;

	//縦線の頂点を設定
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex + i , large , 0 ) );
		vector.Add( math::CBasicVector3( startIndex + i , -( dectype )large , 0 ) );
	}

	//横線の頂点を設定
	for ( vrtxtype i = 0 ; i < large * 2 + 1 ; i++ )
	{
		vector.Add( math::CBasicVector3( startIndex , ( dectype )large - i , 0 ) );
		vector.Add( math::CBasicVector3( large , ( dectype )large - i , 0 ) );
	}

	//頂点は２で初期化
	spVertexBuffer->Initialize( ( vrtxtype )vector.GetNum() , vertex::ePrimitiveType_Linelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	math::CBasicVector3 pos3;
	for ( vrtxtype i = 0 ; i < vector.GetNum() ;  i++ )
	{
		pos3 = vector[ i ];
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos3 );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}

	//インデックスバッファの作成
	CIndexBufferPtr spIndexBuffer = CIndexBuffer::Create();

	spIndexBuffer->Initialize( ( vrtxtype )vector.GetNum() );
	for ( vrtxtype i = 0 ; i < vector.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , i );
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );
}

void CVertexUtil::CreateGridFloor( const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const math::CBasicVector2 &scale , const CColor &color )
{
	vrtxtype maxLevel = 6;
	vrtxtype bufSize = ( vrtxtype )math::CMathUtil::Pow( 2, maxLevel ) + 1;
	vrtxtype vertexNum = bufSize * bufSize; //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateNormalStream( vertexNum );
	spVertexBuffer->CreateColorStream( vertexNum );

    // 頂点をセットアップ
	for ( vrtxtype indexX = 0; indexX < bufSize; indexX++ )
	{
		for ( vrtxtype indexY = 0; indexY < bufSize; indexY++ )
		{
			//対象インデックス
			vrtxtype idx =  indexY + ( indexX * bufSize );

			// 座標と法線と色セットアップ
			spVertexBuffer->GetPositionStream()->WritePosition( idx , math::CBasicVector3( indexX * scale.X , 0 , indexY * scale.Y ) );
			spVertexBuffer->GetNormalStream()->WriteNormal( idx , math::CBasicVector3( indexY , 1.0f , indexY ) );
			spVertexBuffer->GetColorStream()->WriteColor( idx , color );
		}
	}

	//頂点群の中心が原点になるようにする
	spVertexBuffer->TransPosCenter();

	//インデックスを１プリミティブインデックス数２、プリミティブ数１で初期化
	spIndexBuffer->Initialize( bufSize * bufSize );

    // インデックスをセットアップ
	for ( vrtxtype indexY = 1; indexY < bufSize - 1; indexY++ )
	{
		for ( vrtxtype indexX = 1; indexX < bufSize - 1; indexX++)
		{
			vrtxtype idx = 6 * ( ( indexX - 1 ) + ( ( indexY - 1 ) *	( bufSize ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 0 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 0 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 1 ) * bufSize + ( indexX - 0 ) ) );
			spIndexBuffer->SetIndex( idx++ , ( ( indexY - 0 ) * bufSize + ( indexX - 1 ) ) );
			spIndexBuffer->SetIndex( idx   , ( ( indexY - 0 ) * bufSize + ( indexX - 0 ) ) );
		}
	}

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );
}

CVertexBufferPtr CVertexUtil::CreateVertexBuffer( vrtxtype vertexNum , vrtxtype indexNum , vertex::ePrimitiveType ePrimType , const CIndexBufferPtr &spIndexBuffer )
{
	//頂点バッファの作成
	graphic::CVertexBufferPtr spVertex = CVertexBufferCreate_();

	//頂点初期化
	spVertex->Initialize( vertexNum , ePrimType );

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//インデックス初期化
		spIndexBuffer->Initialize( indexNum );
	}

	//返却
	return spVertex;
}

#ifdef SKYLIB_RENDER_DIRECTX11

/// <summary>
/// プリミティブ型テーブル
/// </summary>
static D3D11_PRIMITIVE_TOPOLOGY s_TypeTable[] =
{
	{ D3D11_PRIMITIVE_TOPOLOGY_POINTLIST     } , // vertex::ePrimitiveType_Pointlist
	{ D3D11_PRIMITIVE_TOPOLOGY_LINELIST      } , // vertex::ePrimitiveType_Linelist
	{ D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP     } , // vertex::ePrimitiveType_Linestrip
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST  } , // vertex::ePrimitiveType_Trianglelist
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } , // vertex::ePrimitiveType_Meshtrip
	{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } , // vertex::ePrimitiveType_Trianglefan //D3D11では見当たらない。
};
static s32 s_TypeTable_size = array_sizeof( s_TypeTable );

static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType( vertex::ePrimitiveType eType )
{
	SKY_ASSERT_MSG( s_TypeTable_size <= ( s32 )s_TypeTable || 0 > ( s32 )s_TypeTable , _T( "The value of the primitive type is illegal." ) ); //プリミティブ型の値が不正です。

	return s_TypeTable[ eType ];
}

void CVertexUtil::SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType )
{
	ID3D11Buffer* pBufferTbl [ eVertexBufferStreamType_Max ] = { 0 };
	UINT SizeTbl             [ eVertexBufferStreamType_Max ] = { 0 };
	UINT OffsetTbl           [ eVertexBufferStreamType_Max ] = { 0 };
	u32 setNum = 0;
	FOREACH( CVertexBufferStreamPtrList , it , list )
	{//ここでハングするのは必要なストリームが用意されていないモデルか、用意されていてもGPU側にはまだコピーしていないか。またはスプライトが描画されようとしたとき
		pBufferTbl[ setNum ] = ( *it )->GetMember()->m_spGpuMemoryBuffer.GetPtrNoConst();
		SizeTbl   [ setNum ] = GetVertexBufferStreamTypeSize( ( eVertexBufferStreamType )( *it )->ID );
		OffsetTbl [ setNum ] = 0; //先頭からちゃんと使う
		setNum++;
	}

	//頂点バッファの設定
	CDevice_->GetMember()->m_spD3DDeviceContext->IASetVertexBuffers( 0 , setNum , pBufferTbl , SizeTbl , OffsetTbl );

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//インデックスバッファの設定
		spIndexBuffer->SetGpuMemoryStream();
	}

	//プリミティブのタイプを決定
	CDevice_->GetMember()->m_spD3DDeviceContext->IASetPrimitiveTopology( GetPrimitiveType( ePrimType ) );
}

#endif

#ifdef SKYLIB_RENDER_OPENGL4

void CVertexUtil::SetRenderBufferStream( const CVertexBufferStreamPtrList &list , const CIndexBufferPtr &spIndexBuffer , vertex::ePrimitiveType ePrimType )
{
	SKY_UNUSED_ARG( ePrimType );
	FOREACH( CVertexBufferStreamPtrList , it , list )
	{//ここでハングするのは必要なストリームが用意されていないモデルまたはスプライトが描画されようとしたとき
		glBindBuffer( GL_ARRAY_BUFFER, ( *it )->GetMember()->m_hBuffer );
		glVertexAttribPointer( ( *it )->GetMember()->m_Location , GetVertexBufferStreamTypeSize( ( eVertexBufferStreamType )( *it )->ID ) / sizeof( f32 ) , GL_FLOAT , GL_FALSE , 0 , 0 );
		glEnableVertexAttribArray( ( *it )->GetMember()->m_Location );
	}

	if ( spIndexBuffer.IsEmpty() == skyFalse )
	{
		//インデックスバッファの設定
		spIndexBuffer->SetGpuMemoryStream();
	}
}

#endif

void CVertexUtil::CreateCubeLine( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() == 8 , _T( "Please set 8 position. \n" ) );

	vrtxtype vertexNum = ( vrtxtype )vector.GetNum(); //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );

	//頂点に座標を書き込む
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	math::CMatrix4x3 scMat;

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 24 );

	vrtxtype idx = 0; 

	//インデックスバッファを設定する
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 7 );
	spIndexBuffer->SetIndex( idx++ , 7 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 0 );
	spIndexBuffer->SetIndex( idx++ , 4 );
	spIndexBuffer->SetIndex( idx++ , 1 );
	spIndexBuffer->SetIndex( idx++ , 5 );
	spIndexBuffer->SetIndex( idx++ , 2 );
	spIndexBuffer->SetIndex( idx++ , 6 );
	spIndexBuffer->SetIndex( idx++ , 3 );
	spIndexBuffer->SetIndex( idx++ , 7 );

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}
}

void CVertexUtil::CreatePyramid( const CVertexBufferPtr &spVertexBuffer , const math::CBasicVector3Vector &vector , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh , const CColor &color )
{
	SKY_ASSERT_MSG( vector.GetNum() == 5 , _T( "Please set 5 position. \n" ) );

	vrtxtype vertexNum = ( vrtxtype )vector.GetNum(); //頂点数

	//頂点初期化
	spVertexBuffer->Initialize( vertexNum , vertex::ePrimitiveType_Linelist );

	//頂点に座標を書き込む
	for ( vrtxtype i = 0 ; i < vertexNum ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , vector[ i ] );
	}

	math::CMatrix4x3 scMat;

	//インデックスを１プリミティブインデックス数３、プリミティブ数２で初期化
	spIndexBuffer->Initialize( 24 );

	vrtxtype idx = 0; 

	//インデックスバッファを設定する
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 0 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 1 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 2 ); 
	spIndexBuffer->SetIndex( idx++ , 4 ); 
	spIndexBuffer->SetIndex( idx++ , 3 ); 

	//三角形集合情報にインデックスバッファを設定する
	spMesh->AttachIndexBuffer( spIndexBuffer );

	spVertexBuffer->CreateColorStream( ( vrtxtype )vector.GetNum() );

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color );
	}
}

void CVertexUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color , const CVertexBufferPtr &spVertexBuffer , const CIndexBufferPtr &spIndexBuffer , const CMeshPtr &spMesh )
{
	SKY_ASSERT( pos.GetNum() == color.GetNum() && pos.GetNum() == uv.GetNum() );

	//頂点初期化
	spVertexBuffer->Initialize( ( vrtxtype )pos.GetNum() , vertex::ePrimitiveType_Trianglelist );
	spVertexBuffer->CreateColorStream( ( vrtxtype )color.GetNum() );

	//頂点に座標を書き込む
	for ( vrtxtype i = 0 ; i < pos.GetNum() ; i++ )
	{
		spVertexBuffer->GetPositionStream()->WritePosition( i , pos[ i ] );
	}

	//インデックスバッファ作成
	spIndexBuffer->Initialize( ( vrtxtype )index.GetNum() );

	for ( vrtxtype i = 0 ; i < index.GetNum() ; i++ )
	{
		spIndexBuffer->SetIndex( i , index[ i ] ); 
	}

	spMesh->AttachIndexBuffer( spIndexBuffer );

	//UVの設定
	spVertexBuffer->CreateUvStream( ( vrtxtype )pos.GetNum() );

	for ( vrtxtype i = 0 ; i < uv.GetNum() ; i++ )
	{
		spVertexBuffer->GetUvStream()->WriteUv( i , uv[ i ] );
	}

	//頂点のカラー値を設定
	for ( vrtxtype i = 0 ; i < color.GetNum() ; i++ )
	{
		spVertexBuffer->GetColorStream()->WriteColor( i , color[ i ] );
	}

	//頂点の法線を計算する
	spVertexBuffer->ComputeVertexNormalVector();
	spVertexBuffer->AttachNormalStream( spVertexBuffer->DetachVertexNormalStream() );
	spVertexBuffer->DetachPrimitiveNormalStream();
}

} } }