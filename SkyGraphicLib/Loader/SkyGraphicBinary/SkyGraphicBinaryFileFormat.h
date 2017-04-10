#pragma once

namespace sky { namespace lib { namespace graphic { namespace binary {

	/// <summary>
	/// �o�C�i�����ʃR�[�h
	/// </summary>
	const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 's', 'g', 'b' };

	/// <summary>
	/// �t�@�C���w�b�_
	/// </summary>
	struct FileHeader
	{
		u8		cCode[ FILE_CODE_LENGHT ];			//�o�C�i�����ʃR�[�h
		u8		bIsBigEndian;						//�r�b�O�G���f�B�A���t���O
		u32		nVersion;							//�o�[�W����
        u32	    nFileSize;							//�t�@�C���T�C�Y
	};

} } } }
