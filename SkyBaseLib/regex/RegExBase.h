#pragma once

namespace sky { namespace lib { namespace regex {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

typedef CList< u32 , string::CString > RegResultList;

/// <summary>
/// ���K�\�������N���X
/// </summary>
class IRegEx : public base::SkyObject
{

public:

	/// <summary>
	/// ���K�\�����Ə����Ώە����������ɂ��ď������ʃ��X�g��Ԃ�
	/// </summary>
	virtual void Execute( const skyString *pRegEx , const skyString *pSrc , RegResultList &dest ) = 0;		

	/// <summary>
	/// ���K�\�����Ə����Ώە����������ɂ��ĊY������Ԃ�
	/// </summary>
	virtual u32 Execute( const skyString *pRegEx , const skyString *pSrc ) = 0;		

	/// <summary>
	/// �C���X�^���X��Ԃ��^����
	/// </summary>
	static void					CreateInstance		();
	static IRegEx				*GetInstance		();
	static void					DeleteInstance		();
};

//�C���X�^���X�A�N�Z�X������
#define IRegExCreate_()	sky::lib::regex::IRegEx::CreateInstance()
#define IRegEx_			sky::lib::regex::IRegEx::GetInstance()
#define IRegExDelete_()	sky::lib::regex::IRegEx::DeleteInstance()

//-----------------------------------< ���ێ��� >--------------------------------------------//

class AbsRegEx : public IRegEx
{
};

//-------------------------------------< ���� >---------------------------------------------//

#ifdef SKYLIB_COMPILER_VC10

class CRegExWin32 : public AbsRegEx
{

friend class IRegEx;

public:

	/// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
	/// </summary>
	void Execute	( const skyString *pRegEx , const skyString *pSrc , RegResultList &dest );
	u32  Execute	( const skyString *pRegEx , const skyString *pSrc );


	//�V���O���g���@�\��t��
	friend class Singleton< CRegExWin32 >;
	static Singleton< CRegExWin32 > Singleton;

private:

	//�e�@�\����
	CRegExWin32(){};												//�R���X�g���N�^
	CRegExWin32( const CRegExWin32& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CRegExWin32(){};										//�f�X�g���N�^
	CRegExWin32 &operator=(const CRegExWin32& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#endif

} } }
