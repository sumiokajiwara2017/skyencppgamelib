#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================【Common】====================================================>

//テンプレートTypedef郡

class CViewPort;
typedef SmartPointer< CViewPort >				CViewPortPtr;		//スマートポインタ版定義
typedef CList		< u32 , CViewPortPtr >		CViewPortPtrList;	//リスト定義
typedef CHash		< CViewPortPtr >			CViewPortPtrHash;	//Hash定義
typedef CHashMT		< CViewPortPtr >			CViewPortPtrHashMT;	//Hash定義

class CViewPort : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual                     ~CViewPort(){};

	/// <summary>
	/// 幅と高さと位置を設定する（矩形のサイズ変更は、登録した矩形ポインタの値を変更すれば変わります。削除したり、大きさを変化させるために登録した参照は保持しておいてください。）
	/// </summary>
	void				        SetRect        ( const math::CBasicRect &rect );

	/// <summary>
	/// サイズ取得
	/// </summary>
	const math::CBasicRect&     GetRect        ();

	/// <summary>
	/// サイズ変更
	/// </summary>
	void                        ReSize         ( u32 width , u32 height );
	void                        ReSizeSc       ( dectype wSc , dectype hSc );

	/// <summary>
	/// 深度の最大値、最小値を設定
	/// </summary>
	void                        SetDepth        ( dectype minDepth , dectype maxDepth );

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void		        vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	virtual void                vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// ビューポートをレンダリングシステムに設定する（以降ビューポートの範囲内にレンダリングが行われる）
	/// </summary>
	void                        SetRenderer();

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
	static CViewPortPtr	Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CViewPortPtr spViewPort( NEW__( CViewPort , CViewPort() ) );
		if ( name.IsEmpty() == skyFalse ) spViewPort->Name.SetName( name );
		return spViewPort;
	}

protected:

//---Member

	/// <summary>
	/// 描画領域矩形
	/// </summary>
	math::CBasicRect    m_Rect;

	/// <summary>
	/// 描画深さ最小値
	/// </summary>
	dectype             m_MinDepth;

	/// <summary>
	/// 描画深さ最大値
	/// </summary>
	dectype             m_MaxDepth;

//---Member

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CViewPort() : m_MinDepth( 0.0f ) , m_MaxDepth( 1.0f ){}

};

#define CViewPortCreate_()	         sky::lib::graphic::CViewPort::Create()
#define CViewPortCreateName_( name ) sky::lib::graphic::CViewPort::Create( name )

} } }