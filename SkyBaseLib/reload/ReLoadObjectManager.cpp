#include "StdAfx.h"
#include "SkyBaseLib/ReLoad.h"

namespace sky { namespace lib { namespace reload {

IReLoadObjectPtrList* CReLoadObjectManager::m_pReLoadInfoList = skyNull;
updateTime            CReLoadObjectManager::m_LastReloadTime = 0;

//-------------------------------------< 実装 >---------------------------------------------//

void CReLoadObjectManager::Initialize()
{
    m_pReLoadInfoList = NEW__( IReLoadObjectPtrList , IReLoadObjectPtrList() );
}

void CReLoadObjectManager::Dispose()
{
	DEL m_pReLoadInfoList;
}

void CReLoadObjectManager::Add( IReLoadObjectPtr &pObj )
{
	m_pReLoadInfoList->AddTail( pObj );
}

void CReLoadObjectManager::Clear()
{
	m_pReLoadInfoList->DeleteAll();
}

void CReLoadObjectManager::Update( updateTime nowTime , updateTime reloadCheckIntvlTime )
{
	//時間がきたらリロードする
	if ( nowTime - m_LastReloadTime > reloadCheckIntvlTime )
	{
		FOREACH( IReLoadObjectPtrList , it , *m_pReLoadInfoList )
		{
			//リロードオブジェクトの持っているファイルを全部チェックして一つでも更新時間が違ったらリロードを実行する
			FOREACH( FileUpdateDataPtrList , it2 , ( *it )->GetFilePathList() )
			{
				//更新時間が違ったらリロード
				u32 updateTime = CFileUtil_::GetFileUpdateTime( ( *it2 )->filePath->GetString() );
				if ( ( *it2 )->updateTime !=  updateTime )
				{
					//アンロードして
					( *it )->UnLoad();

					//ロード
					( *it )->Load();

					( *it2 )->updateTime = updateTime;

					break;
				}
			}
		}

		m_LastReloadTime = nowTime;
	}
}

} } }