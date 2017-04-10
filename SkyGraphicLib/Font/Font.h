#pragma once

namespace sky { namespace lib { namespace graphic {

namespace font {

static const u32 ONE_LINE_NUM_MAX       = 32;  //１行文字数
static const u32 ONE_LINE_FONT_NUM_MAX  = 256; //１行文字数
static const u32 LINE_FONT_POOL_NUM_MAX = 256; //１行文字数プール数

}

class CFont;
typedef SmartPointer< CFont >   CFontPtr;
typedef CList< u32 , CFontPtr > CFontPtrList;

/// <summary>
/// 文字クラス
/// 簡易文字表示と文字送り、タグによる色変え、イベント発生などに対応する文字オブジェクトとしての扱い
/// という２種類の文字の出し方がある。
/// </summary>
class CFont : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFont(){}

	/// <summary>
	/// 使用準備
	/// </summary>
	void SetUp();

	/// <summary>
	/// 文字表示
	/// 複数回文字を出力した場合、最後の文字出力で現在の文字出力が上書きされる
	/// </summary>
	void PrintfSentence      ( dectype posX , dectype posY , const CColor &color , dectype sc , u32 outIntvl , const skyString *format , ... );

	/// <summary>
	/// 色設定
	/// </summary>
	void SetColor            ( const CColor &color ){ m_SentenceData.Color = color; };

	/// <summary>
	/// 座標設定
	/// </summary>
	void SetPosition         ( math::CBasicVector2 &pos ){ m_SentenceData.Pos = pos; };

	/// <summary>
	/// 座標取得
	/// </summary>
	math::CBasicVector2 &GetPosition(){ return m_SentenceData.Pos; };

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// 文字列サイズ取得
	/// </summary>
	math::CBasicSize &GetSentenceSize(){ return m_SentenceSize; }

	/// <summary>
	/// 文字サイズ取得
	/// </summary>
	math::CBasicSize &GetFontSize(){ return m_FontSize; }

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
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	      Clone();
	serialize::CSerializeInterfacePtr 		  CloneSharing();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	void Activate();

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CFontPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() , font::eFontStyleType eStyle = font::eFontStyleType_Gosic13 , dectype prio = RENDERING_2D_PRIO_FAST )
	{
		
		CFontPtr spFont( NEW__( CFont , CFont( eStyle , prio ) ) );
		if ( name.IsEmpty() == skyFalse ) spFont->Name.SetName( name );
		return spFont;
	}


private:

//--Branch

	/// <summary>
	/// 出力用シェーダー
	/// </summary>
	CTextureMappingShaderObjectPtr GetShader( skyBool isRequired = skyTrue );

//--Branch

//---Member

	/// <summary>
	/// １文データ
	/// </summary>
	struct CSentenceData_
	{
		CColor               Color;
		dectype              Sc;
		skyString            Sentence[ font::ONE_LINE_FONT_NUM_MAX ];
		u32                  OutIntvl;                          //文字送りする間隔（フレーム）０で一気に出力される。
		math::CBasicVector2  Pos;

		/// <summary>
		/// 代入演算子オーバーライド
		/// </summary>
		virtual CSentenceData_& operator =( const CSentenceData_& src )
		{
			Color    = src.Color;
			Sc       = src.Sc;
			CStringUtil_::Copy( Sentence , font::ONE_LINE_FONT_NUM_MAX , src.Sentence );
			OutIntvl = src.OutIntvl;
			Pos      = src.Pos;
			return *this;
		}
	};
	CSentenceData_  m_SentenceData;

	/// <summary>
	/// スタイル
	/// </summary>
    font::eFontStyleType m_Style;

	/// <summary>
	/// 文字数
	/// </summary>
	vrtxtype  m_printFontNum;

	/// <summary>
	/// 文字列の幅と高さを取得
	/// </summary>
	math::CBasicSize m_SentenceSize;

	/// <summary>
	/// 文字の幅と高さを取得
	/// </summary>
	math::CBasicSize m_FontSize;

	/// <summary>
	/// 優先順位
	/// </summary>
	dectype m_Prio;

//---Member

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// メッシュ
	/// </summary>
    CMeshContenaPtr m_MeshContena;

	/// <summary>
	/// マテリアル
	/// </summary>
	CMaterialPtr m_spMaterial;

	/// <summary>
	/// 文字書き換えフラグ
	/// </summary>
    skyBool  m_IsReWrite;

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CFont( font::eFontStyleType eStyle , dectype prio  );
};

#define CFontCreate_()				sky::lib::graphic::CFont::Create()
#define CFontCreateName_( name )	sky::lib::graphic::CFont::Create( name )

class CFontSimple;
typedef SmartPointer< CFontSimple > CFontSimplePtr;

/// <summary>
/// 簡易文字出力クラス
/// </summary>
class CFontSimple : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CFontSimple(){}

	/// <summary>
	/// 使用準備
	/// </summary>
	void SetUp();

    /// <summary>
	///  基準座標連続描画オン
	///  基準座標描画を行うと、指定の基準座標に最初の文字が出力され、以降の文字はその下に描画されていく。
    ///  各文章に設定された座標はオフセット座標として使用される。
    /// </summary>
	void SetStanderdPositionDraw( skyBool isOn ){ m_IsStanderdPositionDraw = isOn; }

    /// <summary>
	///  基準座標の設定
    /// </summary>
	void SetStanderdPosition( const math::CBasicVector2 &pos ){ m_IsStanderdPosition = pos; };

	/// <summary>
	/// 文字表示
	/// このオブジェクトから複数の文を出力可能。そのかわり出力した文字を文字送りできない。上書きできない。デバッグ表示等に便利
	/// </summary>
	void PrintfSentence  ( dectype posX , dectype posY , const CColor &color , dectype sc , const skyString *format , ... );

    /// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
    /// </summary>
	virtual void  vRendering ( CRenderingParam &param );

	/// <summary>
	/// 描画終了時コールイベントハンドラ
	/// </summary>
	delegate::CDelegateHandler m_RenderingEnd;

	/// <summary>
	/// 描画行数取得
	/// </summary>
	u32 GetPrintNum(){ return m_SentenceList.GetNum(); }

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

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	void Activate();

//---Static

	/// <summary>
	/// 作成（描画の関係でスタイルは作成時に決定する）
	/// </summary>
	static CFontSimplePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() , font::eFontStyleType eStyle = font::eFontStyleType_Gosic13 , u32 lineNum = font::LINE_FONT_POOL_NUM_MAX , dectype prio = RENDERING_2D_PRIO_FAST )
	{
		
		CFontSimplePtr spFont( NEW__( CFontSimple , CFontSimple( eStyle , lineNum , prio ) ) );
		if ( name.IsEmpty() == skyFalse ) spFont->Name.SetName( name );
		return spFont;
	}

private:

	/// <summary>
	/// 文字出力セットアップデータ
	/// </summary>
	struct CSentenceData_
	{
		CColor               Color;
		dectype              Sc;
		skyString            Sentence[ font::ONE_LINE_FONT_NUM_MAX ];
		math::CBasicVector2  Pos;
	};
	typedef CSentenceData_*                   CSentenceDataPtr_;
    typedef CList< u32 , CSentenceDataPtr_ >  CSentenceDataPtrList_;			//リスト定義
	typedef CMemoryPool< CSentenceData_ >     SentencePool;
	SentencePool           m_SentencePool;
	CSentenceDataPtrList_  m_SentenceList;

//--Branch

	/// <summary>
	/// 出力用シェーダーの取得
	/// </summary>
	CTextureMappingShaderObjectPtr GetShader( skyBool isRequired = skyTrue );

	/// <summary>
	/// 幾何情報の取得
	/// </summary>
	CMeshPtr GetMesh( skyBool isRequired = skyTrue );

	/// <summary>
	/// 頂点バッファの取得
	/// </summary>
	CVertexBufferPtr GetVertexBuffer( skyBool isRequired = skyTrue );

//--Branch

//---Member

	/// <summary>
	/// スタイル
	/// </summary>
    font::eFontStyleType   m_Style;

	/// <summary>
	/// 基準座標描画フラグ
	/// </summary>
	skyBool m_IsStanderdPositionDraw;

	/// <summary>
	/// 基準座標
	/// </summary>
	math::CBasicVector2 m_IsStanderdPosition;

	/// <summary>
	/// 優先順位
	/// </summary>
	dectype m_Prio;

//---Member

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CFontSimple( font::eFontStyleType eStyle , u32 lineNum , dectype prio );
};

} } }

#define CFontSimpleCreate_()                       sky::lib::graphic::CFontSimple::Create()
#define CFontSimpleCreateNum_( style , lineNum )   sky::lib::graphic::CFontSimple::Create( hash::CHashStringPtr() , style , lineNum )
#define CFontSimpleCreateName_( name )	           sky::lib::graphic::CFontSimple::Create( name );
