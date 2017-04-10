#pragma once

#include "SkyBaseLib/Controller.h"
#include "SkyMathLib/Controller.h"
#include "SkyGraphicLib/Controller.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../../libs/SkyThirdPartyLibd.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLibd.lib")
#pragma comment(lib, "../../../../libs/SkyMathLibd.lib")
#pragma comment(lib, "../../../../libs/SkyGraphicLibd.lib")
#else
#pragma comment(lib, "../../../../libs/SkyThirdPartyLib.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLib.lib")
#pragma comment(lib, "../../../../libs/SkyMathLib.lib")
#pragma comment(lib, "../../../../libs/SkyGraphicLib.lib")
#endif

//ライブラリの初期化、末期化のまとめ
#define	SKY_LIB_GRAPHIC_INIT()		SkyBaseLibControllerInitialize_( sky::lib::input::DEFAULT_PLAYER_NUM );		\
									SkyMathLibControllerInitialize_();											\
									SkyGraphicLibControllerInitialize_();											
									

#define	SKY_LIB_GRAPHIC_TERM()		SkyGraphicLibControllerDispose_();		\
									SkyMathLibControllerDispose_();			\
									SkyBaseLibControllerDispose_();


#if OFF_

#define TEST_INPUT
#define TEST_SEQUENCE
#define TEST_DATABASE
#define TEST_FONT
#define TEST_UI
#define TEST_TEXTURE
#define TEST_OPENGL
#define TEST_SCREEN
#define TEST_SCENEGRAPH
#define TEST_RENDERER
#define TEST_PROJECTION_TEXTURE
#define TEST_NETWORK
#define TEST_MODEL
#define TEST_MOVE
#define TEST_FADE
#define TEST_CAMERA
#define TEST_COLLISION
#define TEST_CAMERA
#define TEST_SPRITE
#define TEST_MODEL
#define TEST_LOADER

#endif
#define TEST_SHADER
//#define TEST_ANIMATION

//何処でも使うであろうヘッダー
#include "SkyGraphicLib/DataBase.h"
#include "SkyGraphicLib/Debug.h"
