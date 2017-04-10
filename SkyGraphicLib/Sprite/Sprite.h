#pragma once

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
class CSprite;
typedef SmartPointer< CSprite >				CSpritePtr;				//スマートポインタ版定義
typedef CList< u32 , CSpritePtr >			CSpritePtrList;			//リスト定義
typedef CHash< CSpritePtr >					CSpritePtrHash;			//Hash定義
typedef CHashMT< CSpritePtr >				CSpritePtrHashMT;		//Hash定義

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// スプライトクラス
/// スプライトとは２Ｄの矩形で表現される描画要素です。
/// （注意）２Ｄカメラ空間（正射影）で使用してください。スプライトのＺ座標の扱いが２Ｄカメラ空間専用になっています。
/// </summary>
class CSprite : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual                        ~CSprite(){};

	/// <summary>
	/// 使用準備
	/// </summary>
	void                            SetUp();

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	CMaterialPtr       GetMaterial( u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// 幾何学形状情報のDifuseMapを指定の色に設定する
	/// </summary>
	void               AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// 幾何学形状情報のDifuseカラーを指定の色に設定する
	/// </summary>
	void               SetDiffuse( const CColor &color , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// アルファ値の設定
	/// </summary>
	void               SetAlphaValue( f32 alpha , u32 meshNo = 0 , u32 materialNo = 0 );

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	virtual void				    vRendering			( CRenderingParam &param );

    /// <summary>
	///  レンダリング処理実装関数
    /// </summary>
	void                            RenderingImple      ( CRenderingParam &param , const CShaderObjectPtr &shader );

//----Branch

    /// <summary>
	///  Meshの関連付け／取得
    /// </summary>
	void            AttachMeshContena( const CMeshContenaPtr &spMesh );
	CMeshContenaPtr GetMeshContena( skyBool isRequired = skyTrue );

//----Branch

//----Leaf

    /// <summary>
	///  シェーダーを関連付け／関連付け解除をする
    /// </summary>
	void            AttachShader( const CShaderObjectPtr &effectPtr );
	void            ClearAndAttachShader( const CShaderObjectPtr &effectPtr ); //シェーダーを一旦クリアしアタッチする
	void            DetachShader();

//----Leaf

#ifdef SW_SKYLIB_DEBUG_ON

    /// <summary>
	///  デバッグプリント
    /// </summary>
	virtual void NodeTreePrint( u32 indent = 0 , eRelationKind eKind = eRelationKind_Root  );

#endif

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
	const serialize::CSerializeInterfacePtr& Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr& CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr   CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CSpritePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CSpritePtr spSprite( NEW__( CSprite , CSprite() ) );
		if ( name.IsEmpty() == skyFalse ) spSprite->Name.SetName( name );
		return spSprite;
	}

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSprite();
	CSprite( CSceneGraphNode &spNode );

    /// <summary>
	/// 絶対座標を返す（実装）
    /// </summary>
	static void GetAbsoluteCoordinatesImple( CSceneGraphNode *pNode , u32 &count , math::CBasicVector2 &dest );
};

} } }

#define CSpriteCreate_()			    sky::lib::graphic::CSprite::Create()
#define CSpriteCreateName_( name )	    sky::lib::graphic::CSprite::Create( name )
