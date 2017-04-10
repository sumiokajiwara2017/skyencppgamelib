#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CPhongShaderObject       , sky::lib::graphic::CShaderObject );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

skyBool CPhongShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

skyBool CPhongShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

const serialize::CSerializeInterfacePtr& CPhongShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CPhongShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CPhongShaderObject::Clone()
{
	return Copy( CPhongShaderObjectPtr( NEW__( CPhongShaderObject , CPhongShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CPhongShaderObject::CloneSharing()
{
	return Clone();
}

CPhongShaderObject::CPhongShaderObject()
{
	m_PassNum = 1;
}

void CPhongShaderObject::vDraw()
{
	//シェーダーIDの構築（PhongShaderObjectではライトの状態に応じてシェーダーを作る）
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

struct SKY_ALIGN16 VSMatrix_
{
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
};

struct PSVector_
{
    SKY_ALIGN16 XMFLOAT3  LightDir;
    SKY_ALIGN16 XMFLOAT3  ViewDir;
};

struct PSLightColor_
{
    SKY_ALIGN16 XMFLOAT3 LightAmbient;
    SKY_ALIGN16 XMFLOAT3 LightDiffuse;
	SKY_ALIGN16 XMFLOAT3 LightSpecular;
};

struct SKY_ALIGN16 PSMaterial_
{
    XMFLOAT4 Diffuse;              //４番目の値にAlpha
    XMFLOAT4 Specular;             //４番目の値にPower
    XMFLOAT3 Emissive;
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

		ID3DBlob            *pShader = NULL;
		ID3DBlob            *pError = NULL;
		ID3D11InputLayout   *layout = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader = NULL;

		// 頂点シェーダ作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PhongShader.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		ConstDesc.ByteWidth			  = sizeof( VSMatrix_ );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( PSVector_ );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer1.SetPtr( pConstantBuffer );

		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( PSLightColor_ );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer2.SetPtr( pConstantBuffer );

		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( PSMaterial_ );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer3.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PhongShader.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

void CPhongShaderObject::Pass1Draw()
{
	VSMatrix_     cVSMatrix;
	PSVector_     cPSVector;
	PSLightColor_ cPSLightColor;
	PSMaterial_   cPSMaterial;

	//行列パラメーターの設定
	XMMATRIX dw , dv , dp;
	D3DUtil::Convert4x3Matrix( m_WorldMatrix , dw );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetViewMatrix() , dv );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetProjectionMatrix() , dp );
	XMStoreFloat4x4( &cVSMatrix.World      , XMMatrixTranspose( dw ) );
	XMStoreFloat4x4( &cVSMatrix.View       , XMMatrixTranspose( dv ) );
	XMStoreFloat4x4( &cVSMatrix.Projection , XMMatrixTranspose( dp ) );

	//ベクトルパラメーターの設定
	D3DUtil::ConvertVector3ToVector3( m_spLight->GetDirectionalLightData().m_Vec , cPSVector.LightDir );
	D3DUtil::ConvertVector3ToVector3( m_spCamera->GetData().m_ViewVec , cPSVector.ViewDir );

	//ライトパラメーターの設定
	D3DUtil::ConvertColorVector3( m_spLight->GetDirectionalLightData().m_AmbientColor , cPSLightColor.LightAmbient );
	D3DUtil::ConvertColorVector3( m_spLight->GetDirectionalLightData().m_DiffuseColor , cPSLightColor.LightDiffuse );
	D3DUtil::ConvertColorVector3( m_spLight->GetDirectionalLightData().m_SpecularColor , cPSLightColor.LightSpecular );

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
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // ピクセルシェーダー

	// PSに定数バッファを設定
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 1 , 1 , m_spShaderInfo->m_spVertexConstBuffer1.GetPtrPtr() );
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 2 , 1 , m_spShaderInfo->m_spVertexConstBuffer2.GetPtrPtr() );
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 3 , 1 , m_spShaderInfo->m_spVertexConstBuffer3.GetPtrPtr() );

	//定数パラメーターを受け渡す
	CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0 , NULL , &cVSMatrix , 0, 0 );
	CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer1.GetPtrNoConst() , 0 , NULL , &cPSVector , 0, 0 );
	CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer2.GetPtrNoConst() , 0 , NULL , &cPSLightColor , 0, 0 );

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		//マテリアルカラーの引渡し
		D3DUtil::ConvertColorVector4( spMatDrawData->GetMaterial()->Diffuse , cPSMaterial.Diffuse );
		D3DUtil::ConvertColorVector4( spMatDrawData->GetMaterial()->Specular , cPSMaterial.Specular );
		D3DUtil::ConvertColorVector3( spMatDrawData->GetMaterial()->Emissive , cPSMaterial.Emissive );

		//アルファ値の引渡し
		cPSMaterial.Diffuse.w = spMatDrawData->GetMaterial()->Alpha;

		//反射強度の設定
		cPSMaterial.Specular.w = ( FLOAT )spMatDrawData->GetMaterial()->Power;

		//定数パラメーターの設定
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer3.GetPtrNoConst() , 0, NULL , &cPSMaterial , 0 , 0 );

		//ストリームを設定する
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetNormalStream() );
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

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPhongShaderObject::Pass1Draw()
{
}

#endif //SW_SKYGRAPHICLIB_USE_GLSL_ON

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

void CPhongShaderObject::Pass1Draw()
{
}

#endif // SW_SKYGRAPHICLIB_USE_CG_ON

} } }

#endif // SKYLIB_RENDER_OPENGL4
