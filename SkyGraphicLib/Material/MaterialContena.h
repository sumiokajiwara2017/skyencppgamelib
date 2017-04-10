#pragma once

namespace sky { namespace lib { namespace graphic {

class CMaterialDrawData;
typedef SmartPointer< CMaterialDrawData >		CMaterialDrawDataPtr;	  //スマートポインタ版定義

/// <summary>
/// 描画用マテリアルデータ
/// </summary>
struct CMaterialDrawData : public CSceneGraphNode
{

public:

//--Branch

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	CMaterialPtr GetMaterial( skyBool isRequired = skyTrue );

//--Branch

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
	/// インスタンス生成
	/// </summary>
	static CMaterialDrawDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMaterialDrawDataPtr spMaterialDrawData( NEW__( CMaterialDrawData , CMaterialDrawData() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterialDrawData->Name.SetName( name );
		return spMaterialDrawData;
	}

	/// <summary>
	/// 描画開始頂点バッファインデックス
	/// </summary>
	vrtxtype     m_DrawStartIndex;

	/// <summary>
	/// 描画終了頂点バッファインデックス
	/// </summary>
	vrtxtype     m_DrawEndIndex;

};

#define CMaterialDrawDataCreate_()				                              sky::lib::graphic::CMaterialDrawData::Create()
#define CMaterialDrawDataCreateName_( name )	                              sky::lib::graphic::CMaterialDrawData::Create( name )

class CMaterialContena;
typedef SmartPointer< CMaterialContena >		CMaterialContenaPtr;	  //スマートポインタ版定義

/// <summary>
/// マテリアルコンテナクラス
/// </summary>
class CMaterialContena : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

//----Leaf

	/// <summary>
	/// マテリアル追加
	/// </summary>
	void AddMaterial( const CMaterialPtr &spMaterial , vrtxtype startIndex , vrtxtype endIndex );

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	CMaterialPtr GetMaterial( u32 index = 0 );

	/// <summary>
	/// マテリアル情報の取得
	/// </summary>
	CMaterialDrawDataPtr GetMaterialData( u32 index = 0 );

	/// <summary>
	/// 描画インデックス範囲の設定
	/// </summary>
	void SetDrawIndex(  u32 index , vrtxtype drawStartIndex , vrtxtype drawEndIndex );

	/// <summary>
	/// マテリアルリストの参照取得（これで自分でFOREACHしてね）
	/// </summary>
	CSceneGraphNodePtrList &GetMaterialDataArray();

//----Leaf

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

//---Static

	/// <summary>
	/// インスタンス生成
	/// </summary>
	static CMaterialContenaPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMaterialContenaPtr spMaterialContena( NEW__( CMaterialContena , CMaterialContena() ) );
		if ( name.IsEmpty() == skyFalse ) spMaterialContena->Name.SetName( name );
		return spMaterialContena;
	}
	static CMaterialContenaPtr Create( const CMaterialPtr &spMaterial , vrtxtype endIndex )
	{
		CMaterialContenaPtr spMaterialContena( NEW__( CMaterialContena , CMaterialContena() ) );
		spMaterialContena->AddMaterial( spMaterial , 0 , endIndex );

		return spMaterialContena;
	}
};

} } }

#define CMaterialContenaCreate_()				                              sky::lib::graphic::CMaterialContena::Create()
#define CMaterialContenaCreateName_( name )	                                  sky::lib::graphic::CMaterialContena::Create( name )
#define CMaterialContenaCreateSetMaterial_( spMaterial , endIndex )           sky::lib::graphic::CMaterialContena::Create( spMaterial , endIndex )
