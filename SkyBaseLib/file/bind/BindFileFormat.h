#pragma once

namespace sky { namespace lib { namespace file { namespace bind {

/// <summary>
/// �t�@�C���w�b�_
/// </summary>
struct FileHeader
{
	u8		cCode[ FILE_CODE_LENGHT ];			//�o�C�i�����ʃR�[�h
	u8		bIsBigEndian;						//�r�b�O�G���f�B�A���t���O
	u32		nVersion;							//�o�[�W����
	u32		nFileSize;							//�t�@�C���T�C�Y
	u32		nFileBindCount;						//�t�@�C���o�C���h��
};
//���̃w�b�_�[�̎�����f�[�^���ɓ���B


/// <summary>
/// �f�[�^�w�b�_
/// </summary>
struct DataHeader
{
	u32		nCompressFlg;						//���k�t���O(0:�񈳏k 1:zip���k)
	u8		cFilePath[ PATH_LEN_MAX ];		//�t�@�C���p�X
	u32		fileSize;							//�t�@�C���T�C�Y
};

} } } }