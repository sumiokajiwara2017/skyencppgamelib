#pragma once

namespace sky { namespace lib { namespace graphic {

//===================================【Common】====================================================>

//プラットフォーム毎のメンバ変数(pimpl)

struct CVertexBufferMember;
typedef SmartPointer2< CVertexBufferMember > CVertexBufferMemberPtr;		//スマートポインタ版定義

//テンプレートTypedef郡

class CVertexBuffer;
typedef SmartPointer< CVertexBuffer >				CVertexBufferPtr;			//スマートポインタ版定義
typedef CList< u32 , CVertexBufferPtr >				CVertexBufferPtrList;		//リスト定義
typedef CHash< CVertexBufferPtr >					CVertexBufferPtrHash;		//Hash定義
typedef CHashMT< CVertexBufferPtr >					CVertexBufferPtrHashMT;		//Hash定義

class CVertexBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

friend class CFont;
friend class CFontSimple;
friend class CDecalMember;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual                ~CVertexBuffer();

	/// <summary>
	/// 頂点の数を取得
	/// </summary>
	inline vrtxtype			GetVertexNum() const { return m_VertexNum; };

	/// <summary>
	/// 面の法線算出
	/// </summary>
	void					ComputePrimNormalVector();
	void					ComputePrimNormalVectorFromIndexBuffer( const CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// 頂点の法線算出
	/// </summary>
	void					ComputeVertexNormalVector( const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() );

	/// <summary>
	/// 各種算出をすべてやる。
	/// </summary>
	void					ComputeAll();
	void					Initialize( vrtxtype vertexNum , vertex::ePrimitiveType ePrimType = vertex::ePrimitiveType_Trianglelist );
	void					Dispose();

	/// <summary>
	/// CPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	void                   DeleteCpuMemory();

//---Leaf

	/// <summary>
	/// 座標ストリームの取得( eVertexBufferStreamType_Position )
	/// </summary>
	void                   AttachPositionStream( const CVertexBufferStreamPtr &position );
	CVertexBufferStreamPtr GetPositionStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );

	/// <summary>
	/// 法線ストリーム（eVertexBufferStreamType_Normal:ライト計算に使用）
	/// </summary>
	void                   AttachNormalStream( const CVertexBufferStreamPtr &normal );
	CVertexBufferStreamPtr GetNormalStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateNormalStream( vrtxtype num );

	/// <summary>
	/// UVストリーム（eVertexBufferStreamType_Uv:テクスチャマッピングに使用）
	/// </summary>
	void                   AttachUvStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetUvStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateUvStream( vrtxtype num );

	/// <summary>
	/// カラーストリーム（eVertexBufferStreamType_Color:頂点カラー等に使用）
	/// </summary>
	void                   AttachColorStream( const CVertexBufferStreamPtr &color );
	CVertexBufferStreamPtr GetColorStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateColorStream( vrtxtype num , CColor initColor = CColor_White );

	/// <summary>
	/// スキンインデックス
	/// </summary>
	void                   AttachIndexStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetIndexStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateIndexStream( vrtxtype num );

	/// <summary>
	/// ウェイト
	/// </summary>
	void                   AttachWeightsStream( const CVertexBufferStreamPtr &uv );
	CVertexBufferStreamPtr GetWeightsStream   ( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );
	void                   CreateWeightsStream( vrtxtype num );

	/// <summary>
	/// 指定のIDで頂点ストリームを関連付けする
	/// </summary>
	void AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream );

	/// <summary>
	/// 指定のIDで頂点ストリームを関連付け解除する
	/// </summary>
	void DetachStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name = hash::CHashStringPtr() );

	/// <summary>
	/// 指定のIDで頂点ストリームを取得する
	/// </summary>
	CVertexBufferStreamPtr GetStream( eVertexBufferStreamType eType , const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyTrue );

	/// <summary>
	/// プリミティブ毎の法線ストリームの取得
	/// </summary>
	CVertexBufferStreamPtr GetPrimitiveNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse );
	CVertexBufferStreamPtr DetachPrimitiveNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse ); //頂点バッファからは消しつつストリームを取得。取得したストリームはマテリアルと関連付けて使用する。

	/// <summary>
	/// 頂点毎の法線ストリームの取得
	/// </summary>
	CVertexBufferStreamPtr GetVertexNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse );
	CVertexBufferStreamPtr DetachVertexNormalStream( const hash::CHashStringPtr &name = hash::CHashStringPtr() , skyBool isRequired = skyFalse ); //頂点バッファからは消しつつストリームを取得。取得したストリームはマテリアルと関連付けて使用する。

//---Leaf

	/// <summary>
	/// プリミティブ種別の設定／取得
	/// </summary>
	void						SetPrimitiveType			( vertex::ePrimitiveType ePrimType ) { m_ePrimType = ePrimType; }
	vertex::ePrimitiveType      GetPrimitiveType            (){ return m_ePrimType; }

	/// <summary>
	/// 座標に４×３行列をかける
	/// </summary>
	void                        ComputePosMatrix      ( const math::CMatrix4x3 &matrix );

	/// <summary>
	/// 頂点群の中心を( 0 , 0 , 0 )にするように頂点座標情報を移動させる。
	/// </summary>
	void                        TransPosCenter        ();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを作成する
	/// </summary>
	void                        CreateGpuMemory();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	void                        DeleteGpuMemory();

	/// <summary>
	/// GPU側のメモリ上の頂点バッファに書き込む
	/// </summary>
	void                        WirteGpuMemory  ();

	/// <summary>
	/// 頂点を描画する
	/// </summary>
	void                     Draw                        ( vrtxtype startIndex = 0 , vrtxtype drawIndexNum = 0 , const CIndexBufferPtr &spIndexBuffer = CIndexBufferPtr() );

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	const CVertexBufferMemberPtr& GetMember(){ return m_pMember; }

	/// <summary>
	/// クリア（使用頂点数とIndex数を０にする。メモリは確保したままです。メモリを破棄したい場合はDisposeを呼ぶ）
	/// </summary>
	void Clear()
	{ 
		m_UseVertexNum = 0;
		DetachBranch();
	}

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
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CVertexBufferPtr  Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CVertexBufferPtr spVertexBuffer( NEW__( CVertexBuffer , CVertexBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spVertexBuffer->Name.SetName( name );
		return spVertexBuffer;
	}

protected:

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// GPUメモリ確保フラグ
	/// </summary>
	skyBool                      m_IsCreateGpuMemory;

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CVertexBufferMemberPtr       m_pMember;

//---実行時に生成されるバッファ変数（保存の必要なし）

//---Member

	/// <summary>
	/// プリミティブ種別
	/// </summary>
	vertex::ePrimitiveType       m_ePrimType;

	/// <summary>
	/// 頂点数
	/// </summary>
	vrtxtype			         m_VertexNum;

	/// <summary>
	/// 使用頂点数
	/// </summary>
	vrtxtype				     m_UseVertexNum;

//---Member

	/// <summary>
	/// 頂点使用数の更新
	/// </summary>
	void UpdateUseVertexNum( vrtxtype vertexNo )
	{
		m_UseVertexNum = vertexNo;

		FOREACH( CSceneGraphNodePtrList , it , LeafList() )
		{
			if( SkyIsKindOf( CVertexBufferStream , ( *it ).GetPtrNoConst() ) )
			{
				CVertexBufferStream *pStream = SkyDynamicCast( CVertexBufferStream , ( *it ).GetPtrNoConst() );
				switch( pStream->ID )
				{
				case eVertexBufferStreamType_Position :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Pos ) * vertexNo );
					break;
				case eVertexBufferStreamType_Normal :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Normal ) * vertexNo );
					break;
				case eVertexBufferStreamType_Uv :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_UV ) * vertexNo );
					break;
				case eVertexBufferStreamType_Color :
					pStream->UpdateUseSize( sizeof( vertex::CVertexFmt_Color ) * vertexNo );
					break;
				}
			}
		}
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CVertexBuffer();
};

} } }

#define CVertexBufferCreate_()				sky::lib::graphic::CVertexBuffer::Create()
#define CVertexBufferCreateName_( name )	sky::lib::graphic::CVertexBuffer::Create( name )

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Ｄ３Ｄの頂点バッファのメンバ
/// </summary>
struct CVertexBufferMember
{
	/// <summary>
	/// 頂点レイアウト情報
	/// </summary>
	D3DInputVertexLayoutPtr m_spVertexDec;
};

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// 頂点バッファのメンバ
/// </summary>
struct CVertexBufferMember
{

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

	/// <summary>
	/// CgのシェーダーID
	/// </summary>
    CGprogram m_CgProgram;

#endif

};

} } }

#endif
