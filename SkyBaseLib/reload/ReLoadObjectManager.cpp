#include "StdAfx.h"
#include "SkyBaseLib/ReLoad.h"

namespace sky { namespace lib { namespace reload {

IReLoadObjectPtrList* CReLoadObjectManager::m_pReLoadInfoList = skyNull;
updateTime            CReLoadObjectManager::m_LastReloadTime = 0;

//-------------------------------------< ���� >---------------------------------------------//

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
	//���Ԃ������烊���[�h����
	if ( nowTime - m_LastReloadTime > reloadCheckIntvlTime )
	{
		FOREACH( IReLoadObjectPtrList , it , *m_pReLoadInfoList )
		{
			//�����[�h�I�u�W�F�N�g�̎����Ă���t�@�C����S���`�F�b�N���Ĉ�ł��X�V���Ԃ�������烊���[�h�����s����
			FOREACH( FileUpdateDataPtrList , it2 , ( *it )->GetFilePathList() )
			{
				//�X�V���Ԃ�������烊���[�h
				u32 updateTime = CFileUtil_::GetFileUpdateTime( ( *it2 )->filePath->GetString() );
				if ( ( *it2 )->updateTime !=  updateTime )
				{
					//�A�����[�h����
					( *it )->UnLoad();

					//���[�h
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