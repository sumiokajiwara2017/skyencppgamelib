#pragma once

namespace sky { namespace lib { namespace hash {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// Hash�����Ǘ��N���X
/// ���ׂĂ�Hash���������̃}�l�[�W���[�o�R�Ő������邱�ƂŁA�C���X�^���X�̏d��������}�����܂��B
/// CHashString�̓L�[�̖��������Ȃ��̂ŁA�ǂݎ���p�Ȃ̂�1�̕����ɑ΂���C���X�^���X��1�ł����B
/// </summary>
class CHashStringManager : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize( u32 hashSize = C_S32( hash_HASH_STR_MAN_HASH_SIZE ) );

	/// <summary>
	/// ������
	/// </summary>
	static void Dispose();

	/// <summary>
	/// Hash�����쐬
	/// </summary>
	static hash::CHashStringPtr GetPtr( const skyString  *pKey , skyBool isNoCreateOn = skyFalse ); 
	static hash::CHashStringPtr Get( const skyWString *pKey , skyBool isNoCreateOn = skyFalse ); 
	static hash::CHashStringPtr Get( const skyMString *pKey , skyBool isNoCreateOn = skyFalse ); 

	/// <summary>
	/// Hash�����S�j��
	/// </summary>
	static void Clear();

	/// <summary>
	/// �}�l�[�W���[�����Q�Ƃ��Ă��Ȃ��l������
	/// </summary>
	static void GC();

};

} } }

//�C���X�^���X�A�N�Z�X������
#define CHashStringManagerInitialize_()		sky::lib::hash::CHashStringManager::Initialize()
#define CHashStringManager_					sky::lib::hash::CHashStringManager
#define CHashStringManagerDispose_()		sky::lib::hash::CHashStringManager::Dispose()

//���̏����͂��Ȃ菈�����ׂ�����܂��B�}�C�t���[���s�������ɂ͐�΂Ɏg�p���Ȃ��ł��������B
#define CHS( str )							CHashStringManager_::Get( str )
#define CHSC( str )							CHashStringManager_::Get( str , skyTrue ) //������̃o�b�t�@������Ŏ��Ȃ��B�A�h���X�����ێ��Bstatic const���Ŏw�肳��Ă��镶����͂������ł����B�����邱�Ƃ��Ȃ�����B
