#pragma once

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
struct CMesh;
typedef SmartPointer< CMesh >	CMeshPtr;				//スマートポインタ版定義
typedef CList< u32 , CMeshPtr >	CMeshPtrList;			//リスト定義

/// <summary>
/// 三角形群
/// </summary>
struct CMesh : public CSceneGraphNode
{
    SkyTypeDefRTTI;

//---Branch

	/// <summary>
	/// 頂点バッファ
	/// </summary>
	CVertexBufferPtr  GetVertexBuffer( skyBool isRequired = skyTrue );
	void              AttachVertexBuffer( const CVertexBufferPtr &spVertex );

//---Branch

//---Leaf

	/// <summary>
	/// インデックスバッファ取得／設定
	/// </summary>
	CIndexBufferPtr  m_IndexBufferCash;
	CIndexBufferPtr  GetIndexBuffer( skyBool isRequired = skyTrue );
	void             AttachIndexBuffer( const CIndexBufferPtr &spIndexBuffer );

	/// <summary>
	/// マテリアル取得／設定
	/// </summary>
	CMaterialContenaPtr m_MaterialContenaCash;
	CMaterialContenaPtr GetMaterialContena( skyBool isRequired = skyTrue );
	void                AttachMaterialContena( const CMaterialContenaPtr &spMaterialContena );

//---Leaf

	/// <summary>
	/// スケルトン取得／設定
	/// </summary>
	math::CSkeletonPtr   GetSkeleton();
	void                 AttachSkeleton( const math::CSkeletonPtr &spSkeleton );

	/// <summary>
	/// 指定のIDで頂点ストリームを関連付け解除する
	/// </summary>
	void AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream );

	/// <summary>
	/// 指定のIDで頂点ストリームを関連付け解除する
	/// </summary>
	void DetachStream( eVertexBufferStreamType eType );

	/// <summary>
	/// 指定のIDで頂点ストリームを取得する
	/// </summary>
	CVertexBufferStreamPtr GetStream( eVertexBufferStreamType eType , skyBool isRequired = skyTrue );

	/// <summary>
	/// CPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	void DeleteCpuMemory();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを作成する
	/// </summary>
	void CreateGpuMemory();

	/// <summary>
	/// GPU側のメモリ上に頂点バッファを削除する
	/// </summary>
	void DeleteGpuMemory();

	/// <summary>
	/// GPU側のメモリ上の頂点バッファに書き込む
	/// </summary>
	void WirteGpuMemory();

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
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

//---Static

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static CMeshPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMeshPtr spMesh( NEW__( CMesh , CMesh() ) );
		if ( name.IsEmpty() == skyFalse ) spMesh->Name.SetName( name );
		return spMesh;
	}

private:

	/// <summary>
	/// ダミー値
	/// </summary>
	u32 m_Dummy;

	/// <summary>
	/// ジョイントパレット
	/// </summary>
	math::CSkeletonPtr m_spJoinbPallet;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMesh(){}
};

#define CMeshCreate_()				sky::lib::graphic::CMesh::Create()
#define CMeshCreateName_( name )	sky::lib::graphic::CMesh::Create( name )

} } }