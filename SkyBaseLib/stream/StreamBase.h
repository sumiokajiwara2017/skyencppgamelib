#pragma once

namespace sky { namespace lib {

template< typename T , typename KEY , class ThreadingModel >class CHash;

} }

namespace sky { namespace lib { namespace stream {

/// <summary>
/// �}�[�N�ʒu���
/// </summary>
struct MarkInfo
{
	u32 seekPos;
	u32 size;

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	MarkInfo& operator =( const MarkInfo& src )
	{
		seekPos    = src.seekPos;
		size       = src.size;
		return *this;
	};
};
typedef CHash< MarkInfo , s32 , thread::ThreadingModels::CSingleThread > MarkInfoHash;

static const u32 MARK_INFO_HASH_SIZE = 31;

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

static const u32 WRITE_STREAM_BUF_SIZE = 1024;

/// <summary>
/// �X�g���[���C���^�t�F�[�X
/// ���̃N���X���p������͉̂��L�̋@�\����
/// �������Ǎ��^�����@�\
/// �t�@�C���Ǎ��^�����@�\
/// �l�b�g���[�N�Ǎ��^�����@�\
/// �R���\�[���Ǎ��^�����@�\
/// </summary>
class IStream : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ��ނ��擾
	/// </summary>
	virtual eStreamKind vGetKind() = 0;

	/// <summary>
	/// �G���f�B�A���̃X���b�v�t���O�̐ݒ�
	/// </summary>
	virtual void vSetEndianSwap( skyBool isSwap ) = 0;

	/// <summary>
	/// �f�[�^��ǂݎ���p���ǂ����`�F�b�N����
	/// </summary>
	virtual skyBool vIsReadOnly() = 0;

	/// <summary>
	/// �f�[�^���T�C�Y�ύX�\�i�������߂Ώ������ނقǃT�C�Y���L����j���ǂ������肷��
	/// </summary>
	virtual skyBool vIsEnableChengeSize() = 0;

	/// <summary>
	/// �f�[�^���擾����i�X�g���[���̎����ɂ���Ă͎��Ȃ��ꍇ������j
	/// </summary>
	virtual const u32 vGetSeek() = 0;

	/// <summary>
	/// �f�[�^���擾����i�X�g���[���̎����ɂ���Ă͎��Ȃ��ꍇ������j
	/// </summary>
	virtual const u8* vGetSeekData() = 0;

	/// <summary>
	/// �f�[�^���擾����i�X�g���[���̎����ɂ���Ă͎��Ȃ��ꍇ������j
	/// </summary>
	virtual const u8* vGetData() = 0;

	/// <summary>
	/// �f�[�^�̃|�C���^���擾����i�X�g���[���̎����ɂ���Ă͎��Ȃ��ꍇ������j
	/// </summary>
	virtual const u8** vGetDataPtr() = 0;

	/// <summary>
	/// �f�[�^���N���[������
	/// </summary>
	virtual u8*       vCloneData() = 0;

	/// <summary>
	/// �f�[�^�T�C�Y���擾����i�X�g���[���̎����ɂ���Ă͎��Ȃ��ꍇ������j
	/// </summary>
	virtual u32 vGetSize() = 0;

	/// <summary>
	/// �G���f�B�A���̐ݒ�(skyTrue��ݒ肵���ꍇ�A�������݁A�ǂݍ��݂̏u�Ԃɒl���X���b�v����j
	/// </summary>
	virtual skyBool vIsSwap() = 0;

	/// <summary>
	/// �J���Ă��邩�H
	/// </summary>
	virtual skyBool vIsOpen() = 0;

	/// <summary>
	/// �X�g���[�����J��
	/// </summary>
	virtual skyBool vOpen() = 0;

	/// <summary>
	/// �X�g���[�������
	/// </summary>
	virtual skyBool vClose() = 0;

	/// <summary>
	/// ��������Ƀf�[�^���m�ۂ���
	/// </summary>
	virtual void vCreate( u32 size ) = 0;

	/// <summary>
	/// ��������̃f�[�^��j������
	/// </summary>
	virtual void vDestroy() = 0;

	/// <summary>
	/// �X�g���[���ɏ�����
	/// </summary>
	virtual u32 vWriteStream( const skyString *s , ... ) = 0;
	virtual u32 vWriteStream( u8* pData, u32 size ) = 0;
	virtual u32 vWriteStream( u16 data ) = 0;
	virtual u32 vWriteStream( s16 data ) = 0;
	virtual u32 vWriteStream( u32 data ) = 0;
	virtual u32 vWriteStream( s32 data ) = 0;
	virtual u32 vWriteStream( f32 data ) = 0;
	virtual u32 vWriteStream( d64 data ) = 0;

	/// <summary>
	/// �X�g���[������Ǎ��݁i�߂�l�͎��ۂɓǂݍ��񂾃T�C�Y�j
	/// </summary>
	virtual u32 vReadStream( skyString  *data , u32 len ) = 0;
	virtual u32 vReadStream( u8  *pData, u32 size ) = 0;
	virtual u32 vReadStream( u16 &data ) = 0;
	virtual u32 vReadStream( s16 &data ) = 0;
	virtual u32 vReadStream( u32 &data ) = 0;
	virtual u32 vReadStream( s32 &data ) = 0;
	virtual u32 vReadStream( f32 &data ) = 0;
	virtual u32 vReadStream( d64 &data ) = 0;

	/// <summary>
	/// �����݁^�Ǎ���
	/// </summary>
	virtual u32 vWriteReadStream( u8* pData , u32 size , skyBool isWrite ) = 0;

	/// <summary>
	/// �}�[�N��������
	/// </summary>
	virtual u32 vMarkWrite( const skyString *mark , u32 size ) = 0;
	virtual u32 vMarkReplace( const skyString *mark , u8* data , u32 size ) = 0;

	/// <summary>
	/// Seek�ʒu��ݒ肷��
	/// </summary>
	virtual void vSetSeek( u32 seek ) = 0;
	virtual void vAddSeek( u32 seek ) = 0;

	/// <summary>
	/// �X�V���Ԃ��擾����
	/// </summary>
	virtual updateTime vGetUpdateTime() = 0;
	virtual void vSetUpdateTime( updateTime time ) = 0;

	/// <summary>
	/// ���O���擾����
	/// </summary>
	virtual const skyString *vGetName() = 0;
	virtual void vSetName( const skyString *name ) = 0;

	/// <summary>
	/// �P�s�Ǎ�
	/// <summary>
	virtual skyBool vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 ) = 0;

	/// <summary>
	/// �P�s��������
	/// <summary>
	virtual void vWriteLine( const skyString *pBuf , u32 len = 0 ) = 0;
};

//-----------------------------------< ���ێ��� >--------------------------------------------//

class AbsStream : public IStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �����i���ێ����Ŏ�������ꍇ�͕K��virtual�Ő錾����B�����ŃI�[�o�[���C�h����邩������Ȃ�����j
	/// </summary>
	virtual eStreamKind vGetKind(){ return m_eKind; }
	virtual void vSetEndianSwap( skyBool isSwap ){ m_bIsSwap = isSwap; };
	virtual skyBool vIsSwap(){ return m_bIsSwap; };
	virtual skyBool vIsReadOnly() { return m_IsReadOnly; }
	virtual skyBool vIsEnableChengeSize() { return m_IsEnableChengeSize; }
	virtual void vSetSeek( u32 seek ){ m_Seek = seek; }
	virtual void vAddSeek( u32 seek ){ m_Seek += seek; }
	virtual updateTime vGetUpdateTime(){ return m_nUpdateTime; }
	virtual void vSetUpdateTime( updateTime time ){ m_nUpdateTime = time; }
	virtual skyBool vIsOpen(){ return m_IsOpen; };
	virtual const skyString *vGetName(){ return m_Name.Get(); };
	virtual void vSetName( const skyString *name ){ m_Name = name; };

//-------�����ł�����͎̂������遤
	virtual const u8*    vGetSeekData();
	virtual const u32    vGetSeek();
	virtual const u8*    vGetData();
	virtual const u8**   vGetDataPtr();
    virtual u8*          vCloneData();
	virtual u32		     vGetSize();
	virtual u32			 vWriteStream( u8* pData , u32 size );
	virtual u32			 vWriteStream( u16 data );
	virtual u32			 vWriteStream( s16 data );
	virtual u32			 vWriteStream( u32 data );
	virtual u32			 vWriteStream( s32 data );
	virtual u32			 vWriteStream( f32 data );
	virtual u32			 vWriteStream( d64 data );
	virtual u32		     vReadStream( u8* pData , u32 size );
	virtual u32			 vReadStream( u16 &data );
	virtual u32			 vReadStream( s16 &data );
	virtual u32			 vReadStream( u32 &data );
	virtual u32			 vReadStream( s32 &data );
	virtual u32			 vReadStream( f32 &data );
	virtual u32			 vReadStream( d64 &data );
	virtual u32          vWriteReadStream( u8* pData , u32 size , skyBool isWrite );
	virtual void	     vCreate( u32 size );
	virtual void 	     vDestroy();
	virtual u32			 vMarkWrite( const skyString *mark , u32 size );
	virtual u32			 vMarkReplace( const skyString *mark , u8* data , u32 size );
	virtual skyBool      vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 );
	virtual void         vWriteLine( const skyString *pBuf , u32 len = 0 );

//-------�����ł�����͎̂������遢

protected:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	AbsStream( eStreamKind eKind ) : 
		 m_IsEnableChengeSize( skyFalse ) , 
		 m_eKind( eKind ) , 
		 m_bIsSwap( skyFalse ) , 
		 m_Seek( 0 ) , 
		 m_IsReadOnly( skyFalse ) , 
		 m_nUpdateTime( 0 ) , 
		 m_IsOpen( skyFalse ){}

	/// <summary>
	/// �X���b�v������t���O
	/// </summary>
	skyBool m_bIsSwap;

	/// <summary>
	/// �V�[�N�ʒu
	/// </summary>
	u32 m_Seek;

	/// <summary>
	/// �X�V����
	/// </summary>
	updateTime m_nUpdateTime;

	/// <summary>
	/// ���O
	/// </summary>
	string::CString m_Name;
	
	/// <summary>
	/// �Ǎ���p�t���O
	/// </summary>
	skyBool m_IsReadOnly;

	/// <summary>
	/// �T�C�Y�ύX�L���t���O
	/// </summary>
	skyBool m_IsEnableChengeSize;

	/// <summary>
	/// �X�g���[�����
	/// </summary>
	eStreamKind m_eKind;

	/// <summary>
	///  �J���Ă���t���O
	/// </summary>  
	skyBool m_IsOpen;
};

} } }