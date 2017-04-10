#pragma once

namespace sky { namespace lib { namespace framework {

class CMouseArrow;
typedef SmartPointer< CMouseArrow >	CMouseArrowPtr;				//スマートポインタ版定義

/// <summary>
/// マウス矢印モデル
/// このクラスは描画のみ担当します。
/// マウスの各種イベントはCMouseクラスを使用して取得してください。
/// </summary>
class CMouseArrow : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMouseArrow( wndType windowId , dectype prio = graphic::RENDERING_2D_PRIO_FAST  );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMouseArrow( void );

	/// <summary>
	/// 矢印画像描画スプライト
	/// </summary>
	graphic::CSpritePtr m_spRootSprite;

private:

	/// <summary>
	/// 自分が所属しているウィンドウの識別ID
	/// </summary>
	wndType m_WindowId;

	/// <summary>
	/// マウス移動イベントデリゲート
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );
};

} } }