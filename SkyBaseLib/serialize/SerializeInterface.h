#pragma once

namespace sky { namespace lib { namespace serialize {

class CSerializeInterface;
typedef SmartPointer< CSerializeInterface >	CSerializeInterfacePtr;			//スマートポインタ型定義
typedef CHash< CSerializeInterfacePtr >	    CSerializeInterfacePtrHash;		//スマートポインタリスト型

/// <summary>
/// クラスのシリアライズインターフェース
/// </summary>
class CSerializeInterface : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CSerializeInterface() : 
	  m_spIsActivated( NEW__( skyBool , skyBool ) )
	  {
		  *m_spIsActivated = skyFalse;
	  }

	/// <summary>
	/// 活性化済みフラグ（共有する場合を考えてスマートポインタで持っている）
	/// </summary>
	skyBoolPtr m_spIsActivated;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CSerializeInterface(){}

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream ) = 0;
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream ) = 0;

	/// <summary>
	/// コピーをする（各ノードでオーバーライドする）
	/// </summary>
	virtual const CSerializeInterfacePtr &Copy( const CSerializeInterfacePtr &spContainer ) = 0;

	/// <summary>
	/// すべてをコピーをする（各ノードでオーバーライドする）
	/// 共有するものは内部で共有する
	/// </summary>
	virtual const CSerializeInterfacePtr &CopySharing( const CSerializeInterfacePtr &spContainer ) = 0;

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	virtual CSerializeInterfacePtr 	Clone() = 0;

	/// <summary>
	/// すべてを複製をする（各ノードでオーバーライドする）
	/// 共有するものは内部で共有する
	/// </summary>
	virtual CSerializeInterfacePtr 	CloneSharing() = 0;

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	virtual void Activate(){ *m_spIsActivated = skyTrue; }

#define ACTIVATE_COMMON() \
	if ( *m_spIsActivated == skyTrue ) return; \
	this->serialize::CSerializeInterface::Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	virtual void Deactivate(){ *m_spIsActivated = skyFalse; }

#define DEACTIVATE_COMMON() \
	if ( *m_spIsActivated == skyFalse ) return; \
	this->serialize::CSerializeInterface::Deactivate();

};

} } }