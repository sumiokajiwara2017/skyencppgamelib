#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// Squirrelファクトリー（Factoryパターン）
/// </summary>
class SqVmFactory : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqVmFactory();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqVmFactory();

	/// <summary>
	/// 作成
	/// </summary>
	SqVmObjectPtr Create( u32 stackSize );
	SqVmObjectPtr Create( SqVmObjectPtr &sqParentVmPtr , u32 stackSize );

	/// <summary>
	/// 削除
	/// </summary>
	void	Delete( SqVmObjectPtr &sqVmPtr );

	/// <summary>
	/// シングルトン機能付加
	/// <summary>
	friend class Singleton< SqVmFactory >;
    static Singleton< SqVmFactory > SingletonMember;

private:

	/// <summary>
	/// インスタンスリスト
	/// </summary>
	SqVmObjectPtrList		m_InstanceList;

};

} } }

#define SqVmFactoryCreate_()			( sky::lib::squirrel::SqVmFactory::SingletonMember.CreateInstance() )
#define SqVmFactory_					( sky::lib::squirrel::SqVmFactory::SingletonMember.GetInstance() )
#define SqVmFactoryDelete_()			( sky::lib::squirrel::SqVmFactory::SingletonMember.DeleteInstance() )

#endif
