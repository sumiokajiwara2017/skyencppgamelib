#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

SkyImplementRTTI( sky::lib::graphic::CTexture , sky::lib::graphic::CSceneGraphNode );

//===================================【Common】===================================================>

namespace sky { namespace lib { namespace graphic {

const serialize::CSerializeInterfacePtr& CTexture::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CTexture::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CTexturePtr )spContainer )->m_pMember = m_pMember;
	( ( CTexturePtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CTexture::Clone()
{
	return Copy( Create( hash::CHashStringPtr() ) );
}

serialize::CSerializeInterfacePtr CTexture::CloneSharing()
{
	return CopySharing( Clone() );
}

void CTexture::Activate()
{
	ACTIVATE_COMMON();
	Load();
}

void CTexture::Deactivate()
{
	DEACTIVATE_COMMON();
	UnLoad();
}

CTexture::~CTexture()
{
}

} } }

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

/*

この機能は色々未完成です。完成する必要がある理由ができるまでこのままにしておきます。

*/

namespace sky { namespace lib { namespace graphic {

//-----------------Util-------------------▽

template < class T >
static void ConvertTexInfo_(  const texture::CTextureInfo &src , T &dest )
{
	//幅取得
	dest.Width  = src.Width;

	//高さ取得
	dest.Height = src.Height;

	//厳密フォーマット
	if ( src.Format == texture::eTextureFormat_R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_A8R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_A32R32G32B32 )
	{ dest.Format = DXGI_FORMAT_BC1_UNORM; }
	else if ( src.Format == texture::eTextureFormat_P8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_X8R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }

	//ミップマップ数
	dest.MipLevels = src.MipLevels;
}
template < class T >
static void ConvertTexInfo_( const T &src , texture::CTextureInfo &dest )
{
	//幅取得
	if ( src.Width == ( UINT )texture::eTexture_with_1
	  || src.Width == ( UINT )texture::eTexture_with_2
	  || src.Width == ( UINT )texture::eTexture_with_4 
	  || src.Width == ( UINT )texture::eTexture_with_8 
      || src.Width == ( UINT )texture::eTexture_with_16
      || src.Width == ( UINT )texture::eTexture_with_32
      || src.Width == ( UINT )texture::eTexture_with_64
      || src.Width == ( UINT )texture::eTexture_with_128
      || src.Width == ( UINT )texture::eTexture_with_256
      || src.Width == ( UINT )texture::eTexture_with_512
      || src.Width == ( UINT )texture::eTexture_with_768
      || src.Width == ( UINT )texture::eTexture_with_1024 )
	{ dest.Width = ( texture::eTexture_with )src.Width; } 
	else
	{ SKY_PANIC_MSG( _T( "The width of the texture is illegal." ) ); } //テクスチャの横幅が不正です。

	//高さ取得
	if ( src.Height == ( UINT )texture::eTexture_height_1
	  || src.Height == ( UINT )texture::eTexture_height_2
      || src.Height == ( UINT )texture::eTexture_height_4 
      || src.Height == ( UINT )texture::eTexture_height_8 
      || src.Height == ( UINT )texture::eTexture_height_16
      || src.Height == ( UINT )texture::eTexture_height_32
      || src.Height == ( UINT )texture::eTexture_height_64
      || src.Height == ( UINT )texture::eTexture_height_128
      || src.Height == ( UINT )texture::eTexture_height_256
      || src.Height == ( UINT )texture::eTexture_height_512
      || src.Height == ( UINT )texture::eTexture_height_768
      || src.Height == ( UINT )texture::eTexture_height_1024 )
	{ dest.Height = ( texture::eTexture_height )src.Height; } 
	else
	{ SKY_PANIC_MSG( _T( "The width of length of the texture is illegal." ) ); } //テクスチャの縦幅が不正です。

	//厳密フォーマット
	if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{ 
		dest.Format = texture::eTextureFormat_R8G8B8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{ 
		dest.Format = texture::eTextureFormat_A8R8G8B8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{
		dest.Format = texture::eTextureFormat_A32R32G32B32;
	}
	else if ( src.Format == DXGI_FORMAT_BC1_UNORM )
	{
		dest.Format = texture::eTextureFormat_P8;
	}
	else if ( src.Format == DXGI_FORMAT_BC2_UNORM )
	{
		dest.Format = texture::eTextureFormat_P8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{
		dest.Format = texture::eTextureFormat_X8R8G8B8;
	}
	else
	{
		SKY_PANIC_MSG( _T( "The file format of the texture is illegal." ) ); //テクスチャのファイルフォーマットが不正です。
	}

	//ミップマップ数
	dest.MipLevels = src.MipLevels;
}

//-----------------Util-------------------△

CTexture::CTexture() : m_pMember( NEW__( CTextureMember , CTextureMember() ) )
{
	//テクスチャサンプラステートの作成
	D3D11_SAMPLER_DESC SamlerDesc = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,		// D3D11_FILTER Filter;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressU;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressV;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressW;
		0.0f,									// FLOAT MipLODBias;
		1,										// UINT MaxAnisotropy;
		D3D11_COMPARISON_ALWAYS,				// D3D11_COMPARISON_FUNC ComparisonFunc;
		{ 1.0f, 1.0f, 1.0f, 1.0f },				// FLOAT BorderColor[ 4 ];
		0.0f,									// FLOAT MinLOD;
		D3D11_FLOAT32_MAX,						// FLOAT MaxLOD;
	};
	m_pMember->m_SamplerStateDesc = SamlerDesc;
	m_pMember->ReCreateSamplerState();
}

void CTextureMember::ReCreateSamplerState()
{
	//現在の情報を解放する
	if ( m_spTextureSamplerState.IsEmpty() == skyFalse ) 
	{
		m_spTextureSamplerState.Release();
	}

	//新しくステートを作りなおす
	ID3D11SamplerState  *pSamlerState = NULL;
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateSamplerState( &m_SamplerStateDesc, &pSamlerState );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "テクスチャサンプラーステート \n" ) );
	};
	m_spTextureSamplerState.SetPtr( pSamlerState );
}

void CTexture::SetTextureWrapRepeat()
{
	//変更したい部分だけ変更する
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//再作成
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapClamp()
{
	//変更したい部分だけ変更する
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//再作成
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapEdge()
{
	//変更したい部分だけ変更する
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//再作成
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapBorder()
{
	//変更したい部分だけ変更する
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	//再作成
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapBorderColor( CColor color )
{
	//変更したい部分だけ変更する
	m_pMember->m_SamplerStateDesc.BorderColor[ 0 ] = color.r;
	m_pMember->m_SamplerStateDesc.BorderColor[ 1 ] = color.g;
	m_pMember->m_SamplerStateDesc.BorderColor[ 2 ] = color.b;
	m_pMember->m_SamplerStateDesc.BorderColor[ 3 ] = color.a;

	//再作成
	m_pMember->ReCreateSamplerState();
}

void CTexture::Load()
{//同期

	if ( m_pMember->m_spTexture.IsEmpty() == skyTrue )
	{
		if ( CPathUtil_::IsFileOn( Name.GetName() ) == skyFalse )
		{
			SKY_PANIC_MSG( _T( "The file did not exist. " ) ); //テクスチャファイルを読み込もうとしましたが、ファイルが存在しませんでした。
			return;
		}

		stream::IStreamPtr inStream = CFileUtil_::LoadFile( Name.GetName() );

		SKY_PRINTF( _T( "%s \n" ) , Name.GetString() );

		//テクスチャを読む
		inStream->vOpen();
		Load( inStream );
		inStream->vClose();
	}
}

void CTexture::Load( const stream::IStreamPtr& stream )
{
	ID3D11Resource* pTexture = NULL;

	if ( m_pMember->m_spTexture.IsEmpty() == skyTrue )
	{
		//テクスチャを読み込む
		HRESULT hr = D3DX11CreateTextureFromMemory
		( CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst() ,
		  stream->vGetData() ,
		  stream->vGetSize() ,
		  NULL ,
		  NULL ,
		  &pTexture ,
		  NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "CTexture::Load( const stream::IStreamPtr& stream )が失敗しました。\n" ) );
		}

		//テクスチャ情報取得
		D3DX11_IMAGE_INFO imageInfo;
		hr = D3DX11GetImageInfoFromMemory
		( stream->vGetData() ,
		  stream->vGetSize() ,
		  NULL ,
		  &imageInfo ,
		  NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "CTexture::Load( const stream::IStreamPtr& stream )が失敗しました。\n" ) );
		}

		//シェーダーリソースビューを生成
		ID3D11ShaderResourceView *pTextureResView;
		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
		ShaderResourceViewDesc.Format						= imageInfo.Format;
		ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
		ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
		hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( pTexture , &ShaderResourceViewDesc, &pTextureResView );

		//テクスチャ情報を設定する
		ConvertTexInfo_< D3DX11_IMAGE_INFO >( imageInfo , m_Info );

		//スマートポインタに設定
		m_pMember->m_spTexture.SetPtr( pTexture );
		m_pMember->m_spShaderResourceView.SetPtr( pTextureResView );
	}
}

void CTexture::Create( const texture::CTextureInfo &texInfo )
{
	SKY_ASSERT_MSG( m_pMember->m_spTexture.IsEmpty() == skyFalse , _T( "すでに作成済みです。作成しなおすときはUnLoad()を実行してください。\n" ) );

	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D *pTexture = skyNull;

	//入力値をＤ３Ｄ用に変換
	ConvertTexInfo_< D3D11_TEXTURE2D_DESC >( texInfo , desc );

	//固定値（ここの値を外部からカスタマイズ可能にするにはtexture::CTextureInfoに項目を増やしてConvertTexInfo_の中で受け渡し処理を書かないといけない）
	desc.ArraySize          = 1;
	desc.SampleDesc.Count   = 1;                          // サンプリングは1ピクセルのみ
	desc.SampleDesc.Quality = 0;
//	desc.Usage              = D3D11_USAGE_DYNAMIC;        // CPU書き込み可能
//	desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE; // シェーダリソース
//	desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;     // CPUから書き込みアクセス可
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags	    = 0;
	desc.MiscFlags          = 0;

	HRESULT	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &desc, 0, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )が失敗しました。\n" ) );
	}

	//シェーダーリソースビューを生成
	ID3D11ShaderResourceView *pTextureResView;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( pTexture , &ShaderResourceViewDesc, &pTextureResView );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )が失敗しました。\n" ) );
	}

	//テクスチャ情報を格納
	m_Info = texInfo;

	//スマートポインタに設定
	m_pMember->m_spTexture.SetPtr( pTexture );
	m_pMember->m_spShaderResourceView.SetPtr( pTextureResView );
}


void CTexture::SetShader()
{
	SKY_ASSERT_MSG( m_pMember->m_spTexture.IsEmpty() == skyFalse , _T( "This Texture not Load yet." ) );

	//サンプラーを設定
	ID3D11SamplerState* pSamplerTbl[] = { m_pMember->m_spTextureSamplerState.GetPtrNoConst() };
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetSamplers( 0, 1, pSamplerTbl );

	//テクスチャへの参照を設定
	ID3D11ShaderResourceView* pResViewTbl[] = { m_pMember->m_spShaderResourceView.GetPtrNoConst() };
    CDevice_->GetMember()->m_spD3DDeviceContext->PSSetShaderResources( 0  , 1 , pResViewTbl );
}


void CTexture::UnLoad()
{
	//テクスチャを破棄
	m_pMember->m_spTexture.Delete();
	m_pMember->m_spShaderResourceView.Delete();
}

//変換テーブル
D3DX11_IMAGE_FILE_FORMAT d3dformat[] = 
{
    D3DX11_IFF_BMP ,
    D3DX11_IFF_JPG ,
    D3DX11_IFF_PNG ,
    D3DX11_IFF_DDS ,
    D3DX11_IFF_TIFF ,
    D3DX11_IFF_GIF ,
    D3DX11_IFF_WMP ,
};


void CTexture::Save( texture::eTextureFileFormat eFileFormat )
{
	Save( eFileFormat , Name.GetName() );
}


void CTexture::Save(  texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path )
{
	HRESULT hr;

	// マルチサンプリングされたリソースをマルチサンプリングされていないリソースにコピーする
	ID3D11Texture2D * pBackBuffer = ( ID3D11Texture2D * )m_pMember->m_spTexture.GetPtrNoConst();
	ID3D11Texture2D* pTexture = NULL;
	D3D11_TEXTURE2D_DESC texDesc;
	pBackBuffer->GetDesc( &texDesc );
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count   = 1; //ここと
	texDesc.SampleDesc.Quality = 0; //ここがこの値ならコピーは本来必要ない。でもすべてコピーすることで解決

	// コピー先用のテクスチャーを作成する。
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &texDesc, NULL, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CTexture::Saveが失敗しました。\n" ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext->ResolveSubresource( pTexture, NULL, pBackBuffer, NULL, texDesc.Format );

	// ファイル出力
	hr = D3DX11SaveTextureToFile( CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst() , pTexture , d3dformat[ eFileFormat ] , path );

	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CTexture::Saveが失敗しました。\n" ) );
	}
}

skyBool CTexture::IsLoaded ()
{
	return ( m_pMember->m_spTexture.IsEmpty() == skyTrue ) ? skyFalse : skyTrue;
}

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

CTexture::CTexture() : m_pMember( NEW__( CTextureMember , CTextureMember() ) )
{
}

void CTexture::Load()
{//同期

	if ( m_pMember->m_IsLoad == skyFalse )
	{
		//　テクスチャのロード
#ifdef UNICODE
		graphic::CreateTextureFromFileW( Name.GetName()->GetString() , m_pMember->m_TexObj );
#else
		graphic::CreateTextureFromFileA( Name.GetName()->GetString() , m_pMember->m_TexObj );
#endif

		m_pMember->texId[ 0 ] = m_pMember->m_TexObj.GetHandle();

		m_Info.Width = ( texture::eTexture_with )m_pMember->m_TexObj.m_Width;
		m_Info.Height = ( texture::eTexture_height )m_pMember->m_TexObj.m_Height;
	}
}

void CTexture::Load( const stream::IStreamPtr& stream )
{
	SKY_UNUSED_ARG( stream );
}

void CTexture::Create( const texture::CTextureInfo &texInfo )
{
	SKY_UNUSED_ARG( texInfo );
}

void CTexture::SetShader()
{
}

void CTexture::UnLoad()
{
	m_pMember->m_IsLoad = skyFalse;
}

void CTexture::Save( texture::eTextureFileFormat eFileFormat )
{
	SKY_UNUSED_ARG( eFileFormat );
}

void CTexture::Save(  texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path )
{
	SKY_UNUSED_ARG( eFileFormat );
	SKY_UNUSED_ARG( path );
}

skyBool CTexture::IsLoaded ()
{
	return m_pMember->m_TexObj.IsLoaded();
}

void CTexture::SetTextureWrapRepeat()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapClamp()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapEdge()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapBorder()
{

	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapBorderColor( CColor color )
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	f32 colorData[ 3 ];
	color.Copyf32Array3RGB( colorData );
	glTexParameterfv( GL_TEXTURE_2D , GL_TEXTURE_BORDER_COLOR , colorData );
	glBindTexture( GL_TEXTURE_2D , 0 );
}

} } }

#endif

