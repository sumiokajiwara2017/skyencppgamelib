#pragma once

namespace sky { namespace lib { namespace comp {

static const u32 VERSION		 = 1;		//�o�[�W����
static const u32 BIN_CODE_LENGHT = 4;		//�o�C�i�����ʃR�[�h��

/// <summary>
/// �o�C�i�����ʃR�[�h
/// </summary>
const static u8	g_cFileCode[ BIN_CODE_LENGHT ] = { 'c', 'o', 'm' , 'p' };

/// <summary>
/// �t�@�C���w�b�_
/// </summary>
struct FileHeader
{
	u8		cCode[ BIN_CODE_LENGHT ];			//�o�C�i�����ʃR�[�h
	u8		al[3];								//�A���C�����g
	u32		nVersion;							//�o�[�W����
	u32		nOrgDataSize;						//���k�O�f�[�^�T�C�Y
};

} } }