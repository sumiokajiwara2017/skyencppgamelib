#include "StdAfx.h"
#include "SkyBaseLib/ReLoad.h"

SkyImplementRTTI( sky::lib::reload::IReLoadObject   , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::reload::AbsReLoadObject , sky::lib::reload::IReLoadObject );

namespace sky { namespace lib { namespace reload {

//------------------------------------< ���ێ��� >--------------------------------------------//

void AbsReLoadObject::AddFilePath( const hash::CHashStringPtr &ptr )
{
	FileUpdateDataPtr spFile( NEW__( FileUpdateData , FileUpdateData() ) );

	//�t�@�C���p�X��ݒ肷��
	spFile->filePath = ptr;

	//�t�@�C���̍X�V���t���擾����
	spFile->updateTime = CFileUtil_::GetFileUpdateTime( ptr->GetString() );

	//���X�g�ɓo�^
	m_FilePathInfoList.AddTail( spFile );
}

} } }