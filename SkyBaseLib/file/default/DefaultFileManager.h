#pragma once

namespace sky { namespace lib { namespace file { 

struct CDefaultFileManagerMember;

/// <summary>
/// �f�t�H���g�t�@�C���Ǘ�
/// ���O�Ɋg���q���L�[�Ƀt�@�C�������[�h���Ă����ƁA�t�@�C���p�X��n����
/// ���[�h�v�����������ꍇ�A���̃t�@�C���p�X�̊g���q�����ăt�@�C���f�[�^��Ԃ��B
/// �e�N�X�`�����͂��΂��΃t�@�C���������Ă������ŃG���[�����ɓ�����p�����Ăق����B
/// ���̂��߂̋@�\�B
/// </summary>
class CDefaultFileManager : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose();

	/// <summary>
	/// �t�@�C����񓯊��ǂݍ��݂���B
	/// �߂�l�Ƃ��ĕԂ����I�u�W�F�N�g�̃t���O���Ď����邱�ƂŖ��߂��������t�@�C����
	/// �ǂݍ��ݏI������ƁA�t�@�C���̃f�[�^�̎󂯎�肪�ł��邪�A�I������͉��L�̏I��
	/// ����֐����g�p����B�Ȃ��Ȃ畡���̃t�@�C���̃��[�h�v�����o�����Ƃ��z�肳��A
	/// ������ꊇ�ŏI�����肵�����Ƃ����v���������ł͂قƂ�ǂ��Ǝv���邽�߁B
    ///
	/// �t�@�C���ɂ̓J�e�S���ԍ���t���ł��A�J�e�S���ԍ��ō폜�ł���B�J�e�S���ԍ���
	/// �����t�@�C���ł����������Ƃ��ł��A�J�e�S�����̍폜���߂������Ƃ��Ă��A�܂�
	/// �t�@�C���̕t������Ă���J�e�S�����폜����A�������ǂ��̃J�e�S���ł��Ȃ��Ȃ���
	/// �ꍇ�̓t�@�C�����폜�����B
    ///
    /// ���łɑ��݂���t�@�C���̃��[�h�v����������A�t�@�C���̎Q�ƃJ�E���^���t�o���A
	/// �V�K�̃��[�h�͍s��Ȃ��B�폜�����Q�ƃJ�E���^�������A�Q�ƃJ�E���^���O�ɂȂ�����
	/// �폜���s��
	/// 
	/// </summary>
	static void LoadAsync( const hash::CHashStringPtr &pPath , u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// �񓯊��t�@�C���ǂݍ��ݗv����S�ăL�����Z������
	/// </summary>
	static void CancelAll();

	/// <summary>
	/// �񓯊��t�@�C���ǂݍ��ݏI������
	/// �����񓯊��t�@�C���ǂݍ��݂��s�����ꍇ�A�����ŕK���I��������s���Ă��������B��������������A�P���Ԃ���������ǂ܂�Ă���͂��Ƃ����̂͐�΋�����܂��񁚁�
	/// �񓯊��ǂݍ��ݏ����̖߂�l�ł���FileLoadAsyncRs�^�̃I�u�W�F�N�g�͓����Ń��X�g�ŊǗ�����A�I������Ɏg�p�����B�I���������̂̓��X�g����폜�����j
	/// </summary>
	static skyBool IsLoadEnd();

	/// <summary>
	/// �����t�@�C���ǂݍ���
	/// </summary>
	static void Load( const hash::CHashStringPtr &pPath , u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// �t�@�C�����t�@�C���p�X�ō폜����i���������t�@�C���̂Ȃ��̂ǂꂩ�̃t�@�C���������o���Ă���ꍇ�A���̃t�@�C���������ɂȂ�܂��B���̏����͐T�d�Ɏg���Ă��������j
	/// </summary>
	static void Delete( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// �t�@�C�����J�e�S���ō폜����
	/// </summary>
	static void Delete( u32 category = pool::DEFAULT_CATEGORY_NO );

	/// <summary>
	/// �Ǘ��t�@�C���𓯊��ǂݍ��݂���B�iFileController�͕K�v�ɉ����ē����ł�����Ăԁj
	/// �Ǘ��t�@�C���Ƃ̓o�C���h���ꂽ�P�P�̃t�@�C���̂��ƁB���[�U�[�͂��̃t�@�C����ǂ݂����͂��B
	/// </summary>
	static const u8 *LoadFile( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// �Ǘ��t�@�C�����폜����B���ۂɂ͍폜���Ȃ��B�����t�@�C�������Ɏw��p�X�̃t�@�C�������݂��Ă������ǂ��������Ԃ��iFileController�͕K�v�ɉ����ē����ł�����Ăԁj
	/// </summary>
	static skyBool   DeleteFile( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// �c�t�@�C�������X�g���[���Ƀ_���v����
	/// </summary>
	static void Print();

private:

	/// <summary>
	/// �����o���
	/// </summary>
	static CDefaultFileManagerMember *m_pMember;

};

//�C���X�^���X�A�N�Z�X������
#define CDefaultFileManagerInitialize_() sky::lib::file::CDefaultFileManager::Initialize()
#define CDefaultFileManager_             sky::lib::file::CDefaultFileManager
#define CDefaultFileManagerDispose_()    sky::lib::file::CDefaultFileManager::Dispose()

} } }