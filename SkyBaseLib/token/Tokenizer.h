#pragma once

namespace sky { namespace lib { namespace token {
	
/// <summary>
/// �g�[�N���N���X
/// </summary>
template< class T >
class CTokenizer : virtual public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CTokenizer( u32 tokenSize , u32 separatorSize , u32 cutoffSize );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CTokenizer();

	/// <summary>
	/// ��؂蕶��
	/// (�Ăяo����SetSeparator( " \t\r\n,;\"" );)
	/// </summary>
	inline void SetSeparator( const T *separator );

	/// <summary>
	/// �؂�o������
	/// (�Ăяo����SetCutOff( "{}" );)
	/// </summary>
	inline void SetCutOff( const T *cutoff );

	/// <summary>
	/// �o�b�t�@�ݒ�
	/// </summary>
	inline void SetBuffer( T *buffer );

	/// <summary>
	/// ���
	/// </summary>
	inline void Release();

	/// <summary>
	/// ���̃g�[�N�������[�h�i�ŏ��Ɉ��Ăԁj
	/// </summary>
	inline void Next();

	/// <summary>
	/// ���݃��[�h�����g�[�N���ƕ����̔�r
	/// </summary>
	inline skyBool Check( const T *pToken );
	inline skyBool CheckNext( const T *pToken );
	inline skyBool CheckNowAndNext( const T *pToken );

	/// <summary>
	/// �Ō�̃g�[�N���ɍs����������
	/// </summary>
	inline skyBool IsEnd();

	/// <summary>
	/// ���݂̃g�[�N�����擾
	/// </summary>
	inline T* GetChar();

	/// <summary>
	/// d64�^�ŕԂ�
	/// </summary>
	inline d64 GetDouble();

	/// <summary>
	/// f32�^�ŕԂ�
	/// </summary>
	inline f32 GetFloat();

	/// <summary>
	/// s32�^�ŕԂ�
	/// </summary>
	inline s32 GetInt();

	/// <summary>
	/// ���̃g�[�N�����擾�i������Next����̂Ō��݂̃g�[�N���ɂ͖߂�܂���j
	/// </summary>
	inline T*  GetNextChar();

	/// <summary>
	/// d64�^�ŕԂ��i������Next����̂Ō��݂̃g�[�N���ɂ͖߂�܂���j
	/// </summary>
	inline d64 GetNextDouble();

	/// <summary>
	/// f32�^�ŕԂ��i������Next����̂Ō��݂̃g�[�N���ɂ͖߂�܂���j
	/// </summary>
	inline f32 GetNextFloat();

	/// <summary>
	/// s32�^�ŕԂ��i������Next����̂Ō��݂̃g�[�N���ɂ͖߂�܂���j
	/// </summary>
	inline s32 GetNextInt();

private:

	/// <summary>
	/// �f�[�^�o�b�t�@
	/// </summary>
	T *m_pBuffer;

	/// <summary>
	/// �g�[�N���o�b�t�@
	/// </summary>
	T *m_pToken;
	T *m_pTokenUseCheck;

	/// <summary>
	/// ��؂蕶��
	/// </summary>
	T *m_pSeparator;

	/// <summary>
	/// �؂�o������
	/// </summary>
	T *m_pCutOff;

	/// <summary>
	/// �g�[�N���T�C�Y
	/// </summary>
	u32 m_TokenSize;

	/// <summary>
	/// ��؂蕶��
	/// </summary>
	u32 m_SeparatorSize;

	/// <summary>
	/// �؂�o������
	/// </summary>
	u32 m_CutoffSize;
};

//�V���O���o�C�g�g�[�N���N���X
typedef CTokenizer< skyMString >     CTokenizerM;

//�}���`�o�C�g�g�[�N���N���X
typedef CTokenizer< skyWString >     CTokenizerW;

} } }

#include "Tokenizer.inl"