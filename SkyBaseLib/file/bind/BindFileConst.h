#pragma once

namespace sky { namespace lib { namespace file { namespace bind {

	/// <summary>
	/// �o�[�W�������
	/// </summary>
	const static u32 VERSION_NO = 1;

	/// <summary>
	/// �o�C�i�����ʃR�[�h��
	/// </summary>
	const static u32 FILE_CODE_LENGHT = 3;

	/// <summary>
	/// �o�C�i�����ʃR�[�h
	/// </summary>
	const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 'b', 'n', 'd' };

	/// <summary>
	/// �����t�@�C���ő�t�@�C����
	/// </summary>
	const static u32 FILE_NUM_MAX = 256;

	/// <summary>
	/// �����t�@�C���A�N�Z�X�n�b�V���̃T�C�Y
	/// </summary>
	const static u32 FILE_HASH_SIZE = 91;

	/// <summary>
	/// �f�t�H���g�J�e�S���ԍ�
	/// </summary>
	const static u32 DEFAULT_CATEGORY_NO = 0;

} } } }