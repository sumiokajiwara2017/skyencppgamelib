#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_GAMELOOP

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const u32 FRAME_RATE = 60;

TEST( gameloop , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�t���[�����[�g�̐ݒ�
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "�G�X�P�[�v�L�[���������Ă��������B \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//���͎�t
		if ( Ckeyboard_->IsKeyPress( KEY_ENTER ) )
		{
			//���݂̃t���[���Ԃ̌o�ߎ��Ԃƃt���[�����[�g����Z�o����1�t���[��������̌o�ߎ��Ԃ��o�͂���B���̒l�̓C�R�[���ł���̂����z�B
			SKY_PRINTF( _T( "PassTime=[ %f ms ] OneFrameTime=[ %f ms ] \n" ) ,  fpsCtrl.GetPassageTime() , fpsCtrl.GetOneFrameTime() );
		}

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_BASE_LIB_TERM();
}

#endif