#include "StdAfx.h"
#include "GoogleTestInclude.h"
#include "SkyLibInclude.h"

#include "SkyBaseLib/SkyLibCommon.h"

#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	//Windows�̃��������[�N�`�F�b�N������
//	CrtLeakCheckInit();
	{

	SKY_PRINTF( _T( "< =================  SkyBaseLib Test Start !! =========================== >\n" ) );

	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();

	SKY_PRINTF( _T("< =================  SkyBaseLib Test End !! =========================== >\n" ) );

	}
	//Window�̃��������[�N�`�F�b�N
//	CrtLeakCheck();

	SKY_BASE_LIB_INIT();

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "====> Please Push Enter \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		//���͎�t
		if ( Ckeyboard_->IsKeyPress( KEY_ENTER ) )
		{
			GAME_LOOP_EXIT();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_BASE_LIB_TERM();

	return 0;
}

