#pragma once

#include "SkyBaseLib/Controller.h"
#include "SkyMathLib/Controller.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../../libs/SkyBaseLibd.lib")
#pragma comment(lib, "../../../../libs/SkyThirdPartyLibd.lib")
#pragma comment(lib, "../../../../libs/SkyMathLibd.lib")
#else
#pragma comment(lib, "../../../../libs/SkyBaseLib.lib")
#pragma comment(lib, "../../../../libs/SkyThirdPartyLib.lib")
#pragma comment(lib, "../../../../libs/SkyMathLib.lib")
#endif

//���C�u�����̏������A�������̂܂Ƃ�
#define	SKY_MATH_LIB_INIT()			SkyBaseLibControllerInitialize_( sky::lib::input::DEFAULT_PLAYER_NUM ); \
									SkyMathLibControllerInitialize_();

#define	SKY_MATH_LIB_TERM()			SkyMathLibControllerDispose_(); \
									SkyBaseLibControllerDispose_();

//�e�X�g�̂n�m�^�n�e�e �X�C�b�`�Q
#if OFF_
#define TEST_SCRIPT
#define TEST_MATRIX
#define TEST_GEOMETORY
#define TEST_GRAPH
#endif

#define TEST_UTIL