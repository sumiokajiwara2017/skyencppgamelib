#pragma once

namespace sky { namespace lib { namespace reload {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// �t�@�C���X�V���
/// </summary>
struct FileUpdateData : public base::SkyRefObject
{
	u32					        updateTime;
	hash::CHashStringPtr		filePath;
};
typedef SmartPointer< FileUpdateData >			FileUpdateDataPtr;
typedef CList< s32 , FileUpdateDataPtr >		FileUpdateDataPtrList;	

/// <summary>
/// �����[�h���N���X
/// �����[�h�Ƃ͊J�����������߂邽�߂ɁA���s���̍ă��[�h��ړI�Ƃ��Ă���B
/// �ă��[�h���K�v�ȉӏ��̓A�v���P�[�V�����ɂ���ĈقȂ�̂ŋ��ʉ��@�\�Ƃ��Ăł��邱�Ƃ͉��L
/// �@�����[�h�v����������t�@�C���̍X�V���Ԃ�S�����čX�V�������������[�h���Ƀ}�[�N������
/// �APreReLoadFunc����������Ă�
/// �B�����[�h����UnLoadFunc���ĂсALoadFunc���Ă�
/// �CPostReLoadFunc����������Ă�
/// �A�v���P�[�V�����͂��̋@�\���w���܂��x�g���Đݒ�t�@�C���A�摜�t�@�C�����X�V���ꂽ�玩���Ń����[�h�����悤�ɃQ�[������邱�ƁB
/// </summary>
class IReLoadObject : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �t�@�C���p�XHash��o�^����
	/// �����[�h�Ɋ֌W���邷�ׂẴt�@�C���p�X��n��
	/// </summary>
	virtual void			AddFilePath( const hash::CHashStringPtr &ptr ) = 0;

	/// <summary>
	/// �t�@�C���p�X���X�g�̎Q�Ƃ��擾����
	virtual FileUpdateDataPtrList	&GetFilePathList() = 0;

	/// <summary>
	/// ���[�h�֐��i�e�I�u�W�F�N�g�Ŏ������Ă��������j
	/// </summary>
	virtual void			Load() = 0;

	/// <summary>
	/// �A�����[�h�֐��i�K�v�Ȃ�Ίe�I�u�W�F�N�g�ŃI�[�o�[���C�h���Ă��������j
	/// </summary>
	virtual void			UnLoad(){};
};

class IReLoadObject;
typedef SmartPointer< IReLoadObject >	IReLoadObjectPtr;
typedef CList< u32 , IReLoadObjectPtr >	IReLoadObjectPtrList;

//-----------------------------------< ���ێ��� >-------------------------------------------//

class AbsReLoadObject : public IReLoadObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �����i���ێ����Ŏ�������ꍇ�͕K��virtual�Ő錾����B�����ŃI�[�o�[���C�h����邩������Ȃ�����j
	/// </summary>
	virtual void					AddFilePath( const hash::CHashStringPtr &ptr );
	virtual FileUpdateDataPtrList	&GetFilePathList(){ return m_FilePathInfoList; }

protected:

	/// <summary>
	/// �t�@�C����񃊃X�g
	/// </summary>
	FileUpdateDataPtrList	m_FilePathInfoList;
};

} } }