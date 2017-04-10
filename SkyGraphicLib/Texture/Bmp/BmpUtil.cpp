#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

namespace BmpUtil {

static u32 ConvertGradationType_( fontbmp::eGradationType eType )
{
	switch ( eType )
	{
		case fontbmp::eGradationType_2 :
			return GGO_BITMAP;
			break;
		case fontbmp::eGradationType_5 :
			return GGO_GRAY2_BITMAP;
			break;
		case fontbmp::eGradationType_17 :
			return GGO_GRAY4_BITMAP;
			break;
		case fontbmp::eGradationType_65 :
			return GGO_GRAY8_BITMAP;
			break;
		default:
			SKY_PANIC_MSG( _T( "It is a bit map of the uncorrespondence." ) ); //���Ή��̃r�b�g�}�b�v�ł��B
	}

	return GGO_GRAY2_BITMAP;
}

void WriteFontBmp(  AbsBmpFile *bmp , const skyString *data , CFontBmpInfo &bmpinfo )
{
	u32 fontLen = CStringUtil_::Len( data );

	// �t�H���g���
	HFONT hFont = CreateFont(
		   bmpinfo.in_size,             // �t�H���g�̍���
           0,                        // ���ϕ�����
           0,                        // ������������̊p�x
           0,                        // �x�[�X���C���̊p�x
		   ( bmpinfo.in_isBold )? 700 : 400,                      // �t�H���g�̑���(�t�H���g�̑�����\�� 0 ���� 1000 �܂ł͈͓̔��̒l���w�肵�܂��B���Ƃ��΁A400 ���w�肷��ƕW���̑����ɂȂ�A700 ���w�肷��Ƒ����ɂȂ�܂��B0 ���w�肷��ƁA����̑������I������܂��B)
           FALSE,                    // �Α̂ɂ��邩�ǂ���
           FALSE,                    // ������t���邩�ǂ���
		   FALSE,                    // ����������t���邩�ǂ���
           SHIFTJIS_CHARSET,         // �����Z�b�g�̎��ʎq
           OUT_TT_ONLY_PRECIS,       // �o�͐��x(�o�͐��x���w�肵�܂��B�o�͐��x�́A���ۂ̏o�͂��A�v�����ꂽ�t�H���g�̍����A���A�����̌����A��������̕����A�s�b�`�A����уt�H���g���ɂǂ̒��x��v���Ă��Ȃ���΂Ȃ�Ȃ����������܂��B)
           CLIP_DEFAULT_PRECIS,      // �N���b�s���O���x(�N���b�s���O���x���w�肵�܂��B�N���b�s���O���x�Ƃ́A�����̈ꕔ���N���b�s���O�̈�̊O�ɂ͂ݏo���Ƃ��ɁA���̕������N���b�v������@���`������̂ł�)
           PROOF_QUALITY,            // �o�͕i��(�o�͕i�����w�肵�܂��B�o�͕i���Ƃ́AGDI ���A�_���t�H���g�̑����Ǝ��ۂ̕����t�H���g�̑����Ƃ��ǂ̒��x�܂ň�v�����Ȃ���΂Ȃ�Ȃ������`������̂ł�)
           FIXED_PITCH | FF_MODERN,  // �s�b�`�ƃt�@�~��
           bmpinfo.in_pStyle               // �t�H���g��
	);

	math::CBasicVector2 vec( 0 , 0 );
	for ( u32 i = 0 ; i < fontLen ; i++ )
	{
		// �f�o�C�X�R���e�L�X�g�擾
		// �t�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
		HDC hdc = GetDC( NULL );
		HFONT oldFont = ( HFONT )SelectObject( hdc , hFont );

		UINT code;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
	   // unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ��ł�
       code = (UINT)data[ i ];
#else
	   // �}���`�o�C�g�����̏ꍇ�A
	   // 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	   // 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	  if( IsDBCSLeadByte( *c ) )
          code = ( BYTE )data[ i ] << 8 | ( BYTE )data[ i ];
	  else
		  code = data[ i ];
#endif

		//���s�R�[�h�̏ꍇ�͎��̍s��
		if ( i > 0 && ( code == EC || bmpinfo.in_lineNum == vec.X ) )
		{
			vec.Y += 1;
			vec.X =  0;
			continue;
		}

		// �r�b�g�}�b�v�擾
		TEXTMETRIC TM;
		GetTextMetrics( hdc, &TM );
		GLYPHMETRICS GM;
		CONST MAT2 Mat = { { 0 , 1 } , { 0 , 0 } , { 0 , 0 } , { 0 , 1 } };
		u32 d3dGradType = ConvertGradationType_( bmpinfo.in_eGradation );
		DWORD size = GetGlyphOutline( hdc , code , d3dGradType , &GM , 0 , NULL , &Mat );
		BYTE *ptr = NEW_ARRAY__( BYTE , size );
		GetGlyphOutline( hdc, code, d3dGradType, &GM, size, ptr, &Mat );

		//�����r�b�g�}�b�v���ݒ�
		bmpinfo.w_pData        = ptr;
		bmpinfo.out_outLine.W  = GM.gmCellIncX;
		bmpinfo.out_outLine.H  = ( dectype )TM.tmHeight;
		bmpinfo.out_rect.X()   = ( dectype )GM.gmptGlyphOrigin.x;
		bmpinfo.out_rect.Y()   = ( dectype )TM.tmAscent - GM.gmptGlyphOrigin.y;
		bmpinfo.out_rect.W()   = ( dectype )GM.gmBlackBoxX;
		bmpinfo.out_rect.H()   = ( dectype )GM.gmBlackBoxY;

		//�ŏ��̂P�����`��̍ہA�S���������߂��Ă��Q�̂Q��̏c���T�C�Y���v�Z���āA���̃T�C�Y��Bmp���쐬����
		if ( i == 0 )
		{
			//bmp�͏�����
			bmp->Reset();

			//�r�b�g�}�b�v�쐬(�Ƃ肠�������Łj
			bmp->vCreate( bmpinfo.in_texWidth, bmpinfo.in_texHeight , 0 );

			//�K���ɍ��킹�ăp���b�g�����i�C���f�b�N�X�J���[�ȊO�̓X���[�j
			bmp->vMakeColorPallet( bmpinfo );

			//������
			bmp->vWriteFontInit( bmpinfo );

			//UV�����쐬
			bmpinfo.out_uvDataSize = sizeof( fontbmp::CUvFileHeader ) + ( sizeof( fontbmp::CUvData ) * fontLen );
			bmpinfo.out_pUvData = ( fontbmp::CUvFileHeader * )SkyMalloc( bmpinfo.out_uvDataSize );
            CMemoryUtil_::Copy( ( *bmpinfo.out_pUvData ).code , fontbmp::CODE , sizeof( fontbmp::CODE ) );
			bmpinfo.out_pUvData->num = fontLen;
			bmpinfo.out_pUvData->version = fontbmp::VERSION;
			bmpinfo.out_pUvData->num = fontLen;
			bmpinfo.out_pUvData->eKind = bmp->GetKind();
			bmpinfo.out_pUvData->eGrad = bmpinfo.in_eGradation;
			bmpinfo.out_pUvData->isAlphaGrad = bmpinfo.in_isAlphaGradation;
			bmpinfo.out_pUvData->fileWidth  = ( dectype )bmpinfo.in_texWidth;
			bmpinfo.out_pUvData->fileHeight = ( dectype )bmpinfo.in_texHeight;
			bmpinfo.out_pUvData->fontWidth  = ( dectype )bmpinfo.out_outLine.W; //����͂ǂ̕����������͂�
			bmpinfo.out_pUvData->fontHeight = ( dectype )bmpinfo.out_outLine.H; //����͂ǂ̕����������͂�
		}

		//UV�����\�z����
		fontbmp::CUvData *pUvData = ( ( fontbmp::CUvData * )( bmpinfo.out_pUvData + 1 ) ) + i;
		pUvData->code = code;

		//�r�b�g�}�b�v�ɏ�������
		bmpinfo.w_pUvData = pUvData;
		bmp->vWriteFont( vec , bmpinfo );

		//�P���������߂�
		vec.X += 1;

		// �I�u�W�F�N�g�̊J��
		SelectObject( hdc , oldFont );
		ReleaseDC( NULL , hdc );

		// ��Еt��
		delete[] ptr;
	}

	//������`��j��
	DeleteObject( hFont );

}

}

} } }