#pragma once

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

class CBmp32BitColor;
typedef SmartPointer< CBmp32BitColor >	CBmp32BitColorPtr;					//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �萔
/// </summary>
namespace cbmp32bitcolor
{
	static const	u32	ONE_PIXEL_BIT_SIZE	= 32;
	static const	u32	ONE_PIXEL_BYTE_SIZE	= ONE_PIXEL_BIT_SIZE/8;
	static const	u32	LINE_ALIGN_SIZE		= 4;	//�P���C���̃A���C�����g�T�C�Y
}

/// <summary>
/// �Q�S�r�b�g�F�a�����t�@�C���N���X
/// </summary>
/*
+--------------------------------------------+
|                                            |
|             BMP_FILE_HEADER                |
|                                            |
+--------------------------------------------+
|                                            |
|             BMP_FILE_INFO                  |
|                                            |
+--------------------------------------------+
|                                            |
|                                            |
|                                            |
|             BMP_FILE_DATA                  |
|                                            |
|                                            |
|                                            |
|                                            |
+--------------------------------------------+

BMP_FILE_DATA���f�[�^�͉摜�̍�������E��ւƐL�т�f�[�^�������Ă���B
BMP_FILE_DATA���f�[�^�̓��C�����ɃA���C�����g���Ƃ��Ă���

*/
class CBmp32BitColor : public AbsBmpFile
{
public:

	/// <summary>
	/// �P�s�N�Z�����
	/// </summary>
	typedef struct
	{
		u8 b;
		u8 g;
		u8 r;
		u8 a;
	} stdPixelInfo;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CBmp32BitColor() : AbsBmpFile( fontbmp::eBmpKind_bmp32bit ){}

	/// <summary>
	/// �f�X�X�g���N�^
	/// </summary>
	virtual ~CBmp32BitColor(){};

	/// <summary>
	/// �w��̐F���ŏ���������
	/// </summary>
	void Clear( u32 abgr );

	/// <summary>
	/// �f�[�^���̃T�C�Y���擾����(�w�b�_�[�A�p���b�g�ȊO�̏ꏊ�j
	/// </summary>
	u32 GetDataSize();

	/// <summary>
	/// �w��̈ʒu�ɓ_��ł�
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 abgr );

	/// <summary>
	/// �w��̈ʒu�ɓ_��ł�(colorNo�̓p���b�g�ԍ���������RGBA�������肷��͂��j
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 abgr, u8 boldSize );

	/// <summary>
	/// �w��̈ʒu�ɐ�������(colorNo�̓p���b�g�ԍ���������RGBA�������肷��͂��j
	/// </summary>
	void DrawLine( const math::CBasicVector2& start, math::CBasicVector2& Gole , u32 abgr, u8 boldSize );

	/// <summary>
	/// �w��̈ʒu�Ɏl�p�`���^�C����ɑS�̂ɕ`��
	/// </summary>
	void DrawTile( CBmp32BitColor &bmpFile , math::CBasicVector2 &point, u32 nocpyColor );

	/// <summary>
	/// �w��̈ʒu�Ɏl�p�`��`��(colorNo�̓p���b�g�ԍ���������RGBA�������肷��͂��j
	/// </summary>
	void DrawRect( const math::CBasicRect& rect, u32 abgr );

	/// <summary>
	/// �w��̈ʒu�ɎO�p�`��`��(colorNo�̓p���b�g�ԍ���������RGBA�������肷��͂��j
	/// </summary>
	void DrawTriangle( const math::CBasicTriangle3& tri, u32 abgr );

	/// <summary>
	/// �w��̈ʒu�ɉ~�`��`��(colorNo�̓p���b�g�ԍ���������RGBA�������肷��͂��j
	/// </summary>
	void DrawCircle( const math::CBasicCircle& circle , u32 abgr );

	/// <summary>
	/// �w��̈ʒu�̓_�̃J���[�ԍ����擾����
	/// </summary>
	u32 GetPoint( const math::CBasicVector2& start );

	/// <summary>
	/// �������z�֐��̎���
	/// </summary>
	void vWriteFontInit( const CFontBmpInfo &info );
	void vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info );
	void vCreate( u32 width, u32 height , u32 abgr );
	void vSave          ( stream::IStreamPtr &dest );
	void vLoad          ( stream::IStreamPtr &src );
};

#endif

} } }
