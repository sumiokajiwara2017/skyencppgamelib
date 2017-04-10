#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyBaseLib/Rand.h"
#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/DataBase.h"

SkyImplementRTTI( sky::lib::graphic::CBillboardBeam       , sky::lib::graphic::CEffectObject );

namespace sky { namespace lib { namespace graphic {

struct CBillboardBeamMember
{
	/// <summary>
	/// モデル
	/// </summary>
	CModelPtr m_spModel;

	/// <summary>
	/// テクスチャ
	/// </summary>
	CTexturePtr m_spTexture;

	/// <summary>
	/// 幅
	/// </summary>
	f32 m_Width;

	/// <summary>
	/// ノード数
	/// </summary>
	vrtxtype m_NodeNum;

	/// <summary>
	/// カウント
	/// </summary>
	f32 m_Count;

	/// <summary>
	/// 制御点
	/// </summary>
	math::CBasicVector3Vector m_NodePos;
};

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

CBillboardBeam::~CBillboardBeam()
{
	DEL m_pMember;
}

CBillboardBeam::CBillboardBeam()
{
	m_pMember = NEW CBillboardBeamMember();
	m_PassNum = 1;
	m_pMember->m_Count = 0.0f;
	SetJointUse( skyTrue );
	SetAlpha( skyTrue );
}

void CBillboardBeam::Initialize( vrtxtype nodeNum , dectype width , const CTexturePtr &spTexture )
{
	SKY_ASSERT( nodeNum >= 2 );

	m_pMember->m_NodeNum = nodeNum;
	m_pMember->m_Width = width;
	m_pMember->m_NodePos.Reset( nodeNum );

	for( u32 i = 0 ; i < nodeNum ; i++ )
	{
		f32 y = ( 0.2f * i );
		m_pMember->m_NodePos[ i ] = math::CBasicVector3( 0 , y , 0 );
	}

	u32 nVnum  = ( nodeNum * 2 );	//頂点数
	u32 nFace  = ( nVnum - 2 );	    //ポリゴン数
	u32 n4Face = ( nFace / 2 );	    //四角形数
	f32 dv     = 1.0f / ( nodeNum - 1 );

	math::CBasicVector3Vector posArray;
	math::CBasicVector2Vector uvArray;
	graphic::CVector_CColor   colorArray;
	CVector_u16               indexArray;

	posArray.Reset( nVnum );
	uvArray.Reset( nVnum );
	colorArray.Reset( nVnum );
	indexArray.Reset( nFace * 3 );

	//頂点情報作成
	for( u32 i = 0 ; i < nodeNum ; i++ )
	{
		s32 nOff2 = i * 2;

		//位置
		posArray[ nOff2 ].X     = m_pMember->m_NodePos[ i ].X - width; 
		posArray[ nOff2 ].Y     = m_pMember->m_NodePos[ i ].Y;
		posArray[ nOff2 ].Z     = m_pMember->m_NodePos[ i ].Z;
		posArray[ nOff2 + 1 ].X = m_pMember->m_NodePos[ i ].X + width; 
		posArray[ nOff2 + 1 ].Y = m_pMember->m_NodePos[ i ].Y;
		posArray[ nOff2 + 1 ].Z = m_pMember->m_NodePos[ i ].Z;

		//ＵＶ
		uvArray[ nOff2 ].X = 0.0;
		uvArray[ nOff2 ].Y = dv * ( i );
		uvArray[ nOff2 + 1 ].X = 1.0;
		uvArray[ nOff2 + 1 ].Y = dv * ( i );

		//頂点色
		u8 a = ( u8 )( 255.0f * ( 1.0f- dv * ( i ) ) ) ;
		colorArray[ nOff2 ]     = CColor( 1.0f , 1.0f , 1.0f , ( colortype )a / ( colortype )255 );
		colorArray[ nOff2 + 1 ] = CColor( 1.0f , 1.0f , 1.0f , ( colortype )a / ( colortype )255 );
	}

	//インデックス情報作成
	for( u16 i = 0 ; i < n4Face ; i++ )
	{
		u16 nOff6 = ( i * 6 );
		u16 nOff2 = ( i * 2 );

		indexArray[ nOff6 + 0 ] = nOff2 + 0;
		indexArray[ nOff6 + 1 ] = nOff2 + 3;
		indexArray[ nOff6 + 2 ] = nOff2 + 1;

		indexArray[ nOff6 + 3 ] = nOff2 + 0;
		indexArray[ nOff6 + 4 ] = nOff2 + 2;
		indexArray[ nOff6 + 5 ] = nOff2 + 3;
	}

	//スプライトの作成
	m_pMember->m_spModel = graphic::CModelUtil::Create( posArray , indexArray , uvArray , colorArray , spTexture );
}

void CBillboardBeam::Dispose()
{
}

void CBillboardBeam::SetNodePos( u32 no , const math::CBasicVector3 &pos )
{
	SKY_ASSERT( no < m_pMember->m_NodeNum );

	m_pMember->m_NodePos[ no ] = pos;
}

void CBillboardBeam::vDraw()
{
	math::CMatrix4x3 viewMat = m_spCamera->GetViewMatrix();

	//カメラの位置
	math::CBasicVector3 vC = m_spCamera->GetData().m_Eye; 

	CVertexBufferStreamPtr posStream = m_pMember->m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetPositionStream();

	math::CBasicVector3 pos;
	for( vrtxtype i = 1 ; i < ( m_pMember->m_NodeNum - 1 ) ; i++ )
	{
		//ポイント
		math::CBasicVector3 vO = m_pMember->m_NodePos[ i - 1 ];
		math::CBasicVector3 vP = m_pMember->m_NodePos[ i ];
		math::CBasicVector3 vQ = m_pMember->m_NodePos[ i + 1 ];

		//カメラへ向かうベクトル
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//接線ベクトル
		math::CBasicVector3 vT = ( vQ - vO ).Normalize();

		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG = vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF = vP - ( vR * m_pMember->m_Width );   

		//位置
		vrtxtype nOff2 = i * 2;

		//位置を変更
		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//先頭
	{
		math::CBasicVector3 vP = m_pMember->m_NodePos[ 0 ];
		math::CBasicVector3 vQ = m_pMember->m_NodePos[ 1 ];

		//カメラへ向かうベクトル
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//接線ベクトル
		math::CBasicVector3 vT = ( vQ - vP ).Normalize();

		//
		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG =  vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF =  vP - ( vR * m_pMember->m_Width );   

		//位置
		vrtxtype nOff2 = 0;

		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//お尻
	{
		math::CBasicVector3 vO = m_pMember->m_NodePos[ m_pMember->m_NodeNum - 2 ];
		math::CBasicVector3 vP = m_pMember->m_NodePos[ m_pMember->m_NodeNum - 1 ];

		//カメラへ向かうベクトル
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//接線ベクトル
		math::CBasicVector3 vT = ( vP - vO ).Normalize();

		//
		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG =  vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF =  vP - ( vR * m_pMember->m_Width );   

		//位置
		vrtxtype nOff2 = ( m_pMember->m_NodeNum - 1 ) * 2;

		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//GPUに反映
	posStream->WirteGpuMemory();

	//シェーダーIDの構築（BlinnPhongShaderObjectではライトの状態に応じてシェーダーを作る）
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

	if ( m_RenderingPassNo == 0 )
	{
		//描画
		DrawPass1();
	}
}

void CBillboardBeamUtil::Move_Thunder( const CBillboardBeamPtr &spEffect )
{
	math::CBasicVector3 p0;

	for( s32 i = 0 ; i < spEffect->m_pMember->m_NodeNum ; i++ )
	{
		p0.X = ( ( dectype )( rand() % 255 ) / 255.0f ) * 0.4f;
		p0.Y = -1.0f + ( f32 )( i ) * 0.2f;
		p0.Z = ( f32 )( i ) * 0.2f;

		spEffect->SetNodePos( i , p0 );
	}
}

//無限回転
void CBillboardBeamUtil::Move_Rot( const CBillboardBeamPtr &spEffect )
{
	spEffect->m_pMember->m_Count += 0.02f;

	math::CBasicVector3 p0;
	for( s32 i = 0 ; i < spEffect->m_pMember->m_NodeNum ; i++ )
	{
		p0.X = (cos( spEffect->m_pMember->m_Count - ((dectype)i * 0.1f) ) * 1.0f);
		p0.Y = (sin( spEffect->m_pMember->m_Count * 1.5f - ((dectype)i * 0.1f) ) * 1.0f);
		p0.Z = 1.0f + (sin( spEffect->m_pMember->m_Count - ((dectype)i * 0.1f) ) * 1.0f);

		spEffect->SetNodePos( i , p0 );
	}
}

} } }

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//頂点シェーダー入力値構造体
struct BillboardBeamShaderConst
{
	//頂点変換行列
    XMFLOAT4X4 WorldViewProjectionMatrix;
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

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// 頂点シェーダ作成
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BillboardBeam.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT   , 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "COLOR"      , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "TEXCOORD"   , 0 , DXGI_FORMAT_R32G32_FLOAT,       2 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
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
		ConstDesc.ByteWidth			  = sizeof( BillboardBeamShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// ピクセルシェーダ作成
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BillboardBeam.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			SKY_PANIC_MSG( _T( "頂点シェーダーのコンパイルに失敗しました \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

		//IDに関連づくシェーダーファイル保存する
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CBillboardBeam::DrawPass1()
{
	//定数バッファ受け渡し用の変数
	BillboardBeamShaderConst shaderConst;

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

	CMesh *pMesh = ( CMesh * )m_pMember->m_spModel->GetMeshContena()->GetMesh().GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		//アルファ値
		shaderConst.Alpha = spMatDrawData->GetMaterial()->Alpha;

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
		streamList.AddTail( pMesh->GetVertexBuffer()->GetColorStream() );
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

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#endif
