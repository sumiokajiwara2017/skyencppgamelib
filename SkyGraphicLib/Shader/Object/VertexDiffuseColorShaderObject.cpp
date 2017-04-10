#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CVertexDiffuseColorShaderObject       , sky::lib::graphic::CShaderObject );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

const serialize::CSerializeInterfacePtr& CVertexDiffuseColorShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CVertexDiffuseColorShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CVertexDiffuseColorShaderObject::Clone()
{
	return Copy( CVertexDiffuseColorShaderObjectPtr( NEW__( CVertexDiffuseColorShaderObject , CVertexDiffuseColorShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CVertexDiffuseColorShaderObject::CloneSharing()
{
	return Clone();
}

CVertexDiffuseColorShaderObject::CVertexDiffuseColorShaderObject()
{
	m_PassNum = 1;
}

void CVertexDiffuseColorShaderObject::vDraw()
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

//-------------------------------------------------------------------------------------------------------------------------------

//頂点シェーダー入力値構造体
struct VertexDiffuseColorShaderConst
{
	//頂点変換行列
    XMFLOAT4X4	WorldViewProjectionMatrix;
	FLOAT       Alpha;	// アルファ値
	XMFLOAT3    dummy;	// アライメント用ダミー
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

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// 頂点シェーダ作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/VertexDiffuseColor.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

		// 頂点レイアウトの作成
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "SV_POSITION",   0 , DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   ,   0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		D3D11_BUFFER_DESC ConstDesc;
		ID3D11Buffer  *pConstantBuffer;
		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			= sizeof( VertexDiffuseColorShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/VertexDiffuseColor.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CVertexDiffuseColorShaderObject::Pass1Draw()
{
	//定数バッファ受け渡し用の変数
	VertexDiffuseColorShaderConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX	mat;
	D3DUtil::Convert4x3Matrix( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

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

		//定数パラメーターを受け渡す
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//ストリームを設定する
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetColorStream() );
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
			_T( "data/lib/shader/glsl/VertexDiffuseColor.vert" ), 
			_T( "data/lib/shader/glsl/VertexDiffuseColor.frag" ) );

		spShaderInfo->m_Pass1.m_ProgramObject = hm_ProgramObject0;
 	
		//頂点属性情報
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass1.m_DiffuseLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inDiffuse");

		//グローバル情報
		spShaderInfo->m_Pass1.m_WvpLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "PVWMatrix");
		spShaderInfo->m_Pass1.m_AlphaLocation       = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Alpha");

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

void CVertexDiffuseColorShaderObject::Pass1Draw()
{
	//グローバルパラメーター
	GLfloat PVWMatrix [ 16 ];

	//シェーダーの設定
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ).CopyArray< f32 >( PVWMatrix );

    // 座標変換行列
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WvpLocation , 1 , GL_FALSE , PVWMatrix );

	//メッシュグループ毎に描画する
	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//半透明値を設定
		glUniform1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , 1 , &spMatDrawData->GetMaterial()->Alpha );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		spVertexBuffer->GetColorStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_DiffuseLocation;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetColorStream() );
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
		                                            "data/lib/shader/cg/VertexDiffuseColor.vert" , "mainVS" ,
		                                            "data/lib/shader/cg/VertexDiffuseColor.frag" , "mainPS"  );

		spShaderInfo->m_Pass1.m_WvpLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.WorldViewProjectionMatrix" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );
        spShaderInfo->m_Pass1.m_AlphaLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Alpha" );
     	CCg::CheckForCgError( "パラメーターの設定に失敗しました。" );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CVertexDiffuseColorShaderObject::Pass1Draw()
{
	float PVWMatrix [ 16 ];

	//ワールド行列設定( ※※ 注意！ View , Projection 行列との掛け算はMulMatrix4x4関数で行うこと（M44の要素も必用なため）※※ ）
	( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) ).CopyArray< f32 >( PVWMatrix );

	//行列設定
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass1.m_WvpLocation , PVWMatrix );
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

		// 透明度を渡す
		cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , &spMatDrawData->GetMaterial()->Alpha );

		//ストリームを設定する
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetColorStream()->GetMember()->m_Location = 1;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetColorStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//メッシュ毎に描画
		spVertexBuffer->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex , pMesh->GetIndexBuffer() );
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
