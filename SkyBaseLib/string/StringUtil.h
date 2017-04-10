#pragma once

namespace sky { namespace lib { namespace string {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �����񏈗��Q
/// inline�����Ă��悩������ł����A���̃��C�u�����Ƀ����N���ł��Ȃ��Ȃ�̂�static�ɂ��܂����B
/// �v���b�g�t�H�[�����̍��͊֐�����#define�ŋ�؂��ĉ������܂��B
/// ���ǂ��̂ق����H�����Ⴍ�Ď��s���x���o��B�����\�[�X�͉����Ȃ�B
/// Unicode�łƃ}���`�o�C�g�ł̗�����p�ӂ��Ă���̂́AUnicode�v���W�F�N�g�ł��}���`�o�C�g�ɂ����Ή�
/// ���Ă��Ȃ��֐����ɑΉ����邽�߂ł��B_t�n�̏����Ȃ痼������Ă����̂ŕ��ʂ��ꂾ���ł�����ł����B
/// </summary>
class CStringUtil : public base::SkyObject
{

public:

	/// <summary>
	/// ������̃R�s�[
	/// </summary>
	static skyWString* Copy( out__ skyWString *pDest , u32 destLen , in__ const skyWString *pSrc );
	static skyMString* Copy( out__ skyMString *pDest , u32 destLen , in__ const skyMString *pSrc );

	/// <summary>
	/// ������̒������擾����(�������j
	/// </summary>
	static u32 Len( in__ const skyWString *pStr );
	static u32 Len( in__ const skyMString *pStr );

	/// <summary>
	/// ������̒������擾����(�o�C�g��+�I�[�q�pbyte(char�Ȃ�1,wchar�Ȃ�2)������Ɋ��Z�j
	/// </summary>
	static u32 LenByte( in__ const skyWString *pStr );
	static u32 LenByte( in__ const skyMString *pStr );

	/// <summary>
	/// �w��̕����񂪂Q�o�C�g�����������ꍇ�̒������擾����
	/// </summary>
	static u32 LenChar( in__ const skyWString *pStr );

	/// <summary>
	/// ��������r����
	/// </summary>
	static skyBool Comp( in__ const skyWString *pStrA, in__ const skyWString *pStrB );
	static skyBool Comp( in__ const skyMString *pStrA, in__ const skyMString *pStrB );
	static skyBool NComp( in__ const skyWString *pStrA, in__ const skyWString *pStrB , u32 num );
	static skyBool NComp( in__ const skyMString *pStrA, in__ const skyMString *pStrB , u32 num );

	/// <summary>
	/// pSrc�������pDest������Ɍ�������
	/// </summary>
	static skyWString* Cat( inout__ skyWString *pDest , in__ const skyWString *pSrc );
	static skyMString* Cat( skyMString *pDest , u32 destByte , const skyMString *pSrc );

	/// <summary>
	/// pSrc�������pDest������Ɍ�������(�ԂɃX�y�[�X������j
	/// </summary>
	static skyWString* CatSpace( inout__ skyWString *pDest , in__ const skyWString *pSrc );
	static skyMString* CatSpace( inout__ skyMString *pDest , u32 destByte , in__ const skyMString *pSrc );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̎��傪�ŏ��Ɍ��������ʒu�̃|�C���^�[��Ԃ��B������Ȃ������Ƃ�NULL��Ԃ�
	/// </summary>
	static skyWString* Strchr( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyMString* Strchr( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̎��傪�ŏ��Ɍ��������ʒu�܂ł̋�����Ԃ��B������Ȃ����skyFalse��Ԃ��A�Ō�܂ł̋�����Ԃ��B
	/// </summary>
	static skyBool StrchrDistance( const in__ skyWString *pSrcString , in__ const skyWString pMark , u32 &size );
	static skyBool StrchrDistance( const in__ skyMString *pSrcString , in__ const skyMString pMark , u32 &size );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̎��傪���݂��邩�ǂ����`�F�b�N����
	/// </summary>
	static skyBool IsStrchrIn( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyBool IsStrchrIn( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̎��傪�Ō�Ɍ��������ʒu�̃|�C���^�[��Ԃ�
	/// </summary>
	static skyWString* Strrchr( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyMString* Strrchr( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̎��傪�Ō�Ɍ��������ʒu�̃|�C���^�[��Ԃ�
	/// </summary>
	static skyWString *Sprintf( inout__ skyWString *pBuf , s32 bufSize , in__ const skyWString *format , ... );
	static skyMString *Sprintf( inout__ skyMString *pBuf , s32 bufSize , in__ const skyMString *format , ... );

	/// <summary>
	/// ������u�����܂��B
	/// </summary>
	static skyBool	Replace( const skyString *src , const skyString *srcKeyWord , const skyString *srcReplaceWord , skyString *destBuf , u32 destBufSize , const skyString *start = _T( "<##" ) , const skyString *end = _T( "#>" ) );

	/// <summary>
	/// �啶�����������ɂ��ĕԂ�
	/// </summary>
	static void ConvToLower( inout__ skyWString *pStr );
	static void ConvToLower( inout__ skyMString *pStr );

	/// <summary>
	/// �P�U�i���������u32�^�ɕϊ����ĕԂ��B
	/// </summary>
	static u32 ConvHexStrTou32( const skyWString *pSrc );
	static u32 ConvHexStrTou32( const skyMString *pSrc );

	/// <summary>
	/// ��������啶���ɂ��ĕԂ�
	/// </summary>
	static void ConvToUpper( inout__ skyWString *pStr );
	static void ConvToUpper( inout__ skyMString *pStr );

	/// <summary>
	/// �������s32�^�ɕϊ����ĕԂ��B�ϊ��Ɏ��s������INT_MAX��Ԃ��܂�
	/// </summary>
	static s32 ConvStringTos32( in__ const skyWString *pSrc , s32 baseNum = 10 );
	static s32 ConvStringTos32( in__ const skyMString *pSrc , s32 baseNum = 10 );

	/// <summary>
	/// �������u32�^�ɕϊ����ĕԂ��B�ϊ��Ɏ��s������UINT_MAX��Ԃ��܂�
	/// </summary>
	static u32 ConvStringTou32( in__ const skyWString *pSrc , s32 baseNum = 10 );
	static u32 ConvStringTou32( in__ const skyMString *pSrc , s32 baseNum = 10 );

	/// <summary>
	/// �������f32�^�ɕϊ����ĕԂ��B�ϊ��Ɏ��s������FLT_MAX��Ԃ��܂�
	/// </summary>
	static f32 ConvStringTof32( in__ const skyWString *pSrc );
	static f32 ConvStringTof32( in__ const skyMString *pSrc );

	/// <summary>
	/// �������d64�^�ɕϊ����ĕԂ��B�ϊ��Ɏ��s������DBL_MAX��Ԃ��܂�
	/// </summary>
	static d64 ConvStringTod64( in__ const skyWString *pSrc );
	static d64 ConvStringTod64( in__ const skyMString *pSrc );

	/// <summary>
	/// �������skyBool�^�ɕϊ����ĕԂ��B�ϊ��Ɏ��s������skyFalse��Ԃ��܂�
	/// </summary>
	static skyBool ConvStringToBool( in__ const skyWString *pSrc );
	static skyBool ConvStringToBool( in__ const skyMString *pSrc );

	/// <summary>
	/// �����𕶎��ɂ��ĕԂ�
	/// </summary>
	static const skyString *ConvNumToString( s16 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( u16 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( s32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( u32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( f32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( d64 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( skyBool value , skyString *pBuf , s32 bufSize );

	/// <summary>
	/// �����̕���������ϐ��W�J����������ɒu�����ĕԂ��B
	/// </summary>
	static void ConvEnvValue( const in__ skyWString *pSrc , out__ skyWString *pDest , u32 destSize );
	static void ConvEnvValue( const in__ skyMString *pSrc , out__ skyMString *pDest , u32 destSize );

	/// <summary>
	/// �w��̕����񒆂̎w��̕������w��̕����ɒu��������
	/// </summary>
	static void Replace( inout__ skyWString *pStr , const skyWString *pSrc , const skyWString *pDest );
	static void Replace( inout__ skyMString *pStr , const skyMString *pSrc , const skyMString *pDest );

	/// <summary>
	/// �}���`�o�C�g����������C�h������ɕϊ�����
	/// ���ۂ��̂悤�ȕ����ϊ����v���O������ōs���̂���ۂǂ̂��Ƃ��Ȃ����薳�����ƁB
	/// ���̊֐��̓V���v���ȕ��댯�ł��B�d�g�݂𗝉�������Ŏg�p���ĉ������B
	/// </summary>
	static u32 ConvMCharToWCharLen( const skyMString *pSrc );
	static u32 ConvMCharToWCharSize( const skyMString *pSrc );
	static u32 ConvMCharToWChar( in__ const skyMString *pSrc , inout__ skyWString *pDest , in__ u32 destSize );

	/// <summary>
	/// ���C�h��������}���`�o�C�g������ɕϊ�����
	/// ���ۂ��̂悤�ȕ����ϊ����v���O������ōs���̂���ۂǂ̂��Ƃ��Ȃ����薳�����ƁB
	/// ���̊֐��̓V���v���ȕ��댯�ł��B�d�g�݂𗝉�������Ŏg�p���ĉ������B
	/// </summary>
	static u32 ConvWCharToMCharSize( const skyWString *pSrc );
	static u32 ConvWCharToMChar( in__ const skyWString *pSrc , inout__ skyMString *pDest , in__ u32 destSize );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̕����񂪑��݂��邩�`�F�b�N���s��
	/// </summary>
	static skyBool IsStrstrIn( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
	static skyBool IsStrstrIn( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// �w��̕�����ɂ����Ďw��̕����񂪍ŏ��Ɍ����A�h���X��Ԃ�
	/// </summary>
    static const skyWString *Strstr( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
    static const skyMString *Strstr( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// �w��̕�����ɂ����āA�w��̕����񂪍ŏ��Ɍ��������ʒu�܂ł̋�����Ԃ��B������Ȃ���΍Ō�܂ł̋�����Ԃ��B
	/// </summary>
	static u32 StrstrDistance( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
	static u32 StrstrDistance( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// �������ϊ����A�o�͂���B�ϊ��㕶����̓q�[�v���g�p���Ă���̂ŕK��������邱��
	/// </summary>
	static skyMString	*CreateMString( const skyWString *pSrc );
	static skyMString	*CreateMString( const skyMString *pSrc );
	static void			DeleteMString( skyMString *pSrc );

	static skyWString	*CreateWString( const skyWString *pSrc );
	static skyWString	*CreateWString( const skyMString *pSrc );
	static void			DeleteWString( skyWString *pSrc );

	static skyString	*CreateString( const skyWString *pSrc );
	static skyString	*CreateString( const skyMString *pSrc );
	static void			DeleteString( skyString *pSrc );

	/// <summary>
	/// �v���O�����̕����R�[�h��Unicode���ǂ������肷��
	/// </summary>
	static skyBool IsProgramUnicode			();

	/// <summary>
	/// ������0����9�܂ł̂����ꂩ�̒l�ł���΃[���łȂ��l��Ԃ�isdigit�̋@�\�𗘗p���ĂP�����𐔎����ǂ������肷��
	/// </summary>
	static skyBool      IsNum( const skyString c );

	/// <summary>
	/// �����𐔎��ɕϊ�����
	/// </summary>
	static s32          ConvertNum( const skyString c );

	/// <summary>
	/// �w��̃f�[�^�̖����ɏI�[�q��t���w��̃o�b�t�@�ɃR�s�[����
	/// </summary>
	static void         CopyAdde0( skyWString *pDest , u32 destLen , const skyWString *src , u32 srcLen );
	static void         CopyAdde0( skyMString *pDest , u32 destLen , const skyMString *src , u32 srcLen );

private:

	//�e�@�\����
	CStringUtil(){};													//�R���X�g���N�^
	CStringUtil( const CStringUtil& ){};								//�R�s�[�R���X�g���N�^
	virtual ~CStringUtil(){};											//�f�X�g���N�^
	CStringUtil &operator=(const CStringUtil& ){ return *this; };		//������Z�q�I�[�o�[���[�h
};

#define CStringUtil_			sky::lib::string::CStringUtil

} } }

#include "StringUtil.inl"