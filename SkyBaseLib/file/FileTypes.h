#pragma once

/// <summary>
/// file�@�\���ʌ^(enum�Astruct�Afunctor�Adelegate�A�֐��|�C���^�^���̒�`�j
/// </summary>
namespace sky { namespace lib { namespace file { 

/// <summary>
/// �t�@�C���̎��
/// </summary>
enum eFileType
{
	eFileType_Binary = 0,
	eFileType_Text,
	eFileType_Bind,
};

/// <summary>
/// �t�@�C���I�[�v�����[�h
/// </summary>
enum eOpenMode
{
	eOpenMode_Text_R_NoCreate = 0, //�e�L�X�g���[�h�Ǎ���p�B�t�@�C����������΃G���[�i�e�L�X�g���[�h���Ə���ɕϊ�������̂Œ��Ӂj
	eOpenMode_Text_RW_Create,	   //�e�L�X�g���[�h�ǂݏ����\�B�t�@�C����������΍쐬�A����Ώ㏑���i�e�L�X�g���[�h���Ə���ɕϊ�������̂Œ��Ӂj //Open�̎��ɂO�o�C�g�̃t�@�C���ɂ��Ă��܂��܂��B
	eOpenMode_Binary_R_NoCreate,   //�o�C�i�����[�h�Ǎ���p�B�t�@�C����������΃G���[�i�e�L�X�g���[�h���Ə���ɕϊ�������̂Œ��Ӂj
	eOpenMode_Binary_RW_Create,	   //�o�C�i�����[�h�ǂݏ����\�B�t�@�C����������΍쐬�A����Ώ㏑���i�e�L�X�g���[�h���Ə���ɕϊ�������̂Œ��Ӂj //Open�̎��ɂO�o�C�g�̃t�@�C���ɂ��Ă��܂��܂��B
};

} } }