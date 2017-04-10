#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)
struct  CIndexBufferMember;
typedef SmartPointer2< CIndexBufferMember >	CIndexBufferMemberPtr;		//スマートポインタ版定義


//テンプレートTypedef郡
class CIndexBuffer;
typedef SmartPointer< CIndexBuffer >		CIndexBufferPtr;		//スマートポインタ版定義
typedef CList< u32 , CIndexBufferPtr >		CIndexBufferPtrList;	//リスト定義
typedef CHash< CIndexBufferPtr >			CIndexBufferPtrHash;	//Hash定義
typedef CHash< CIndexBufferPtr >			CIndexBufferPtrHashMT;	//Hash定義

class CIndexBuffer : public CSceneGraphNode
{
    SkyTypeDefRTTI;

friend class GLCgVertexBuffer;
friend class CFont;
friend class CFontSimple;
friend class CDecalMember;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CIndexBuffer();

	/// <summary>
	/// インデックスバッファのサイズを取得する
	/// </summary>
	inline u16				GetIndexNum           () const { return m_IndexNum; };

	/// <summary>
	/// インデックスバッファの使用数
	/// </summary>
	inline u16				GetUseIndexNum        () const { return m_UseIndexNum; };

	/// <summary>
	/// 内容をコピーする
	/// </summary>
	void			Copy				( CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// 指定の添え字のインデックスを取得する
	/// </summary>
	vrtxtype		Index				( vrtxtype indexNo ) const;

	/// <summary>
	/// 指定の添え字にインデックスを設定する
	/// </summary>
	void			SetIndex			( vrtxtype indexNo , u16 value );

	/// <summary>
	/// 内容を初期化
	/// </summary>
	void			Initialize			( vrtxtype indexNum );

	/// <summary>
	/// 内容を初期化
	/// </summary>
	void			Dispose		         ();

	/// <summary>
	/// CPU側のメモリをクリアする
	/// </summary>
	virtual void    DeleteCpuMemory();

	/// <summary>
	/// GPU側のメモリ上にインデックスバッファを作成する
	/// </summary>
	virtual void CreateGpuMemory();

	/// <summary>
	/// GPU側のメモリ上のインデックスバッファを削除する
	/// </summary>
	virtual void DeleteGpuMemory();

	/// <summary>
	/// GPU側のメモリ上のインデックスバッファに書き込む
	/// </summary>
	virtual void WirteGpuMemory();

	/// <summary>
	/// GPU側のメモリのストリームを設定する
	/// </summary>
	virtual void SetGpuMemoryStream();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
    inline const CIndexBufferMemberPtr &GetMember();

	/// <summary>
	/// クリア（使用インデックス数を０にする。メモリは破棄しません。メモリを破棄したい場合はDispose()を呼ぶこと。
	/// </summary>
	inline void Clear()
	{ 
        m_UseIndexNum = 0;
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
	serialize::CSerializeInterfacePtr         Clone();
	serialize::CSerializeInterfacePtr 		  CloneSharing();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	virtual void Deactivate();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CIndexBufferPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CIndexBufferPtr spIndexBuffer( NEW__( CIndexBuffer , CIndexBuffer() ) );
		if ( name.IsEmpty() == skyFalse ) spIndexBuffer->Name.SetName( name );
		return spIndexBuffer;
	}

	/// <summary>
	/// 情報プリント
	/// </summary>
	virtual void InfoPrint();

protected:

//---Member

	/// <summary>
	/// インデックス数（１プリミティブの頂点数×プリミティブ数）
	/// </summary>
	vrtxtype	m_IndexNum;

	/// <summary>
	/// 使用インデックス数
	/// </summary>
	vrtxtype     m_UseIndexNum;

	/// <summary>
	/// インデックスバッファ
	/// </summary>
	vrtxtype	*m_pCpuMemoryIndexBuffer;

//---Member

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// GPUメモリ確保フラグ
	/// </summary>
	skyBool m_IsCreateGpuMemory;

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CIndexBufferMemberPtr  m_pMember;

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// インデックス使用数の更新
	/// </summary>
	inline void UpdateUseIndexNum( vrtxtype indexNo )
	{
		m_UseIndexNum = indexNo;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CIndexBuffer();
};

} } }

#define CIndexBufferCreate_()			sky::lib::graphic::CIndexBuffer::Create()
#define CIndexBufferCreateName_( name )	sky::lib::graphic::CIndexBuffer::Create( name )

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

typedef SmartPointerCom< ID3D11Buffer > D3DIndexBufferPtr;

/// <summary>
/// Ｄ３Ｄのインデックスバッファのメンバ
/// </summary>
struct CIndexBufferMember
{
	/// <summary>
	/// グラフィックハード上のメモリ上のインデックスバッファ
	/// </summary>
	D3DIndexBufferPtr	m_pGpuMemoryIndexBuffer;
};

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// インデックスバッファのメンバ
/// </summary>
struct CIndexBufferMember
{
	/// <summary>
	/// インデックスバッファハンドル
	/// </summary>
	GLuint                 m_hBuffer;

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

	/// <summary>
	/// CgのシェーダーID
	/// </summary>
    CGprogram m_CgProgram;

#endif

};

} } }

#endif
