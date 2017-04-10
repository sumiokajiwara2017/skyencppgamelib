#pragma once

namespace sky { namespace lib { namespace graphic {

const u32 SCREEN_LAYER_MAX_NUM = 16;

/// <summary>
/// スクリーンセットアップデータ
/// </summary>
struct CScreenObjectSetUpData
{
	hash::CHashStringPtr      Name;                  //スクリーン名
	math::CBasicRect          WindowRect;            //Window矩形
	CameraPtr                 Camera;                //カメラ
	CColor                    ClearColor;            //画面クリアカラー
	CWindowPtr                Window;                //ウィンドウ（自分で作成したWindowを使用したい場合は設定する）
	CRenderTargetPtr	      RenderTarget;          //レンダーターゲット
	CDepthStencilBufferPtr    DepthStencilBuffer;    //深度ステンシルバッファ
    CRenderStatePtr           RenderState;           //レンダーステート
    skyBool                   IsSetParentWindowSize; //親のWindowのサイズをVieportやSissorサイズに使用する

	CScreenObjectSetUpData() : IsSetParentWindowSize( skyFalse ){}
};

class CScreenObject;
typedef SmartPointer< CScreenObject >                 CScreenObjectPtr;			//スマートポインタ型定義
typedef CList< s32 , CScreenObjectPtr >               CScreenObjectPtrList;		//スマートポインタリスト型
typedef CHash< CScreenObjectPtr >	                  CScreenObjectPtrHash;		//スマートポインタリスト型
typedef CStack< CScreenObjectPtr >	                  CScreenObjectPtrStack;    //スマートポインタリスト型

/// <summary>
/// スクリーンオブジェクトクラス
/// スクリーンオブジェクトは、簡単に画面を出すためのフレームワークです。
/// </summary>
class CScreenObject : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CScreenObject();

	/// <summary>
	/// スクリーンの無効設定（無効にされたスクリーンは描画されません。）
	/// </summary>
	void SetDisableOn( skyBool isDisable );
	skyBool IsDisableOn();

	/// <summary>
	/// スクリーンの無効設定（無効にされたスクリーンは描画されません。）
	/// </summary>
	void SetReSizeOn( skyBool isReSize );
	skyBool IsReSizeOn();

	/// <summary>
	/// シーンのを取得する
	/// </summary>
	CScenePtr GetScene( skyBool isRequired = skyTrue );

	/// <summary>
	/// 子スクリーンを追加する（オフラインレンダリングなどに使用。追加されたスクリーンが先に描画される）
	/// </summary>
	CScreenObjectPtr AddChild( const CScreenObjectSetUpData &param );

	/// <summary>
	/// スクリーンを上乗せする（３Ｄ描画の上に２Ｄ描画をのせるなどに使用。上乗せされたスクリーンが後に描画される）
	/// </summary>
	CScreenObjectPtr PushLayer( const CScreenObjectSetUpData &param );

	/// <summary>
	/// 描画
	/// </summary>
	void Rendering( const fps::CFpsController &fps );

//-------------Rootのみ持っている(Branch)----------------▽

	/// <summary>
	/// セットアップ時に生成されたウィンドウ
	/// </summary>
	CWindowPtr             GetWindow( skyBool isRequired = skyTrue );

	/// <summary>
	/// セットアップ時に生成されたレンダリングターゲット
	/// </summary>
	CRenderTargetPtr       GetRenderTarget( skyBool isRequired = skyTrue );

	/// <summary>
	/// セットアップ時に生成された深度ステンシルバッファ
	/// </summary>
	CDepthStencilBufferPtr GetDepthStencilBuffer( skyBool isRequired = skyTrue );

	/// <summary>
	/// セットアップ時に生成された全体にかけるレンダーステート
	/// </summary>
	CRenderStatePtr        GetRenderState( skyBool isRequired = skyTrue );

//-------------Rootのみ持っている----------------△

//---Serialize
			
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
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

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
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// 名前でスクリーンを取得（レイヤースクリーンにも子スクリーンにも名前でここからグローバルアクセスできる）
	/// </summary>
	static CScreenObjectPtr Get( const hash::CHashStringPtr &name );

	/// <summary>
	/// スクリーンを生成する
	/// </summary>
	static CScreenObjectPtr Create( const CScreenObjectSetUpData &param );

	/// <summary>
	/// Loaderから呼ぶScreenObjectの生成
	/// </summary>
	static CScreenObjectPtr Create( const hash::CHashStringPtr &name )
	{
		
		CScreenObjectPtr spScreenObject( NEW__( CScreenObject , CScreenObject() ) );
		if ( name.IsEmpty() == skyFalse ) spScreenObject->Name.SetName( name );
		return spScreenObject;
	}

private:

	/// <summary>
	/// 無効化フラグ
	/// </summary>
	skyBool               m_IsDisable;

	/// <summary>
	/// リサイズ実行
	/// </summary>
	skyBool               m_IsReSizeOn;

	/// <summary>
	/// レイヤースクリーンスタック
	/// </summary>
	CScreenObjectPtrStack m_LayerScreenStack;

	/// <summary>
	/// 子スクリーンリスト
	/// </summary>
	CScreenObjectPtrList  m_ChildScreenList;

	/// <summary>
	/// サイズ変更感知デリゲート
	/// </summary>
	delegate::IDelegateObjectPtr m_spReSizeDelegate;

	/// <summary>
	/// レンダリングステップ管理
	/// </summary>
	CRenderStepManagerPtr m_RenderStepManager;

	/// <summary>
	/// コンストラクタ（封印）
	/// </summary>
	CScreenObject();
	CScreenObject( const CScreenObjectPtr &spParent );
	void Constractor();

	/// <summary>
	/// リサイズ時デリゲート
	/// マルチウィンドウタイプのＯＳではウィンドウモードでゲームを行った場合、ウィンドウのサイズ変更が行われる可能性がある。
	/// ウィンドウのサイズ変更にはレンダリングターゲットの変更、ビューポートの変更、カメラのアスペクト比の変更（DirectXではステンシルバッファ
	/// の変更、スワップチェインのリロード、スワップチェインからバックバッファの再取得、D3Dオブジェクトの破棄、再生成など）
	/// などを同時に行わなければならず面倒な上、たいしてユーザーにメリットもないので、固定解像度で、何種類かユーザーが選べる
	/// 解像度パターンを用意してあげるというのがいい。一応このCScreenObjectクラスではリサイズ対応してみた。
	/// </summary>
    void ReSize      ( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );
    void ReSizeChild ( dectype scw , dectype sch );

	/// <summary>
	/// スクリーンを上乗せする（３Ｄ描画の上に２Ｄ描画をのせるなどに使用。上乗せされたスクリーンが後に描画される）
	/// </summary>
	CScreenObjectPtr CreateSubScreen( const CScreenObjectSetUpData &param );
};

} } }

#define CScreenObjectInitialize_()	sky::lib::graphic::CScreenObject::Initialize()
#define CScreenObject_				sky::lib::graphic::CScreenObject
#define CScreenObjectDispose_()		sky::lib::graphic::CScreenObject::Dispose()

#define CScreenObjectCreateName_( name )  sky::lib::graphic::CScreenObject::Create( name );
