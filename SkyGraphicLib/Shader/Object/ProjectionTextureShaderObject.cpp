#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CProjectionTextureShaderObject       , sky::lib::graphic::CShaderObject );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_POS       = math::CBasicVector3( 10.0f, 5.0f, 10.0f );
static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_TAG       = math::CBasicVector3( 0.0f , 0.0f , 0.0f );
static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_UP_VECTOR = math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_NEAR      = 0.25;
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FAR	    = 20; 
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FOV	    = math::util::KPI / 4;
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_ASPECT    = 1 / 1;

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.Z , sizeof( d64 ) , isWrite ); \

skyBool CProjectionTextureShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CProjectionTextureShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CProjectionTextureShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CProjectionTextureShaderObjectPtr )spContainer )->m_TextureLightPosition = m_TextureLightPosition;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CProjectionTextureShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CProjectionTextureShaderObject::Clone()
{
	return Copy( CProjectionTextureShaderObjectPtr( NEW__( CProjectionTextureShaderObject , CProjectionTextureShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CProjectionTextureShaderObject::CloneSharing()
{
	return Clone();
}

CProjectionTextureShaderObject::CProjectionTextureShaderObject() : 
m_TextureLightPosition( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_POS ) ,
m_TextureLightTag( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_TAG ) ,
m_TextureLightFov( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FOV ) ,
m_TextureLightAspect( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_ASPECT ) ,
m_TextureLightNear( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_NEAR ) ,
m_TextureLightFar( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FAR ) ,
m_TextureLightUp( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_UP_VECTOR ) 
{
	m_PassNum = 1;
}

void CProjectionTextureShaderObject::vDraw()
{
	//シェーダーIDの構築（ProjectionTextureShaderObjectではライトの状態に応じてシェーダーを作る）
	u32 shaderId = 0;

	//シェーダーIDが変化していたらシェーダーを取得
	if ( m_spShaderInfo.IsEmpty() || m_ShaderId != shaderId )
	{
		//Hashが無ければつくる
		if ( s_pShaderHash == skyNull )
		{
			s_pShaderHash = NEW__( CShaderInfoPtrHash , CShaderInfoPtrHash( SHADER_HASH_SIZE ) );
		}

		m_spShaderInfo = GetShaderInfo( shaderId );
		m_ShaderId = shaderId; //現在のシェーダーを変更
	}

	//パス毎の描画
	if ( m_RenderingPassNo == 0 )
	{
		Pass1Draw();
	}
}

} } }

//===================================【DirectX】===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// 定数バッファのデータ定義
struct ProjectionTextureShaderConst
{
	XMFLOAT4X4 World;		// ワールド変換行列
	XMFLOAT4X4 View;		// ビュー変換行列
	XMFLOAT4X4 Projection;	// 透視変換行列
	XMFLOAT4X4 WorldViewProjectionTexture;	//テクスチャ光源行列
	XMFLOAT4   Diffuse;		// ディフューズ色
	FLOAT      Alpha;	// アルファ値
	XMFLOAT3   dummy;	// アライメント用ダミー
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //シェーダークリエーターが初期化されていません。

	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//(未実装）IDに関連づくシェーダーファイルを探す。ファイルであったらそれをロードしてシェーダーとする

		//(未実装）shaderIdに応じてシェーダーのソースを文字列操作で変化させる。

		//デバイスを取得
        ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		ID3DBlob            *pShader = NULL;
		ID3DBlob            *pError = NULL;
		ID3D11InputLayout   *layout = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader = NULL;

		// 頂点シェーダ作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/ProjectionTexture.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( ( const DWORD * )pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		spShaderInfo->m_Pass1.m_spVertexShader.SetPtr( vertexShader );

		// 頂点レイアウトの作成
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{"POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr =CDevice_->GetMember()->m_spD3DDevice->CreateInputLayout(
										ElementDesc ,
										array_sizeof( ElementDesc ) ,
										pShader->GetBufferPointer() ,
										pShader->GetBufferSize() ,
										&layout );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_Pass1.m_spVertexLayout.SetPtr( layout );
		pShader->Release();

        // グローバル変数バッファ作成
		ID3D11Buffer  *pConstantBuffer;
		D3D11_BUFFER_DESC ConstDesc;
		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( ProjectionTextureShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/ProjectionTexture.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "ピクセルシェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreatePixelShader( ( const DWORD* )pShader->GetBufferPointer() , pShader->GetBufferSize() , NULL , &pixelShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "ピクセルシェーダーのコンパイルに失敗しました \n" ) );
		};

		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CProjectionTextureShaderObject::Pass1Draw()
{
	//定数バッファ受け渡し用の変数
	ProjectionTextureShaderConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX dw , dv , dp;
	D3DUtil::Convert4x3Matrix( m_WorldMatrix , dw );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetViewMatrix() , dv );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetProjectionMatrix() , dp );
	XMStoreFloat4x4( &shaderConst.World      , XMMatrixTranspose( dw ) );
	XMStoreFloat4x4( &shaderConst.View       , XMMatrixTranspose( dv ) );
	XMStoreFloat4x4( &shaderConst.Projection , XMMatrixTranspose( dp ) );

	//テクスチャ光源変換マトリックス
	XMMATRIX	texMat;
	math::CMatrix4x3 texViewMat_ , texProjeMat_ , texUvMat_;

	texViewMat_.SetupViewLH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionLH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	D3DUtil::Convert4x3Matrix( MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ) , texMat ); //ホントはこれであってほしい
//	D3DUtil::Convert4x3Matrix( MulMatrix4x4( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ) , texMat );     //これが一番見れる
//	D3DUtil::Convert4x3Matrix( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texMat );                                 //これもまあ見れる
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionTexture , XMMatrixTranspose( texMat ) );

//-----描画する-----▽

	//デバイスを取得
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IAに入力レイアウト・オブジェクトを設定
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass1.m_spVertexLayout.GetPtrNoConst() );

	//VSに頂点シェーダを設定
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass1.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0, 1, m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PSにピクセル・シェーダを設定
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // ピクセルシェーダー

	// PSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//メッシュグループ毎に描画する
	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();

	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;

		//アルファ値
		shaderConst.Alpha = spMatDrawData->GetMaterial()->Alpha;

		//マテリアル情報をシェーダーに引き継ぎ
		D3DUtil::ConvertColorVector4( spMatDrawData->GetMaterial()->Diffuse , shaderConst.Diffuse );

		//定数パラメーターを受け渡す
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//ディビューズマップを使用する
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			spMatDrawData->GetMaterial()->GetDiffuseMap()->SetShader();
		}

		//ストリームを設定する
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

		//頂点の描画
		pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
	}

	//各種シェーダーのクリア
	pD3DDeviceContext->VSSetShader( NULL , NULL , 0 ); // 頂点シェーダー
	pD3DDeviceContext->PSSetShader( NULL , NULL , 0 ); // ピクセルシェーダー
}

} } }

#endif
//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //シェーダークリエーターが初期化されていません。

	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		// シェーダの読み込み
		GLuint hm_ProgramObject0;
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			hm_ProgramObject0 ,
			_T( "data/lib/shader/glsl/ProjectionTexture.vert" ), 
			_T( "data/lib/shader/glsl/ProjectionTexture.frag" ) );

		spShaderInfo->m_Pass1.m_ProgramObject = hm_ProgramObject0;
 	
		//頂点属性情報
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");

		//グローバル情報
		spShaderInfo->m_Pass1.m_WorldLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "World");
		spShaderInfo->m_Pass1.m_ViewLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "View");
		spShaderInfo->m_Pass1.m_ProjectionLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Projection");
		spShaderInfo->m_Pass1.m_WvptLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "WorldViewProjectionTexture");
		spShaderInfo->m_Pass1.m_DiffuseLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Diffuse");
		spShaderInfo->m_Pass1.m_AlphaLocation       = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Alpha");
		spShaderInfo->m_Pass1.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#endif

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CProjectionTextureShaderObject::Pass1Draw()
{
	//グローバルパラメーター
	GLfloat World     [ 16 ];
	GLfloat View      [ 16 ];
	GLfloat Projection[ 16 ];
	GLfloat WorldViewProjectionTexture[ 16 ];
	GLfloat Diffuse   [ 4 ];

	//シェーダーの設定
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//プロジェクション行列の設定
	m_spCamera->GetProjectionMatrix().CopyArray< f32 >( Projection );


	//視点座標変換
	m_spCamera->GetViewMatrix().CopyArray< f32 >( View );

	//ワールド行列設定
	m_WorldMatrix.CopyArray< f32 >( World );

    // 座標変換行列
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WorldLocation , 1 , GL_FALSE , World );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ViewLocation , 1 , GL_FALSE , View );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ProjectionLocation , 1 , GL_FALSE , Projection ); 

	math::CMatrix4x3 texViewMat_ , texProjeMat_ , texUvMat_;
	texViewMat_.SetupViewRH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionRH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ).CopyArray< f32 >( WorldViewProjectionTexture );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WvptLocation , 1 , GL_FALSE , WorldViewProjectionTexture ); 

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//半透明値を設定
		glUniform1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , 1 , &spMatDrawData->GetMaterial()->Alpha );

		//マテリアル情報をシェーダーに引き継ぎ
		Diffuse[ 0 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.r;
		Diffuse[ 1 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.g;
		Diffuse[ 2 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.b;
		Diffuse[ 3 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.a;
		glUniform4fv( m_spShaderInfo->m_Pass1.m_DiffuseLocation , 1 , Diffuse );

		//ディビューズマップテクスチャを使用する
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			//テクスチャの設定
			glUniform1i( m_spShaderInfo->m_Pass1.m_Texture0Location , 0 ); // テクスチャユニット番号 0
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, spMatDrawData->GetMaterial()->GetDiffuseMap()->GetMember()->texId[ 0 ] );
		}

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex , pMesh->GetIndexBuffer() );
	}
}

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//注意）cgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); で破棄
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass1.m_ProgramObject , 
		                                            "data/lib/shader/cg/ProjectionTexture.vert" , "mainVS" ,
		                                            "data/lib/shader/cg/ProjectionTexture.frag" , "mainPS"  );

		spShaderInfo->m_Pass1.m_WorldLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.World" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_ViewLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.View" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_ProjectionLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Projection" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_WvptLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.WorldViewProjectionTexture" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_DiffuseLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Diffuse" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_AlphaLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Alpha" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "texCoord0Location" );//名前
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CProjectionTextureShaderObject::Pass1Draw()
{
	//グローバルパラメーター
	float World     [ 16 ];
	float View      [ 16 ];
	float Projection[ 16 ];
	float WorldViewProjectionTexture[ 16 ];
	float Diffuse   [ 4 ];

	//プロジェクション行列の設定
	m_spCamera->GetProjectionMatrix().CopyArray< f32 >( Projection );

	//視点座標変換
	m_spCamera->GetViewMatrix().CopyArray< f32 >( View );

	//ワールド行列設定
	m_WorldMatrix.CopyArray< f32 >( World );

	math::CMatrix4x3 vptMat_ , texViewMat_ , texProjeMat_ , texUvMat_;
	texViewMat_.SetupViewRH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionRH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ).CopyArray< f32 >( WorldViewProjectionTexture );

	//行列設定
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_ProjectionLocation , Projection );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_ViewLocation , View );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_WorldLocation , World );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_WvptLocation , WorldViewProjectionTexture );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass1.m_ProgramObject.Program , 0 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass1.m_ProgramObject.Program , 1 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	//頂点シェーダーを設定
	cgGLBindProgram( m_spShaderInfo->m_Pass1.m_ProgramObject.Program );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//ディビューズマップテクスチャを使用する
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			cgGLSetTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location , spMatDrawData->GetMaterial()->GetDiffuseMap()->GetMember()->texId[ 0 ] ); //テクスチャとの関連付け
			CCg::CheckForCgError( "テクスチャの設定に失敗しました。" );
			cgGLEnableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
			CCg::CheckForCgError( "テクスチャを有効にできませんでした。" );
		}

		//マテリアル情報をシェーダーに引き継ぎ
		Diffuse[ 0 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.r;
		Diffuse[ 1 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.g;
		Diffuse[ 2 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.b;
		Diffuse[ 3 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.a;
		cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_DiffuseLocation , Diffuse );

		// 透明度を渡す
		cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , &spMatDrawData->GetMaterial()->Alpha );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

		//ディビューズマップテクスチャを破棄する
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			cgGLDisableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
			CCg::CheckForCgError( "テクスチャを無効にできませんでした。" );
		}
	}

	//シェーダーを設定解除
    cgGLDisableProfile( profile0 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
    cgGLDisableProfile( profile1 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
	cgGLUnbindProgram( profile0 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
	cgGLUnbindProgram( profile1 );
    CCg::CheckForCgError( "シェーダーの開放に失敗しました。" );
}

#endif

} } }


#endif
