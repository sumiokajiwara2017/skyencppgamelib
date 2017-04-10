#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< ���� >---------------------------------------------//
class CDiscFileStream : public stream::AbsStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
	/// </summary>
	u32		    vGetSize();
	skyBool     vOpen();
	skyBool     vClose();
	u32			vWriteStream( const skyString *format , ... );
	u32			vWriteStream( u8* pData , u32 size );
	u32			vWriteStream( u16 data );
	u32			vWriteStream( s16 data );
	u32			vWriteStream( u32 data );
	u32			vWriteStream( s32 data );
	u32			vWriteStream( f32 data );
	u32			vWriteStream( d64 data );
	u32		    vReadStream( skyString  *data , u32 len );
	u32		    vReadStream( u8  *pData , u32 size );
	u32			vReadStream( u16 &data );
	u32			vReadStream( s16 &data );
	u32			vReadStream( u32 &data );
	u32			vReadStream( s32 &data );
	u32			vReadStream( f32 &data );
	u32			vReadStream( d64 &data );
	u32			vMarkWrite( const skyString *mark , u32 size );
	u32			vMarkReplace( const skyString *mark , u8* data , u32 size );

	/// <summary>
	///  �R���X�g���N�^
	/// </summary>  
	CDiscFileStream( const skyString *name , eOpenMode eMode );

	/// <summary>
	///  �f�X�g���N�^
	/// </summary>  
	virtual ~CDiscFileStream();

private:

	/// <summary>
	///  �t�@�C�����
	/// </summary>  
	IFileInfoPtr m_spFileInfo;

	/// <summary>
	///  �I�[�v�����[�h
	/// </summary>  
	eOpenMode m_eMode;

	/// <summary>
	/// �}�[�N���g������
	/// </summary>    
	stream::MarkInfoHash *m_pMarkInfoHash;

};

} } }