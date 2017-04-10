#pragma once

namespace sky { namespace lib { namespace graphic {

class AbsBmpFile;

//�����X�^�C��
static const skyString* FONT_STYLE_MS_MINCHO = _T( "MS Mincho" );
static const skyString* FONT_STYLE_MS_GOSIC  = _T( "MS Gothic" );

/// <summary>
/// �t�H���g�r�b�g�}�b�v���
/// </summary>
struct CFontBmpInfo
{
	CColor                  in_color;           //�i���́j�����F
	CColor                  in_clearcolor;      //�i���́j�w�i�F
	u32                     in_lineNum;         //�i���́j�P�s�̕�����
	u32                     in_size;            //�i���́j�f�[�^�T�C�Y
	fontbmp::eGradationType in_eGradation;      //�i���́j�F�K��
	const skyString*        in_pStyle;          //�i���́j�X�^�C��
	u32                     in_isBold;          // (���́j���������H
	skyBool                 in_isAlphaGradation;//�i���́j�A���t�@�l�ŃO���f�[�V���������邩�H
	u32                     in_texWidth;        // (���́j�r�b�g�}�b�v���T�C�Y�i�Q�̗ݏ�j
	u32                     in_texHeight;       // (���́j�r�b�g�}�b�v�c�T�C�Y�i�Q�̗ݏ�j
	math::CSize< d64 >      out_outLine;        //�i�o�́j�O�g�̏c���T�C�Y
	math::CRect< d64 >      out_rect;           //�i�o�́j�ʒu
    fontbmp::CUvFileHeader* out_pUvData;        // (�o�́j�t�u���
    u32                     out_uvDataSize;     // (�o�́j�t�u���T�C�Y
	u8*                     w_pData;            //�i���[�N�j�f�[�^�J�n�A�h���X
	fontbmp::CUvData*       w_pUvData;          //�i���[�N�j�t�u�f�[�^�J�n�A�h���X

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CFontBmpInfo()
    {
		in_color = graphic::CColor_White;
		in_clearcolor = graphic::CColor_Violetw;
		in_lineNum = 16;
		in_size = 16;
		in_eGradation = fontbmp::eGradationType_2;
		in_pStyle = FONT_STYLE_MS_GOSIC;
		in_isBold = skyFalse;
		w_pData = skyNull;
		in_isAlphaGradation = skyTrue;
	    in_texWidth = 512;
	    in_texHeight = 512;
    }
};

namespace BmpUtil {

/// <summary>
/// Bmp�Ɏw��̕����������o���܂��B����������Bmp�͏���������AlineNum�ɍ��킹�����̒����A�S�Ă̕��������܂�c�̒����̃f�[�^����������܂��B
/// </summary>
void WriteFontBmp(  AbsBmpFile *bmp , const skyString *data , CFontBmpInfo &bmpinfo );

}

} } }
