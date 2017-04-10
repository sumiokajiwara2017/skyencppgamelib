#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"

#include "Data/Data.h"
#include "Helper/Helper.h"
#include "Host/Host.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//�f�[�^�쐬
	DataSmPtr	data( NEW Data() );

	//�z�X�g�쐬
	Host host;

	//�d�����s
	host.DoWork( L"�d�����˗����܂����B\n" , data );

	SKY_PRINTF( L"�����͂����ɕԂ�\n" );

	//�f�[�^�𓾂�i�����Œ�~�j
	const skyWString *pData = data->GetData();

	SKY_PRINTF( L"�d������= %s \n" , pData );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�
}