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

	_tsetlocale( LC_ALL , L"japanese"); //�����錾�����_tprintf�œ��{�ꂪ�����������Ȃ��Ȃ�B

	_tprintf( _T( "< =================  SkyBaseLib Test Start !! =========================== >\n" ) );

	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();
	_tprintf( _T( "< =================  SkyBaseLib Test End !! =========================== >\n" ) );

	SKY_MATH_LIB_INIT();

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	_tprintf( _T( "====> Please Push Enter \n" ) );

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

	SKY_MATH_LIB_TERM();

	return 0;
}