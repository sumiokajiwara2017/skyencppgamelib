#pragma once

namespace sky { namespace lib { namespace graphic {

class CWindowSystemManager;

typedef SmartPointer< CWindowSystemManager >   CWindowSystemManagerPtr;		//スマートポインタ版定義

/// <summary>
/// Window管理クラス
/// </summary>
class CWindowSystemManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWindowSystemManager(){};

	/// <summary>
	/// Windowノードの作成
	/// </summary>
	CWindowSystemNodePtr CreateWindowNode();

	/// <summary>
	/// Windowノードの破棄
	/// </summary>
	void DestroyWindowNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// シーングラフルート取得（これを２Ｄカメラシーンのシーングラフにアタッチする）
	/// </summary>
	CSceneGraphNodePtr GetSceneGraphRoot();

	/// <summary>
	/// Windowノードのルートを登録
	/// </summary>
	void AttachWindowGroupRootNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// Windowノードグループのルートを登録解除
	/// </summary>
	void DetachWindowGroupRootNode( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// Windowノードグループのトグル選択
	/// </summary>
	void ToggleSelectWindowGroup();

	/// <summary>
	/// マウスイベントデリゲート（マウスイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// キーボードイベントデリゲート（キーボードイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBoradEventDelegate;
	void KeyBoradEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	//シングルトン機能を付属
	friend class SingletonPtr< CWindowSystemManager >;
	static SingletonPtr< CWindowSystemManager > SingletonMember;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CWindowSystemManager();

private:

	/// <summary>
	/// Windowノードグループのルートリスト
	/// </summary>
	CWindowSystemNodePtrList m_WindowGroupRootNodeList;

	/// <summary>
	/// 現在選択中のノードの参照
	/// </summary>
	CWindowSystemNodePtr m_SelectedNode;

	/// <summary>
	/// シーングラフノード
	/// </summary>
	CSceneGraphNodePtr m_SceneGraphAttachNode;
};

} } }