#pragma once

namespace sky { namespace lib { namespace reload {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �����[�h���Ǘ��N���X
/// </summary>
class CReLoadObjectManager : public base::SkyRefObject
{

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
	/// �����[�h���o�^
	/// </summary>
	static void Add( IReLoadObjectPtr &pObj );

	/// <summary>
	/// �����[�h���S�N���A
	/// </summary>
	static void Clear();

	/// <summary>
	/// �X�V
	/// reloadIntvl:�����[�h�`�F�b�N�Ԋu(�~���b�j
	/// </summary>
	static void Update( updateTime nowTime , updateTime reloadCheckIntvlTime = 2000.0 );

private:

	//�����[�h��񃊃X�g
	static IReLoadObjectPtrList* m_pReLoadInfoList;

	//�Ō�Ƀ����[�h�������t���[����
	static updateTime            m_LastReloadTime;

	//�e�@�\����
	CReLoadObjectManager(){};													    //�R���X�g���N�^
	CReLoadObjectManager( const CReLoadObjectManager& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CReLoadObjectManager(){};											    //�f�X�g���N�^
	CReLoadObjectManager &operator=(const CReLoadObjectManager& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

//�C���X�^���X�A�N�Z�X������
#define CReLoadObjectManagerInitialize_()       sky::lib::reload::CReLoadObjectManager::Initialize()
#define CReLoadObjectManager_                   sky::lib::reload::CReLoadObjectManager
#define CReLoadObjectManagerDispose_()          sky::lib::reload::CReLoadObjectManager::Dispose()

} } }

