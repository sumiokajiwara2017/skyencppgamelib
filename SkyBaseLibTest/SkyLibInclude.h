#pragma once

#include "SkyBaseLib/SkyLibCommon.h"

#ifdef _DEBUG
#pragma comment(lib, "../../../../libs/SkyThirdPartyLibd.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLibd.lib")
#else
#pragma comment(lib, "../../../../libs/SkyThirdPartyLib.lib")
#pragma comment(lib, "../../../../libs/SkyBaseLib.lib")
#endif

//ÉâÉCÉuÉâÉäÇÃèâä˙âªÅAññä˙âªÇÃÇ‹Ç∆Çﬂ
#define	SKY_BASE_LIB_INIT()			SkyBaseLibControllerInitialize_( sky::lib::input::DEFAULT_PLAYER_NUM );

#define	SKY_BASE_LIB_TERM()			CheckMemBreak(); \
									SkyBaseLibControllerDispose_();


//ÉeÉXÉgÇÃÇnÇmÅ^ÇnÇeÇe ÉXÉCÉbÉ`åQ
//#define TEST_DOS

#if OFF_

#define TEST_EXCEL
#define TEST_REGEX
#define TEST_CONST
#define TEST_INPUT
#define TEST_SEQUENCE
#define TEST_UTIL
#define TEST_MEMORY
#define TEST_COMPRESS
#define TEST_FPS
#define TEST_GAMELOOP
#define TEST_STREAM
#define TEST_POINTER
#define TEST_CONSOLE
#define TEST_DIRECTORY
#define TEST_COROUTINE
#define TEST_HASH
#define TEST_DEBUG_LOGIC
#define TEST_DELEGATE
#define TEST_GRAPHIC
#define TEST_CONTROLLER
#define TEST_THREAD
#define TEST_STRING
#define TEST_RTTI
#define TEST_PROCESS
#define TEST_EVENT
#define TEST_FILE
#define TEST_COMPRESS
#define TEST_PARSER
#define TEST_SCRIPT
#define TEST_THREAD
#define TEST_NETWORK
#define TEST_KFM
#define TEST_INTERP
#define TEST_SKYTL
#define TEST_TOKEN
#define TEST_STATE

#endif

#define TEST_SKYTL
