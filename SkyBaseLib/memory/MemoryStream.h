#pragma once

namespace sky { namespace lib { namespace memory {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// ��������ɑ��݂���̈�Ɋւ��ēǂݏ�������N���X�B
/// �S���ǂݍ��񂾃t�@�C���A�ʐM�Ŏ擾�����f�[�^�A�R���\�[��������͂����f�[�^���������Ɋi�[����\��
/// �f�[�^���������ސl�Ɠǂݍ��ސl�ł��̃N���X�̃C���X�^���X�����L�����炢���̂����B�ł����̂Q�l�͓����X���b�h��œ������A���̃N���X�ɃN���e�B�J���Z�N�V�����𒣂�K�v������
/// </summary>
class CMemoryStream : public stream::AbsStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
	/// </summary>  
	const u8*    vGetSeekData() { return vGetData() + m_Seek; };
	const u32    vGetSeek()     { return m_Seek; };
	const u8*    vGetData()     { return m_pData; };
	const u8**   vGetDataPtr()  { return ( const u8 ** )&m_pData; };

    u8*          vCloneData();
	u32		     vGetSize(){ return m_Size; };
	skyBool      vOpen();
	skyBool      vClose();
	u32			 vWriteStream( const skyString *format , ... );
	u32			 vWriteStream( u8* pData , u32 size );
	u32			 vWriteStream( u16 data );
	u32			 vWriteStream( s16 data );
	u32			 vWriteStream( u32 data );
	u32			 vWriteStream( s32 data );
	u32			 vWriteStream( f32 data );
	u32			 vWriteStream( d64 data );
	u32		     vReadStream( skyString  *data , u32 len );
	u32		     vReadStream( u8  *data , u32 size );
	u32		     vReadStream( u16 &data );
	u32			 vReadStream( s16 &data );
	u32			 vReadStream( u32 &data );
	u32			 vReadStream( s32 &data );
	u32			 vReadStream( f32 &data );
	u32			 vReadStream( d64 &data );
	void	     vCreate( u32 size );
	void 	     vDestroy();
	u32			 vMarkWrite( const skyString *mark , u32 size );
	u32			 vMarkReplace( const skyString *mark , u8* data , u32 size );

	/// <summary>
	///  �R���X�g���N�^
	/// </summary>
	CMemoryStream( const skyString *name , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize  );
	CMemoryStream( const skyString *name , void* data , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

	/// <summary>
	///  �f�X�g���N�^
	/// </summary>
	virtual ~CMemoryStream();

protected:

	/// <summary>
	/// �f�[�^
	/// </summary>    
	u8 *m_pData;

	/// <summary>
	/// �f�[�^�T�C�Y
	/// </summary>    
	u32 m_Size;

	/// <summary>
	/// �f�[�^������֐��Ŕj�����邩�i�O������n�����f�[�^�̏ꍇ�A�j�����s��Ȃ��ق��������ꍇ������j
	/// </summary>
	skyBool m_IsEnableDelete;

	/// <summary>
	/// �}�[�N���g������
	/// </summary>    
	stream::MarkInfoHash *m_pMarkInfoHash;

	/// <summary>
	/// �R���X�g���N�^���ʏ�����
	/// </summary>
	void ComInit( const skyString *name , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

};

} } }