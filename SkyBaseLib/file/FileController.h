#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �t�@�C������N���X
/// �t�@�C������N���X�̓t�@�C���ɑ΂���₢���킹�ɑS�ē�����B
/// �A�v���P�[�V�����́AFileController�Ɋւ��Ă�Load�����s��Ȃ��B
/// �A�v���P�[�V�����͑��ɁABindFileManager�APoolFileManager�ɑ΂���
/// ���O�Ƀt�@�C���̓ǂݍ��݂��˗����邱�Ƃ��ł���B��������Ă������ƂŁA
/// FileController��Load�̃��X�|���X�������ɂȂ�B
/// �A�v���P�[�V�������ɖ��v��Ƀt�@�C�����[�h�̏������ו��U�̏������X�ɓ����̂�
/// ������ł��o�O�̉����ɂȂ�B��΂ɋ֎~�ł���B�A�v���P�[�V�������̐��䏈����
/// �A�v���P�[�V�����̓s���ɍ��킹���^�C�~���O��BindFileManager�APoolFileManager
/// �ɑ΂��Ĕ񓯊��̃��[�h�v���������A�A�v���P�[�V�����̐���ȊO�̏�����CDiscFileController
/// ��Load�����𓯊��œǂނׂ��B
///
/// +------------------+
/// |                  |
/// |  FileController  |�|�|�|�|�|�|�{�|�|�|�|�|�|�|�|�|�|�|�{�|�|�|�|�|�|�|�|�|�|�|�|�{
/// |                  |�@�@�@�@�@�@�b�@�@�@�@�@�@�@�@�@�@�@�b�@�@�@�@�@�@�@�@�@�@�@  �b
/// +------------------+�@�@�@�@�@�@�b                     �b                      �b
///         �b�@�i������1�j�@�@�@�@  �b�A�i������1�j         �b�B�i�ᑬ��2�j         �b�C�i������1�j
///         ���@�@�@�@�@�@�@�@�@�@   ��					   ���@�@�@�@�@�@�@�@�@�@�@��
/// +------------------+    +------------------+�@�@+------------------+    +------------------+
/// |                  |	|                  |	|                  |    |                  |
/// |     BindFile     |	|     PoolFile     |	|    DiscFile      |    |    DefaultFile�@ |
/// |     Manager      |	|     Manager      |	|                  |    |    Manager       |
/// |                  |	|                  |	|                  |    |                  |
/// +------------------+	+------------------+	+------------------+    +------------------+
///
/// ��1 ���O���[�h���Ă����΍����ȃ��X�|���X��������B
/// ��2 HDD��DVD���ɕK���A�N�Z�X����̂ŁA��΂ɒx���B�{�Ԃł͇B�ƇC�ɗ��邱�Ƃ͊�{�I�ɖ������ɂȂ�͂��B
/// </summary>
class CDiscFileController : public base::SkyObject
{
    SkyTypeDefRTTI;

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
	/// �ǂݍ���(�A�v���P�[�V�����͂���������ĂԁB�|�C���^�̒��g���Ȃ����(IsEmpty()��true)�t�@�C�����ǂ��ɂ����݂��Ȃ����ƂɂȂ�j
	/// �����ł̓t�@�C���̓ǂݍ��݃^�C�~���O�Ǝg�p�^�C�~���O���S���Ⴄ�Ƃ������Ƃ��قƂ�ǂȂ̂ŁA�����ł����ǂݍ��݂Ƃ͑����̏ꍇ�A
	/// ���łɃ�������ɂ���t�@�C�����󂯎��Ƃ�����ƂɂȂ邱�Ƃ��]�܂����B���ۂ̓ǂݍ��݂́ABindFileManager�AFilePoolContena�A
	/// DefaultFileManager�ɑ΂��Ė��߂��o���B
	/// </summary>
	static const u8 *Load( const hash::CHashStringPtr &filePath );

	/// <summary>
	/// �폜
	/// </summary>
	static skyBool Delete( const hash::CHashStringPtr &filePath );

};

//�C���X�^���X�A�N�Z�X������
#define CDiscFileControllerInitialize_()     sky::lib::file::CDiscFileController::Initialize()
#define CDiscFileController_                 sky::lib::file::CDiscFileController
#define CDiscFileControllerDispose_()        sky::lib::file::CDiscFileController::Dispose()


} } }
