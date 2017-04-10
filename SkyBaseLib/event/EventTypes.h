#pragma once

namespace sky { namespace lib { namespace event {

typedef hash::CHashStringPtr EventId;

/// <summary>
/// �萔
/// </summary>
static const u32 HAHS_SIZE = 131;
static const u32 FIFO_SIZE = 1024 * 2;
static const s32 EXEC_NUM_ETERNITY = -1;
static const u32 DEFAULT_UNIQ_ID = 0;
static const u32 DEFAULT_ONE_FRAME_EVENT_EXEC_NUM = 8;

static const skyString *SYSTEM_EVENT_EXIT          = _T( "System_Exit" );          //�A�v���P�[�V���������[�U�[�ɋ����I�����ꂽ�B
static const skyString *SYSTEM_EVENT_WINDOW_MOVE   = _T( "System_Window_Move" );   //�A�v���P�[�V������Window���������B
static const skyString *SYSTEM_EVENT_WINDOW_RESIZE = _T( "System_Window_Resize" ); //�A�v���P�[�V������Window���ύX���ꂽ�B

} } }