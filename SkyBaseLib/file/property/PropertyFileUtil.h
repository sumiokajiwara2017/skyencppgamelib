#pragma once

namespace sky { namespace lib { namespace file {

/// <summary>
/// �v���p�e�B�t�@�C������N���X
/// �v���p�e�B�t�@�C���̃t�H�[�}�b�g�͉��L�̒ʂ�
///---------------< propertyfile >---------------
/// #�R�����g�̓V���[�v����
/// key1=value1						//���l�͊�{�L�[�ƒl�ŏ����B
/// key2=value1,value2,value3		//���l�͕������Ă�B
///
/// key1=value2						//���L�[�����Ԃ�ƃG���[�ɂȂ�B
///
///
/// key3=value						//���󔒍s�͉��s����ł��\��Ȃ��B
/// #key4=value						//������͂������R�����g�ɂȂ�
///
/// �ق��ق�						//���R�����g�ł��Ȃ��w�L�[=�l�x�̃t�H�[�}�b�g�ł��Ȃ����̂̓G���[�ɂȂ�B
///
///---------------< propertyfile >---------------
/// < Memo >
/// ��1�o�C�i���ۑ�����ۂɃL�[��Hash�l�ɂ��Ă���ۑ����悤�ƍl�������A���̕����񂪎�����ƐF�X�f�����b�g������̂ň�U�ۗ��ɂ���
/// </summary>
class CPropertyFileUtil : base::SkyObject
{

public:

	/// <summary>
	/// �v���p�e�B�t�@�C�����[�h
	/// </summary>
	static void Load( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest );

	/// <summary>
	/// �v���p�e�B�t�@�C���R���p�C���i�o�C�i��������j
	/// </summary>
	static void Compile( stream::IStreamPtr &src ,  stream::IStreamPtr &dest );

	/// <summary>
	/// �L�[�ƃC���f�b�N�X�Œl�𕶎��Ƃ��Ď擾
	/// </summary>
	static const hash::CHashStringPtr &GetString( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// �L�[�ƃC���f�b�N�X�Œl�𐮐��l�Ƃ��Ď擾
	/// </summary>
	static s32 Gets32( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// �L�[�ƃC���f�b�N�X�Œl�������l�Ƃ��Ď擾
	/// </summary>
	static d64 Getd64( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// �v���p�e�B�t�@�C���̓��e���_���v����
	/// </summary>
	static void Dump( hash::CHashStringPtrListPtrHash &src , stream::IStreamPtr &dest );

};

#define CPropertyFileUtil_					sky::lib::file::CPropertyFileUtil

} } }