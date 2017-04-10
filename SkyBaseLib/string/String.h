#pragma once

namespace sky { namespace lib { namespace string {

/// <summary>
/// ������N���X�i�Œ蒷�j
/// </summary>
class CString : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CString();
	CString( const skyWString *pStr );
	CString( const skyMString *pStr );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CString();

	/// <summary>
	/// ��������N���A����
	/// </summary>
	void Clear();

	/// <summary>
	/// �������ɂ���
	/// </summary>
	void ToLower();

	/// <summary>
	/// �������Ԃ�
	/// </summary>
	const skyString* Get() const;

	/// <summary>
	/// �������ݒ肷��
	/// </summary>
	virtual CString& operator =( const skyWString *pStr );
	virtual CString& operator =( const skyMString *pStr );
	virtual CString& operator =( const CString& str );

	/// <summary>
	/// ��������r����
	/// </summary>
	virtual skyBool operator ==( const skyString c ) const;
	virtual skyBool operator ==( const skyString *pStr ) const;
	virtual skyBool operator ==( const CString& str ) const;
	virtual skyBool operator !=( const skyString c ) const;
	virtual skyBool operator !=( const skyString *pStr ) const;
	virtual skyBool operator !=( const CString& str ) const;

	/// <summary>
	/// �������A������i�o�b�t�@����ꂽ���ꂽ���͗��Ƃ��j
	/// </summary>
	virtual CString& operator +=( const skyString *pStr );
	virtual CString& operator +=( const CString& str );

	/// <summary>
	/// �L���X�g�̃I�[�o�[���C�h
	/// </summary>
    operator const skyString*() const;

	/// <summary>
	/// ������̒�����Ԃ�
	/// </summary>
	u32 GetLen(){ return m_nStrBufLen; }

	/// <summary>
	/// �����f�[�^�̃o�C�g�T�C�Y��Ԃ�
	/// </summary>
	u32 GetSize(){ return m_nStrBufSize; }

	/// <summary>
	/// _MallocStrBuf�Ŋm�ۂ����̈���J������
	/// </summary>
	void Free();

protected:

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
	skyBool		m_bIsFix;		//���łɕ����񂪊i�[���ꂽ���H
	skyString	*m_pStrBuf;		//�Ǘ����Ă��镶����
	u32			m_nStrBufLen;	//�Ǘ����Ă��镶����̒���
	u32			m_nStrBufSize;	//�Ǘ����Ă��镶����̃������T�C�Y

	/// <summary>
	/// �w��̕����œ����o�b�t�@���쐬
	/// </summary>
	void _Create( const skyString *pStr );

	/// <summary>
	/// �w��̕����񒷂��̃��������m�ۂ��Đ擪�A�h���X��m_StrBufLen�Ɋi�[����
	/// </summary>
	void _MallocStrBuf( u32 len );

	/// <summary>
	/// _MallocStrBuf�Ŋm�ۂ����̈�ɕ�������R�s�[����
	/// </summary>
	void _Copy( const skyString *pStr );

	/// <summary>
	/// _MallocStrBuf�Ŋm�ۂ����̈�ɕ������A������
	/// </summary>
	void _Cat( const skyString *pStr );
};

} } }


