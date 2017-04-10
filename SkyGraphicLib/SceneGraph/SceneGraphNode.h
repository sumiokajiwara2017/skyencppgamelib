#pragma once

namespace sky { namespace lib { namespace graphic {

namespace scenegraph {

const u32 NODE_STACK_DEFAULT_SIZE = 32;

}
/// <summary>
/// シーングラフスマートポインタ型
/// </summary>
class CSceneGraphNode;
typedef SmartPointer< CSceneGraphNode >			CSceneGraphNodePtr;			//スマートポインタ型定義
typedef CList< u32 , CSceneGraphNodePtr >		CSceneGraphNodePtrList;		//スマートポインタリスト型
typedef CHash< CSceneGraphNodePtr >				CSceneGraphNodePtrHash;     //Hash定義
typedef CStack< CSceneGraphNodePtr >			CSceneGraphNodePtrStack;    //Stack定義

class CModel;
typedef SmartPointer< CModel >		CModelPtr;			//スマートポインタ版定義
typedef CList< u32 , CModelPtr >	CModelPtrList;		//リスト定義
typedef CHash< CModelPtr >			CModelPtrHash;		//Hash定義
typedef CHashMT< CModelPtr >		CModelPtrHashMT;	//Hash定義

/// <summary>
/// シーングラフのレンダリングパラメーター
/// </summary>
struct CRenderingParam
{
	/// <summary>
	/// 行列スタック
	/// </summary>
	math::CMatrix4x3Stack	       MatrixStack;

	/// <summary>
	/// 半透明データリスト
	/// </summary>
	CAlphaRenderingCommandPtrList  AlphaDataList;

	/// <summary>
	/// アルファデータリスト
	/// </summary>
	CSceneGraphNodePtr             Window;

	/// <summary>
	/// レンダリングターゲット
	/// </summary>
	CSceneGraphNodePtrStack        RenderTargetStack;

	/// <summary>
	/// 深度ステンシルバッファ
	/// </summary>
	CSceneGraphNodePtrStack	       DepthStencilBufferStack;

	/// <summary>
	/// ビューポート
	/// </summary>
	CSceneGraphNodePtrStack	       ViewPortStack;

	/// <summary>
	/// シザリング矩形
	/// </summary>
	CSceneGraphNodePtrStack        ScissorRectStack;

	/// <summary>
	/// カメラ
	/// </summary>
	CSceneGraphNodePtrStack	       CameraStack;

	/// <summary>
	/// レンダーステート
	/// </summary>
	CSceneGraphNodePtrStack	       RenderStateStack;

	/// <summary>
	/// シェーダー
	/// </summary>
	CSceneGraphNodePtrStack	       ShaderStack;

	/// <summary>
	/// エキストラデータ
	/// </summary>
	CSceneGraphNodePtrStack	       ExtraDataStack;

	/// <summary>
	/// ライト
	/// </summary>
	CSceneGraphNodePtrStack	       LightStack;

	/// <summary>
	/// ツリーの深さ
	/// </summary>
	skyBool                        Indent;

	/// <summary>
	/// 更新時間
	/// </summary>
	fps::CFpsController            Fps;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// デバッグプリント
	/// </summary>
	skyBool                        IsNodeTreePrint;

#endif

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderingParam() : 
	CameraStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ScissorRectStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ViewPortStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	DepthStencilBufferStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	RenderTargetStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	RenderStateStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ShaderStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	ExtraDataStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
	LightStack( scenegraph::NODE_STACK_DEFAULT_SIZE ) ,
#ifdef SW_SKYLIB_DEBUG_ON
	IsNodeTreePrint( skyFalse ) ,
#endif
	Indent( 0 )
	{}

	/// <summary>
	/// 半透明オブジェクトの描画
	/// </summary>
	void AlphaDataRendering();
};

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// シーングラフのノード。
/// シーングラフの要素となるクラスはすべてこのクラスを継承する。
/// </summary>
class CSceneGraphNode : public CEffector
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CSceneGraphNode(){}

	/// <summary>
	/// 識別ID
	/// </summary>
	s32   ID;

	/// <summary>
	/// 優先順位の設定（実際はジョイントのＺ値をレンダリングエンジン毎にコンバートして設定してるだけ）
	/// </summary>
	void                            SetPrio( dectype prio );

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vRendering          ( CRenderingParam &param )              { SKY_UNUSED_ARG( param ); };

	/// <summary>
	/// 子ノードの描画（再帰）
	/// </summary>
	void         RenderingChild      ( CRenderingParam &param );

	/// <summary>
	/// 描画後処理
	/// </summary>
	virtual void vPostRendering       ( CRenderingParam &param )              { SKY_UNUSED_ARG( param ); };

	/// <summary>
	/// アプリケーションカリング設定
	/// </summary>
	void         SetAppCulling        ( skyBool isCulling )                  { m_IsCulling = isCulling; }
	skyBool      IsAppCulling         ()                                     { return m_IsCulling; }

	/// <summary>
	/// 深度バッファクリア設定
	/// </summary>
	void         SetDepthBufferClear  ( skyBool isDepthBufferClear )          { m_IsDepthBufferClear = isDepthBufferClear; }
	skyBool      IsDepthBufferClear   ()                                      { return m_IsDepthBufferClear; }

	/// <summary>
	/// ステンシルバッファクリア設定
	/// </summary>
	void         SetStencilBufferClear( skyBool isDepthBufferClear )          { m_IsStencilBufferClear = isDepthBufferClear; }
	skyBool      IsStencilBufferClear ()                                      { return m_IsStencilBufferClear; }

	/// <summary>
	/// 半透明設定（半透明で出したいスプライトやモデルの場合、このフラグをＯＮにすることで描画順を変更して正しく描画できる）
	/// </summary>
	void         SetAlpha             ( skyBool isAlpha )                     { m_IsAlpha = isAlpha; }
	skyBool      IsAlpha              ()                                      { return m_IsAlpha; }

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// 描画時デバッグプリントをするか
	/// </summary>
	void                        SetNodeTreePrint        ( skyBool isPrint )                     { m_IsNodeTreePrintOn = isPrint; }
	skyBool                     IsNodeTreePrint         ()                                      { return m_IsNodeTreePrintOn; }

#endif

	/// <summary>
	/// 親の行列をロックする（反映しないように）する設定
	/// </summary>
	void                        SetLockJoint     ( skyBool isLockedJoint )                        { m_IsLockedJoint = isLockedJoint; }

	/// <summary>
	/// 親の行列を計算する際、回転はロックする（反映しないように）する設定
	/// </summary>
	void                        SetLockParentRotAndScale     ( skyBool isLockedRot )            { m_IsLockedRotAndScale = isLockedRot; }

	/// <summary>
	/// ジョイント使用
	/// </summary>
	void SetJointUse( skyBool isUse = skyTrue );

	/// <summary>
	/// ジョイントの使用判定
	/// </summary>
	skyBool IsJointUse();

	/// <summary>
	/// 葉のリストを返す
	/// </summary>
	CSceneGraphNodePtrList &LeafList();

	/// <summary>
	/// 葉の関連付け
	/// </summary>
	CSceneGraphNodePtr AttachLeaf     ( u32 key, const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// 葉の関連付け
	/// </summary>
	CSceneGraphNodePtr AttachLeaf     ( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// 葉の関連付け
	/// </summary>
	CSceneGraphNodePtr AttachLeafFront( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// 葉の関連付け解除
	/// </summary>
	void               DetachLeaf     ( const CSceneGraphNodePtr &spLeaf );

	/// <summary>
	/// 葉の関連付け解除
	/// </summary>
	void               DetachAllLeaf  ();

	/// <summary>
	/// 枝の関連付け
	/// </summary>
	void               AttachBranch    ( const CSceneGraphNodePtr &spBranch );

	/// <summary>
	/// 枝の関連付け解除
	/// </summary>
	void               DetachBranch    ();
	void               DetachBranch    ( const CSceneGraphNodePtr &spBranch );

	/// <summary>
	/// 枝の取得
	/// </summary>
	const CSceneGraphNodePtr &GetBranchRoot();

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
	virtual const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	virtual const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	virtual serialize::CSerializeInterfacePtr 	Clone();
	virtual serialize::CSerializeInterfacePtr 	CloneSharing();
	serialize::CSerializeInterfacePtr 	        CloneTraverse();
	serialize::CSerializeInterfacePtr 	        CloneSharingTraverse();
	static CSceneGraphNodePtr                   CloneTraverseImple( const CSceneGraphNodePtr &spNode , skyBool isSharing = skyFalse );

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	virtual void Activate(){ ACTIVATE_COMMON(); };
	void ActivateTraverse();
	void ActivateTraverseImple( const CSceneGraphNodePtr &spNode );

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	virtual void Deactivate(){ DEACTIVATE_COMMON(); };

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CSceneGraphNodePtr	   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSceneGraphNodePtr spNode( NEW__( CSceneGraphNode , CSceneGraphNode() ) );
		if ( name.IsEmpty() == skyFalse ) spNode->Name.SetName( name );
		return spNode;
	}

//---Debug

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// デバッグプリント（シーングラフのツリー構造を見たい場合はこれをする）
	/// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

	/// <summary>
	/// トラバースデバッグプリント
	/// </summary>
	void NodeTreePrintTraverse( u32 indent = 0 );
	static void NodeTreePrintTraverseImple( CSceneGraphNode *pNode , u32 indent , eRelationKind eKind );

	/// <summary>
	/// 情報プリント（デバッグプリントより詳細な情報を出す,インデントはしない。）
	/// </summary>
	virtual void InfoPrint();

	/// <summary>
	/// トラバース情報プリント
	/// </summary>
	void InfoPrintTraverse();
	static void InfoPrintTraverseImple( CSceneGraphNode *pNode );

#endif

protected:

//---Member

	/// <summary>
	/// カリングフラグ
	/// </summary>
	skyBool             m_IsCulling;

	/// <summary>
	/// 深度バッファクリアフラグ
	/// </summary>
	skyBool             m_IsDepthBufferClear;

	/// <summary>
	/// ステンシルバッファクリアフラグ
	/// </summary>
	skyBool             m_IsStencilBufferClear;

	/// <summary>
	/// 半透明フラグ
	/// マテリアルのAlpha値で半透明の判定してもよいが、画像のAlpha値を利用する場合、
	/// マテリアルのAlpha値とは関係ない透過処理なので明示的にフラグを立てる必要があると判断。
	/// </summary>
	skyBool             m_IsAlpha;

	/// <summary>
	/// 親の行列情報は反映しないフラグ
	/// </summary>
	skyBool             m_IsLockedJoint;

	/// <summary>
	/// 親の回転情報は反映しないフラグ
	/// </summary>
	skyBool             m_IsLockedRotAndScale;

	/// <summary>
	/// ジョイント使用フラグ
	/// </summary>
	skyBool             m_IsJointUse;

//---Member

	/// <summary>
	/// ジョイントキャッシュ
	/// </summary>
	math::CJointPtr           m_spJoint;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// デバッグプリントOn
	/// </summary>
	skyBool             m_IsNodeTreePrintOn;

#endif

	/// <summary>
	/// 葉リスト
	/// </summary>
	CSceneGraphNodePtrList	m_LeafList;

	/// <summary>
	/// 枝
	/// </summary>
	CSceneGraphNodePtr 	    m_Branch;

	/// <summary>
	/// SetUpフラグ(セットアップが必要なノードで使用する）
	/// </summary>
	skyBool m_IsSetUp;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSceneGraphNode( const CSceneGraphNodePtr &spParent = CSceneGraphNodePtr() );
};

#define CSceneGraphNodeCreate_()            sky::lib::graphic::CSceneGraphNode::Create()
#define CSceneGraphNodeCreateName_( name )	sky::lib::graphic::CSceneGraphNode::Create( name )

} } }
