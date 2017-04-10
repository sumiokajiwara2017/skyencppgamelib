#include "StdAfx.h"
#include "SkyGraphicLib/Model.h"

#include "SkyGraphicLib/Loader.h"
#include "SkyMathLib/Graph.h"

namespace sky { namespace lib { namespace graphic {

CModelPtr CModelUtil::CreateModelFromMesh( const CMeshContenaPtr &spMesh , CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CShaderObjectPtr &effectPtr )
{
	//頂点カラーを設定する
	spMesh->GetMesh()->GetVertexBuffer()->CreateColorStream( spMesh->GetMesh()->GetVertexBuffer()->GetVertexNum() );
	spMesh->GetMesh()->GetVertexBuffer()->GetColorStream()->WriteColorAll( color );

	//初期位置、回転、スケールを反映
	CVertexUtil::ComputeEntity3D( spMesh->GetMesh()->GetVertexBuffer() , math::CEntity3D::Create( pos , rot , sc ) );

	//モデルの作成／パラメーター設定／初期化
	CModelPtr model = CModelCreate_();
	model->AttachMeshContena( spMesh );
	model->AttachShader( effectPtr );

	//セットアップ
	model->ActivateTraverse();

	return model;
}

CModelPtr CModelUtil::CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//メッシュの作成
	CMeshContenaPtr spMeshContena = CWavefrontOBJLoader_::Load( pPath , spVertex , spIndex );

	//初期位置、回転、スケールを反映
	CVertexUtil::ComputeEntity3D( spMeshContena->GetMesh()->GetVertexBuffer() , math::CEntity3D::Create( pos , rot , sc ) );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr model = CModelCreate_();
	model->AttachMeshContena( spMeshContena );
	model->AttachShader( effectPtr );

	//セットアップ
	model->ActivateTraverse();

	//マテリアルに色情報を設定する
	model->SetDiffuse( color );

	return model;
}

#define _CUBE_PATH _T( "testcube.obj" )
CModelPtr  CModelUtil::CreateTestCube( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CUBE_PATH ) , color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _CONE_PATH _T( "testcone.obj" )
CModelPtr  CModelUtil::CreateTestCone( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CONE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _PLANE_PATH _T( "testplane10x10.obj" )
CModelPtr  CModelUtil::CreateTestPlane( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _PLANE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

#define _SPHERE_PATH _T( "testsphere.obj" )
CModelPtr  CModelUtil::CreateTestSphere( CColor color , math::CBasicVector3 sc , math::CBasicAngleVector3 rot , math::CBasicVector3 pos , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _SPHERE_PATH ), color , sc , rot , pos , spVertex , spIndex , effectPtr );
}

CModelPtr CModelUtil::CreatePoint( const math::CBasicVector3 &point , CColor color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreatePoint( point , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	return spModel;
}

CModelPtr CModelUtil::CreateLine3D( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr  )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine3D( spVertex , vector , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateLine3D( const math::CBasicLine3List &lineList , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine3D( spVertex , lineList , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateStandardLine3( dectype length )
{
	//親ノードを作成
	CModelPtr spResult = CModelCreate_(); //子をもつだけの自身は描画しない空モデル
	spResult->ActivateTraverse();

	//X軸の作成
	math::CBasicVector3Vector vectorX;
	vectorX.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorX.Add( math::CBasicVector3( length , 0 , 0 ) );
	spResult->AttachChild( CreateLine3D( vectorX ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Red , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 0.0f , 0.0f , 90.0f ) , math::CBasicVector3( length , 0.0f , 0.0f ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	//Y軸の作成
	math::CBasicVector3Vector vectorY;
	vectorY.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorY.Add( math::CBasicVector3( 0 , length , 0 ) );
	spResult->AttachChild( CreateLine3D( vectorY , CColor_Blue ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Blue , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , length , 0.0f ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	//Z軸の作成
	math::CBasicVector3Vector vectorZ;
	vectorZ.Add( math::CBasicVector3( 0 , 0 , 0 ) );
	vectorZ.Add( math::CBasicVector3( 0 , 0 , length ) );
	spResult->AttachChild( CreateLine3D( vectorZ , CColor_Green ) );
	spResult->AttachChild( graphic::CModelUtil::CreateTestCone( CColor_Green , math::CBasicVector3( 0.3f , 0.3f , 0.3f ) , math::CBasicAngleVector3( 2 , 90.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , length ) , CVertexBuffer::Create() , CIndexBuffer::Create() , CMaterialDiffuseColorShaderObject::Create() ) );

	return spResult;
}

CModelPtr CModelUtil::CreateGrid_XZ( dectype large , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateGrid_XZ( large , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateGrid_YX( dectype large , const CColor &color , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateGrid_YX( large , spVertex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateSquare( const math::CBasicVector2 &scale , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//頂点バッファ、インデックスバッファを使用して立方体の頂点情報を作成する。
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , scale , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );
	return spModel;
}

CModelPtr CModelUtil::CreateCircle_XZ( const math::CBasicCircle& circle , const CColor &color , u32 granularity )
{
	math::CBasicVector3 polar;
	math::CBasicVector3 rectangular;

	//並行移動の行列を作成
	math::CMatrix4x3 mat;
	mat.SetupTranslation( math::CBasicVector3( circle.X() , 0 , circle.Y() ) );

	//XZ平面
	math::CBasicVector3Vector vectorXZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = circle.GetRadius();
		polar.Y = 0;
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( ( dectype )( 360 / granularity ) ) * index ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXZ.Add( rectangular * mat );
	}
	return CreateLine3D( vectorXZ , color );
}

CModelPtr CModelUtil::CreateLine3DCube( const math::CMatrix4x3 &trans , const CColor &color )
{
	math::CBasicVector3Vector vec;
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, +1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, -1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, -1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, +1.0f, -1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, +1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -1.0f, -1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, -1.0f, +1.0f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +1.0f, +1.0f, +1.0f ) , trans ) );

	return CreateLine3DCube( vec , color );
}

CModelPtr CModelUtil::CreateFrustum( const math::CBasicVector3 &pos , const CameraPtr &spCamera , const CColor &color )
{
	math::CMatrix4x3 trans;
	trans.SetupTranslation( pos );
	trans = MulMatrix4x4( trans , Invert4x4( spCamera->GetCameraMatrix() ) );
	return CreateLine3DCube( trans , color );
}

CModelPtr CModelUtil::CreateLine3DCube( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateCubeLine( spVertex , vector , spIndex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	//マテリアルに色情報を設定する
	spModel->SetDiffuse( color );

	return spModel;
}

CModelPtr CModelUtil::CreateLinePyramid( const math::CBasicVector3Vector &vector , const CColor &color , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreatePyramid( spVertex , vector , spIndex , spMesh , color );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	//マテリアルに色情報を設定する
	spModel->SetDiffuse( color );

	return spModel;
}

CModelPtr CModelUtil::CreateLinePyramid( const math::CMatrix4x3 &trans , const CColor &color )
{
	math::CBasicVector3Vector vec;
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.5f, -0.0f, +0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +0.5f, -0.0f, +0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( +0.5f, -0.0f, -0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.5f, -0.0f, -0.5f ) , trans ) );
	vec.Add( MulMatrix4x4( math::CBasicVector3( -0.0f, +1.0f, +0.0f ) , trans ) );

	return CreateLinePyramid( vec , color );
}

//極座標 (r,θ,φ) 半径r、XY平面回転角度θ、仰角φ
CModelPtr CModelUtil::CreateLine3DSphere( const math::CBasicSphere &sphere , const CColor &color , u32 granularity )
{
	//親ノードを作成
	CModelPtr spResult = CModelCreate_(); //子をもつだけの自身は描画しない空モデル
	spResult->ActivateTraverse();

	math::CBasicVector3 polar;
	math::CBasicVector3 rectangular;

	//並行移動の行列を作成
	math::CMatrix4x3 mat;
	mat.SetupTranslation( sphere.GetPos() );

	//XY平面
	math::CBasicVector3Vector vectorXY;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		polar.Z = 0;
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXY.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorXY , color ) );

	//XZ平面
	math::CBasicVector3Vector vectorXZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = 0;
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorXZ.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorXZ , color ) );

	//YZ平面
	math::CBasicVector3Vector vectorYZ;
	for ( u32 i = 0 ; i <= granularity ; i++ )
	{
		u32 index = i;
		if ( index == granularity ) index = 0;

		polar.X = sphere.GetRadius();
		polar.Y = math::CMathUtil::DegToRad( 90 );
		polar.Z = ( dectype )( math::CMathUtil::DegToRad( ( dectype )( ( 360 / granularity ) * index ) ) );
		math::CGraphUtil::ConvertFromPolarCoordinatesToRectangularCoordinate( rectangular , polar );
		vectorYZ.Add( rectangular * mat );
	}
	spResult->AttachChild( CreateLine3D( vectorYZ , color ) );

	return spResult;
}

CModelPtr CModelUtil::CreateAABBDisp( const math::CAABB3 &aabb , const CColor &color )
{
	SKY_ASSERT( aabb.IsZero() == skyFalse );

	math::CBasicVector3Vector vec;
	vec.Add( aabb.Corner( 2 ) );
	vec.Add( aabb.Corner( 3 ) );
	vec.Add( aabb.Corner( 1 ) );
	vec.Add( aabb.Corner( 0 ) );
	vec.Add( aabb.Corner( 6 ) );
	vec.Add( aabb.Corner( 7 ) );
	vec.Add( aabb.Corner( 5 ) );
	vec.Add( aabb.Corner( 4 ) );

	CModelPtr spResult = CreateLine3DCube( vec , color );
	spResult->SetLockParentRotAndScale( skyTrue );

	return spResult;
}

CModelPtr CModelUtil::CreateOBBDisp( const math::COBB3 &obb , const CColor &color )
{
	SKY_ASSERT( obb.IsZero() == skyFalse );

	math::CBasicVector3Vector vec;
	vec.Add( obb.GetCorner( 5 ) );
	vec.Add( obb.GetCorner( 4 ) );
	vec.Add( obb.GetCorner( 7 ) );
	vec.Add( obb.GetCorner( 6 ) );
	vec.Add( obb.GetCorner( 1 ) );
	vec.Add( obb.GetCorner( 2 ) );
	vec.Add( obb.GetCorner( 3 ) );
	vec.Add( obb.GetCorner( 0 ) );

	return CreateLine3DCube( vec , color );
}

CModelPtr CModelUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//頂点バッファ作成
	CVertexUtil::Create( pos , index , uv , color ,  spVertex , spIndex , spMesh );

	//マテリアルに描画頂点数を通知
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CModelPtr spModel = CModelCreate_();
	spModel->ActivateTraverse();
	spModel->AttachMeshContena( spMeshContena );
	spModel->AttachShader( effectPtr );

	return spModel;
}

} } }