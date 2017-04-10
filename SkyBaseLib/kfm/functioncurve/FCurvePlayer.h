#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�Đ��@
/// �t�@���N�V�����J�[�u�̍Đ��v�����L���[�ŕۑ��������Đ����Ă����B
/// �Đ����̃t�@���N�V�����J�[�u�Ǝ��ɍĐ�����t�@���N�V�����J�[�u�Ńu�����h�����邱�Ƃ��ł���B
/// </summary>
template < class T >
class CFCurvePlayer : public base::SkyObject
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CFCurvePlayer(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFCurvePlayer(){};

	/// <summary>
	/// �V�[�P���X�Đ��v��������(�Đ��L���[��Push����j
	/// startIntvl    : �J�n�҂�����
	/// speed         : �Đ��X�s�[�h(1.0f�Œʏ�X�s�[�h�j�����蓙���ł���B
	/// startSeqTime  : �Đ��J�n�V�[�P���X���ԁB(0.0f�ōŏ�����Đ��j�V�[�P���X�̓r������Đ������ł���B
	/// </summary>
	inline void PlayFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End );
	inline void PlayReverseFCurveRequest( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End );

	/// <summary>
	/// �V�[�P���X���Đ�����i���ݍĐ����̃L���[���N���A���A�v���̃V�[�P���X���Đ�����j
	/// </summary>
	inline void PlayFCurve( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End );
	inline void PlayReverseFCurve( const CFCurve< T >* pFCurve , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End );

	/// <summary>
	/// �O���[�o���V�[�P���X�̍Đ��i�O���[�o���V�[�P���X�Đ����X�g�ɓo�^����A�L���[�Đ�����Ă���V�[�P���X�ƕ��s�ōĐ������j
	/// </summary>
	inline CKfmSequencePlayer< T >* PlayGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End ); 
	inline CKfmSequencePlayer< T >* PlayReverseGlobalFCurve( const CFCurve< T >* pFCurve , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End ); 

	/// <summary>
	/// �V�[�P���X�ꎞ��~
	/// </summary>
	inline void StopCurrentFCurve();
	inline void StopGlobalFCurve();
	inline void StopFCurveAll();

	/// <summary>
	/// �L���[�ɗ��܂��Ă��鎟�̃V�[�P���X�Ɉڍs���߁i�J��Ԃ��A�����Ȃǂ̍Đ����I���Ɍ����킹��j
	/// </summary>
	inline void NextCurrentFCurve();
	inline void NextGlobalFCurve();
	inline void NextAllFCurve();

	/// <summary>
	/// �V�[�P���X�L�����Z��
	/// </summary>
	inline void CancelCurrentFCurve();
	inline void CancelGlobalFCurve();
	inline void CancelFCurveAll();

	/// <summary>
	/// �V�[�P���X�ꎞ��~�ĊJ
	/// </summary>
	inline void ReStartCurrentFCurve(); 
	inline void ReStartGlobalFCurve(); 
	inline void ReStartFCurveAll(); 

	/// <summary>
	/// �Đ�(�����͐i�߂鎞�ԁj
	/// </summary>
	inline const CKfmSequence< T >* Play( updateTime timeOffset );

///---�Đ��p�����[�^�[�r���ύX

	/// <summary>
	/// �Đ��X�s�[�h�ݒ�
	/// </summary>
	inline void SetSpeed( updateTime speed );

	/// <summary>
	/// �V�[�P���X���Ԑݒ�
	/// </summary>
	inline void SetNowTime( updateTime time );

	/// <summary>
	/// �Đ���Ԑݒ�
	/// </summary>
	inline void SetPlayState( ePlayState state );

	/// <summary>
	/// �Đ��ŏ����Ԑݒ�
	/// </summary>
	inline void SetMiniTime( updateTime time );

	/// <summary>
	/// �Đ��ő厞�Ԑݒ�
	/// </summary>
	inline void SetMaxTime( updateTime time );

	/// <summary>
	/// �Đ��I����ʐݒ�
	/// </summary>
	inline void SetEndType( ePlayEndType eEndType );

	/// <summary>
	/// �E�F�C�g��ݒ�(wrrning!! ���Ӂj�����ōs���鎟�̃V�[�P���X�Ƃ̃u�����h�����ɉe����^���܂��B�j
	/// </summary>
	inline void SetWeights( f32 weights );

	/// <summary>
	/// �V�l�Z�o���C�x���g�擾
	/// </summary>
	inline delegate::CDelegateHandler &GetColValueEventHndl();

private:

	/// <summary>
	/// �V�l�Z�o���C�x���g�n���h��
	/// </summary>
	delegate::CDelegateHandler  m_ColValueEventHndl;

	/// <summary>
	/// ���g�̓V�[�P���X�v���C���[
	/// </summary>
	CKfmSequencePlayer< T > m_SeqPlayer;
};

typedef CFCurvePlayer< dectype > DecTypeFCurvePlayer;

} } }


#include "FCurvePlayer.inl"