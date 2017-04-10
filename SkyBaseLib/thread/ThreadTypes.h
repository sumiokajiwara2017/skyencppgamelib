#pragma once

namespace sky { namespace lib { namespace thread {

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// �X���b�hID
/// </summary>
typedef unsigned tread_id;

/// <summary>
/// �X���b�h�n���h��
/// </summary>
typedef HANDLE tread_hndl;

#endif

/// <summary>
///	Event�҂����\�b�h�̖߂�l�̎��
/// </summary>
enum eWaitResultKind
{
	eWaitResultKind_Signaled = 0 ,				//�I�u�W�F�N�g���V�O�i����ԂɂȂ���
	eWaitResultKind_TimeOut ,					//�҂����ԃ^�C���A�E�g�ɂȂ���
	eWaitResultKind_Error						//����ȊO�i���肦�Ȃ��͂��j
};

/// <summary>
/// ���[�J�[�f�t�H���g��
/// </summary>
static const u32 DEFAULT_WORKER_NUM = 2;

/// <summary>
/// ���[�J�[�f�t�H���g�X�^�b�N�T�C�Y
/// </summary>
static const u32 DEFAULT_STACK_SIZE = 1024 * 2;

} } }