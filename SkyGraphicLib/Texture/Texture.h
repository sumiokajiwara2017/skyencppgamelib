#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)

struct CTextureMember;
typedef SmartPointer2< CTextureMember >	CTextureMemberPtr;		//スマートポインタ版定義

//テンプレートTypedef郡
class CTexture;
typedef SmartPointer< CTexture >        CTexturePtr;		//スマートポインタ版定義
typedef CList< u32 , CTexturePtr >      CTexturePtrList;	//リスト定義
typedef CHash< CTexturePtr >            CTexturePtrHash;	//Hash定義(ポインタ）

class CTexture : public CSceneGraphNode
{
	friend class CRenderTarget;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CTexture();

	/// <summary>
	/// ロード済みフラグ
	/// </summary>
	skyBool                     IsLoaded ();

	/// <summary>
	/// 読み込み
	/// </summary>
	void                         Load    ();
	void                         Load    ( const stream::IStreamPtr& stream );

	/// <summary>
	/// 書き込み
	/// </summary>
	void                         Save    ( texture::eTextureFileFormat eFileFormat ); //内部保持しているパスで出力
	void                         Save    ( texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path ); //指定のパスで出力

	/// <summary>
	/// 作成
	/// </summary>
	void						 Create  ( const texture::CTextureInfo &texInfo );

	/// <summary>
	/// 破棄
	/// </summary>
	void                         UnLoad  ();

	/// <summary>
	/// テクスチャリピート設定
	/// </summary>
	void                         SetTextureWrapRepeat();  //はみ出した分を繰り返す
	void                         SetTextureWrapClamp();   //はみ出した分を境界の色で埋める
	void                         SetTextureWrapEdge();    //はみ出した分を境界の色で埋める
	void                         SetTextureWrapBorder();  //はみ出した分を指定の境界の色で埋める
	void                         SetTextureWrapBorderColor( CColor color );  //はみ出した分を境界の色で埋める

	/// <summary>
	/// 実装（抽象実装で実装する場合は必ずvirtualで宣言する。実装でオーバーライドされるかもしれないから）
	/// </summary>
	const hash::CHashStringPtr	 GetPath () { return Name.GetName(); }

	/// <summary>
	/// テクスチャ情報取得
	/// </summary>
	texture::CTextureInfo&       GetInfo () { return m_Info;  }

	/// <summary>
	/// テクスチャをピクセルシェーダーに設定する
	/// </summary>
	void SetShader();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline const CTextureMemberPtr& GetMember(){ return m_pMember; }

//---Serialize

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// 再ロードやデバッグ情報のためにパスは必ず引数で渡す
	/// </summary>
	static CTexturePtr	Create( const hash::CHashStringPtr &path = hash::CHashStringPtr() )
	{
		
		CTexturePtr spTexture( NEW__( CTexture , CTexture() ) );
		spTexture->Name.SetName( path );
		return spTexture;
	}

protected:

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CTextureMemberPtr   m_pMember;

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// テクスチャ情報
	/// </summary>
	texture::CTextureInfo  m_Info;

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CTexture();
};

} } }

#define CTextureCreate_()           sky::lib::graphic::CTexture::Create()
#define CTextureCreateName_( path ) sky::lib::graphic::CTexture::Create( path )
//===================================【DirectX】===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//型の再定義
typedef SmartPointerCom< ID3D11Resource >             D3D11ResourceComPtr;
typedef SmartPointerCom< ID3D11ShaderResourceView >   D3D11ShaderResourceViewPtr;
typedef SmartPointerCom< ID3D11SamplerState >         D3D11SamplerStatePtr;

struct CTextureMember
{
	/// <summary>
	/// D3Dテクスチャ
	/// </summary>
	D3D11ResourceComPtr		   m_spTexture;

	/// <summary>
	/// シェーダーリソースビューを生成する
	/// ※テクスチャをシェーダーで使わない場合は使用しない
	/// </summary>
	D3D11ShaderResourceViewPtr m_spShaderResourceView;

	/// <summary>
	/// サンプラステートの元
	/// </summary>
	D3D11_SAMPLER_DESC         m_SamplerStateDesc;

	/// <summary>
	/// サンプラステート
	/// </summary>
	D3D11SamplerStatePtr       m_spTextureSamplerState;

	/// <summary>
	/// サンプラステートの再作成
	/// </summary>
	void ReCreateSamplerState();
};

} } }

#endif
//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

#include "GLTextureLoader.h"

namespace sky { namespace lib { namespace graphic {

struct CTextureMember
{
	/// <summary>
	/// GLテクスチャハンドル
	/// </summary>
    GLuint texId[ 1 ];
	GLTextureLoader m_TexObj;

	skyBool m_IsLoad;

	CTextureMember() : m_IsLoad( skyFalse )
	{
	}

	virtual ~CTextureMember()
	{
		glDeleteTextures( 1 , &texId[ 0 ] );
	}
};

} } }

#endif
