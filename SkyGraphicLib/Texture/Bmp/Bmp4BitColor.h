#pragma once

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

class CBmp4BitColor;
typedef SmartPointer< CBmp4BitColor >	CBmp4BitColorPtr;					//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �萔
/// </summary>
namespace cbmp4bitcolor
{
	static const	u32	PALLET_NUM				= 16;
	static const	u32	PALLET_SIZE				= PALLET_NUM * sizeof(RGBQUAD);
	static const	u32	NO_PALLET_IDX			= PALLET_NUM;			//�����ȃp���b�g�C���f�b�N�X
	static const	d64	ONE_PIXEL_BIT_SIZE		= 4.0f;					//���L�Ōv�Z�Ɏg�����߂�����d64
	static const	d64	ONE_PIXEL_BYTE_SIZE		= ONE_PIXEL_BIT_SIZE/8;
	static const	u32	LINE_ALIGN_SIZE			= 4;					//�P���C���̃A���C�����g�T�C�Y
	static const	s32	NO_DRAW_PALLET_IDX_NONE	= -1;				
}

/// <summary>
/// �P�U�F�a�����t�@�C���N���X
/// </summary>
/*
+--------------------------------------------+
|                                            |
|             BITMAPFILEHEADER               |
|                                            |
+--------------------------------------------+
|                                            |
|               BITMAPINFO		             |
|                                            |
+--------------------------------------------+
|                                            |
|               COLOR_PALLET                 |
|                                            |
+--------------------------------------------+
|                                            |
|                                            |
|                                            |
|               COLOR_INDEX                  |
|                                            |
|                                            |
|                                            |
|                                            |
+--------------------------------------------+

���|�C���g��
COLOR_INDEX���f�[�^�͉摜�̍�������E��ւƐL�т�f�[�^�������Ă���B
COLOR_INDEX���f�[�^�̓��C�����ɃA���C�����g���Ƃ��Ă���

*/
class CBmp4BitColor : public AbsBmpFile
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CBmp4BitColor();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CBmp4BitColor(){};

	/// <summary>
	/// �f�[�^���̃T�C�Y���擾����(�w�b�_�[�A�p���b�g�ȊO�̏ꏊ�j
	/// </summary>
	u32 GetDataSize();

	/// <summary>
	/// �w��̐F���ŏ���������
	/// </summary>
	void Clear( u32 colorNo );

	/// <summary>
	/// �w��̐F���ŏ���������
	/// </summary>
	void SetNoDrawPalletNo( u32 palletNo );
	void SetNoDrawPalletNoClear();

	/// <summary>
	/// �w��̈ʒu�ɓ_��ł�
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 palletNo );

	/// <summary>
	/// �w��̈ʒu�ɓ_��ł�
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize );

	/// <summary>
	/// �w��̈ʒu�ɐ�������
	/// </summary>
	void DrawLine( const math::CBasicVector2& start, const math::CBasicVector2& Gole , u32 palletNo, u8 boldSize );

	/// <summary>
	/// �w��̈ʒu�Ɏl�p�`���^�C����ɑS�̂ɕ`��
	/// </summary>
	void DrawTile( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

	/// <summary>
	/// �w��̈ʒu�Ɏl�p�`��`��
	/// </summary>
	void DrawRect( const math::CBasicRect& rect, u32 palletNo );

	/// <summary>
	/// �w��̈ʒu�ɎO�p�`��`��
	/// </summary>
	void DrawTriangle( const math::CBasicTriangle3& tri, u32 palletNo );

	/// <summary>
	/// �w��̈ʒu�ɉ~�`��`��
	/// </summary>
	void DrawCircle( const math::CBasicCircle& circle , u32 palletNo );

	/// <summary>
	/// �w��̈ʒu��Bmp��`��
	/// </summary>
	void DrawBmp( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

	/// <summary>
	/// �w��̈ʒu�̓_�̃J���[�ԍ����擾����
	/// </summary>
	u32 GetPoint( const math::CBasicVector2 &pos );

	/// <summary>
	/// �T�C�Y�ύX������
	/// </summary>
	CBmp4BitColor * CloneBmp( u32 width, u32 height, u32 palletNo );

	/// <summary>
	/// �p���b�g�A�h���X�̎擾
	/// </summary>
	RGBQUAD*			GetPalletAdr();

	/// <summary>
	/// �T�C�Y�ύX
	/// </summary>
	void ReSize( dectype sc );

	/// <summary>
	/// �w���Bmp�̃p���b�g���㏑������
	/// </summary>
	void SetPallet( CBmp4BitColor &bmpFile );

	/// <summary>
	/// �w��̃e�N�X�`�����W����ċA�Ŏw��̐F�œh��Ԃ�
	/// </summary>
	void PaintsOut( math::CBasicVector2& vec2, u32 palletNo, CBmp4BitColor& flgBmp );

	/// <summary>
	/// ���z�֐��̎���
	/// </summary>
	void vWriteFontInit   ( const CFontBmpInfo &info );
	void vWriteFont       ( const math::CBasicVector2 &pos , const CFontBmpInfo &info );
	void vCreate          ( u32 width, u32 height, u32 palletNo );
	void vMakeColorPallet ( const CFontBmpInfo &info );
	void vSave            ( stream::IStreamPtr &dest );
	void vLoad            ( stream::IStreamPtr &src );

private:

	/// <summary>
	/// �������݋֎~�p���b�g�C���f�b�N�X
	/// </summary>
	s32	m_NoDrawPalletIdx;

	/// <summary>
	/// �h��Ԃ��ċA
	/// </summary>
	void PaintsOut_Child( math::CBasicVector2& vec2, u32 startPalNo, u32 paintsOutPalNo, CBmp4BitColor& flgBmp );
};

#endif

} } }
