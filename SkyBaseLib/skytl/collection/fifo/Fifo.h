#pragma once

namespace sky { namespace lib {

namespace fifo
{
	static const u32 DEFAULT_FIFO_SIZE	= 128;
}
using namespace fifo;

/// <summary>
/// �ėp�^�̂e�h�e�n�N���X�i���̃N���X�͑���ɂ͑Ή����Ă��܂���B�������悤�Ȏg���������Ȃ��ł��������j
/// </summary>
template< typename T , class ThreadingModel = thread::ThreadingModels::CSingleThread >
class CFifo : public IteratorIf< T > , public ThreadingModel
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFifo( s32 size = DEFAULT_FIFO_SIZE );

	/// <summary>
	/// �R�s�[�R���X�g���N�^�i���Ή��j
	/// </summary>
	inline CFifo( const CFifo& src ){ SKY_PANIC(); }

	/// <summary>
	/// ������Z�q�i���Ή��j
	/// </summary>
	inline CFifo& operator =( const CFifo& src ){ SKY_UNUSED_ARG( src ); SKY_PANIC(); return *this; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFifo();

	/// <summary>
	/// �e�h�e�n�ǉ�
	/// </summary>
	inline skyBool Push( const T &data );
	inline skyBool Push();
	inline skyBool IsFull();

	/// <summary>
	/// �w��̒l�ɃL�����Z���t���O�����Ă�BIsFrontCancel��IsCancel�ȂǂŒl�̃L�����Z�����肪�ł���B
	/// �L�����Z���w�肵���Ƃ��Ă��L���[����͂Ȃ��Ȃ�Ȃ��A�g�p�҂����肵�A�l���X�L�b�v���邱�ƁB
	/// </summary>
	inline void SetCancelFlag( const T &data , skyBool isCancel );

	/// <summary>
	/// �擪�̒l���L�����Z������Ă��邩�ǂ������肷��
	/// </summary>
	inline skyBool IsFrontCancel();

	/// <summary>
	/// Fifo�̖����̒l�̎Q�Ƃ��擾
	/// </summary>
	inline T &GetBack();

	/// <summary>
	/// Fifo�̐擪�̒l�̎Q�Ƃ��擾
	/// </summary>
	inline T &GetFront();

	/// <summary>
	/// �e�h�e�n����l�����o��
	/// </summary>
	inline skyBool Pop( T &dest );
	inline skyBool Pop();

	/// <summary>
	/// �e�h�e�n����l��S�����o���BisExecDataFree��skyTrue���ƒl�����ׂăt���[�����B�i�l���|�C���^����Ȃ��ƃn���O�j
	/// </summary>
	inline void AllPop();

	/// <summary>
	/// �e�h�e�n����w��̈ʒu�̃L�����Z�����������
	/// </summary>
	skyBool IsCancel( u32 idx ) const;

	/// <summary>
	/// �e�h�e�n����w��̈ʒu�̒l�����o���B
	/// </summary>
	inline skyBool	GetItem( u32 idx , T* &data ) const;
	inline T	&operator[]( s32 idx ) const;

	/// <summary>
	/// �l�����݂��Ȃ����ǂ�������
	/// </summary>
	inline skyBool IsNoData() const;

	/// <summary>
	/// ���݂̊i�[�����擾����
	/// </summary>
	inline u32 GetNum() const;

	/// <summary>
	/// �e�h�e�n�̃T�C�Y���擾����
	/// </summary>
	inline u32 GetSize() const;


	/// <summary>
	/// �󔻒�
	/// </summary>
	inline skyBool IsEmpty() const { return m_PushNum == 0; };

//---------------------�C�e���[�^�[�֘A-----------------------��

	/// <summary>
	/// �C�e���[�^�[�N���X
	/// </summary>
	class iterator : public Iterator< CFifo< T , ThreadingModel > , T >
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		inline iterator( CFifo< T , ThreadingModel > *pFifo , s32 cursor );
	};

	/// <summary>
	/// �J�[�\����Begin()����O�ɂ��鎞True�iFOREACH_REVERS�̂��߁j
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// �����ʒu�J�[�\�����w���C�e���[�^�[�擾
	/// </summary>
	inline iterator Begin();

	/// <summary>
	/// �C�e���[�^�[�擾
	/// </summary>
	inline iterator End();

	/// <summary>
	/// �C�e���[�^�[�̎w�������ʒu���폜
	/// </summary>
	inline iterator Erases( iterator &it );

	/// <summary>
	/// �w��̃C���f�b�N�X�ɃJ�[�\���𓖂Ă�
	/// </summary>
	inline skyBool SetItCursor( u32 idx , T* &data ) const;

	/// <summary>
	/// �C���f�b�N�X�Ń��X�g�폜
	/// </summary>
	inline skyBool DeleteIdx( u32 idx );

//---------------------�C�e���[�^�[�֘A-----------------------��

protected:

	/// <summary>
	/// �����o�[
	/// </summary>

	struct Item
	{
		T data;
		skyBool isCancel;
	};

	Item	*m_pDataBuffer;		//�f�[�^�z��
	u32		m_BufferSize;		//�f�[�^�T�C�Y
	u32 	m_Rp;         	 	//�ǂݍ��݃|�C���^
	u32 	m_Wp;         	 	//�������݃|�C���^
	u32 	m_PushNum;      	//�i�[��
};

template< typename T >
class CFifoMT : public CFifo< T , thread::ThreadingModels::CMultiThread >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFifoMT( s32 size = DEFAULT_FIFO_SIZE );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFifoMT();
};

} }

#include "Fifo.inl"
