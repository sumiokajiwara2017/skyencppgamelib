#pragma once

namespace sky { namespace lib { namespace graphic {

//テンプレートTypedef郡
class CNilNode;
typedef SmartPointer< CNilNode >	CNilNodePtr;		//スマートポインタ版定義
typedef CList< u32 , CNilNodePtr >	CNilNodePtrList;	//リスト定義
typedef CHash< CNilNodePtr >		CNilNodePtrHash;	//Hash定義
typedef CHashMT< CNilNodePtr >		CNilNodePtrHashMT;	//Hash定義

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 無ノード。自分自身を描画しない。Jointを持って見えない関節として動作させるとかできる
/// </summary>
class CNilNode : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

    /// <summary>
    /// デストラクタ
    /// </summary>
	virtual ~CNilNode(){};

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
	void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	void Deactivate();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CNilNodePtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CNilNodePtr spNil( NEW__( CNilNode , CNilNode() ) );
		if ( name.IsEmpty() == skyFalse ) spNil->Name.SetName( name );
		return spNil;
	}

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CNilNode();
};

} } } 


#define CNilNodeCreate_()			sky::lib::graphic::CNilNode::Create()
#define CNilNodeCreateName_( name )	sky::lib::graphic::CNilNode::Create( name )
