#pragma once

namespace sky { namespace lib { namespace graphic {

static const s32 WINDOW_SYSTEM_INVALID_NODE_ID = -1;
static const dectype WINDOW_SYSTEM_DEFAULT_NODE_WIDTH = 100;
static const dectype WINDOW_SYSTEM_DEFAULT_NODE_HEIGHT = 30;

class CWindowSystemNode;

typedef SmartPointer< CWindowSystemNode >   CWindowSystemNodePtr;		//スマートポインタ版定義
typedef CList< u32 , CWindowSystemNodePtr > CWindowSystemNodePtrList;	//リスト定義

/// <summary>
/// Windowノード
/// </summary>
class CWindowSystemNode : public CSprite
{

friend class CWindowSystemManager;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWindowSystemNode();

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	virtual void vRendering	( CRenderingParam &param );
		
	/// <summary>
	/// 文字表示要素としてセットアップ
	/// </summary>
	void SetUpFontDispMode( const skyString *headerfont );

	/// <summary>
	/// 文字を設定
	/// </summary>
	void SetFont( const skyString *font );

	/// <summary>
	/// 文字アドレスとバインディング
	/// </summary>
	void BindFontAddress( const skyString *font );

	/// <summary>
	/// f32表示要素としてセットアップ
	/// </summary>
	void SetUpf32DispMode( const skyString *headerfont , f32 initValue );

	/// <summary>
	/// f32値設定
	/// </summary>
	void Setf32Value( f32 value );

	/// <summary>
	/// f32アドレスとバインディング
	/// </summary>
	void Bindf32Address( f32 *pValue );

	/// <summary>
	/// s32表示要素としてセットアップ
	/// </summary>
	void SetUps32DispMode( const skyString *headerfont , s32 initValue );

	/// <summary>
	/// s32値設定
	/// </summary>
	void Sets32Value( s32 value );

	/// <summary>
	/// s32アドレスとバインディング
	/// </summary>
	void Binds32Address( s32 *pValue );

	/// <summary>
	/// 子ノードを関連付け
	/// </summary>
	void AttachChild( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// 子ノードを関連付け解除
	/// </summary>
	void DetachChild( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// 兄弟ノード関連付け(指定のノードが自分の上の兄弟になる。もし上の兄弟が居たらその上の兄弟になる。もし・・（無限））
	/// </summary>
	void AttachBeforeBrotherNodeList( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// 兄弟ノード関連付け(指定のノードが自分の下の兄弟になる。もし下の兄弟が居たらその下の兄弟になる。もし・・（無限））
	/// </summary>
	void AttachAfterBrotherNodeList( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// 兄弟ノード関連付け解除（自分が兄弟リストから抜ける）
	/// </summary>
	void DetachBrotherNodeList();

	/// <summary>
	/// コンストラクタ（位置指定はルート以外は結果的に無効になります）
	/// </summary>
	CWindowSystemNode( dectype posX = 0 , dectype posY = 0 , dectype width = WINDOW_SYSTEM_DEFAULT_NODE_WIDTH , dectype height = WINDOW_SYSTEM_DEFAULT_NODE_HEIGHT );

	/// <summary>
	/// ユニークＩＤ
	/// </summary>
	s32 m_UniqId;

	/// <summary>
	/// フォーカスフラグ
	/// </summary>
	skyBool m_IsOnFocus;

	/// <summary>
	/// 位置
	/// </summary>
	math::CBasicVector2 m_Pos;

	/// <summary>
	/// 幅高さ
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// 親の参照
	/// </summary>
	CWindowSystemNodePtr m_Parent;

	/// <summary>
	/// 子リスト
	/// </summary>
	CWindowSystemNodePtrList m_ChildList;

	/// <summary>
	/// 下の兄弟の参照
	/// </summary>
	CWindowSystemNodePtr m_BeforeBrotherNode;
	CWindowSystemNodePtr m_AfterBrotherNode;
};

} } }
