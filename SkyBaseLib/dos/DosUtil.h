#pragma once

#ifdef SKYLIB_PLATFORM_WIN32

namespace sky { namespace lib { namespace dos {

//�R�}���h�̒����̍ő�
static const u32 CMD_LEN_MAX = 1024;

/// <summary>
/// �c�����R�}���h�v�����v�g�����C�u����������s�A���䂷��
/// �������Windows�̂�
/// </summary>
class CDosUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �c�����R�}���h�𓯊����s���A���ʂ��󂯎��
	/// cmd��bat�t�@�C���̃p�X��n���Ƃ��́A�K��\\��؂�ɂ���/���ƔF�����Ȃ��E�E
	/// </summary>
	static u32 ExecDosCommand( const skyString *cmd , const skyString *outFilePath = skyNull , skyBool isDisplayWindow = skyTrue , const skyString *currentDir = skyNull );
};

//�C���X�^���X�A�N�Z�X������
#define CDosUtil_			sky::lib::dos::CDosUtil

} } }

#endif