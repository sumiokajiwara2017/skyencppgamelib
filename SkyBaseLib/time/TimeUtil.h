#pragma once

namespace sky { namespace lib { namespace time {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���Ԍn�֗��֐��Q
/// </summary>
class CTimeUtil : public base::SkyObject
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
	/// ���ݎ��Ԏ擾�i�~���b�j
	/// </summary>
	static updateTime	GetNowTime();

	/// <summary>
	/// ���Ԓ�~�i�~���b�j
	/// </summary>
	static void Wait( updateTime waitTime );

private:

#ifdef SKYLIB_PLATFORM_WIN32
	/// <summary>
	/// ���ԏ��
	/// </summary>
	static TIMECAPS m_Caps;
#endif

	//�e�@�\����
	CTimeUtil(){};												//�R���X�g���N�^
	CTimeUtil( const CTimeUtil& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CTimeUtil(){};										//�f�X�g���N�^
	CTimeUtil &operator=(const CTimeUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

#define CTimeUtilInitialize_() sky::lib::time::CTimeUtil::Initialize()
#define CTimeUtil_             sky::lib::time::CTimeUtil
#define CTimeUtilDispose_()    sky::lib::time::CTimeUtil::Dispose()

} } }
