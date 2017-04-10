#pragma once

namespace sky { namespace lib {

static const u32 VECTOR_DEFAULT_SIZE = 32;
static const u32 VECTOR_DEFAULT_UP_SIZE = 16;

/// <summary>
/// �ėp�^�̓��I�g���z��N���X
/// �������A�������A�h���X�Ȃ̂ŁAstd::sort���g�p�ł��܂��B
/// Add���ɗe�ʕs�������������ꍇ�v�f���c�傾�Ƌɒ[�ɒx���Ȃ�܂��Bsize�͎��ۂ̍ő�g�p�ʂ�ݒ肵�āA���I�g�����������Ȃ��悤�ɂ��Ă��������B
/// </summary>
template< typename T >
class CVector : public IteratorIf< T >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVector( s32 size = VECTOR_DEFAULT_SIZE , u32 upSize = VECTOR_DEFAULT_UP_SIZE ); //�ݒ萔�O�ŃX�^�[�g

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CVector( const CVector& src );

	/// <summary>
	/// ������Z�q�i��������:���j
	/// </summary>
	inline CVector& operator =( const CVector& src );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CVector();

	/// <summary>
	/// �ǉ��i��������:��{��A���X���j
	/// �񂪑���Ȃ��Ȃ�����SAlloc���������ăf�[�^�z��S�̂�u�������܂��B
	/// </summary>
	T &Add( const T &value, u32* idx = skyNull );
	T &Add( const T *value = skyNull , u32* idx = skyNull );

	/// <summary>
	/// �폜�i��������:���j
	/// �z��̔C�ӂ̕������폜���A�㑱���l�߂܂�
	/// </summary>
	skyBool Delete( const T &value );

	/// <summary>
	/// �擾�i��������:���j
	/// </summary>
	skyBool Index( const T &value , s32 &dest );

	/// <summary>
	/// �擾�i��������:��j
	/// </summary>
	skyBool Get( u32 idx , T &value );

	/// <summary>
	/// ���e��j�����A������Ԃɂ��܂��i�����Ӂ������ݒ萔��0�ɂȂ�܂��j
	/// </summary>
	void Clear( s32 size = VECTOR_DEFAULT_SIZE );

	/// <summary>
	/// ���Z�b�g�i�����Ӂ������ݒ萔��size�ɂȂ�܂��j
	/// </summary>
	void Reset( u32 size = VECTOR_DEFAULT_SIZE );

	/// <summary>
	/// ���e����x�j�����Asize�������̈���m�ۂ��Avalue�Ŗ��߂�i�����Ӂ������ݒ萔��size�ɂȂ�܂��j
	/// </summary>
	void Assign( u32 size , T value );

	/// <summary>
	/// �T�C�Y���擾
	/// </summary>
	u32 GetSize() const;

	/// <summary>
	/// �ݒ萔���擾
	/// </summary>
	u32 GetNum() const;

	/// <summary>
	/// ���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T &operator[]( s32 idx ) const;

	/// <summary>
	/// �����o�b�t�@�̐擪�v�f�A�h���X���擾����
	/// </summary>
	T* GetTopAdr() const;

	/// <summary>
	/// �����o�b�t�@�̖����v�f�A�h���X���擾����
	/// </summary>
	T* GetBottomAdr() const;

//---------------------�C�e���[�^�[�֘A-----------------------��

	/// <summary>
	/// �C�e���[�^�[
	/// </summary>
	class iterator : public Iterator<CVector< T >,T>
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		iterator( CVector< T > *pVector, s32 cursor );
	};

	/// <summary>
	/// �J�[�\����Begin()����O�ɂ��鎞True�iFOREACH_REVERS�̂��߁j
	/// </summary>
	inline skyBool IsBeginBefor( iterator &it ) const;

	/// <summary>
	/// �����ʒu�J�[�\�����w���C�e���[�^�[�擾
	/// </summary>
	iterator Begin();

	/// <summary>
	/// �C�e���[�^�[�擾
	/// </summary>
	iterator End();

	/// <summary>
	/// �C�e���[�^�[�̎w�������ʒu���폜
	/// </summary>
	iterator Erases( iterator &it );

	/// <summary>
	/// �w��̃C���f�b�N�X�ɃJ�[�\���𓖂Ă�
	/// </summary>
	skyBool SetItCursor( u32 idx , T* &data ) const;

	/// <summary>
	/// �C���f�b�N�X�Ń��X�g�폜
	/// </summary>
	skyBool DeleteIdx( u32 idx );

	/// <summary>
	/// �R�s�[
	/// </summary>
	inline void Copy( CVector &dest ) const;

//---------------------�C�e���[�^�[�֘A-----------------------��

protected:

	/// <summary>
	/// �����o�[
	/// </summary>
	T*		m_pData;
	u32		m_Size;
	u32		m_nUpSize;
	u32		m_nSetNum;	//�z��̓o�^��

private:

	/// <summary>
	/// ���݂̃J�[�\���ʒu�A�h���X���擾
	/// </summary>
	skyBool	_SetCursor( u32 idx , T* &pCur ) const;

};

/// <summary>
/// �ėp�^�̓��I�g���z��N���X(�v���~�e�B�u�Ȍ^�Au8 , s8 , u16 , s16 , u32 , s32 , u64 , f32 , d64 , ���z�e�[�u���������Ȃ�class struct ���͂�����g�p���ق��������ł��j
/// �������A�������A�h���X�Ȃ̂ŁAstd::sort���g�p�ł��܂��B
/// ���Ӂj�������I�g���R���e�i(Fifo,Hash,List,Stack,Vector)�̔z����쐬�������ꍇ�́A�R���e�i�̃|�C���^��v�f�Ɏ��x�N�^�[�ɂ��Ă��������B������MemCopy���s���Ă��邽�߁A�N���X�A�\���̂̔z��ł͖�肪�������܂��B
/// </summary>
template< typename T >
class CVectorPrimitive : public CVector< T >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVectorPrimitive( s32 size = VECTOR_DEFAULT_SIZE , u32 upSize = VECTOR_DEFAULT_UP_SIZE ); //�ݒ萔�O�ŃX�^�[�g

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CVectorPrimitive( const CVectorPrimitive& src );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CVectorPrimitive();

	/// <summary>
	/// ������Z�q
	/// </summary>
	inline CVectorPrimitive& operator =( const CVectorPrimitive& src );

	/// <summary>
	/// �ǉ��i��������:��{��A���X���j
	/// �񂪑���Ȃ��Ȃ�����SAlloc���������ăf�[�^�z��S�̂�u�������܂��B
	/// </summary>
	skyBool Add( const T &value, u32* idx = skyNull );
	skyBool Add( const T *value = skyNull , u32* idx = skyNull );

	/// <summary>
	/// �R�s�[
	/// </summary>
	inline void Copy( CVector &dest ) const;
};

typedef CVectorPrimitive< u8 >  CVector_u8;
typedef CVectorPrimitive< s8 >  CVector_s8;
typedef CVectorPrimitive< u16 > CVector_u16;
typedef CVectorPrimitive< s16 > CVector_s16;
typedef CVectorPrimitive< u32 > CVector_u32;
typedef CVectorPrimitive< s32 > CVector_s32;
typedef CVectorPrimitive< f32 > CVector_f32;
typedef CVectorPrimitive< d64 > CVector_d64;
typedef CVectorPrimitive< u64 > CVector_u64;

typedef CVectorPrimitive< vrtxtype > CVector_vrtxtype;
typedef CVectorPrimitive< dectype > CVector_dectype;

} }

#include "Vector.inl"