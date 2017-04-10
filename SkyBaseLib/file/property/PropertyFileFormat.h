#pragma once

namespace sky { namespace lib { namespace file { namespace property {

/// <summary>
/// �o�C�i�����ʃR�[�h
/// </summary>
const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 'p', 'r', 'o' };

/// <summary>
/// �t�@�C���w�b�_
/// </summary>
struct FileHeader
{
	u8		cCode[ FILE_CODE_LENGHT ];			//�o�C�i�����ʃR�[�h
	u8		bIsBigEndian;						//�r�b�O�G���f�B�A���t���O
	u32		nVersion;							//�o�[�W����
	u32		nFileSize;							//�t�@�C���T�C�Y
	u32		nPropertyCount;						//�v���p�e�B��
};
//���̃w�b�_�[�̎�����f�[�^���ɓ���B


/// <summary>
/// �f�[�^�w�b�_
/// �f�[�^�͕����I�[�ŋ�؂�ꂽ������B���C���[�W���wKEY �l�P value2�x
/// �ŏ��̕����̓L�[�ƂȂ�A�Ȍ�̕����͒l�ƂȂ�B�ǂݍ��݃T�C�Y��valueSize��������܂œǂݍ���
/// </summary>
struct DataHeader
{
	u32		valueSize;
};

} } } }