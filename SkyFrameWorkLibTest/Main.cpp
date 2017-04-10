#include "StdAfx.h"
#include "GoogleTestInclude.h"
#include "SkyLibInclude.h"
#include "SkyBaseLib/Base.h"

//#include "glut/glut.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();

	SKY_LIB_GRAPHIC_INIT();

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

	SKY_LIB_GRAPHIC_TERM();

	return 0;
}
