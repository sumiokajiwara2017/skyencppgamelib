#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)
struct CDepthStencilBufferMember;

//テンプレートTypedef郡
class CDepthStencilBuffer;
typedef SmartPointer< CDepthStencilBuffer >	  CDepthStencilBufferPtr;		//スマートポインタ版定義
typedef CList< u32 , CDepthStencilBufferPtr > CDepthStencilBufferPtrList;

/// <summary>
/// 深度ステンシルノード
/// </summary>
class CDepthStencilBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual               ~CDepthStencilBuffer ();

	/// <summary>
	/// 準備
	/// </summary>
	void                  SetUp       ( const math::CBasicSize &size ); 

	/// <summary>
	/// サイズの取得
	/// </summary>
	math::CBasicSize      GetSize        (){ return m_Size; }

	/// <summary>
	/// サイズの設定
	/// </summary>
	void                  SetSize        ( const math::CBasicSize &size ){ m_Size = size; }

	/// <summary>
	/// サイズの変更
	/// </summary>
	void                  ReSize         ( u32 newWidth , u32 newHeight );

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	void				  vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	void				  vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// 開放
	/// </summary>
	void                   Release();

	/// <summary>
	/// 深度／ステンシルバッファのクリア
	/// </summary>
    void                   ClearDepthStencilBuffer();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
    void                   ClearDepthBuffer();

	/// <summary>
	/// ステンシルバッファのクリア
	/// </summary>
    void                   ClearStencilBuffer();

	/// <summary>
	/// 深度／ステンシルをテクスチャとして取得する。これはSetUpFrameBufferで生成されたレンダリングターゲット以外行えない）
	/// </summary>
	CTexturePtr            GetDepthStencilTexture();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline CDepthStencilBufferMember* GetMember(){ return m_pMember; }


//---Serialize
			
	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr 		  Clone();
	serialize::CSerializeInterfacePtr 		  CloneSharing();

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
	/// </summary>
	static CDepthStencilBufferPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CDepthStencilBufferPtr spDepthStencilBuffer( NEW__( CDepthStencilBuffer , CDepthStencilBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spDepthStencilBuffer->Name.SetName( name );
		return spDepthStencilBuffer;
	}

protected:

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CDepthStencilBufferMember*    m_pMember;

//---Member

	/// <summary>
	/// 画面縦横の長さ
	/// </summary>
	math::CBasicSize	          m_Size;

//---Member

	/// <summary>
	/// 深度バッファテクスチャ
	/// </summary>
	CTexturePtr                   m_spTexture;

	/// <summary>
	/// 自分からクローンしたインスタンスのリスト
	/// </summary>
	CDepthStencilBufferPtrList    m_spCloneList;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDepthStencilBuffer();

	/// <summary>
	/// 再帰リリース
	/// </summary>
   static void Release_( CDepthStencilBuffer *pNode );

	/// <summary>
	/// 再帰アタッチ
	/// </summary>
   static void Rettach_( CDepthStencilBuffer *pNode );

};

} } }

#define CDepthStencilBufferCreate_()	        sky::lib::graphic::CDepthStencilBuffer::Create()
#define CDepthStencilBufferCreateName_( name )	sky::lib::graphic::CDepthStencilBuffer::Create( name )

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11DepthStencilView >    D3D11DepthStencilViewPtr;

struct CDepthStencilBufferMember
{
	/// <summary>
	/// 深度／ステンシルビュー
	/// </summary>
	D3D11DepthStencilViewPtr            m_spDepthStencilView;
};

} } }

#endif