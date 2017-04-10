#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

//===================================【Common】===================================================>

SkyImplementRTTI( sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject       , sky::lib::graphic::CShaderObject );

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

skyBool CPerspectiveShadowMapCreateShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

skyBool CPerspectiveShadowMapCreateShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

const serialize::CSerializeInterfacePtr& CPerspectiveShadowMapCreateShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CPerspectiveShadowMapCreateShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CPerspectiveShadowMapCreateShaderObject::Clone()
{
	return Copy( CPerspectiveShadowMapCreateShaderObjectPtr( NEW__( CPerspectiveShadowMapCreateShaderObject , CPerspectiveShadowMapCreateShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CPerspectiveShadowMapCreateShaderObject::CloneSharing()
{
	return Clone();
}

CPerspectiveShadowMapCreateShaderObject::CPerspectiveShadowMapCreateShaderObject()
{
	m_PassNum = 1;
	m_LightView.Identity();
	m_LightProjection.Identity();
	m_LightViewProjection.Identity();
	m_PsmMatrix.Identity();
	m_View.Identity();
	m_Projection.Identity();
	m_ViewProjection.Identity();
	m_WorldSpaceLight = math::CBasicVector4_ZERO;
	m_NearClip = 1.0f;
	m_FarClip = 50.0f;
}

void CPerspectiveShadowMapCreateShaderObject::SetShadowLightPosition( const math::CBasicVector4 &lightPos )
{
	m_WorldSpaceLight = lightPos;
}

void CPerspectiveShadowMapCreateShaderObject::vDraw()
{
	//シェーダーIDの構築（CPerspectiveShadowMapCreateShaderObjectではライトの状態に応じてシェーダーを作る）
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

	//更新する
	UpdateShadowMatrix();

	//パス毎の描画
	if ( m_RenderingPassNo == 0 )
	{
		Pass1Draw();
	}
}

void CPerspectiveShadowMapCreateShaderObject::UpdateShadowMatrix()
{
	//ワールド空間のライト
	math::CBasicVector4 wsLight = m_WorldSpaceLight;
	wsLight.Normalize();

	//カメラの行列を求める
	CalcCameraMtx(wsLight);

	// World Space　→　Post Perspective Space
	math::CBasicVector4 ppsLight = MulMatrix4x4( wsLight, m_ViewProjection );

	//ライトの行列を求める
	if ( math::CMathUtil::Abs( ppsLight.W ) < DBL_EPSILON )
	{
		//平行光源の場合
		CalcLightMtx_ParalleLight(ppsLight);
	}
	else
	{
		//点光源の場合
		CalcLightMtx_PointLight(ppsLight);
	}
			
	//PSMに用いる行列を求める
	m_PsmMatrix = MulMatrix4x4Full( m_ViewProjection , m_LightViewProjection );

}

void CPerspectiveShadowMapCreateShaderObject::CalcCameraMtx( const math::CBasicVector4 &wsLight )
{
#ifdef VIEW_LEFT_HAND
	//ビュー行列・射影行列を作成
	m_View.SetupViewLH( m_spCamera->GetData().m_Eye, m_spCamera->GetData().m_Tag, m_spCamera->GetData().m_UpVec );
#endif

#ifdef VIEW_RIGHT_HAND
	//ビュー行列・射影行列を作成
	m_View.SetupViewRH( m_spCamera->GetData().m_Eye, m_spCamera->GetData().m_Tag, m_spCamera->GetData().m_UpVec );
#endif

	dectype d = 0.0f;

	// Check1 : 近クリップ面までの最小距離
	if ( m_NearClip < MIN_Z_NEAR )
	{
		d = MIN_Z_NEAR - m_NearClip;
	}

	// Check2 : Viewerの後ろから影をキャストするオブジェクトがあるか？
	math::CBasicVector3 col2;
	math::CBasicVector4 viewDir( m_View.GetCol2( col2 ) , 0.0f );
	viewDir.Normalize();

	//ライトと視線ベクトルのなす角を算出
	dectype dot = wsLight.Dot( viewDir );

	//カメラの後ろからライトが当たるか判定
	if ( dot < 0.0f && ( f32 )math::CMathUtil::Abs(wsLight.Y) > FLT_EPSILON)
	{
		dectype t = -( MAX_Y_SCENE - m_spCamera->GetData().m_Eye.Y ) / wsLight.Y;
		d = TemplateUtil::Max< dectype >( d , t * ( dectype )math::CMathUtil::Sqrt(wsLight.X * wsLight.X + wsLight.Z * wsLight.Z ) );
	}
#ifdef VIEW_LEFT_HAND
	//ビュー行列・射影行列を作成
	m_Projection.SetupProjectionLH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip , m_FarClip );
#endif

#ifdef VIEW_RIGHT_HAND
	//ビュー行列・射影行列を作成
	m_Projection.SetupProjectionRH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip , m_FarClip );
#endif

	//視錐台の調整が必要か判定
	if ( d > 0.0f )
	{
		//z軸に沿ってスライドバック
		m_View.Tz -= d;
#ifdef VIEW_LEFT_HAND
		m_Projection.SetupProjectionLH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip + d, m_FarClip + d);
#endif

#ifdef VIEW_RIGHT_HAND
		m_Projection.SetupProjectionRH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip + d, m_FarClip + d);
#endif
	}

	//ビュー射影行列を作成
	m_ViewProjection = MulMatrix4x4Full( m_View , m_Projection );
}


void CPerspectiveShadowMapCreateShaderObject::CalcLightMtx_ParalleLight( const math::CBasicVector4 &ppsLight )
{
#ifdef VIEW_LEFT_HAND
	m_LightView.SetupViewLH( math::CBasicVector3_ZERO , ppsLight.GetVector3() , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionOffsetCenterLH( -SQRT2, SQRT2, -SQRT2, SQRT2, -SQRT2, SQRT2 );
#endif

#ifdef VIEW_RIGHT_HAND
	m_LightView.SetupViewRH( math::CBasicVector3_ZERO , -ppsLight.GetVector3() , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionOffsetCenterRH( -SQRT2, SQRT2, -SQRT2, SQRT2, -SQRT2, SQRT2 );
#endif

	//ライトのビュー射影行列を作成
	m_LightViewProjection = MulMatrix4x4Full( m_LightView , m_LightProjection );
}

void CPerspectiveShadowMapCreateShaderObject::CalcLightMtx_PointLight( const math::CBasicVector4 &ppsLight_ )
{
	//ワークに放り込む
	math::CBasicVector4 ppsLight = ppsLight_;

	//ブラックホールかどうかチェック
	if ( ppsLight.W < 0.0f )
	{
		//マイナスをかけて反転させる
		math::CMatrix4x3 scale;
		scale.SetupScale( math::CBasicVector3( 1.0f , 1.0f , -1.0f ) );
		ppsLight = MulMatrix4x4( ppsLight , scale );
	}

	//W成分で割って変換
	ppsLight *= ( 1.0f / ppsLight.W );
	ppsLight.W = 0.0f;

	//ライトの視錐台を調整
	dectype radius = SQRT3;
	dectype dist = math::CMathUtil::Sqrt( ppsLight.Dot( ppsLight ) );
	dectype fov = 2.0f * ( dectype )math::CMathUtil::Atan( radius / dist );
	dectype nearDist = TemplateUtil::Max< dectype >( dist - radius , 0.0001f );
	dectype farDist = dist + radius;

#ifdef VIEW_LEFT_HAND
	//ライトのビュー行列・射影行列を作成
	m_LightView.SetupViewLH( ppsLight.GetVector3() , math::CBasicVector3_ZERO , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionLH( fov, 1.0f, nearDist, farDist );
#endif

#ifdef VIEW_RIGHT_HAND
	//ライトのビュー行列・射影行列を作成
	m_LightView.SetupViewRH( ppsLight.GetVector3() , -math::CBasicVector3_ZERO , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionRH( fov, 1.0f, nearDist, farDist );
#endif

	//ライトのビュー射影行列を作成
	m_LightViewProjection = MulMatrix4x4( m_LightView , m_LightProjection );

}


} } }

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// 定数バッファのデータ定義
struct CPerspectiveShadowMapCreateShaderConst
{
    XMFLOAT4X4 World;
    XMFLOAT4X4 PsmMatrix;
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
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PerspectiveShadowMapCreate.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",      0 , DXGI_FORMAT_R32G32B32_FLOAT,  1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0 , DXGI_FORMAT_R32G32_FLOAT,     1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
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
		ConstDesc.ByteWidth			  = sizeof( CPerspectiveShadowMapCreateShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PerspectiveShadowMapCreate.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
	//定数バッファ受け渡し用の変数
	CPerspectiveShadowMapCreateShaderConst shaderConst;

	//ワールド行列 * ビュー行列 * プロジェクション行列の設定
	XMMATRIX dw;
	D3DUtil::Convert4x3Matrix( m_WorldMatrix , dw );
	XMStoreFloat4x4( &shaderConst.World , XMMatrixTranspose( dw ) );

	XMMATRIX dPsm;
	D3DUtil::Convert4x3Matrix( m_PsmMatrix , dPsm );
	XMStoreFloat4x4( &shaderConst.PsmMatrix , XMMatrixTranspose( dPsm ) );

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

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;

		//定数パラメーターを受け渡す
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//ストリームを設定する
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetNormalStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
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

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
}

#endif //SW_SKYGRAPHICLIB_USE_GLSL_ON

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
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

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
}

#endif // SW_SKYGRAPHICLIB_USE_CG_ON

} } }

#endif // SKYLIB_RENDER_OPENGL4
