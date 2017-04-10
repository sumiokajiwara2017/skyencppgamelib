#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >---------------------------------------------//

//テンプレートTypedef郡
class CExtraData;
typedef SmartPointer< CExtraData >			CExtraDataPtr;
typedef CList< u32 , CExtraData >			CExtraDataPtrList;

class CExtraData : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CExtraData(){};

//---Member

	/// <summary>
	/// double
	/// </summary>
	d64 m_DoubleValue;

	/// <summary>
	/// int
	/// </summary>
	s32 m_IntValue;

//---Member

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	void vPostRendering   ( CRenderingParam &param );

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
	serialize::CSerializeInterfacePtr 		  CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CExtraDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CExtraDataPtr spExtraData( NEW__( CExtraData , CExtraData() ) );
		if ( name.IsEmpty() == skyFalse ) spExtraData->Name.SetName( name );
		return spExtraData;
	}

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CExtraData(){};
};

} } }

#define CExtraDataCreate_()	          sky::lib::graphic::CExtraData::Create()
#define CExtraDataCreateName_( name ) sky::lib::graphic::CExtraData::Create( name )
