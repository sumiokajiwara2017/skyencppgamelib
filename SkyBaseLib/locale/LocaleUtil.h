#pragma once

namespace sky { namespace lib { namespace locale {

/// <summary>
/// �萔
/// </summary>
enum Country
{
	Country_English = 0,	//�p��
	Country_French,			//�t�����X��
	Country_Japanese,		//���{
	Country_Max,
};


//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���P�[���ݒ聕�擾
/// </summary>
/*
setlocale�֐��̎d�l

LC_ALL
�ȉ��Ɏ������ׂẴJ�e�S���B
LC_COLLATE
strcoll�A_stricoll�Awcscoll�A_wcsicoll�Astrxfrm�A_strncoll�A_strnicoll�A_wcsncoll�A_wcsnicoll�A����� wcsxfrm �̊e�֐��B
LC_CTYPE
���������֐� (isdigit�Aisxdigit�Ambstowcs�A����� mbtowc �̊e�֐��͏���)�B
LC_MONETARY
localeconv �֐�����Ԃ����ʉ݌`���̏��B
LC_NUMERIC
�����t���o�̓��[�`�� (printf �Ȃ�) �ƃf�[�^�ϊ����[�`���̏����_�����A����� localeconv ���Ԃ���ʉ݌`���̏��̏����_�����BLC_NUMERIC �́A�����_�����̑��ɁA��P�ʂ̌���؂�L���� localeconv �֐����Ԃ��O���[�v�����䕶������ݒ肵�܂��B
LC_TIME
strftime �֐��� wcsftime �֐��B

*/
class CLocaleUtil : public base::SkyObject
{
public:

	/// <summary>
	/// �������P�[����ݒ�
	/// </summary>
	static void SetCharacterLocale	( Country country );

	/// <summary>
	/// �ʉ݃��P�[����ݒ�
	/// </summary>
	static void SetMoneyLocale		( Country country );

	/// <summary>
	/// �������P�[�����擾
	/// </summary>
	static void SetNumberLocale		( Country country );

	/// <summary>
	/// ���ԃ��P�[�����擾
	/// </summary>
	static void SetTimeLocale			( Country country );

	/// <summary>
	/// ���ׂẴ��P�[��
	/// </summary>
	static void SetAllLocale			( Country country );

private:

	static const char *s_ccLocalTbl[ Country_Max ];

	//�e�@�\����
	CLocaleUtil(){};													//�R���X�g���N�^
	CLocaleUtil( const CLocaleUtil& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CLocaleUtil(){};											//�f�X�g���N�^
	CLocaleUtil &operator=(const CLocaleUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#define CLocaleUtil_							sky::lib::locale::CLocaleUtil

} } }