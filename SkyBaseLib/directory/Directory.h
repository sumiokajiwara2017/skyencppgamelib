#pragma once

namespace sky { namespace lib { namespace directory {

class CDirectory;
typedef SmartPointer< CDirectory >			CDirectoryPtr;
typedef CList< s32 , CDirectory * >			DirectoryList;
typedef CComposite< CDirectory * , s32 >	DirectoryBase;

//---------------------------------< �t�@�N�g���[ >-----------------------------------------//

class CDirectoryFactory : public base::SkyObject
{

public:

	/// <summary>
	/// �쐬�i�X�}�[�g�|�C���^�ŕی삵�Ă邽�ߍ폜�̕K�v�Ȃ��j
	/// </summary>
	static CDirectoryPtr	Create		( const skyString *pPath , const skyString *pWildCard );
	static CDirectoryPtr	Create		( const skyString *pPath );
};

#define CDirectoryFactory_ sky::lib::directory::CDirectoryFactory

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �f�B���N�g���N���X
/// </summary>
class CDirectory : public DirectoryBase
{
    SkyTypeDefRTTI;

friend class CDirectoryFactory;
friend class SmartPointer< CDirectory >;

public:

	/// <summary>
	/// �������g���t�@�C�����ǂ������肷��
	/// </summary>
	skyBool				IsFile			();

	/// <summary>
	/// �p�X�̎擾
	/// </summary>
	const skyString*	GetPath		();

	/// <summary>
	/// �t�@�C�����X�g�̏o��(isR��On�̏ꍇ�q�̃t�H���_�ȉ��܂ŒT���ɍs���j
	/// </summary>
	void				WriteFileList	( file::CFileStreamPtr &textFilePtr , skyBool isR = skyTrue , const skyString *wildCard = skyNull );

	/// <summary>
	/// �f�B���N�g�����X�g�̏o��
	/// </summary>
	void				WriteDirList	( file::CFileStreamPtr &textFilePtr , skyBool isR = skyTrue );

	/// <summary>
	/// �t�@�C���p�X���X�g�̎擾
	/// </summary>
	void				GetFilePathList( hash::CHashStringPtrList &filePathList , skyBool isR = skyTrue , const skyString *wildCard = skyNull );

	/// <summary>
	/// �f�B���N�g���p�X���X�g�̎擾
	/// </summary>
	void				GetDirPathList	( hash::CHashStringPtrList &dirPathList , skyBool isR = skyTrue );

	/// <summary>
	/// ������
	/// �R���X�g���N�^�Ə����������͕����čl����B�������ɕK�v�ȃp�����[�^�[�̓R���X�g���N�^�̈������A�ʓr�ݒ�֐��ŃI�u�W�F�N�g�Ɏ������B
	/// </summary>
	void				Initialize		();

	/// <summary>
	/// ���
	/// �f�X�g���N�^�ƊJ�������͕����čl����B
	/// </summary>
	void				Dispose		();

private:

	/// <summary>
	/// �������t���O
	/// </summary>
	skyBool m_bIsInitialized;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDirectory					( const skyString *pPath , CDirectory *pParent , const skyString *pWildCard );	//�f�B���N�g���p�R���X�g���N�^
	CDirectory					( const skyString *pPath , CDirectory *pParent );								//�t�@�C���p�R���X�g���N�^

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual		~CDirectory	();

	/// <summary>
	/// �q�̗v�f�����ׂď����i�c���[�����ǂ��ĉ�����S�������j
	/// </summary>
	void		_DeleteChild		();

	/// <summary>
	/// �f�[�^�̎��
	/// </summary>
	typedef enum
	{
		Type_Directory = 0,	//�t�@�C���ł��B
		Type_File ,			//�f�B���N�g���ł��B
	} Type;

	/// <summary>
	/// �p�X
	/// </summary>
	string::CString		m_Path;

	/// <summary>
	/// ���C���h�J�[�h
	/// </summary>
	string::CString		m_WildCard;

	/// <summary>
	/// ����
	/// </summary>
	Type		m_eType;

	/// <summary>
	/// �ċA��������
	/// </summary>
	void SearchDir( const skyString *pRootDir, const skyString *pWildCard );
};

} } }
