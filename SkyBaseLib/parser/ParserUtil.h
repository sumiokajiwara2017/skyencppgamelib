#pragma once

namespace sky { namespace lib { namespace parser {

static const u32 TOKEN_NUM_MAX    = 64; //�g�[�N���̍ő吔
static const u32 TOKEN_LENGTH_MAX = 64; //�P�g�[�N���̍ő咷��
static const u32 KEY_LENGHT_MAX   = 32;
static const u32 VALUE_LENGHT_MAX = TOKEN_LENGTH_MAX - KEY_LENGHT_MAX;


/// <summary>
/// �������͕֗������Q
/// </summary>
class CParserUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �������̓p�^�[���O
	/// �w a b c d e �x���̕������
	/// �w a , b , c , d �x�Ƃ���CString�^�̃��X�g�ɂ���B
	/// </summary>
	static void ParseStringPtn0( const skyWString *src , string::CStringList &destlist );
	static void ParseStringPtn0( const skyMString *src , string::CStringList &destlist );

	/// <summary>
	/// �������̓p�^�[���P
	/// �w hoge011_mog99_g100m_ll99m �x���̕������
	/// �w hoge=1.0f , mog=99.0f , g=-100.0f , ll=-99.0f �x�Ƃ���d64�^�̃}�b�v�ɂ���B�����̖�����m���t���Ă�����}�C�i�X
	/// </summary>
	static void ParseStringPtn1( const skyWString *src , CValues32Map &destmap );
	static void ParseStringPtn1( const skyMString *src , CValues32Map &destmap );

	/// <summary>
	/// �������̓p�^�[���Q
	/// �w _bit0100_ �x���̕������
	/// �w list[0]=skyTrue , list[1]=skyTrue , list[2]=skyFalse , list[3]=skyFalse �x�Ƃ���skyBool�^�̃��X�g�ɂ���
	/// �wbit�x�̕�����keyWord�ŔC�ӂ̕�������w��ł���
	/// </summary>
	static void ParseStringPtn2( const skyWString *src , const skyWString *keyWord , CValueBoolList &destlist );
	static void ParseStringPtn2( const skyMString *src , const skyMString *keyWord , CValueBoolList &destlist );

	/// <summary>
	/// �������̓p�^�[���R
	/// �w _flg9185_ �x���̕������
	/// �w list[0]=9 , list[1]=1 , list[2]=8 , list[3]=5 �x�Ƃ���u8�^�̃��X�g�ɂ���
	/// �wflg�x�̕�����keyWord�ŔC�ӂ̕�������w��ł���
	/// </summary>
	static void ParseStringPtn3( const skyWString *src , const skyWString *keyWord , CValueu8List &destlist );
	static void ParseStringPtn3( const skyMString *src , const skyMString *keyWord , CValueu8List &destlist );

};

#define CParserUtil_			sky::lib::parser::CParserUtil

} } }