#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

Singleton< SqVmFactory >	SqVmFactory::SingletonMember;

SqVmFactory::SqVmFactory()
{
}

SqVmFactory::~SqVmFactory()
{
	m_InstanceList.DeleteAll();
}

SqVmObjectPtr SqVmFactory::Create( u32 stackSize )
{
	//VMを作成
	SqVmObjectPtr sqVmPtr( NEW__( SqVmObject , SqVmObject( stackSize ) ) );

	//VMをインスタンスリストにつなぐ
	m_InstanceList.AddTail( sqVmPtr );

	//返す
	return sqVmPtr;
}
SqVmObjectPtr SqVmFactory::Create( SqVmObjectPtr &sqParentVmPtr , u32 stackSize )
{
	//クローンを使って実体を作成
	SqVmObjectPtr sqVmPtr( NEW__( SqVmObject , SqVmObject( sqParentVmPtr , stackSize )  ) );

	//VMをインスタンスリストにつなぐ
	m_InstanceList.AddTail( sqVmPtr );

	//返す
	return sqVmPtr;
}


void SqVmFactory::Delete( SqVmObjectPtr &sqVmPtr )
{
	m_InstanceList.Delete( sqVmPtr );
}

} } }

#endif
