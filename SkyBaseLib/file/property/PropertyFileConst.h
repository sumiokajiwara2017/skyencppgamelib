#pragma once

namespace sky { namespace lib { namespace file { namespace property {

	/// <summary>
	/// �o�[�W�������
	/// </summary>
	static const u32 VERSION_NO = 1;

	/// <summary>
	/// �o�C�i�����ʃR�[�h��
	/// </summary>
	static const u32 FILE_CODE_LENGHT = 3;

	/// <summary>
	/// �L�[�l�ő�T�C�Y
	/// </summary>
	static const u32 KEY_SIZE_MAX = 64;

	/// <summary>
	/// �n�b�V���f�t�H���g�T�C�Y
	/// </summary>
	static const u32 DEFAULt_HASH_SIZE = 31;

	/// <summary>
	/// �f�B���N�g�������p�X
	/// </summary>
	static const skyString *PROPERTY_SEARCH_PATH = _T( "data/property/*" );

	/// <summary>
	/// �v���p�e�B�t�@�C���g�`�r�g�T�C�Y
	/// </summary>
	static const u32 PROPERTY_FILE_HASH_SIZE = 61;


} } } }