#pragma once

#include "SkyBaseLib/Controller.h"
#include "SkyMathLib/Controller.h"
#include "SkyGraphicLib/Controller.h"
#include "SkyPhysicsLib/Controller.h"
#include "SkyFrameWorkLib/Controller.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../../libs/SkyThirdPartyLibd.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLibd.lib")
#pragma comment(lib, "../../../../libs/SkyMathLibd.lib")
#pragma comment(lib, "../../../../libs/SkyGraphicLibd.lib")
#pragma comment(lib, "../../../../libs/SkyPhysicsLibd.lib")
#pragma comment(lib, "../../../../libs/SkyFrameWorkLibd.lib")
#else
#pragma comment(lib, "../../../../libs/SkyThirdPartyLib.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLib.lib")
#pragma comment(lib, "../../../../libs/SkyMathLib.lib")
#pragma comment(lib, "../../../../libs/SkyGraphicLib.lib")
#pragma comment(lib, "../../../../libs/SkyPhysicsLib.lib")
#pragma comment(lib, "../../../../libs/SkyFrameWorkLib.lib")
#endif

//ÉâÉCÉuÉâÉäÇÃèâä˙âªÅAññä˙âªÇÃÇ‹Ç∆Çﬂ
#define	SKY_LIB_GRAPHIC_INIT()		SkyBaseLibControllerInitialize_( sky::lib::input::DEFAULT_PLAYER_NUM );		\
									SkyMathLibControllerInitialize_();											\
									SkyGraphicLibControllerInitialize_();									    \
									SkyPhysicsLibControllerInitialize_();									    \
									SkyFrameWorkLibControllerInitialize_();											
									

#define	SKY_LIB_GRAPHIC_TERM()		SkyFrameWorkLibControllerDispose_();	\
                                    SkyPhysicsLibControllerDispose_();		\
                                    SkyGraphicLibControllerDispose_();		\
									SkyMathLibControllerDispose_();			\
									SkyBaseLibControllerDispose_();


#if OFF_

#define TEST_INPUT
#define TEST_PHYSICS
#define TEST_COLLISION
#define TEST_UI
#define TEST_SHADER
#define TEST_KEYFRAMEANIMATION
#define TEST_RENDERTARGET
#define TEST_UNDOREDO

#endif

#define TEST_AI