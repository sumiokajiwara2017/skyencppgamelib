#pragma once



namespace sky { namespace lib { namespace directory {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �p�X�����񑀍�p�֐��Q
/// <summary>
class CPathUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �f�B���N�g���쐬
	/// </summary>
	static skyBool CreateDir( const skyString *srcPath );

	/// <summary>
	/// �f�B���N�g���폜
	/// </summary>
	static skyBool DeleteDir( const skyString *srcPath );

	/// <summary>
	/// �J�����g�f�B���N�g���p�X�擾
	/// </summary>
	static skyBool GetCurrentDir( skyString *destPath , u32 destLen );

	/// <summary>
	/// �J�����g�h���C�u�擾
	/// </summary>
	static s32 GetCurrentDrive();

	/// <summary>
	/// �J�����g�f�B���N�g���ύX
	/// </summary>
	static skyBool ChangeCurrentDirectory( const skyString *srcPath );

	/// <summary>
	/// �J�����g�h���C�u�ύX
	/// </summary>
	static skyBool ChangeCurrentDrive( s32 drive );

	/// <summary>
	/// �t�@�C���p�X����g���q�����i���̊֐��͓��̓f�[�^�����������܂��j
	/// </summary>
	void CutFileExt( const skyString *srcPath , skyString *destPath , u32 destLen );

	/// <summary>
	/// �t�@�C���p�X����h���C�u�����擾����
	/// </summary>
	static void		GetDrive		( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �t�@�C���p�X����g���q���擾����
	/// </summary>
	static void		GetFileExt		( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �t�@�C���p�X�̊g���q��t���ւ���
	/// </summary>
	static void		ReplaceFileExt	( const skyString *srcPath, skyString *destBuf , u32 destBufSize, const skyString *pExt );

	/// <summary>
	/// �t�@�C���p�X�̊g���q���w��̕����̏ꍇ�^��Ԃ�
	/// </summary>
	static skyBool  IsExt( const skyString *srcPath , const skyString *srcExt );

	/// <summary>
	/// �t�@�C���p�X�̃t�@�C���������ւ���
	/// </summary>
    static void     ReplaceFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pName );

	/// <summary>
	/// �t�@�C���p�X�̃t�@�C�����̌��ɃT�C����t����ihoge/fileNaem.hg �� hoge/fileName_H.hg�i_H��t�����j�j
	/// </summary>
    static void     AddTailFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pNameSignature );

	/// <summary>
	/// �t�@�C���p�X����g���q�t���t�@�C�����𒊏o����
	/// </summary>
	static void		GetFileName	( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �t�@�C���p�X�g���q���̂������t�@�C�����݂̂��擾
	/// </summary>
	static void		GetFileNameNoExt( const skyString *srcPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �t�@�C���p�X����f�B���N�g���p�X�𒊏o����
	/// </summary>
	static void	    GetDirPath ( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ���΃p�X�����΃p�X���擾����
	/// </summary>
	static void		GetFullPath ( const skyString *srcPath, skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �w��̃p�X����t�@�C���������Ƃ��āA�w��̃f�B���N�g���p�X�ƌ��������p�X��Ԃ�
	/// </summary>
	static void     GetPathFileNameAddPath( const skyString *srcPath , const skyString *srcDirPath , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// �t�@�C�����݃`�F�b�N
	/// </summary>
	static skyBool		IsFileOn		( const skyString *pPath );

	/// <summary>
	/// �f�B���N�g�����݃`�F�b�N
	/// </summary>
	static skyBool		IsDirectoryOn	( const skyString *pPath );

	/// <summary>
	/// �����X���b�V���`�F�b�N
	/// </summary>
	static skyBool		IsSlashEnd		( const skyString *pPath );

	/// <summary>
	/// �w��̃��C���h�J�[�h�Ɏw��̃p�X����v���邩�ǂ����Ԃ�
	/// </summary>
	static skyBool		IsWildCardMatch ( const skyString *wildCard , const skyString *filePath );

	/// <summary>
	/// ���ϐ��̒l���擾
	/// </summary>
	static void	GetEnv( memSize &destlen , const skyString *name , skyString *destBuf , u32 destBufSize );

	/// <summary>
	/// ���ϐ��̒l��ǉ�
	/// </summary>
	static void PutEnv( const skyString *name , const skyString *val );

	/// <summary>
	/// ���ϐ����܂ޕ������n���A�ϊ���̒l��Ԃ�
	/// </summary>
	static skyBool ReplaceEnv( const skyString *src , skyString *destBuf , u32 destBufSize );

private:

	//�e�@�\����
	CPathUtil(){};											//�R���X�g���N�^
	CPathUtil( const CPathUtil& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CPathUtil(){};									//�f�X�g���N�^
	CPathUtil &operator=(const CPathUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

//�C���X�^���X�A�N�Z�X������
#define CPathUtil_			sky::lib::directory::CPathUtil

} } }