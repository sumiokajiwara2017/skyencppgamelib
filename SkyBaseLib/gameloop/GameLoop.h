#pragma once

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �Q�[�����[�v��`
/// </summary>
#ifdef SKYLIB_PLATFORM_WIN32

//�J�n
#define GAME_LOOP_START	MSG msg;															\
						skyBool isGameExec = skyTrue;										\
						while( isGameExec )													\
						{																	\
							/* ���b�Z�[�W�����邩�ǂ���	*/									\
							if ( PeekMessage( &msg , NULL , 0 , 0 , PM_NOREMOVE) )			\
							{																\
								/* ���b�Z�[�W���擾���AWM_QUIT���ǂ��� */					\
								if ( !GetMessage( &msg , NULL , 0 , 0 ) ) break;			\
																							\
								TranslateMessage( &msg ); /* �L�[�{�[�h���p���\�ɂ��� */	\
								DispatchMessage( &msg );  /* �����Windows�ɖ߂� */			\
							}																\
							else															\
							{																
								/* �����ɏ������������ */
//�I��
#define GAME_LOOP_END		}																\
						}
//�I��
#define GAME_LOOP_EXIT()		break

#endif