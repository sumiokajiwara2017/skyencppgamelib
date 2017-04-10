#pragma once

namespace sky { namespace lib { namespace graphic {

	/// <summary>
	/// サンプラーデータ
	/// 各サンプラーの使い方はポストエフェクトによる
	/// </summary>
	struct CSamplerData
	{
		/// <summary>
		/// サブサンプラー０
		/// </summary>
		CTexturePtr m_Sampler0;

		/// <summary>
		/// サブサンプラー１
		/// </summary>
		CTexturePtr m_Sampler1;

		/// <summary>
		/// サブサンプラー２
		/// </summary>
		CTexturePtr m_Sampler2;

		/// <summary>
		/// 代入演算子
		/// </summary>
		CSamplerData& operator =( const CSamplerData& sampler )
		{
			m_Sampler0 = sampler.m_Sampler0;
			m_Sampler1 = sampler.m_Sampler1;
			m_Sampler2 = sampler.m_Sampler2;

			return *this;
		}
	};

} } }

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// シェーダーインスタンス格納Hashサイズ
/// </summary>
static const u32 SHADER_HASH_SIZE = 31;

/// <summary>
/// Ｄ３Ｄ頂点シェーダー
/// </summary>
typedef SmartPointerCom< ID3D11VertexShader > ID3D11VertexShaderPtr;

/// <summary>
/// Ｄ３Ｄシェーダーグローバル変数バッファ
/// </summary>
typedef SmartPointerCom< ID3D11Buffer >       ID3DXConstantBufferPtr;

/// <summary>
/// Ｄ３Ｄピクセルシェーダー
/// </summary>
typedef SmartPointerCom< ID3D11PixelShader >  ID3D11PixelShaderPtr;

/// <summary>
/// Ｄ３Ｄピクセルシェーダー
/// </summary>
typedef SmartPointerCom< ID3D11InputLayout >  ID3D11InputLayoutPtr;

/// <summary>
///  パス情報情報
/// </summary>
struct PassInfo
{
	ID3D11VertexShaderPtr  m_spVertexShader;
	ID3D11PixelShaderPtr   m_spPixelShader;
	ID3D11InputLayoutPtr   m_spVertexLayout;
};

/// <summary>
///  シェーダー情報クラス
/// </summary>
struct CShaderInfo : public base::SkyRefObject
{
	//シェーダーパス情報
	PassInfo           m_Pass1;
	PassInfo           m_Pass2;
	PassInfo           m_Pass3;

	/// <summary>
	///  グローバル変数テーブル
	/// </summary>
	ID3DXConstantBufferPtr m_spVertexConstBuffer0;
	ID3DXConstantBufferPtr m_spVertexConstBuffer1;
	ID3DXConstantBufferPtr m_spVertexConstBuffer2;
	ID3DXConstantBufferPtr m_spVertexConstBuffer3;
	ID3DXConstantBufferPtr m_spVertexConstBuffer4;
	ID3DXConstantBufferPtr m_spVertexConstBuffer5;
	ID3DXConstantBufferPtr m_spVertexConstBuffer6;
	ID3DXConstantBufferPtr m_spVertexConstBuffer7;
	ID3DXConstantBufferPtr m_spVertexConstBuffer8;
};
typedef SmartPointer< CShaderInfo >	CShaderInfoPtr;		//スマートポインタ版定義
typedef CHash< CShaderInfoPtr >		CShaderInfoPtrHash;	//Hash定義

} } }

// シェーダのコンパイル オプション
#if defined( DEBUG ) || defined( _DEBUG )
static const u16 SHADER_COMPILE_OPTION = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
static const u16 SHADER_COMPILE_OPTION = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif

#endif

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// シェーダーインスタンス格納Hashサイズ
/// </summary>
static const u32 SHADER_HASH_SIZE = 31;

/// <summary>
///  UBO情報
/// </summary>
struct UBOInfo
{
	GLuint id[1];
	GLuint index;
	GLint  blockSize;
};
#define UBO_MAX_NUM 16

/// <summary>
///  パス情報情報
/// </summary>
struct PassInfo
{
#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON
	GLuint             m_ProgramObject;
#else
	CCgCombineProgram  m_ProgramObject;
#endif

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON
#define LOCATION GLint
#endif
#ifdef SW_SKYGRAPHICLIB_USE_CG_ON
#define LOCATION CGparameter
#endif

	//頂点属性（各モデルの各頂点毎の情報）
	LOCATION  m_VertexLocation;
	LOCATION  m_NormalLocation;
	LOCATION  m_TexCoord0Location;

	//インスタンス属性（各モデル毎の情報）
    LOCATION m_WvpLocation;
    LOCATION m_WvptLocation;
    LOCATION m_WorldLocation;
    LOCATION m_ViewLocation;
    LOCATION m_ProjectionLocation;
	LOCATION m_JointsLocation;
    LOCATION m_Texture0Location;
    LOCATION m_Texture1Location;
    LOCATION m_Texture2Location;

	//マテリアル
    LOCATION m_AmbientLocation;
    LOCATION m_DiffuseLocation;
    LOCATION m_SpecularLocation;
    LOCATION m_AlphaLocation;
    LOCATION m_PowerLocation;

	//ライト
    LOCATION m_LightAmbientLocation;
    LOCATION m_LightDiffuseLocation;
	LOCATION m_LightSpecularLocation;

	//ベクトル
    LOCATION m_LightDirLocation;
    LOCATION m_ViewDirLocation;

	//自由に使ってよし。配列にしなかったのは問題が起こるのが嫌だったから。
	LOCATION m_FreeLocation0;
	LOCATION m_FreeLocation1;
	LOCATION m_FreeLocation2;
	LOCATION m_FreeLocation3;
	LOCATION m_FreeLocation4;
	LOCATION m_FreeLocation5;
	LOCATION m_FreeLocation6;
	LOCATION m_FreeLocation7;
	LOCATION m_FreeLocation8;
	LOCATION m_FreeLocation9;

	UBOInfo  m_Ubo[ UBO_MAX_NUM ];
};

/// <summary>
///  シェーダー情報クラス
/// </summary>
struct CShaderInfo : public base::SkyRefObject
{
	//シェーダーパス情報
	PassInfo           m_Pass1;
	PassInfo           m_Pass2;
	PassInfo           m_Pass3;
};
typedef SmartPointer< CShaderInfo >	CShaderInfoPtr;		//スマートポインタ版定義
typedef CHash< CShaderInfoPtr >		CShaderInfoPtrHash;	//Hash定義

#define BUFFER_OFFSET( bytes ) ( ( GLubyte * )NULL + ( bytes ) )

} } }

#endif
