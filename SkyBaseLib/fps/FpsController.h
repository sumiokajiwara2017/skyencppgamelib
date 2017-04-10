#pragma once

namespace sky { namespace lib { namespace fps {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// �e�o�r�Ǘ�
/// </summary>
class CFpsController : public base::SkyObject
{
public:

	/// <summary>
	/// �X�V�t���[�����[�g�ݒ�^�擾
	/// </summary>
	inline void		    SetFrameRate( u16 frameRate = DEFAULT_FPS );
	inline u16		    GetFrameRate();

	/// <summary>
	/// 1�t���[��������̎��ԁi�~���b�j
	/// </summary>
	inline updateTime	GetOneFrameTime();

	/// <summary>
	/// �J�n����(�~���b�j�ݒ�^�擾
	/// </summary>
	inline void			SetStartTime( u32 startTime );
	inline updateTime	GetStartTime();

	/// <summary>
	/// �o�߃t���[���擾/�J�E���g�t�o
	/// </summary>
	inline void		    IncElapsedFrame();

	/// <summary>
	/// �t���[���J�n
	/// </summary>
	inline void		    StartFrame();

	/// <summary>
	/// �܂��t���[���I���\�莞�Ԃ𒴂��Ă��Ȃ����H�i�����������Ă��Ȃ����H�jskyTrue:�z���Ă��Ȃ��i���Ԃ��]���Ă���jskyFalse:�z���Ă���i���̃t���[���ɂ����Ȃ��Ă͂����Ȃ��j
	/// </summary>
	inline skyBool      IsEndTimeStillRate();

	/// <summary>
	/// �t���[���I���i�X�V����������ꍇ�����ő҂j
	/// </summary>
	inline void	        EndFrame();

	/// <summary>
	/// �O��̃t���[���J�n���Ԃ��獡��̃t���[���J�n���ԂƂ̌o�ߎ���
	/// </summary>
	inline updateTime   GetPassageTime_Msec() const;      //�~���Z�b�N(1/1000�b)
	inline updateTime   GetPassageTime_Second() const;    //�b

	/// <summary>
	/// �A�v���P�[�V�����J�n���猻�݂܂ł̌o�ߎ���
	/// </summary>
	inline updateTime   GetPassageAllTime_Msec() const;   //�~���Z�b�N(1/1000�b)
	inline updateTime   GetPassageAllTime_Second() const; //�b

	/// <summary>
	/// �A�v���P�[�V�����J�n���猻�݂܂ł̌o�߃t���[����
	/// </summary>
	inline u32		    GetElapsedFrame() const;

	/// <summary>
	/// �w��t���[�����̂P�t���[��������̕��ώ��Ԃ�Ԃ�
	/// </summary>
	inline skyBool      GetAverageOneFrameTime( u32 colFrameNum , updateTime &destOneFrameTime );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline              CFpsController();

private:

	/// <summary>
	/// �A�v���P�[�V�����J�n����
	/// </summary>
	updateTime m_StartAppTime;

	/// <summary>
	/// ���݂̃t���[�����[�g
	/// </summary>
	u16 m_FrameRate;

	/// <summary>
	/// �o�߃t���[����
	/// </summary>
	u32 m_ElapsedFrame;

	/// <summary>
	/// �O��J�n���ԁi�~���b�j
	/// </summary>
	updateTime m_BeforStartTime;

	/// <summary>
	/// �J�n���ԁi�~���b�j
	/// </summary>
	updateTime m_StartTime;

	/// <summary>
	/// �I�����ԁi�~���b�j
	/// </summary>
	updateTime m_EndTime;

	/// <summary>
	/// �P�t���[��������̎���
	/// </summary>
	updateTime m_OneFrameTime;

	/// <summary>
	/// �P�t���[��������̕��ώ��Ԃ��o�����߂̌v���t���[����
	/// </summary>
	u32 m_ColAverageFrameNum;

	/// <summary>
	/// �P�t���[��������̕��ώ��Ԃ��o�����߂̍��v�l�i�[�v�Z�o�b�t�@
	/// </summary>
	updateTime m_ColAverageSumTimeBuffer;

	/// <summary>
	/// �P�t���[��������̕��ώ��Ԃ��o�����߂̉��Z�t���[���J�E���^�[
	/// </summary>
	u32 m_ColAverageFrameCounter;
};

} } }

#include "FpsController.inl"