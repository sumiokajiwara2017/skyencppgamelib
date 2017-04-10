#pragma once

namespace sky { namespace lib { namespace graphic {

class CMeshContena;
typedef SmartPointer< CMeshContena >			CMeshContenaPtr;				//スマートポインタ版定義
typedef CList< u32 , CMeshContenaPtr >		CMeshContenaPtrList;			//リスト定義
typedef CHash< CMeshContenaPtr >			    CMeshContenaPtrHash;			//ハッシュ定義

/// <summary>
/// メッシュクラス
/// </summary>
class CMeshContena : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMeshContena(){}

    /// <summary>
	/// レンダリング処理
    /// </summary>
	virtual void vRendering ( CRenderingParam &param );

//---Leaf

	/// <summary>
	/// 幾何学形状情報の作成
	/// </summary>
	void     AttachMesh( const CMeshPtr &spMesh );
	CMeshPtr GetMesh( u32 meshNo = 0 );
	void     DeleteAllMesh();
	u32      GetMeshNum() const;

//---Leaf

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
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

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
	/// インスタンス生成
	/// </summary>
	static CMeshContenaPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CMeshContenaPtr spMesh( NEW__( CMeshContena , CMeshContena() ) );
		if ( name.IsEmpty() == skyFalse ) spMesh->Name.SetName( name );
		return spMesh;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMeshContena(){}

};

} } }

#define CMeshContenaCreate_()				sky::lib::graphic::CMeshContena::Create()
#define CMeshContenaCreateName_( name )	sky::lib::graphic::CMeshContena::Create( name )

