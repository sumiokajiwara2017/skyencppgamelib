#pragma once

namespace sky { namespace lib { namespace graphic {

class AbsBmpFile;
typedef SmartPointer< AbsBmpFile >	AbsBmpFilePtr;					//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �r�b�g�}�b�v�x�[�X�N���X
/// </summary>
class AbsBmpFile : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	AbsBmpFile( fontbmp::eBmpKind eKind ) : m_pData( skyNull )
	{
		m_eKind = eKind;
		Reset();
	};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~AbsBmpFile()
	{
		Reset();
	};

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset()
	{
		SAFE_DELETE( m_pData );

		m_pHed = skyNull;
		m_pBmpInfo = skyNull;
		m_pPalletAdr = skyNull;
		m_pDataAdr = skyNull;
		m_pData = skyNull;
		m_nDataSize = 0;
	}

	/// <summary>
	/// �F�K�����ɃJ���[�p���b�g���쐬����(�C���f�b�N�X�J���[�̃r�b�g�}�b�v������������j
	/// </summary>
	virtual void vMakeColorPallet( const CFontBmpInfo &info ){ SKY_UNUSED_ARG( info ); };

	/// <summary>
	/// ���������������ޏ����̏�������
	/// </summary>
    virtual void vWriteFontInit( const CFontBmpInfo &info ) = 0;

	/// <summary>
	/// �P����������������
	/// </summary>
	virtual void vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info ) = 0;

	/// <summary>
	/// �w��̃T�C�Y��Bmp����������ɍ쐬����
	/// </summary>
	virtual void vCreate( u32 width, u32 height, u32 color ) = 0;

	/// <summary>
	/// �Z�[�u
	/// </summary>
	virtual void vSave( stream::IStreamPtr &dest ) = 0;

	/// <summary>
	/// ���[�h
	/// </summary>
	virtual void vLoad( stream::IStreamPtr &src ) = 0;

	/// <summary>
	/// ��`���̎擾
	/// </summary>
	math::CBasicRect&  GetRect(){ return m_Rect; };

	/// <summary>
	/// ��`���̎擾
	/// </summary>
	fontbmp::eBmpKind  GetKind(){ return m_eKind; }

protected:

	/// <summary>
	/// ���
	/// </summary>
	fontbmp::eBmpKind   m_eKind;

	/// <summary>
	/// Bitmap�w�b�_�[
	/// </summary>
	BITMAPFILEHEADER*	m_pHed;

	/// <summary>
	/// Bitmap�w�b�_�[���
	/// </summary>
	BITMAPINFO*			m_pBmpInfo;

	/// <summary>
	/// �p���b�g���擪�A�h���X
	/// </summary>
	RGBQUAD*			m_pPalletAdr;

	/// <summary>
	/// �f�[�^���擪�A�h���X
	/// </summary>
	u8*					m_pDataAdr;

	/// <summary>
	/// ��`���
	/// </summary>
	math::CBasicRect	m_Rect;

	/// <summary>
	/// �A���C�����g�l���ς݂̂P���C���̕�
	/// �r�b�g�}�b�v�͂Q�̔{���łȂ��T�C�Y�̂΂����A�P���C��������̃T�C�Y���S�o�C�g�A���C�����g���Ƃ���B
	/// </summary>
	u32					m_LineWidth;

	/// <summary>
	/// �f�[�^�i�[�|�C���^
	/// </summary>
	u8					*m_pData;

	/// <summary>
	/// �f�[�^�T�C�Y
	/// </summary>
	u32					m_nDataSize;
};

} } }