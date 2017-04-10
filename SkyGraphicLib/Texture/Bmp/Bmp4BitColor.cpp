#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

using namespace cbmp4bitcolor;

CBmp4BitColor::CBmp4BitColor() :
AbsBmpFile( fontbmp::eBmpKind_bmp4bit ) ,
m_NoDrawPalletIdx( cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE )
{
}

u32 CBmp4BitColor::GetDataSize()
{
	return ( u32 )(m_pBmpInfo->bmiHeader.biWidth * m_pBmpInfo->bmiHeader.biHeight * cbmp4bitcolor::ONE_PIXEL_BYTE_SIZE);
}

void CBmp4BitColor::Clear( u32 palletNo )
{
	DrawRect( m_Rect, palletNo );
}

void CBmp4BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo )
{
	//�f�[�^��������n�܂��Ă���̂�Y���W�𔽓]������
	dectype dotPosY = (m_Rect.GetSize().H-1)-pos.Y;

	//�_���O����
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , dotPosY ) ) == skyFalse ) return;

	//�񏑂����ݔ���
	if ( m_NoDrawPalletIdx != cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE )
	{
		if ( m_NoDrawPalletIdx == ( s32 )GetPoint( math::CBasicVector2( pos.X , pos.Y ) ) )
		{
			return;
		}
	}

	//�C���f�b�N�X�̐擪�|�C���^�擾
	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) / 2 );
	if( ( u32 )( pos.X ) & 0x1 )
	{
		m_pDataAdr[ Tgt ] = ( u8 )( ( m_pDataAdr[ Tgt ] & 0xF0 ) | palletNo );
	}
	else
	{
		m_pDataAdr[ Tgt ] = ( u8 )( ( palletNo << 4 ) | ( m_pDataAdr[ Tgt ] & 0xF ) );
	}
}

void CBmp4BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize )
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

void CBmp4BitColor::DrawLine( const math::CBasicVector2& start, const math::CBasicVector2& Gole , u32 palletNo, u8 boldSize )
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
		
		DrawPoint( math::CBasicVector2( ( dectype )drawBasePosX , ( dectype )drawBasePosY ) , palletNo, boldSize );
	}
}

void CBmp4BitColor::DrawTile( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 fromPalNo	= 0;
	s32 toPalNo	= 0;
	dectype tileWNum = m_Rect.GetSize().W / bmpFile.GetRect().GetSize().W;
	dectype tileHNum = m_Rect.GetSize().H / bmpFile.GetRect().GetSize().H;
	
	for( u32 tw = 0; tw<tileWNum; tw++ )
	{
		for( u32 th = 0; th<tileHNum; th++ )
		{
			for ( dectype x=0; x<width; x++ )
			{
				for ( dectype y=0; y<height; y++ )
				{
					fromPalNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
					toPalNo	  = GetPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) );
					if( copyPalNo == NO_DRAW_PALLET_IDX_NONE || copyPalNo == toPalNo )
					{
						DrawPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) , fromPalNo );
					}
				}
			}
		}
	}
}

void CBmp4BitColor::DrawRect( const math::CBasicRect& rect, u32 palletNo )
{
	dectype width	= rect.W();
	dectype height	= rect.H();
	dectype posX	= rect.X();
	dectype posY	= rect.Y();
	for ( dectype x=0; x<width; x++ )
	{
		for ( dectype y=0; y<height; y++ )
		{
			DrawPoint( math::CBasicVector2( x+posX , y+posY ) , palletNo, 1 );
		}
	}
}

void CBmp4BitColor::DrawTriangle( const math::CBasicTriangle3& tri, u32 palletNo )
{
	tri;
	palletNo;
}

void CBmp4BitColor::DrawCircle( const math::CBasicCircle& circle , u32 palletNo )
{
	dectype width	= m_Rect.GetSize().W;
	dectype height	= m_Rect.GetSize().H;
	dectype posX	= circle.X();
	dectype posY	= circle.Y();
	dectype radius_2 = circle.R() * circle.R();

	for ( dectype y=0; y<height; y++ )
	{
		for ( dectype x=0; x<width; x++ )
		{
			if ( ( (y-posY)*(y-posY) + (x-posX)*(x-posX) ) < radius_2 )
			{
				DrawPoint( math::CBasicVector2( x , y) , palletNo );
			}
		}
	}
}

void CBmp4BitColor::DrawBmp( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 fromPalNo	= 0;
	s32 toPalNo	= 0;
	
	for ( dectype x=0; x<width; x++ )
	{
		for ( dectype y=0; y<height; y++ )
		{
			fromPalNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
			toPalNo	  = GetPoint( math::CBasicVector2( x+posX , y+posY ) );
			if( copyPalNo == NO_DRAW_PALLET_IDX_NONE || copyPalNo == toPalNo )
			{
				DrawPoint( math::CBasicVector2( x+posX , y+posY ) , fromPalNo );
			}
		}
	}
}

u32 CBmp4BitColor::GetPoint( const math::CBasicVector2 &pos )
{
	//�f�[�^��������n�܂��Ă���̂�Y���W�𔽓]������
	dectype dotPosY = ( (m_Rect.GetSize().H-1)-pos.Y);

	//�_���O����
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , dotPosY ) ) == skyFalse )
	{
		return cbmp4bitcolor::NO_PALLET_IDX;
	}

	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) / 2 );
	if ( ( u32 )pos.X & 0x1 )
	{
		return ( u8 )( m_pDataAdr[ Tgt ] & 0x0F );
	}
	else
	{
		return ( u8 )( (m_pDataAdr[ Tgt ] >> 4) & 0x0F );
	}
}

void CBmp4BitColor::ReSize( dectype sc )
{
	//�V�����c���T�C�Y���v�Z����
	u32 new_Width  = ( u32 )(m_Rect.GetSize().W * sc);
	u32 new_Height = ( u32 )(m_Rect.GetSize().H * sc);

	//�V����Bmp���쐬����
	CBmp4BitColor *pNew_Bmp = CloneBmp( new_Width, new_Height , 0 );

	//��f�ʒu���X�P�[�����Ȃ���R�s�[���Ă���
	math::CMatrix3x2	scCMatrix3x2;
	scCMatrix3x2.SetupScale( sky::lib::math::CBasicVector2( sc, sc ) );
	math::CBasicVector2 fromPos , toPos;
	for ( dectype w=0; w<m_Rect.GetSize().W; w++ )
	{
		for ( dectype h=0; h<m_Rect.GetSize().H; h++ )
		{
			fromPos.Set( w, h );
			toPos.Set( w, h );
			toPos *= scCMatrix3x2;
			toPos.Floor();
			pNew_Bmp->DrawPoint( toPos , GetPoint( fromPos ) );
		}
	}

	//�Â��̈��V�����T�C�Y�Ń��A���b�N����
	m_pData = ( u8 * )SkyRealloc( m_pData , pNew_Bmp->m_nDataSize );

	//���A���b�N���ꂽ�̈�ɐV����������������
	CMemoryUtil_::Copy( m_pData , pNew_Bmp->m_pData , pNew_Bmp->m_nDataSize );

	//�e����̍X�V
	m_pHed			= ( BITMAPFILEHEADER * )m_pData;
	m_pBmpInfo		= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	m_pPalletAdr	= ( RGBQUAD * )m_pBmpInfo->bmiColors;
	m_pDataAdr		= ( u8 * )( m_pData + m_pHed->bfOffBits );
	m_LineWidth		= PAGE_ALIGN( new_Width, cbmp4bitcolor::LINE_ALIGN_SIZE );
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )new_Width, ( dectype )new_Height ) );

	//�V��������j������
	SkyFree( pNew_Bmp );
}


RGBQUAD* CBmp4BitColor::GetPalletAdr()
{
	return m_pPalletAdr;
}

void CBmp4BitColor::SetPallet( CBmp4BitColor &bmpFile )
{
	CMemoryUtil_::Copy( m_pPalletAdr, bmpFile.GetPalletAdr() , cbmp4bitcolor::PALLET_SIZE );
}

void CBmp4BitColor::PaintsOut_Child( math::CBasicVector2& vec2, u32 startPalNo, u32 paintsOutPalNo, CBmp4BitColor& flgBmp )
{
	CStack<math::CBasicVector2*>	st( ( u32 )(m_Rect.GetSize().W * m_Rect.GetSize().H) );

	//�ŏ��̈��Push
	st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( vec2.X , vec2.Y ) ) );

	math::CBasicVector2*pCheckVec2;
	while (st.GetNum() > 0) //�X�^�b�N����l���Ȃ��Ȃ�����I��
	{
		//�X�^�b�N�̍ŏ�i�̍��W�𔻒�
		st.GetItem( st.GetNum() - 1 , pCheckVec2 );

		//�_��ł��A�t���O�𗧂Ă�
		u32 pos = GetPoint( * pCheckVec2 );
		DrawPoint( * pCheckVec2 , paintsOutPalNo );
		pos = GetPoint( * pCheckVec2 );
		flgBmp.DrawPoint( * pCheckVec2 , 1 );

		//������
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X - 1 , pCheckVec2->Y ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2( pCheckVec2->X - 1 , pCheckVec2->Y ) ) == 0 &&
			 pCheckVec2->X-1 < m_Rect.GetSize().W &&
			 pCheckVec2->Y < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X-1, pCheckVec2->Y ) ) );
			continue;
		}

		//������
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) == 0 &&
			 pCheckVec2->X   < m_Rect.GetSize().W &&
			 pCheckVec2->Y+1 < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) );
			continue;
		}

		//�E����
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X+1, pCheckVec2->Y ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2(pCheckVec2->X+1, pCheckVec2->Y ) ) == 0 &&
			 pCheckVec2->X+1 < m_Rect.GetSize().W &&
			 pCheckVec2->Y   < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X+1, pCheckVec2->Y ) ) );
			continue;
		}

		//�����
		if ( GetPoint( math::CBasicVector2(pCheckVec2->X , pCheckVec2->Y-1 ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2(pCheckVec2->X , pCheckVec2->Y-1 ) ) == 0 &&
			 pCheckVec2->X   < m_Rect.GetSize().W &&
			 pCheckVec2->Y-1 < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y-1 ) ) );
			continue;
		}

		//����̏I��������W��Pop
		math::CBasicVector2 *pVec;

		if ( st.Pop( pVec ) == skyTrue )
		{
			DEL pVec;
		}
	}
}
void CBmp4BitColor::PaintsOut( math::CBasicVector2& vec2, u32 palletNo, CBmp4BitColor& flgBmp )
{
	//���ۂ̓h��Ԃ������B
	PaintsOut_Child( vec2, GetPoint( vec2 ) , palletNo, flgBmp );
}

void CBmp4BitColor::vLoad( stream::IStreamPtr &src )
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

	// �p���b�g���擪�A�h���X�擾
	m_pPalletAdr = ( RGBQUAD * )m_pBmpInfo->bmiColors;

	// �f�[�^���擪�A�h���X�擾
	m_pDataAdr = m_pData + m_pHed->bfOffBits;

	//��`���̍\�z
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )m_pBmpInfo->bmiHeader.biWidth, ( dectype )m_pBmpInfo->bmiHeader.biHeight ) );

	//���̓A���C�����g���
	m_LineWidth = PAGE_ALIGN( m_Rect.GetSize().W, cbmp4bitcolor::LINE_ALIGN_SIZE );
}

void CBmp4BitColor::vSave( stream::IStreamPtr &dest )
{
	//���ׂăX�g���[���ɏ�������
	dest->vWriteStream( m_pData , m_nDataSize );
}


void CBmp4BitColor::vCreate( u32 width, u32 height, u32 palletNo )
{
	//���̓A���C�����g���
	m_LineWidth = PAGE_ALIGN( width, cbmp4bitcolor::LINE_ALIGN_SIZE );

	u32 dwSizeImage = ( u32 )( m_LineWidth * height * cbmp4bitcolor::ONE_PIXEL_BYTE_SIZE );

	//�t�@�C���S�̃T�C�Y
	m_nDataSize = sizeof(BITMAPFILEHEADER) +	
				 sizeof(BITMAPINFOHEADER) +
				 cbmp4bitcolor::PALLET_SIZE + 
				 dwSizeImage;

	//DIB�p�o�b�t�@�m��(BITMAPFILEHEADER
	m_pData = ( u8 * )SkyMalloc( m_nDataSize );

	//�t�@�C���w�b�_�[���쐬
	m_pHed = ( BITMAPFILEHEADER * )m_pData;
	CMemoryUtil_::ClearZero( m_pHed , sizeof( BITMAPFILEHEADER ) );
	m_pHed->bfType    = *( LPWORD )"BM";
	m_pHed->bfSize    = m_nDataSize;
	m_pHed->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + cbmp4bitcolor::PALLET_SIZE;

	//��{�����쐬
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	CMemoryUtil_::ClearZero( m_pBmpInfo , sizeof( BITMAPINFOHEADER ) );
	m_pBmpInfo->bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	m_pBmpInfo->bmiHeader.biWidth		= width;
	m_pBmpInfo->bmiHeader.biHeight		= height;
	m_pBmpInfo->bmiHeader.biPlanes		= 1;
	m_pBmpInfo->bmiHeader.biBitCount	= (WORD)cbmp4bitcolor::ONE_PIXEL_BIT_SIZE;				//1�s�N�Z��������̏���4bit�܂�1�o�C�g�ɂ�2�s�N�Z��
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	// �p���b�g���擪�A�h���X�擾
	m_pPalletAdr = ( RGBQUAD * )m_pBmpInfo->bmiColors;
	CMemoryUtil_::ClearZero( m_pPalletAdr, cbmp4bitcolor::PALLET_SIZE );

	//�f�t�H���g�p���b�g�i�K���j
	RGBQUAD pal[] = 
	{
		{ 255 , 255 , 255 , 100 }, //0:��
		{ 255 , 0   , 0   , 100 }, //1:��
		{ 0   , 255 , 0   , 100 }, //2:��
		{ 0   , 0   , 255 , 100 }, //3:��
		{ 255 , 255 , 255 , 100 }, //4:��
	};
	u32 palNum = array_sizeof( pal );
	for ( u32 i = 0; i < palNum ; i++ )
	{
		m_pPalletAdr[ i ] = pal[ i ];
	}

	// �f�[�^���擪�A�h���X�擾
	m_pDataAdr = ( u8 * )( m_pData + m_pHed->bfOffBits );
	CMemoryUtil_::ClearZero( m_pDataAdr , dwSizeImage );

	//��`���̍\�z
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )width, ( dectype )height ) ); //��`���͔z��̃C���f�b�N�X�Ƃ������̂ŁA������-1����

	Clear( palletNo );
}

CBmp4BitColor * CBmp4BitColor::CloneBmp( u32 width, u32 height, u32 palletNo )
{
	//�V����Bmp���쐬����
	CBmp4BitColor *pNew_Bmp = NEW__( CBmp4BitColor , CBmp4BitColor() );
	pNew_Bmp->vCreate( width, height, palletNo );

	//�p���b�g���R�s�[
	pNew_Bmp->SetPallet( *this );

	return pNew_Bmp;
}

void CBmp4BitColor::SetNoDrawPalletNo( u32 palletNo )
{
	m_NoDrawPalletIdx = palletNo;
}

void CBmp4BitColor::SetNoDrawPalletNoClear()
{
	m_NoDrawPalletIdx = cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE;
}

//�K���f�[�^���p���b�g�ԍ��ɕϊ�����
static u32 GetDataPallet( fontbmp::eGradationType eType , u32 data )
{
	u32 result = 0;
	switch( eType )
	{//�Q�ƂT�K���̓f�[�^�����̂܂܎g�p�ł���
		case fontbmp::eGradationType_2:
		case fontbmp::eGradationType_5:
			result = data;
			break;
		case fontbmp::eGradationType_17:
		case fontbmp::eGradationType_65:
			result = COL_HIRITU_1( eType , data , cbmp4bitcolor::PALLET_NUM );
			break;
	}
	return result;
}

void CBmp4BitColor::vWriteFontInit( const CFontBmpInfo &info )
{
	SKY_UNUSED_ARG( info );

	//�w�i�F�ŃN���A�i�C���f�b�N�X�J���[�̓p���b�g�O���w�i�F�j
	Clear( 0 );
}

void CBmp4BitColor::vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info )
{
	info.w_pUvData->x = ( u32 )( info.out_outLine.W * pos.X );
	info.w_pUvData->y = ( u32 )( info.out_outLine.H * pos.Y );
	info.w_pUvData->w = ( u32 )( info.out_outLine.W );
	info.w_pUvData->h = ( u32 )( info.out_outLine.H );

	info.w_pUvData->ult = info.w_pUvData->x / info.out_pUvData->fileWidth;                                       //����t
	info.w_pUvData->vlt = info.w_pUvData->y / info.out_pUvData->fileHeight;                                      //����u
	info.w_pUvData->urt = ( info.w_pUvData->x + info.out_outLine.W ) / info.out_pUvData->fileWidth;     //�E��t
	info.w_pUvData->vrt = info.w_pUvData->vlt;                                                                   //�E��u
	info.w_pUvData->ulb = info.w_pUvData->ult;                                                                   //�����t
	info.w_pUvData->vlb = ( info.w_pUvData->y + info.out_outLine.H ) / info.out_pUvData->fileHeight;   //�����u
	info.w_pUvData->urb = info.w_pUvData->urt;
	info.w_pUvData->vrb = info.w_pUvData->vlb;

	//�r�b�g�}�b�v�̕��Ƃw���W�ƕ��������o���w���W����e�N�X�`�������o���w���W���Z�o����
	u32 sx = ( u32 )( info.w_pUvData->x + info.out_rect.X() );

	//�r�b�g�}�b�v�̕��Ƃx���W�ƕ��������o���x���W����e�N�X�`�������o���x���W���Z�o����
	u32 sy = ( u32 )( info.w_pUvData->y + info.out_rect.Y() );

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
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , p );
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
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataPallet( info.in_eGradation , p ) );
			}
		}
	}
}

void CBmp4BitColor::vMakeColorPallet ( const CFontBmpInfo &info )
{
	u32 colornum = ( cbmp4bitcolor::PALLET_NUM > info.in_eGradation ) ? info.in_eGradation : cbmp4bitcolor::PALLET_NUM;
	colornum--; //�v�Z�̂��߂ЂƂ��炷�B
	for( u32 i = 0 ; i <= colornum ; i++ ) //�p���b�g�O����~���l�߂Ă���
	{
		f32 X = ( f32 )i / colornum;

		//�A���t�@�O���f�[�V�����̏ꍇ�̓A���t�@�l�ŊK����\������
		if ( info.in_isAlphaGradation )
		{
			if ( i == 0 ) //�p���b�g�O�͔w�i�F
			{
				m_pPalletAdr[ i ].rgbBlue     = CFTOI( info.in_clearcolor.b );
				m_pPalletAdr[ i ].rgbGreen    = CFTOI( info.in_clearcolor.g );
				m_pPalletAdr[ i ].rgbRed      = CFTOI( info.in_clearcolor.r );
				m_pPalletAdr[ i ].rgbReserved = 0; //���S����
			}
			else
			{
				m_pPalletAdr[ i ].rgbBlue     = CFTOI( info.in_color.b );
				m_pPalletAdr[ i ].rgbGreen    = CFTOI( info.in_color.g );
				m_pPalletAdr[ i ].rgbRed      = CFTOI( info.in_color.r );
				m_pPalletAdr[ i ].rgbReserved = CFTOI( X );
			}
		}
		else
		{
			m_pPalletAdr[ i ].rgbBlue     = CFTOI( ( ( info.in_color.b - info.in_clearcolor.b ) * X + info.in_clearcolor.b ) );
			m_pPalletAdr[ i ].rgbGreen    = CFTOI( ( ( info.in_color.g - info.in_clearcolor.g ) * X + info.in_clearcolor.g ) );
			m_pPalletAdr[ i ].rgbRed      = CFTOI( ( ( info.in_color.r - info.in_clearcolor.r ) * X + info.in_clearcolor.r ) );
			m_pPalletAdr[ i ].rgbReserved = 255;
		}
	}
}

#endif

} } }
