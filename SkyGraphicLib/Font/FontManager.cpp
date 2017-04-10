#include "StdAfx.h"
#include "SkyGraphicLib/Font.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/DataBase.h"

namespace sky { namespace lib { namespace graphic {

//UV�l�ϊ�
inline dectype ConvertUVValue( dectype v )
{
#if defined( SKYLIB_RENDER_OPENGL4 )
	return ReverseValue( v );
#else
	return v;
#endif
}

static skyBool s_IsInit = skyFalse;

//�����r�b�g�}�b�v���f�[�^
struct _FontBmpFileData
{
	const skyString*      bmpPath;
	const skyString*      uvPath;
};

//�S�V�b�N�����e�[�u��
static _FontBmpFileData s_Gosic11FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Gosic11.png" ) , _T( "data/lib/font/FontData_Gosic11.uvb" ) } ,
};
static _FontBmpFileData s_Gosic13FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Gosic13.png" ) , _T( "data/lib/font/FontData_Gosic13.uvb" ) } ,
};
static _FontBmpFileData s_Gosic16FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Gosic16.png" ) , _T( "data/lib/font/FontData_Gosic16.uvb" ) } ,
};

//���������e�[�u��
static _FontBmpFileData s_Mincho11FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Mincho11.png" ) , _T( "data/lib/font/FontData_Mincho11.uvb" ) } ,
};
static _FontBmpFileData s_Mincho13FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Mincho13.png" ) , _T( "data/lib/font/FontData_Mincho13.uvb" ) } ,
};
static _FontBmpFileData s_Mincho16FontDataTable[] =
{
	{ _T( "data/lib/font/FontData_Mincho16.png" ) , _T( "data/lib/font/FontData_Mincho16.uvb" ) } ,
};

//�f�t�H���g�t�H���g���
struct _DefaultFontTable
{
	font::eFontStyleType  eStyle;
	_FontBmpFileData*     pTbl;
	u32                   count;
};
static _DefaultFontTable s_DefaultFontTable[] =
{
	{ font::eFontStyleType_Gosic11  , s_Gosic11FontDataTable  , array_sizeof( s_Gosic11FontDataTable ) },
	{ font::eFontStyleType_Gosic13  , s_Gosic13FontDataTable  , array_sizeof( s_Gosic13FontDataTable ) },
//	{ font::eFontStyleType_Gosic16  , s_Gosic16FontDataTable  , array_sizeof( s_Gosic16FontDataTable ) },
//	{ font::eFontStyleType_Mincho11 , s_Mincho11FontDataTable , array_sizeof( s_Mincho11FontDataTable ) },
//	{ font::eFontStyleType_Mincho13 , s_Mincho13FontDataTable , array_sizeof( s_Mincho13FontDataTable ) },
//	{ font::eFontStyleType_Mincho16 , s_Mincho16FontDataTable , array_sizeof( s_Mincho16FontDataTable ) },
};

typedef CHash< COneFontData >       COneFontDataHash;
struct FontStyleData
{
	CTexturePtr       spTexture;
	COneFontDataHash  fontHash;
	FontStyleData() : fontHash( font::FONT_HASH_SIZE ){}
};

typedef CHash< FontStyleData * > _StyleDataHash;

static _StyleDataHash *s_StyleDataHash = skyNull;

void CFontManager::Initialize()
{//�������x�ɖ�肪�ł�悤�Ȃ炱�̏������̂�񓯊�������K�v�����邩������Ȃ��B

	if ( s_IsInit == skyTrue ) return;
	s_IsInit = skyTrue;

	s_StyleDataHash = NEW__( _StyleDataHash , _StyleDataHash( font::STYLE_HASH_SIZE ) );

	for ( u32 i = 0 ; i < array_sizeof( s_DefaultFontTable ) ; i++ )
	{
		//�����i�[�p�n�b�V�����쐬����
		s_StyleDataHash->Set( s_DefaultFontTable[ i ].eStyle , NEW__( FontStyleData , FontStyleData() ) );

		//�������n�b�V���Ɋi�[���Ă���
		for( u32 j = 0 ; j < s_DefaultFontTable[ i ].count ; j++ )
		{
			_FontBmpFileData *pData = &s_DefaultFontTable[ i ].pTbl[ j ];

			Load( s_DefaultFontTable[ i ].eStyle , pData->bmpPath , pData->uvPath );
		}
	}
}

void CFontManager::Dispose()
{
	if ( s_StyleDataHash == NULL ) return;

	//�q������
	FOREACH( _StyleDataHash , it , *s_StyleDataHash )
	{
		DEL *it;
	}

	SAFE_DELETE( s_StyleDataHash );
}

void CFontManager::Load( font::eFontStyleType eStyle , const skyString *bmpPath , const skyString *uvPath )
{
	//�i�[�p�̃n�b�V�����擾
	FontStyleData *pFontStyleData = s_StyleDataHash->Get( eStyle );

	//�e�N�X�`���̃��[�h
	if ( CDataBase_::IsCreatedTexture( CHS( bmpPath ) ) == skyFalse )
	{
//		TIMER_OBJCET( L"textrue Load" ); //���׌v��

		pFontStyleData->spTexture = CTextureCreateName_( CHS( bmpPath ) );
		CRenderCommand::LoadTexture( pFontStyleData->spTexture );
	}
	else
	{
		//�f�[�^�x�[�X����Q�Ƃ��擾
		pFontStyleData->spTexture = CDataBase_::GetTexture( CHS( bmpPath ) , skyFalse , skyTrue );

		//�܂����[�h����Ă��Ȃ������烍�[�h����
		if ( pFontStyleData->spTexture->IsLoaded() == skyFalse )
		{
			pFontStyleData->spTexture->Activate();
		}
	}

	//�t�u�o�C�i���̃��[�h
	stream::IStreamPtr spFile = CFileUtil_::LoadFile( CHS( uvPath ) );

	//�t�u�o�C�i���t�@�C���w�b�_���擾
	fontbmp::CUvFileHeader *pUvHeader = ( fontbmp::CUvFileHeader * )spFile->vGetData();

//	{ TIMER_OBJCET( L"data create" ); //���׌v��

	//�t�u�f�[�^�����\�z���ăR���e�i�Ɋi�[����
	fontbmp::CUvData *pUvData = ( fontbmp::CUvData * )( ( u8 * )pUvHeader + sizeof( fontbmp::CUvFileHeader ) );
	for( u32 i = 0 ; i < pUvHeader->num ; i++ )
	{
		//�n�b�V���ɕ����R�[�h���L�[�ɗ̈���m�ۂ���
		COneFontData &data = pFontStyleData->fontHash.Set( pUvData[ i ].code ); //�����ꂪ�d��

		//�����̕��ƍ�����n��
		data.size.W = pUvData[ i ].w;
		data.size.H = pUvData[ i ].h;

		dectype oneX = ( dectype )data.size.W / 2;
		dectype oneY = ( dectype )data.size.H / 2;

		//���_�̍��W��ݒ�
		data.pos[ 0 ].Set( -oneX ,  oneY ); //����
		data.pos[ 1 ].Set(  oneX ,  oneY ); //�E��
		data.pos[ 2 ].Set( -oneX , -oneY ); //����
		data.pos[ 3 ].Set(  oneX , -oneY ); //�E��

		//�t�u���W��ݒ�
		data.uv[ 0 ].Set( pUvData[ i ].ult , ConvertUVValue( ( dectype )pUvData[ i ].vlt ) ); //����
		data.uv[ 1 ].Set( pUvData[ i ].urt , ConvertUVValue( ( dectype )pUvData[ i ].vrt ) ); //�E��
		data.uv[ 2 ].Set( pUvData[ i ].ulb , ConvertUVValue( ( dectype )pUvData[ i ].vlb ) ); //����
		data.uv[ 3 ].Set( pUvData[ i ].urb , ConvertUVValue( ( dectype )pUvData[ i ].vrb ) ); //�E��
	}

//	} //���׌v��
}

COneFontData &CFontManager::GetFont( u32 srcCode , font::eFontStyleType eStyle )
{
    SKY_ASSERT_MSG( s_StyleDataHash != skyNull , _T( "CFontManager is not inishalized. CFontManager must inishalize after device inishalize.\n" ) );

	FontStyleData *pFonts;
	s_StyleDataHash->Get( eStyle , pFonts );
	return pFonts->fontHash.Get( srcCode );
}

CTexturePtr &CFontManager::GetFontStyleTex( font::eFontStyleType eStyle)
{
    SKY_ASSERT_MSG( s_StyleDataHash != skyNull , _T( "CFontManager is not inishalized. CFontManager must inishalize after device inishalize.\n" ) );

	FontStyleData *pFonts;
	s_StyleDataHash->Get( eStyle , pFonts );
	return pFonts->spTexture;
}

} } }
