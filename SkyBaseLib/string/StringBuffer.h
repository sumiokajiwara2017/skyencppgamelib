#pragma once

namespace sky { namespace lib { namespace string {

/// <summary>
/// ������N���X�i�ϒ��j
/// </summary>
class CStringBuffer : public CString
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CStringBuffer();
	CStringBuffer( const skyString *pStr );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CStringBuffer();

	/// <summary>
	/// �������ݒ肷��
	/// </summary>
	virtual CStringBuffer& operator =( const skyString *pStr );
	virtual CStringBuffer& operator =( const CStringBuffer &str );

	/// <summary>
	/// �������A������i�o�b�t�@����ꂽ��o�b�t�@���g������j
	/// </summary>
	virtual CStringBuffer& operator +=( const skyString *pStr );
	virtual CStringBuffer& operator +=( const CStringBuffer &str );
	virtual CStringBuffer& operator +=( skyString str );
	virtual CStringBuffer& operator +=( s32 value );
	virtual CStringBuffer& operator +=( u32 value );
	virtual CStringBuffer& operator +=( f32 value );
	virtual CStringBuffer& operator +=( d64 value );
	virtual CStringBuffer& operator +=( skyBool value );
};

} } }