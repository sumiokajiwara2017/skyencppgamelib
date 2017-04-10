#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >----------------------------------------------//

class CScene;
typedef SmartPointer< CScene >			    CScenePtr;			//スマートポインタ型定義
typedef CList		< dectype , CScenePtr > CScenePtrList;		//スマートポインタリスト型
typedef CHash		< CScenePtr >           CScenePtrHash;		//Hash定義

/// <summary>
/// シーンクラス
/// ★★★このクラスの情報が１描画エリアを表す★★★
/// もし画面分割で対戦のゲームの場合、画面分このクラスが居るはずになる。
/// ライトをメンバに加えなかった理由：ライトは各シーングラフのノードに対して当てるものだから。
/// シーングラフのルートをメンバに加えなかった理由：シーングラフは違うシーンで同じものを使用する可能性があったため。
/// </summary>
class CScene : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CScene(){}

	/// <summary>
	/// 使用準備
	/// </summary>
	void               SetUp();

//--Leaf

	/// <summary>
	/// レンダリングターゲット（シーンをテクスチャに書き出したい等の場合はテクスチャレンダリングターゲットを設定する）
	/// </summary>
	CRenderTargetPtr       GetRenderTarget( skyBool isRequired = skyTrue );
	void                   SetRenderTarget( const CRenderTargetPtr &spRenderTarget );

	/// <summary>
	/// ルートの深度ステンシルバッファ
	/// </summary>
	CDepthStencilBufferPtr GetDepthStencilBuffer( skyBool isRequired = skyTrue );
	void                   SetDepthStencilBuffer( const CDepthStencilBufferPtr &spDepthStencilBuffer );

	/// <summary>
	/// ビューポート
	/// </summary>
	CViewPortPtr           GetViewPort( skyBool isRequired = skyTrue );
	void                   SetViewPort( const CViewPortPtr &spViewPort );

	/// <summary>
	/// シザリング矩形
	/// </summary>
	CScissorRectPtr        GetScissorRect( skyBool isRequired = skyTrue );
	void                   SetScissorRect( const CScissorRectPtr &spScissorRect );

	/// <summary>
	/// カメラ管理
	/// </summary>
	CameraManagerPtr       GetCameraManager( skyBool isRequired = skyTrue );
	void                   SetCameraManager( const CameraManagerPtr &spCamera );

	/// <summary>
	/// ライト
	/// </summary>
	CLightPtr              GetLight( skyBool isRequired = skyTrue );
	void                   SetLight( const CLightPtr &spLight );

	/// <summary>
	/// レンダーステート
	/// </summary>
	CRenderStatePtr        GetRenderState( skyBool isRequired = skyTrue );
	void                   SetRenderState( const CRenderStatePtr &spRenderState );

	/// <summary>
	/// ポストエフェクト管理準備（使用するときは必ず呼ぶ）
	/// </summary>
	void SetUpPostEffect();
	const CPostEffectManagerPtr &GetPostEffectManager(); //操作のため取得

	/// <summary>
	/// シーングラフのルート
	/// </summary>
	CSceneGraphNodePtr     GetSceneGraphRoot( skyBool isRequired = skyTrue );

//---Member

	/// <summary>
	/// シーンをレンダリングしない
	/// </summary>
	skyBool			       m_IsOff;

	/// <summary>
	/// シーンをレンダリングする際に深度バッファをクリアするかどうか
	/// </summary>
	skyBool			       m_IsDepthBufferClear;

	/// <summary>
	/// シーンをレンダリングする際にステンシルバッファをクリアするかどうか
	/// </summary>
	skyBool			       m_IsStencilBufferClear;

//---Member
		
	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	skyBool LoadBinary( const stream::IStreamPtr& stream );

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

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CScenePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CScenePtr spScene( NEW__( CScene , CScene() ) );
		if ( name.IsEmpty() == skyFalse ) spScene->Name.SetName( name );
		return spScene;
	}

private:

	/// <summary>
	/// ポストエフェクト管理（任意設定）
	/// </summary>
	CPostEffectManagerPtr m_spPostEffectManager;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CScene() : m_IsDepthBufferClear( skyTrue ) , m_IsStencilBufferClear( skyFalse ) , m_IsOff( skyFalse ){}

};

#define CSceneCreate_()	            sky::lib::graphic::CScene::Create()
#define CSceneCreateName_( name )	sky::lib::graphic::CScene::Create( name )

} } }