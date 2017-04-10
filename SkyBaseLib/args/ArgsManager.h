#pragma once

namespace sky { namespace lib { namespace args {

/// <summary>
/// �萔
/// </summary>
static const u32 OPTION_VALUE_HASH_SIZE = 31;
static const u32 ARGS_INFO_HASH_SIZE = 31;
	
/// <summary>
/// �����̎��
/// </summary>
enum eOptionType
{
    eOptionType_Switch = 0, //On/Off�̔���Ɏg�p����
    eOptionType_OneValue,   //�������P��
    eOptionType_ListValue,  //����������
};

/// <summary>
/// �����Ǘ����
/// </summary>
class ArgsInfo : public base::SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ArgsInfo()
	{
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ArgsInfo( eOptionType eType, const hash::CHashStringPtr &option, const hash::CHashStringPtr &info, skyBool isRequired , skyBool isEnvReplaceExec )
	{
		m_eType = eType;
		m_Option = option;
		m_Info = info;
		m_IsRequired = isRequired;
		m_IsEnvReplaceExec = isEnvReplaceExec;
	}

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	ArgsInfo		( const ArgsInfo& src )
	{
		m_eType = src.m_eType;
		m_Option = src.m_Option;
		m_Info = src.m_Info;
		m_IsRequired = src.m_IsRequired;
		m_IsEnvReplaceExec = src.m_IsEnvReplaceExec;
	}

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
	eOptionType				m_eType;
	hash::CHashStringPtr	m_Option;
	hash::CHashStringPtr	m_Info;
	skyBool					m_IsRequired;
	skyBool					m_IsEnvReplaceExec;

};

/// <summary>
/// �����Ǘ�
/// �����̕�����̗���̈����ɂ��A-option�Œl��C�ӂ̌����Ă����Ď󂯎������ɂ��Ή����Ă���B
/// 
/// �y�~�j�R���� �ݒ�t�@�C�������������D��Ă���_�z
/// �����쐬�����A�v���P�[�V�������A�����̏ꏊ�ňႤ�Ă΂���������ꍇ�A�ݒ�t�@�C���̏ꍇ���̉ӏ�����
/// �ݒ�t�@�C����p�ӂ��Ȃ��Ƃ����Ȃ��B�����̏ꍇ�͌Ăяo�������ɋL�q�������邾���łn�j�B
/// </summary>
class CArgsManager : public base::SkyRefObject
{

public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	CArgsManager();

	//�V���O���g���@�\��t��
	friend class Singleton< CArgsManager >;
    static Singleton< CArgsManager > SingletonMember;

    /// <summary>
    /// �����i�[HashTable
    /// </summary>
    hash::CHashStringPtrList			m_List;
    hash::CHashStringPtrListPtrHash		m_OptionValueHash;
	typedef CHash< ArgsInfo >			ArgsInfoHash;
    ArgsInfoHash						m_ArgsInfoHash;

    /// <summary>
    /// �������ݒ�
    /// </summary>
    void AddArgsInfo( const ArgsInfo &argsInfo );

    /// <summary>
    /// ���������R���\�[���ɏo�͂���
    /// </summary>
    void WriteConsoleArgsInfo();

    /// <summary>
    /// �������
    /// </summary>
    void Parse( int argc , skyString * argv[] );

    /// <summary>
    /// ������ǉ�
    /// </summary>
    void Add( const hash::CHashStringPtr &arg );

    /// <summary>
    /// �����̑��݃`�F�b�N
    /// </summary>
    skyBool IsArgsOn( u32 index );

    /// <summary>
    /// �����𕶎��Ƃ��Ď擾
    /// </summary>
    const hash::CHashStringPtr &GetString( s32 index );

    /// <summary>
    /// ������s32�Ƃ��Ď擾
    /// </summary>
    s32 GetS32( s32 index );

    /// <summary>
    /// �I�v�V�������݊m�F
    /// </summary>
    skyBool IsOptionOn( const hash::CHashStringPtr &optionName);

    /// <summary>
    /// �w��̃I�v�V�����̕K�{�t���O�𑀍삷��
    /// </summary>
    void SetReqFlg( const hash::CHashStringPtr &optionName , skyBool isRequired );

    /// <summary>
    /// �I�v�V�������L�[�ŗl�X�Ȍ`���Ŏ擾
    /// </summary>
    const hash::CHashStringPtr &GetOptionValue( const hash::CHashStringPtr &optionName );
    s32 GetOptionValue_s32( const hash::CHashStringPtr &optionName);
    u32 GetOptionValue_u32( const hash::CHashStringPtr &optionName);
    d64 GetOptionValue_d64( const hash::CHashStringPtr &optionName );

    /// <summary>
    /// �I�v�V�������L�[�ŕ������X�g�Ƃ��Ď擾
    /// </summary>
    hash::CHashStringPtrListPtr &GetOptionValues( const hash::CHashStringPtr &optionName);

    /// <summary>
    /// �K�{�`�F�b�N
    /// </summary>
    void AssertCheckReqOption();
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CArgsManagerCreate_()			sky::lib::args::CArgsManager::SingletonMember.CreateInstance()
#define CArgsManager_			sky::lib::args::CArgsManager::SingletonMember.GetInstance()
#define CArgsManagerDelete_()			sky::lib::args::CArgsManager::SingletonMember.DeleteInstance()
