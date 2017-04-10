#include "StdAfx.h"
#include "SkyGraphicLib/Sprite.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Loader.h"

namespace sky { namespace lib { namespace graphic {

CSpritePtr CSpriteUtil::CreateLine2D( const math::CBasicVector2Vector &vector , const CColor &diffuseColor , dectype prio , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	SKY_ASSERT_MSG( vector.GetNum() > 0 , _T( "num is over 0. \n" ) );

	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateLine2D( spVertex , vector , spMesh , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );

	return sprite;

}

CSpritePtr CSpriteUtil::CreateSquareLine2D( dectype width , dectype height , const CColor &diffuseColor ,  dectype prio , const CVertexBufferPtr &spVertex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	CVertexUtil::CreateSquareLine2D( spVertex , spMesh , width , height , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

CSpritePtr CSpriteUtil::CreateSquare(  const math::CBasicVector2 &scale , const CColor &diffuseColor , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//頂点バッファ、インデックスバッファを使用して立方体の頂点情報を作成する。
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , scale , diffuseColor );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}


CSpritePtr CSpriteUtil::CreateSquare( const CTexturePtr &texPtr , dectype prio , const math::CBasicVector2 &scale , const CColor &diffuseColor , const math::CBasicVector2 &offset , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateSquare( texPtr , math::CBasicVector2( ( dectype )texPtr->GetInfo().Width , ( dectype )texPtr->GetInfo().Height ) , prio , math::CBasicVector2( scale.X , scale.Y ) , diffuseColor , offset , spVertex , spIndex , effectPtr );
}

CSpritePtr CSpriteUtil::CreateSquare( const CTexturePtr &texPtr , const math::CBasicVector2 &size , dectype prio , const math::CBasicVector2 &scale , const CColor &diffuseColor , const math::CBasicVector2 &offset , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//頂点バッファ、インデックスバッファを使用して四角形の頂点情報を作成する。
	CVertexUtil::CreateSquare( spVertex , spIndex , spMesh , math::CBasicVector2( size.X * scale.X , size.Y * scale.Y ) , diffuseColor );

	//初期位置を変更
	CVertexUtil::ComputeEntity3D( spVertex , math::CEntity3D::Create( math::CBasicVector3( offset.X , offset.Y , 0.0f ) , math::CBasicAngleVector3( 0.0f , 0.0f , 0.0f ) ,  math::CBasicVector3( 1.0f , 1.0f , 1.0f ) ) );
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

CSpritePtr CSpriteUtil::CreateFromWavefrontOBJ( const hash::CHashStringPtr &pPath , dectype prio , CColor color , math::CBasicVector2 pos , dectype rot , math::CBasicVector2 sc , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//メッシュの作成
	CMeshContenaPtr spMeshContena = CWavefrontOBJLoader_::Load( pPath , spVertex , spIndex );

	//初期位置、回転、スケールを反映
	CVertexUtil::ComputeEntity2D( spMeshContena->GetMesh()->GetVertexBuffer() , math::CEntity2D::Create( pos , rot , sc ) );

	//テスト用を想定してるのでここでGPUメモリも確保してしまう。CPUメモリからGPUメモリの書き込みも行う。
	spMeshContena->ActivateTraverse();

	//モデルの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );

	//マテリアルカラーを設定する
	sprite->SetDiffuse( color );

	return sprite;
}

CSpritePtr CSpriteUtil::CreateStandardLine2( dectype length , dectype prio )
{
	//親ノードを作成
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );

	//X軸の作成
	math::CBasicVector2Vector vectorX;
	vectorX.Add( math::CBasicVector2( 0 , 0 ) );
	vectorX.Add( math::CBasicVector2( length , 0 ) );
	sprite->AttachChild( CreateLine2D( vectorX ) );

	//Y軸の作成
	math::CBasicVector2Vector vectorY;
	vectorY.Add( math::CBasicVector2( 0 , 0 ) );
	vectorY.Add( math::CBasicVector2( 0 , length ) );
	sprite->AttachChild( CreateLine2D( vectorY , CColor_Blue , prio ) );

	return sprite;
}

#define _CIRCLE_PATH _T( "testcircle.obj" )
CSpritePtr CSpriteUtil::CreateTestCircle( CColor color , math::CBasicVector2 pos , dectype rot , math::CBasicVector2 sc , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	return CreateFromWavefrontOBJ( CHS( _CIRCLE_PATH ) , prio , color , pos , rot , sc , spVertex , spIndex , effectPtr );
}

CSpritePtr CSpriteUtil::Create( const math::CBasicVector3Vector &pos , const CVector_u16 &index ,  const math::CBasicVector2Vector &uv , const CVector_CColor &color ,  const CTexturePtr &texPtr , dectype prio , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex , const CShaderObjectPtr &effectPtr )
{
	//マテリアルの作成
	CMaterialPtr mat = CMaterialCreate_();
	mat->AttachDiffuseMap( texPtr );
	CMeshPtr spMesh = CMeshCreate_();
	spMesh->AttachMaterialContena( CMaterialContenaCreateSetMaterial_( mat , 0 ) );

	//頂点バッファ作成
	CVertexUtil::Create( pos , index , uv , color ,  spVertex , spIndex , spMesh );

	//描画頂点数を通知
	spMesh->GetMaterialContena()->SetDrawIndex( 0 , 0 , spMesh->GetIndexBuffer()->GetIndexNum() );

	//メッシュの作成
	CMeshContenaPtr  spMeshContena = CMeshContenaCreate_();
	spMeshContena->AttachMesh( spMesh );
	spMeshContena->GetMesh()->AttachVertexBuffer( spVertex );
	spMeshContena->ActivateTraverse();

	//スプライトの作成／パラメーター設定／初期化
	CSpritePtr sprite = CSpriteCreate_();
	sprite->SetUp();
	sprite->SetPrio( prio );
	sprite->AttachMeshContena( spMeshContena );
	sprite->AttachShader( effectPtr );
	return sprite;
}

} } }