#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�V�[�P���X�x�[�X
/// </summary>
class CKfmSequenceBase : public base::SkyObject 
{

public:

	/// <summary>
	/// ����ID�i�C�x���g���Ɉ����Ƃ��ēn���B�K�v���Ȃ��Ȃ�ݒ肵�Ȃ��Ă��悢�j
	/// </summary>
	s32 ID;

	/// <summary>
	/// �Đ��C�x���g( 0.0f���ɊJ�n�C�x���g�Am_TimeRange���ɏI���C�x���g�𔭐��j
	/// </summary>
	delegate::CDelegateHandler m_PlayEventHndl;

	/// <summary>
	/// ���ԕ����擾����
	/// </summary>
	updateTime GetTimeRange() const;

protected:

	/// <summary>
	/// �V�[�P���X����������܂ł̎���
	/// </summary>
	updateTime m_TimeRange;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CKfmSequenceBase();
};

/// <summary>
/// �t�@���N�V�����J�[�u�V�[�P���X
/// </summary>
template < class T >
class CKfmSequence : public CKfmSequenceBase
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CKfmSequence(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CKfmSequence();

	/// <summary>
	/// �t�@���N�V�����J�[�u��o�^����
	/// </summary>
	inline u32 AddFCurve( updateTime startTime , const CFCurve< T > *pFCurve );

	/// <summary>
	/// �t�@���N�V�����J�[�u��o�^��������
	/// </summary>
	inline void DeleteAll();

	/// <summary>
	/// �C���f�b�N�X�Ŏ擾
	/// </summary>
	inline const CFCurve< T > *GetFCurve( u32 index ) const;

	/// <summary>
	/// �t�@���N�V�����J�[�u���̎擾
	/// </summary>
	inline u32 GetFCurveNum() const;

	/// <summary>
	/// �Đ�����i�������z�̎����j
	/// </summary>
	inline void Play( updateTime time , f32 weight = 1.0f , skyBool isCallBackValue = skyTrue , const CKfmSequence< T > *pBlendSequence = skyNull );

private:

	/// <summary>
	/// �t�@���N�V�����J�[�u���
	/// </summary>
	struct CFcurveInfo
	{
		updateTime    m_StartTime;
		CFCurve< T >* m_pFCurvePtr;
	};
	typedef CList< u32 , CFcurveInfo > CFcurveInfoPtrList;

	/// <summary>
	/// �t�@���N�V�����J�[�u��񃊃X�g
	/// </summary>
	CFcurveInfoPtrList m_FCurveList;
};

} } }

#include "KfmSequence.inl"