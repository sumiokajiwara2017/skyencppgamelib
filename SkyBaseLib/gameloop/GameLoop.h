#pragma once

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ゲームループ定義
/// </summary>
#ifdef SKYLIB_PLATFORM_WIN32

//開始
#define GAME_LOOP_START	MSG msg;															\
						skyBool isGameExec = skyTrue;										\
						while( isGameExec )													\
						{																	\
							/* メッセージがあるかどうか	*/									\
							if ( PeekMessage( &msg , NULL , 0 , 0 , PM_NOREMOVE) )			\
							{																\
								/* メッセージを取得し、WM_QUITかどうか */					\
								if ( !GetMessage( &msg , NULL , 0 , 0 ) ) break;			\
																							\
								TranslateMessage( &msg ); /* キーボード利用を可能にする */	\
								DispatchMessage( &msg );  /* 制御をWindowsに戻す */			\
							}																\
							else															\
							{																
								/* ここに処理が書かれる */
//終了
#define GAME_LOOP_END		}																\
						}
//終了
#define GAME_LOOP_EXIT()		break

#endif