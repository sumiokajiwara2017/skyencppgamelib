#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

void CBmp32BitColor::Clear( u32 abgr )
{
	DrawRect( m_Rect, abgr );
}

u32 CBmp32BitColor::GetDataSize()
{
	return m_pBmpInfo->bmiHeader.biWidth * m_pBmpInfo->bmiHeader.biHeight * cbmp32bitcolor::ONE_PIXEL_BYTE_SIZE;
}

void CBmp32BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 abgr )
{
	//�f�[�^��������n�܂��Ă���̂�Y���W�𔽓]������
	u32 dotPosY = ( u32 )( (m_Rect.GetSize().H-1)-pos.Y);

	//�_���O����
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , ( dectype )dotPosY ) ) == skyFalse ) return;

	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) );
	stdPixelInfo*pData = &( (stdPixelInfo*)m_pDataAdr)[ Tgt ];
	u8*	pInputData = ( u8 * )&abgr;
	pData->r = pInputData[ 0 ];
	pData->g = pInputData[ 1 ];
	pData->b = pInputData[ 2 ];
	pData->a = pInputData[ 3 ];
}

void CBmp32BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize )
{
	//�}�C�i�X���W�̏ꍇ���^�[��
	if ( pos.X < 0 || pos.Y < 0 ) return;

	// �`��̊J�n���W���牡���A�c�������߂�
	s32 Width	= (s32)(pos.X+boldSize);
	s32 Height	= (s32)(pos.Y+boldSize);
	
	// �_��ł�����ł���
	u16 texDotX = ( u16 )pos.X;
	u16 texDotY = ( u16 )pos.Y;
	for ( ; texDotX < Width; texDotX++ )
	{
		//�����e�N�X�`���̃C���f�b�N�X�𒴂����疳��
		if ( texDotX >= m_Rect.GetSize().W ) continue;

		for ( ; texDotY < Height; texDotY++ )
		{
			//�������e�N�X�`���̃C���f�b�N�X�𒴂����疳��
			if ( texDotY >= m_Rect.GetSize().H ) continue;
	
			DrawPoint( math::CBasicVector2( texDotX , texDotY ) , palletNo );
		}
		texDotY = ( u16 )pos.Y;
	}
}

void CBmp32BitColor::DrawLine( const math::CBasicVector2& start, math::CBasicVector2& Gole , u32 abgr, u8 boldSize )
{
	s32 offsetX ,offsetY,distNum ,drawBasePosX ,drawBasePosY;
	u32 twoPointDist;
	d64	test1,test2,testX ,testY;

	// �Q�_�ԋ����̂Q����Z�o
	offsetX = (s32)(start.X - Gole.X);
	offsetY = (s32)(start.Y - Gole.Y);
	twoPointDist = ( u32 )( (offsetX*offsetX) + (offsetY*offsetY) );
	
	// �Q�_�ԋ��������ɁA���Ԋu�ŉ���h�邩���Z�o
	distNum = (s32)( (twoPointDist / 2) + 1 );
	
	// �⊮���ēh���Ă���
	for ( s32 i = 1; i <= distNum; i++ )
	{
		test1 = ( ( d64 )offsetX/( d64 )distNum);
		test2 = ( ( d64 )offsetY/( d64 )distNum);
	
		testX = test1*i;
		testY = test2*i;
	
		drawBasePosX = (s32)(Gole.X + testX);
		drawBasePosY = (s32)(Gole.Y + testY);
		
		DrawPoint( math::CBasicVector2( ( dectype )drawBasePosX , ( dectype )drawBasePosY ) , abgr, boldSize );
	}
}

void CBmp32BitColor::DrawTile( CBmp32BitColor &bmpFile , math::CBasicVector2 &point, u32 nocpyColor )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 palNo	= 0;
	d64 tileWNum = m_Rect.GetSize().W / bmpFile.GetRect().GetSize().W;
	d64 tileHNum = m_Rect.GetSize().H / bmpFile.GetRect().GetSize().H;
	
	for( u32 tw = 0; tw<tileWNum; tw++ )
	{
		for( u32 th = 0; th<tileHNum; th++ )
		{
			for ( dectype x=0; x<width; x++ )
			{
				for ( dectype y=0; y<height; y++ )
				{
					palNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
					if( ( s32 )nocpyColor == cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE || ( s32 )nocpyColor != palNo )
					{
						DrawPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) , palNo );
					}
				}
			}
		}
	}
}

void CBmp32BitColor::DrawRect( const math::CBasicRect& rect, u32 abgr )
{
	u32 width	= ( u32 )rect.W();
	u32 height	= ( u32 )rect.H();
	u32 posX	= ( u32 )rect.X();
	u32 posY	= ( u32 )rect.Y();
	for ( u32 x=0; x<width; x++ )
	{
		for ( u32 y=0; y<height; y++ )
		{
			DrawPoint( math::CBasicVector2( ( dectype )(x+posX) , ( dectype )(y+posY) ) , abgr, 1 );
		}
	}
}

void CBmp32BitColor::DrawTriangle( const math::CBasicTriangle3& tri, u32 abgr )
{
	abgr;
	tri;
}

void CBmp32BitColor::DrawCircle( const math::CBasicCircle& circle , u32 abgr )
{
	u32 width	= ( u32 )m_Rect.GetSize().W;
	u32 height	= ( u32 )m_Rect.GetSize().H;
	u32 posX	= ( u32 )circle.X();
	u32 posY	= ( u32 )circle.Y();
	u32 radius_2 = ( u32 )(circle.R() * circle.R() );

	for ( u32 y=0; y<height; y++ )
	{
		for ( u32 x=0; x<width; x++ )
		{
			if ( ( (y-posY)*(y-posY) + (x-posX)*(x-posX) ) < radius_2 )
			{
				DrawPoint( math::CBasicVector2( ( dectype )x , ( dectype )y) , abgr, 1 );
			}
		}
	}
}

u32 CBmp32BitColor::GetPoint( const math::CBasicVector2 &pos )
{
	//�_���O����
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , pos.Y ) ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "It is beyond the limits of the bit map." ) ); //�a�����̊O�̓_�̐F�����擾���悤�Ƃ��܂����B
	}

	u32 Tgt = ( u32 )( ( m_pBmpInfo->bmiHeader.biWidth *pos.Y + pos.X ) );
	stdPixelInfo*pData = &( (stdPixelInfo*)m_pDataAdr)[ Tgt ];
	return MK_4BYTE_ID( pData->r, pData->g , pData->b, 255 );
}

void CBmp32BitColor::vLoad( stream::IStreamPtr &src )
{
	//���Z�b�g
	Reset();

	//�f�[�^��S�R�s�[����
	m_pData = src->vCloneData();

	//�f�[�^�T�C�Y���擾
	m_nDataSize = src->vGetSize();

	//�t�@�C���w�b�_�[�����
	m_pHed = ( BITMAPFILEHEADER * )m_pData;

	//�O�̂��߂Ƀt�@�C���T�C�Y���㏑������i���̏����́E�E�H�j
	m_pHed->bfSize = m_nDataSize;

	//��{���𓾂�
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );

	// �f�[�^���擪�A�h���X�擾
	m_pDataAdr = m_pData + m_pHed->bfOffBits;

	//��`���̍\�z
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )m_pBmpInfo->bmiHeader.biWidth, ( dectype )m_pBmpInfo->bmiHeader.biHeight ) );

	//���̓A���C�����g���
	m_LineWidth = PAGE_ALIGN( m_Rect.GetSize().W, cbmp32bitcolor::LINE_ALIGN_SIZE );
}

void CBmp32BitColor::vSave( stream::IStreamPtr &dest )
{
	//���ׂăX�g���[���ɏ�������
	dest->vWriteStream( m_pData , m_nDataSize );
}

void CBmp32BitColor::vCreate( u32 width, u32 height , u32 abgr )
{
	//���̓A���C�����g���
	m_LineWidth = PAGE_ALIGN( width, cbmp32bitcolor::LINE_ALIGN_SIZE );

	u32 dwSizeImage = ( m_LineWidth * height * cbmp32bitcolor::ONE_PIXEL_BYTE_SIZE );

	//�T�C�Y���擾
	m_nDataSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + dwSizeImage;

	//DIB�p�o�b�t�@�m��(BITMAPFILEHEADER
	m_pData = ( u8 * )SkyMalloc( m_nDataSize );

	//�t�@�C���w�b�_�[���쐬
	m_pHed = ( BITMAPFILEHEADER * )m_pData;
	CMemoryUtil_::ClearZero( m_pHed , sizeof( BITMAPFILEHEADER ) );
	m_pHed->bfType    = *( LPWORD )"BM";
	m_pHed->bfSize    = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + dwSizeImage;
	m_pHed->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );

	//��{�����쐬
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	CMemoryUtil_::ClearZero( m_pBmpInfo , sizeof( BITMAPINFOHEADER ) );
	m_pBmpInfo->bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	m_pBmpInfo->bmiHeader.biWidth		= width;
	m_pBmpInfo->bmiHeader.biHeight		= height;
	m_pBmpInfo->bmiHeader.biPlanes		= 1;
	m_pBmpInfo->bmiHeader.biBitCount	=  cbmp32bitcolor::ONE_PIXEL_BIT_SIZE;				//1�s�N�Z��������̏���4bit�܂�1�o�C�g�ɂ�2�s�N�Z��
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	// �f�[�^���擪�A�h���X�擾
	m_pDataAdr = ( u8 * )( m_pData + m_pHed->bfOffBits );
	CMemoryUtil_::ClearZero( m_pDataAdr , dwSizeImage );

	//��`���̍\�z
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )width, ( dectype )height ) );

	Clear( abgr );
}

static u32 GetDataColor( u32 data , const CFontBmpInfo &info )
{
	u32 result = 0xFFFFFF;
	u8* pResult = ( u8 * )&result;

	f32 X = ( f32 )data / info.in_eGradation;

	if ( info.in_isAlphaGradation == skyTrue )
	{
		//�f�[�^���O�̏ꍇ�͔w�i�F
		if ( data == 0 )
		{
			pResult[ 0 ] = CFTOI( info.in_clearcolor.b );
			pResult[ 1 ] = CFTOI( info.in_clearcolor.g );
			pResult[ 2 ] = CFTOI( info.in_clearcolor.r );
			pResult[ 3 ] = 0; //���S����
		}
		else
		{
			pResult[ 0 ] = CFTOI( info.in_color.b );
			pResult[ 1 ] = CFTOI( info.in_color.g );
			pResult[ 2 ] = CFTOI( info.in_color.r );
			pResult[ 3 ] = CFTOI( X );
		}
	}
	else
	{
		pResult[ 0 ] = CFTOI( ( ( info.in_color.b - info.in_clearcolor.b ) * X + info.in_clearcolor.b ) );
		pResult[ 1 ] = CFTOI( ( ( info.in_color.g - info.in_clearcolor.g ) * X + info.in_clearcolor.g ) );
		pResult[ 2 ] = CFTOI( ( ( info.in_color.r - info.in_clearcolor.r ) * X + info.in_clearcolor.r ) );
		pResult[ 3 ] = 0xFF; //�F�ŊK�����������Ă���̂Ŋ��S�s�����ɂ��Ă���B
	}

	return *( ( u32 * )&result );
}

void CBmp32BitColor::vWriteFontInit( const CFontBmpInfo &info )
{
	//�w�i�F�ŃN���A�i�C���f�b�N�X�J���[�̓p���b�g�O���w�i�F�j
	Clear( info.in_clearcolor.GetAsABGR() );
}

void CBmp32BitColor::vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info )
{
	info.w_pUvData->x = ( u32 )( info.out_outLine.W * pos.X );
	info.w_pUvData->y = ( u32 )( info.out_outLine.H * pos.Y );
	info.w_pUvData->w = ( u32 )( info.out_pUvData->fontWidth );
	info.w_pUvData->h = ( u32 )( info.out_pUvData->fontHeight );

	info.w_pUvData->ult = info.w_pUvData->x / info.out_pUvData->fileWidth;                                       //����t
	info.w_pUvData->vlt = info.w_pUvData->y / info.out_pUvData->fileHeight;                                      //����u
	info.w_pUvData->urt = ( info.w_pUvData->x + info.out_pUvData->fontWidth ) / info.out_pUvData->fileWidth;     //�E��t
	info.w_pUvData->vrt = info.w_pUvData->vlt;                                                                   //�E��u
	info.w_pUvData->ulb = info.w_pUvData->ult;                                                                   //�����t
	info.w_pUvData->vlb = ( info.w_pUvData->y + info.out_pUvData->fontHeight ) / info.out_pUvData->fileHeight;   //�����u
	info.w_pUvData->urb = info.w_pUvData->urt;
	info.w_pUvData->vrb = info.w_pUvData->vlb;

	//�r�b�g�}�b�v�̕��Ƃw���W�ƕ��������o���w���W����e�N�X�`�������o���w���W���Z�o����
	u32 sx = ( u32 )( info.out_outLine.W * pos.X + info.out_rect.X() );

	//�r�b�g�}�b�v�̕��Ƃx���W�ƕ��������o���x���W����e�N�X�`�������o���x���W���Z�o����
	u32 sy = ( u32 )( info.out_outLine.H * pos.Y + info.out_rect.Y() );

	if ( info.in_eGradation == fontbmp::eGradationType_2 )
	{//�Q�K������r�b�g�}�b�v�������ݏ���

		// �r�b�g�}�b�v�ꕶ���̉��̒������o�C�g���Z
		//�i�����F2�K���f�[�^�̓r�b�g�ŕ�����\�����Ă���B�������r�b�g�}�b�v��4byte�A���C�����g�Ȃ̂Ŏg��Ȃ��Ă�
		//  �Œ�4�o�C�g����B4�o�C�g��32bit�Ȃ̂ŕ�������32�܂ł͍Œ��4byte�ŕ\���ł���j
		u32 aw = PAGE_ALIGN( 1 + ( info.out_rect.W() / 32 ) , 4 );
		for ( u32 y = 0 ; y < info.out_rect.H() ; y++ )
		{
            for ( u32 x = 0 ; x < info.out_rect.W() ; x++ )
			{
				//�f�[�^����r�b�g�����擾
				DWORD num = x / 8;                                    // ���݂�x��1�s�̉�BYTE�ڂ����Z�o
				BYTE bit  = x % 8;                                    // ���݂�x��1�o�C�g���̉��r�b�g�ڂ����Z�o
				BYTE mask = ( ( BYTE ) 1 ) << ( 7 - bit );            // ���݂̃r�b�g���̃}�X�N�쐬
				BYTE Cur  = *( ( BYTE * )info.w_pData + aw * y + num ); // ���݂̃o�C�g�ʒu�ɂ���r�b�g�}�b�v�����擾
				Cur      &= mask;                                     // �쐬�����}�X�N�ƌ��݂̃o�C�g�ʒu�ƂŘ_���ω��Z
				u32 p     = ( Cur >> ( 7-bit ) );                     // Cur�ɗ������r�b�g�t���O���ŉ��ʃr�b�g�܂ŃV�t�g���ăA���t�@�l�ɕϊ�
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataColor( p , info ) );
			}
		}
	}
	else
	{//�Q�K������̊K���̃r�b�g�}�b�v�������ݏ���

		// �r�b�g�}�b�v�ꕶ���̉��̒������v�Z
		u32 aw = PAGE_ALIGN( info.out_rect.W() , 4 );
		for ( u32 y = 0 ; y < info.out_rect.H() ; y++ )
		{
			for ( u32 x = 0 ; x < aw ; x++ )
			{
				//�f�[�^����r�b�g�����擾
				u32 p = ( info.w_pData[ x + aw * y ] );
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataColor( p , info ) );
			}
		}
	}
}

#endif

} } }
