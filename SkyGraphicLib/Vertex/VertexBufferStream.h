#pragma once

namespace sky { namespace lib { namespace graphic {

enum eVertexBufferStreamType
{
	//CVertexBufferがデフォルトで扱っている要素
	eVertexBufferStreamType_Position = 0 ,
	eVertexBufferStreamType_Normal ,      
	eVertexBufferStreamType_Uv ,          
	eVertexBufferStreamType_Color ,       
	eVertexBufferStreamType_Index ,
	eVertexBufferStreamType_Weights ,

	eVertexBufferStreamType_Max ,
};

inline u32 GetVertexBufferStreamTypeSize( eVertexBufferStreamType eType )
{
	u32 result = 0;
	switch( eType )
	{
	case eVertexBufferStreamType_Position:     
		result = sizeof( vertex::CVertexFmt_Pos );
		break;
	case eVertexBufferStreamType_Normal:       
		result = sizeof( vertex::CVertexFmt_Normal );
		break;
	case eVertexBufferStreamType_Uv:           
		result = sizeof( vertex::CVertexFmt_UV );
		break;
	case eVertexBufferStreamType_Color:        
		result = sizeof( vertex::CVertexFmt_Color );
		break;
	case eVertexBufferStreamType_Index:
		result = sizeof( vertex::CVertexFmt_Index );
		break;
	case eVertexBufferStreamType_Weights:
		result = sizeof( vertex::CVertexFmt_Weights );
		break;
	}
	return result;
}

//===================================【Common】====================================================>

//プラットフォーム毎のメンバ変数(pimpl)

struct CVertexBufferStreamMember;
typedef SmartPointer2< CVertexBufferStreamMember > CVertexBufferStreamMemberPtr;		//スマートポインタ版定義

//テンプレートTypedef郡

class CVertexBufferStream;
typedef SmartPointer< CVertexBufferStream >				CVertexBufferStreamPtr;			//スマートポインタ版定義
typedef CList< u32 , CVertexBufferStreamPtr >			CVertexBufferStreamPtrList;		//リスト定義
typedef CHash< CVertexBufferStreamPtr >					CVertexBufferStreamPtrHash;		//Hash定義
typedef CHashMT< CVertexBufferStreamPtr >				CVertexBufferStreamPtrHashMT;	//Hash定義

class CVertexBufferStream : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual                    ~CVertexBufferStream();

	/// <summary>
	/// 初期化
	/// </summary>
	inline void						Initialize         ( u32 size );
	inline void						InitializePosition ( u32 num );
	inline void						InitializeNormal   ( u32 num );
	inline void						InitializeUv       ( u32 num );
	inline void						InitializeColor    ( u32 num );
	inline void						InitializeIndex    ( u32 num );
	inline void						InitializeWeights  ( u32 num );

	/// <summary>
	/// 各値に換算した時の個数を返す
	/// </summary>
	inline u32                      GetNumPosition();
	inline u32                      GetNumNormal();
	inline u32                      GetNumUv();
	inline u32                      GetNumColor();
	inline u32                      GetNumIndex();
	inline u32                      GetNumWeights();

	/// <summary>
	/// 末期化
	/// </summary>
	inline void						Dispose					    ();

	/// <summary>
	/// CPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	inline void                     DeleteCpuMemory();

	/// <summary>
	/// CPU側のメモリ上に書きこむ
	/// </summary>
	inline void                        WriteCpuMemory( vrtxtype index , f32 *p , u32 num  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3  );
	inline void                        WriteCpuMemory( vrtxtype index , f32 p1 , f32 p2 , f32 p3 , f32 p4  );
	inline void                        WritePosition( vrtxtype index , const vertex::CVertexFmt_Pos &pos );
	inline void                        WriteNormal  ( vrtxtype index , const vertex::CVertexFmt_Normal &normal );
	inline void                        WriteUv      ( vrtxtype index , const vertex::CVertexFmt_UV &uv );
	inline void                        WriteColor   ( vrtxtype index , const vertex::CVertexFmt_Color &color );
	inline void                        WriteIndex( vrtxtype index , const vertex::CVertexFmt_Index &indexs );
	inline void                        WriteWeights( vrtxtype index , const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// CPU側のメモリを指定の値で塗りつぶす
	/// </summary>
	inline void                        WriteMemoryValueAll( u8 *pData , u32 size );
	inline void                        WritePositionAll( const vertex::CVertexFmt_Pos &pos );
	inline void                        WriteNormalAll  ( const vertex::CVertexFmt_Normal &normal );
	inline void                        WriteUvAll      ( const vertex::CVertexFmt_UV &uv );
	inline void                        WriteColorAll   ( const vertex::CVertexFmt_Color &color );
	inline void                        WriteIndexAll( const vertex::CVertexFmt_Index &indexs );
	inline void                        WriteWeightsAll( const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// CPU側のメモリを指定の値で加算する
	/// </summary>
	inline void                        AddPositionAll( vrtxtype index , const vertex::CVertexFmt_Pos &pos );
	inline void                        AddNormalAll  ( vrtxtype index , const vertex::CVertexFmt_Normal &normal );
	inline void                        AddUvAll      ( vrtxtype index , const vertex::CVertexFmt_UV &uv );
	inline void                        AddColorAll   ( vrtxtype index , const vertex::CVertexFmt_Color &color );
	inline void                        AddIndexAll   ( vrtxtype index , const vertex::CVertexFmt_Index &indexs );
	inline void                        AddWeightsAll ( vrtxtype index , const vertex::CVertexFmt_Weights &weights );

	/// <summary>
	/// 値に行列をかけて変換する
	/// </summary>
	inline void                        ComputePosMatrixPositionAll( const math::CMatrix4x3 &matrix );
	inline void                        ComputePosMatrixNormalAll( const math::CMatrix4x3 &matrix );

	/// <summary>
	/// CPU側のメモリの参照の取得
	/// </summary>
	inline void*                        GetCpuMemory( vrtxtype index , u32 size );
	inline vertex::CVertexFmt_Pos*      GetPosition( vrtxtype index );
	inline vertex::CVertexFmt_Normal*   GetNormal( vrtxtype index );
	inline vertex::CVertexFmt_UV*       GetUv( vrtxtype index );
	inline vertex::CVertexFmt_Color*    GetColor( vrtxtype index );
	inline vertex::CVertexFmt_Index*    GetIndex( vrtxtype index );
	inline vertex::CVertexFmt_Weights*  GetWeights( vrtxtype index );

	/// <summary>
	/// バッファが座標だと仮定してY座標とZ座標を入れ替える
	/// </summary>
	inline void                        SwapPositionYAndZ();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを作成する
	/// </summary>
	inline void                        CreateGpuMemory();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	inline void                        DeleteGpuMemory();

	/// <summary>
	/// GPU側のメモリ上の頂点バッファに書き込む
	/// </summary>
	inline void                        WirteGpuMemory  ();

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	inline const CVertexBufferStreamMemberPtr& GetMember();

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

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	virtual void Deactivate();

	/// <summary>
	/// 頂点使用数の更新
	/// </summary>
	inline void UpdateUseSize( u32 size )
	{
		m_UseSize = size;
	}

	/// <summary>
	/// 情報プリント（デバッグプリントより詳細な情報を出す,インデントはしない。）
	/// </summary>
	virtual void InfoPrint();
	void InfoPrintPosition();
	void InfoPrintNormal();
	void InfoPrintUv();
	void InfoPrintColor();
	void InfoPrintIndex();
	void InfoPrintWeights();

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CVertexBufferStreamPtr  Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CVertexBufferStreamPtr spBuffer( NEW__( CVertexBufferStream , CVertexBufferStream() ) );
		if ( name.IsEmpty() == skyFalse ) spBuffer->Name.SetName( name );
		return spBuffer;
	}

protected:

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CVertexBufferStreamMemberPtr m_pMember;

	/// <summary>
	/// ＣＰＵメモリ上バッファ
	/// </summary>
	u8*                        m_pCpuMemoryBuffer;

	/// <summary>
	/// ＧＰＵメモリ作成済みフラグ
	/// </summary>
	skyBool                    m_IsCreateGpuMemory;

	/// <summary>
	/// サイズ
	/// </summary>
	u32                        m_Size;

	/// <summary>
	/// 使用サイズ
	/// </summary>
	u32                        m_UseSize;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CVertexBufferStream();
};

} } }

#define CVertexBufferStreamCreate_()			sky::lib::graphic::CVertexBufferStream::Create()
#define CVertexBufferStreamCreateName_( name )	sky::lib::graphic::CVertexBufferStream::Create( name )

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//型の再定義
typedef SmartPointerCom< ID3D11Buffer >      D3VertexBufferPtr;
typedef SmartPointerCom< ID3D11InputLayout > D3DInputVertexLayoutPtr;

/// <summary>
/// Ｄ３Ｄの頂点バッファのメンバ
/// </summary>
struct CVertexBufferStreamMember
{
	/// <summary>
	/// ＧＰＵメモリ上座標バッファ
	/// </summary>
	D3VertexBufferPtr   m_spGpuMemoryBuffer;
};

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// 頂点バッファのメンバ
/// </summary>
struct CVertexBufferStreamMember
{
	/// <summary>
	/// バッファハンドル
	/// </summary>
	GLuint  m_hBuffer;

	/// <summary>
	/// ロケーション（シェーダーの変数との関連付け）
	/// </summary>
	GLint    m_Location;
};

} } }

#endif

#include "VertexBufferStream.inl"