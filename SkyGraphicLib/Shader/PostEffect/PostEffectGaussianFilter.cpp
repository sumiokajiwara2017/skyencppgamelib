#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Sprite.h"

SkyImplementRTTI( sky::lib::graphic::CPostEffectGaussianFilter , sky::lib::graphic::CPostEffectObject );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

void CPostEffectGaussianFilter::ComputeGaussWeights( f32 dispersion )
{
    f32 total = 0.0f;
    const int NUM_WEIGHT = 8;

    for( s32 i = 0 ; i < NUM_WEIGHT ; ++i )
    {
        f32 pos = 1.0f + 2.0f * ( f32 )i;
        m_Weights[ i ] = expf( -0.5f * ( pos * pos ) / dispersion );
        total += 2.0f * m_Weights[ i ];
    }

    f32 invTotal = 1.0f / total;
    for( s32 i = 0 ; i < NUM_WEIGHT ; ++i )
    {
        m_Weights[ i ] *= invTotal;
    }
}

void CPostEffectGaussianFilter::SetDispersion( f32 value )
{
	m_Dispersion_sq = value;
}

CPostEffectGaussianFilter::CPostEffectGaussianFilter() : m_Dispersion_sq( 5.0f )
{
	ComputeGaussWeights( m_Dispersion_sq * m_Dispersion_sq );
	m_PassNum = 2;
}

void CPostEffectGaussianFilter::vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix )
{

	//シェーダーIDの構築（BasicShaderObjectではライトの状態に応じてシェーダーを作る）
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

	switch( passNo )
	{
	case 0:

		//パス１
		Pass1Draw( spRenderTargetTexture , samplerData , spMesh , spCameraMatrix );

		break;
	case 1:

		//パス２
		Pass2Draw( spRenderTargetTexture , samplerData , spMesh , spCameraMatrix );

		break;
	}
}

} } }

//===================================【DirectX】===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//-------------------------------------------------------------------------------------------------------------------------------

//頂点シェーダー入力値構造体
struct CPostEffectGaussianFilterConst
{
	//頂点変換行列
    XMFLOAT4X4 WorldViewProjectionMatrix;
	FLOAT      Weights[ 8 ];
	FLOAT      OffsetX;
	FLOAT      OffsetY;
	FLOAT      w;
	FLOAT      h;
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //シェーダークリエーターが初期化されていません。

	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//デバイスを取得
        ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// 頂点レイアウトの作成
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT   , 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT,       1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
		};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// 頂点シェーダ（パス１用）作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.vert" ) , NULL , NULL , "mainVS_Pass1" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( (const DWORD*)pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		spShaderInfo->m_Pass1.m_spVertexShader.SetPtr( vertexShader );

		//レイアウトの作成
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// 頂点シェーダ（パス２用）作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.vert" ) , NULL , NULL , "mainVS_Pass2" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( (const DWORD*)pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		spShaderInfo->m_Pass2.m_spVertexShader.SetPtr( vertexShader );

		//レイアウトの作成
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
		spShaderInfo->m_Pass2.m_spVertexLayout.SetPtr( layout );
		pShader->Release();

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        // グローバル変数バッファ作成
		D3D11_BUFFER_DESC ConstDesc;
		ID3D11Buffer  *pConstantBuffer;
		ConstDesc.Usage				= D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	= 0;
		ConstDesc.MiscFlags			= 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			= sizeof( CPostEffectGaussianFilterConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.frag" ) , NULL , NULL , "mainPS_Pass1" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			D3DUtil::PrintD3D11Result( hr );
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.frag" ) , NULL , NULL , "mainPS_Pass2" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "ピクセルシェーダーのコンパイルに失敗しました \n" ) );
		};
		hr = pD3DDevice->CreatePixelShader( ( const DWORD* )pShader->GetBufferPointer() , pShader->GetBufferSize() , NULL , &pixelShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "ピクセルシェーダーのコンパイルに失敗しました \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass2.m_spPixelShader.SetPtr( pixelShader );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

//-----パラメーター設定-----▽

	//定数バッファ受け渡し用の変数
	CPostEffectGaussianFilterConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX mat;
	D3DUtil::Convert4x3Matrix( cameraMatrix , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	//ウェイトの設定
	memory::CMemoryUtil::Copy( shaderConst.Weights , m_Weights ,  sizeof( m_Weights ) );

	//オフセットの設定
	shaderConst.OffsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	shaderConst.OffsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;

	//サイズの設定
	shaderConst.w = ( FLOAT )spRenderTargetTexture->GetInfo().Width;
	shaderConst.h = ( FLOAT )spRenderTargetTexture->GetInfo().Height;

//-----描画する-----▽

	//デバイスを取得
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IAに入力レイアウト・オブジェクトを設定
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass1.m_spVertexLayout.GetPtrNoConst() );

	//VSに頂点シェーダを設定
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass1.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PSにピクセル・シェーダを設定
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );

	// PSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
		FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
		{
			CMaterialDrawDataPtr spMatDrawData = *itMat;

			//定数パラメーターを受け渡す
			CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

			//サンプラーを渡す
			spRenderTargetTexture->SetShader();

			//ストリームを設定する
			CVertexBufferStreamPtrList streamList;
			streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
			streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
			CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

			//頂点の描画
			pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
		}
	}

	//各種シェーダーのクリア
	pD3DDeviceContext->VSSetShader( NULL , NULL , 0 ); // 頂点シェーダー
	pD3DDeviceContext->PSSetShader( NULL , NULL , 0 ); // ピクセルシェーダー
}

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

//-----パラメーター設定-----▽

	//定数バッファ受け渡し用の変数
	CPostEffectGaussianFilterConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX mat;
	D3DUtil::Convert4x3Matrix( cameraMatrix , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	//ウェイトの設定
	memory::CMemoryUtil::Copy( shaderConst.Weights , m_Weights ,  sizeof( m_Weights ) );

	//オフセットの設定
	shaderConst.OffsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	shaderConst.OffsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;

	//サイズの設定
	shaderConst.w = ( FLOAT )spRenderTargetTexture->GetInfo().Width;
	shaderConst.h = ( FLOAT )spRenderTargetTexture->GetInfo().Height;

//-----描画する-----▽

	//デバイスを取得
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IAに入力レイアウト・オブジェクトを設定
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass2.m_spVertexLayout.GetPtrNoConst() );

	//VSに頂点シェーダを設定
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass2.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0, 1, m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PSにピクセル・シェーダを設定
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass2.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // ピクセルシェーダー

	// PSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
		FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
		{
			CMaterialDrawDataPtr spMatDrawData = *itMat;

			//定数パラメーターを受け渡す
			CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

			//サンプラーを渡す
			spRenderTargetTexture->SetShader();

			//ストリームを設定する
			CVertexBufferStreamPtrList streamList;
			streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
			streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
			CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

			//頂点の描画
			pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
		}
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

//グローバルパラメーター
static GLfloat PVWMatrix [ 16 ];
static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //シェーダークリエーターが初期化されていません。

	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

//-------------Pass1------------------▽

		// シェーダの読み込み
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			spShaderInfo->m_Pass1.m_ProgramObject ,
			_T( "data/lib/shader/glsl/GaussianFilterPass1.vert" ) , 
			_T( "data/lib/shader/glsl/GaussianFilterPass1.frag" ) );
 	
		//頂点属性情報
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass1.m_TexCoord0Location = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inTexcoord0");

		//グローバル情報
		spShaderInfo->m_Pass1.m_WvpLocation   = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "PVWMatrix");
		spShaderInfo->m_Pass1.m_FreeLocation0 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Weights0"); //vec4
		spShaderInfo->m_Pass1.m_FreeLocation1 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Weights1"); //vec4
		spShaderInfo->m_Pass1.m_FreeLocation2 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "OffsetX");  //float
		spShaderInfo->m_Pass1.m_FreeLocation3 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "OffsetY");  //float
		spShaderInfo->m_Pass1.m_FreeLocation4 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "w");        //float
		spShaderInfo->m_Pass1.m_FreeLocation5 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "h");        //float

		spShaderInfo->m_Pass1.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#endif

//-------------Pass2------------------▽

		// シェーダの読み込み
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			spShaderInfo->m_Pass2.m_ProgramObject ,
			_T( "data/lib/shader/glsl/GaussianFilterPass2.vert" ) , 
			_T( "data/lib/shader/glsl/GaussianFilterPass2.frag" ) );
 	
		//頂点属性情報
		spShaderInfo->m_Pass2.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass2.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass2.m_TexCoord0Location = glGetAttribLocation(spShaderInfo->m_Pass2.m_ProgramObject, "inTexcoord0");

		//グローバル情報
		spShaderInfo->m_Pass2.m_WvpLocation   = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "PVWMatrix");
		spShaderInfo->m_Pass2.m_FreeLocation0 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Weights0"); //vec4
		spShaderInfo->m_Pass2.m_FreeLocation1 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Weights1"); //vec4
		spShaderInfo->m_Pass2.m_FreeLocation2 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "OffsetX");  //float
		spShaderInfo->m_Pass2.m_FreeLocation3 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "OffsetY");  //float
		spShaderInfo->m_Pass2.m_FreeLocation4 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "w");        //float
		spShaderInfo->m_Pass2.m_FreeLocation5 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "h");        //float

		spShaderInfo->m_Pass2.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass2.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass2.m_ProgramObject, 0, "fragColor");
#endif

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//シェーダーの設定
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

    // 座標変換行列
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WvpLocation , 1 , GL_FALSE , PVWMatrix );

	//ウェイトの設定
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , 1 , weights0 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , 1 , weights1 );

	//オフセットの設定
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation2 , 1 , &offsetX );
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation3 , 1 , &offsetY );

	//サイズの設定
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation4 , 1 , &w );
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation5 , 1 , &h );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//テクスチャの設定
		glUniform1i( m_spShaderInfo->m_Pass1.m_Texture0Location , 0 ); // テクスチャユニット番号 0
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, spRenderTargetTexture->GetMember()->texId[ 0 ] );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_TexCoord0Location;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );
	}
}

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//シェーダーの設定
	glUseProgram( m_spShaderInfo->m_Pass2.m_ProgramObject );

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

    // 座標変換行列
    glUniformMatrix4fv( m_spShaderInfo->m_Pass2.m_WvpLocation , 1 , GL_FALSE , PVWMatrix );

	//ウェイトの設定
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , 1 , weights0 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , 1 , weights1 );

	//オフセットの設定
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation2 , 1 , &offsetX );
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation3 , 1 , &offsetY );

	//サイズの設定
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation4 , 1 , &w );
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation5 , 1 , &h );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//テクスチャの設定
		glUniform1i( m_spShaderInfo->m_Pass2.m_Texture0Location , 0 ); // テクスチャユニット番号 0
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, spRenderTargetTexture->GetMember()->texId[ 0 ] );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass2.m_VertexLocation;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location       = m_spShaderInfo->m_Pass2.m_TexCoord0Location;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );
	}
}

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static float PVWMatrix [ 16 ];
static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//シェーダーIDに紐づくシェーダーがなかったらシェーダーを作る
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

//---------Pass1用----------

		//注意）cgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); で破棄
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass1.m_ProgramObject , 
		                                            "data/lib/shader/cg/GaussianFilter.vert" , "mainVS_Pass1" ,
		                                            "data/lib/shader/cg/GaussianFilter.frag" , "mainPS_Pass1"  );

		spShaderInfo->m_Pass1.m_WvpLocation = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.WorldViewProjectionMatrix" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation0 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Weights0" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation1 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Weights1" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation2 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.OffsetX" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation3 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.OffsetY" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation4 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.w" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_FreeLocation5 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.h" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

        spShaderInfo->m_Pass1.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "texCoord0Location" );//名前
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

//---------Pass2用----------

		//注意）cgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); で破棄
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass2.m_ProgramObject , 
		                                            "data/lib/shader/cg/GaussianFilter.vert" , "mainVS_Pass2" ,
		                                            "data/lib/shader/cg/GaussianFilter.frag" , "mainPS_Pass2"  );

		spShaderInfo->m_Pass2.m_WvpLocation = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.WorldViewProjectionMatrix" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation0 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.Weights0" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation1 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.Weights1" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation2 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.OffsetX" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation3 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.OffsetY" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation4 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.w" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass2.m_FreeLocation5 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.h" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

        spShaderInfo->m_Pass2.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "texCoord0Location" );//名前
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

	//行列設定
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass1.m_WvpLocation , PVWMatrix );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

	//ウェイトの設定
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_FreeLocation0 , weights0 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_FreeLocation1 , weights1 );

	//オフセットの設定
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation2 , &offsetX );
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation3 , &offsetY );

	//サイズの設定
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation4 , &w );
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation5 , &h );

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

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//ディビューズマップテクスチャを使用する
        cgGLSetTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location , spRenderTargetTexture->GetMember()->texId[ 0 ] ); //テクスチャとの関連付け
        CCg::CheckForCgError( "テクスチャの設定に失敗しました。" );
        cgGLEnableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
        CCg::CheckForCgError( "テクスチャを有効にできませんでした。" );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location   = 1;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

        cgGLDisableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
        CCg::CheckForCgError( "テクスチャを無効にできませんでした。" );
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

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

	//行列設定
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass2.m_WvpLocation , PVWMatrix );
    CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

	//ウェイトの設定
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , weights0 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , weights1 );

	//オフセットの設定
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation2 , &offsetX );
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation3 , &offsetY );

	//サイズの設定
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation4 , &w );
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation5 , &h );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass2.m_ProgramObject.Program , 0 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass2.m_ProgramObject.Program , 1 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	//頂点シェーダーを設定
	cgGLBindProgram( m_spShaderInfo->m_Pass2.m_ProgramObject.Program );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "シェーダーの準備に失敗しました。" );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//ディビューズマップテクスチャを使用する
        cgGLSetTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location , spRenderTargetTexture->GetMember()->texId[ 0 ] ); //テクスチャとの関連付け
        CCg::CheckForCgError( "テクスチャの設定に失敗しました。" );
        cgGLEnableTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location );
        CCg::CheckForCgError( "テクスチャを有効にできませんでした。" );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location   = 1;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

        cgGLDisableTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location );
        CCg::CheckForCgError( "テクスチャを無効にできませんでした。" );
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
