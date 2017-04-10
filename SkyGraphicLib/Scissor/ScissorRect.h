#pragma once

namespace sky { namespace lib { namespace graphic {

namespace scissor
{
	//シザー矩形最大数
	const u32 SCISSOR_MAX_NUM = 16;
}

//テンプレートTypedef郡
class CScissorRect;
typedef SmartPointer< CScissorRect >	CScissorRectPtr;		//スマートポインタ版定義

/// <summary>
/// シザリング矩形クラス
/// </summary>
class CScissorRect : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CScissorRect(){}

	/// <summary>
	/// シザー矩形の設定
	/// </summary>
	void				        SetRect        ( const math::CBasicRect &rect );

	/// <summary>
	/// シザー矩形の設定
	/// </summary>
	math::CBasicRect&           GetRect        (){ return m_Rect; }

	/// <summary>
	/// サイズ変更
	/// </summary>
	void                        ReSizeSc       ( dectype wSc , dectype hSc );

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void		        vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	virtual void                vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// シザリング矩形をレンダリングシステムに設定する（以降シザリング矩形の範囲内にレンダリングが行われる）
	/// </summary>
	void                        SetRenderer();

	/// <summary>
	/// レンダリングシステムのシザリング矩形をクリアする
	/// </summary>
	void                        ClearRenderer();

	/// <summary>
	/// Windowサイズの設定(OpenGLでは左下原点のため、左下原点座標を左上原点座標に変換するには全体の大きさが必要になる）
	/// </summary>
	void                        SetWindowSize( const math::CBasicSize size ){ m_WindowSize = size; };

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
	serialize::CSerializeInterfacePtr         CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CScissorRectPtr	Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CScissorRectPtr spScissorRect( NEW__( CScissorRect , CScissorRect() ) );
		if ( name.IsEmpty() == skyFalse ) spScissorRect->Name.SetName( name );
		return spScissorRect;
	}

#ifdef SW_SKYLIB_DEBUG_ON

    /// <summary>
	///  デバッグプリント
    /// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

#endif

private:

	/// <summary>
	/// 矩形リスト
	/// </summary>
	math::CBasicRect           m_Rect;

	/// <summary>
	/// 自分が所属しているWindowの幅と高さ（シーングラフのトラバース時に取得）
	/// </summary>
	math::CBasicSize           m_WindowSize;

};

} } }

#define CScissorRectCreate_()	sky::lib::graphic::CScissorRect::Create()
#define CScissorRectCreateName_( name ) sky::lib::graphic::CScissorRect::Create( name )
