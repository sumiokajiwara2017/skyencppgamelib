#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �Đ��I���^�C�v
/// </summary>
enum ePlayEndType
{
	ePlayEndType_End = 0    , //�Đ��I��
	ePlayEndType_Repetition , //������Đ��J�n�i���������Ă������j
	ePlayEndType_Back       , //�I����Ԃ���߂�i���������Ă������j
};

/// <summary>
/// �Đ����
/// </summary>
enum ePlayState
{
	ePlayState_Wait = 0 ,
	ePlayState_Start , //�J�n���A�C�x���g�n���h�����R�[�����鎞�ɃC�x���g�n���h���̈����ɂ��̃X�e�[�^�X��n���B���ۂ̃X�e�[�g��ePlayState_Play�B
	ePlayState_Play ,
	ePlayState_Play_Reverse ,
	ePlayState_Stop ,
	ePlayState_End ,   //�I�����A�C�x���g�n���h�����R�[�����鎞�ɃC�x���g�n���h���̈����ɂ��̃X�e�[�^�X��n���B���ۂ̃X�e�[�g��ePlayState_Wait�B
};

/// <summary>
/// �V�[�P���X�n�b�V���T�C�Y
/// </summary>
const u32 SEQUENCE_HASH_SIZE = 16;

} } }