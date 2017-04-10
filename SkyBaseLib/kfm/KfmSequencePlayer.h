#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�V�[�P���X�Đ��@
/// �V�[�P���X�̍Đ��v�����L���[�ŕۑ��������Đ����Ă����B
/// �Đ����̃V�[�P���X�Ǝ��ɍĐ�����V�[�P���X�Ńu�����h�����邱�Ƃ��ł���B
/// </summary>
template < class T >
class CKfmSequencePlayer : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CKfmSequencePlayer(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CKfmSequencePlayer();

	/// <summary>
	/// �V�[�P���X�Đ��v��������(�Đ��L���[��Push����j
	/// startIntvl    : �J�n�҂�����
	/// speed         : �Đ��X�s�[�h(1.0f�Œʏ�X�s�[�h�j�����蓙���ł���B
	/// startSeqTime  : �Đ��J�n�V�[�P���X���ԁB(0.0f�ōŏ�����Đ��j�V�[�P���X�̓r������Đ������ł���B
	/// </summary>
	inline void PlaySequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );
	inline void PlayReverseSequenceRequest( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );

	/// <summary>
	/// �V�[�P���X���Đ�����i���ݍĐ����̃L���[���N���A���A�v���̃V�[�P���X���Đ�����j
	/// </summary>
	inline void PlaySequence( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );
	inline void PlayReverseSequence( const CKfmSequence< T >* spSequence , updateTime blendTime = 0.0f , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse );

	/// <summary>
	/// �O���[�o���V�[�P���X�̍Đ��i�O���[�o���V�[�P���X�Đ����X�g�ɓo�^����A�L���[�Đ�����Ă���V�[�P���X�ƕ��s�ōĐ������j
	/// </summary>
	inline CKfmSequencePlayer< T >* PlayGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = 0.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse ); 
	inline CKfmSequencePlayer< T >* PlayReverseGlobalSequence( const CKfmSequence< T >* spSequence , updateTime startIntvl = 0.0f , updateTime speed = 1.0f , updateTime startSeqTime = -1.0f , ePlayEndType eEndType = ePlayEndType_End , skyBool isSequenceDelete = skyFalse ); 

	/// <summary>
	/// �V�[�P���X�ꎞ��~
	/// </summary>
	inline void StopCurrentSequence();
	inline void StopGlobalSequence();
	inline void StopSequenceAll();

	/// <summary>
	/// �L���[�ɗ��܂��Ă��鎟�̃V�[�P���X�Ɉڍs���߁i�J��Ԃ��A�����Ȃǂ̍Đ����I���Ɍ����킹��j
	/// </summary>
	inline void NextCurrentSequence();
	inline void NextGlobalSequence();
	inline void NextAllSequence();

	/// <summary>
	/// �V�[�P���X�L�����Z��
	/// </summary>
	inline void CancelCurrentSequence();
	inline void CancelGlobalSequence();
	inline void CancelSequenceAll();

	/// <summary>
	/// �V�[�P���X�ꎞ��~�ĊJ
	/// </summary>
	inline void ReStartCurrentSequence(); 
	inline void ReStartGlobalSequence(); 
	inline void ReStartSequenceAll(); 

	/// <summary>
	/// �Đ�
	/// �߂�l�ɍĐ������V�[�P���X��Ԃ��B�Đ����Ă��Ȃ��ꍇ��skyNull���A��
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
	/// �V�[�P���X�̐ݒ�
	/// </summary>
	inline void SetSequence( const CKfmSequence< T >* spSequence );

	/// <summary>
	/// �E�F�C�g��ݒ�(wrrning!! ���Ӂj�����ōs���鎟�̃V�[�P���X�Ƃ̃u�����h�����ɉe����^���܂��B�j
	/// </summary>
	inline void SetWeights( f32 weights );

	/// <summary>
	/// ���ݍĐ����̃V�[�P���X���擾����
	/// </summary>
	const CKfmSequence< T >* GetCurrentSequence() const;

	/// <summary>
	/// ���ݍĐ����̃V�[�P���X�̎��Ԃ��擾����
	/// </summary>
	updateTime GetCurrentSequenceTime() const;

	/// <summary>
	/// �V�[�P���X�Đ����
	/// </summary>
	struct CSequencePlayInfo
	{
		virtual ~CSequencePlayInfo()
		{
			if ( m_IsSequenceDelete == skyTrue )
			{
				SAFE_DELETE( m_spSequencePtr );
			}
		}

		//�����Ǘ����
		ePlayState                   m_eState;     //��{�X�e�[�g
		updateTime                   m_NowTime;

		//�Đ��������
		updateTime                   m_StartIntvl;
		updateTime                   m_MaxTime;
		updateTime                   m_MinTime;
		updateTime                   m_Speed;
		ePlayEndType                 m_EndType;
		updateTime                   m_BlendTime;
		eInterpolateType             m_BlendInterpType;

		//�V�[�P���X
		CKfmSequence< T >*           m_spSequencePtr;
		skyBool                      m_IsSequenceDelete;

		/// <summary>
		/// ������Z�q�I�[�o�[���C�h
		/// </summary>
		virtual CSequencePlayInfo& operator =( const CSequencePlayInfo& src )
		{
			m_eState           = src.m_eState;
			m_NowTime          = src.m_NowTime;
			m_MaxTime          = src.m_MaxTime;
			m_MinTime          = src.m_MinTime;
			m_StartIntvl       = src.m_StartIntvl;
			m_Speed            = src.m_Speed;
			m_EndType          = src.m_EndType;
			m_BlendTime        = src.m_BlendTime;
			m_BlendInterpType  = src.m_BlendInterpType;
			m_IsSequenceDelete = skyFalse; //�R�s�[��ł͍폜���Ȃ��B
			m_spSequencePtr    = src.m_spSequencePtr;

			return *this;
		}

	};
	typedef CFifo< CSequencePlayInfo >       CSequencePlayInfoFifo;
	typedef CList< u32 , CSequencePlayInfo > CSequencePlayInfoList;

	/// <summary>
	/// �Đ��ς݂̃V�[�P���X���L���[�ɒ��߂Ă������H�i������g�p���ē����V�[�P���X���Đ����邱�Ƃ�A�t���ԍĐ����ł���j
	/// </summary>
	inline void IsSavePlayedSequence( skyBool isSaveExec );

	/// <summary>
	/// �Đ��ς݃V�[�P���X�L���[�̎擾
    /// �L���[�̍ŏ��̃V�[�P���X�͕ۑ����J�n����������Đ����I��������ԍŏ��̃V�[�P���X�������Ă���
	/// �܂�L���[�����Ԃɏ��Đ�����Ɠ��������̍Č����ł���B�L���[�𖖔����珇�Ԃɋt�Đ�����ƍĐ�����������
	/// �t�Đ��ɂȂ�
	/// </summary>
	inline CSequencePlayInfoFifo &GetPlayedSequenceFifo();

private:

	/// <summary>
	/// �V�[�P���X�Đ��v���L���[�i�o�^���ꂽ���ɍĐ�����Ă����j
	/// </summary>
	CSequencePlayInfoFifo              m_SequencePlayInfoFifo;

	/// <summary>
	/// �Đ��ς݃V�[�P���X�ۑ��t���O
	/// </summary>
	skyBool                            m_IsSaveExec;

	/// <summary>
	/// �Đ��ς݃V�[�P���X���L���[
	/// </summary>
	CSequencePlayInfoFifo              m_SequencePlayedInfoFifo;

	/// <summary>
	/// ���ݍĐ����V�[�P���X���
	/// </summary>
	CSequencePlayInfo                  m_CurrentPlaySequenceInfo;

	/// <summary>
	/// ���ݍĐ����V�[�P���X�̃u�����h���V�[�P���X���
	/// </summary>
	CSequencePlayInfo                  m_CurrentPlayBlendSequenceInfo;

	/// <summary>
	/// �O���[�o���V�[�P���X
	/// </summary>
	typedef CList< u32 , CKfmSequencePlayer< T >* >  CKfmSequencePlayerPtrList;
	CKfmSequencePlayerPtrList          m_GlobalSequencePlayList;

	/// <summary>
	/// ���ݍĐ����V�[�P���X�̃E�F�C�g�l( 1.0f - m_CurrentPlaySequenceBlendValue �Ńu�����h�V�[�P���X�� )
	/// </summary>
	f32                                m_CurrentPlaySequenceWeights;

	/// <summary>
	/// �Đ����̐ݒ�
	/// </summary>
	inline void SetSequencePlayInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete ); 

	/// <summary>
	/// �t�Đ����̐ݒ�
	/// </summary>
	inline void SetSequencePlayReverseInfo_( CSequencePlayInfo &dest , const CKfmSequence< T >* spSequence , updateTime blendTime , updateTime startIntvl , updateTime speed , updateTime startSeqTime , ePlayEndType eEndType , skyBool isSequenceDelete ); 
};

} } }

#include "KfmSequencePlayer.inl"