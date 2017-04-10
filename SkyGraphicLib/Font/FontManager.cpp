#include "StdAfx.h"
#include "SkyGraphicLib/Font.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/DataBase.h"

namespace sky { namespace lib { namespace graphic {

//UV値変換
inline dectype ConvertUVValue( dectype v )
{
#if defined( SKYLIB_RENDER_OPENGL4 )
	return ReverseValue( v );
#else
	return v;
#endif
}

static skyBool s_IsInit = skyFalse;

//文字ビットマップ情報データ
struct _FontBmpFileData
{
	const skyString*      bmpPath;
	const skyString*      uvPath;
};

//ゴシック文字テーブル
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

//明朝文字テーブル
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

//デフォルトフォント情報
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
{//もし速度に問題がでるようならこの処理自体を非同期化する必要があるかもしれない。

	if ( s_IsInit == skyTrue ) return;
	s_IsInit = skyTrue;

	s_StyleDataHash = NEW__( _StyleDataHash , _StyleDataHash( font::STYLE_HASH_SIZE ) );

	for ( u32 i = 0 ; i < array_sizeof( s_DefaultFontTable ) ; i++ )
	{
		//文字格納用ハッシュを作成する
		s_StyleDataHash->Set( s_DefaultFontTable[ i ].eStyle , NEW__( FontStyleData , FontStyleData() ) );

		//文字をハッシュに格納していく
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

	//子を消す
	FOREACH( _StyleDataHash , it , *s_StyleDataHash )
	{
		DEL *it;
	}

	SAFE_DELETE( s_StyleDataHash );
}

void CFontManager::Load( font::eFontStyleType eStyle , const skyString *bmpPath , const skyString *uvPath )
{
	//格納用のハッシュを取得
	FontStyleData *pFontStyleData = s_StyleDataHash->Get( eStyle );

	//テクスチャのロード
	if ( CDataBase_::IsCreatedTexture( CHS( bmpPath ) ) == skyFalse )
	{
//		TIMER_OBJCET( L"textrue Load" ); //負荷計測

		pFontStyleData->spTexture = CTextureCreateName_( CHS( bmpPath ) );
		CRenderCommand::LoadTexture( pFontStyleData->spTexture );
	}
	else
	{
		//データベースから参照を取得
		pFontStyleData->spTexture = CDataBase_::GetTexture( CHS( bmpPath ) , skyFalse , skyTrue );

		//まだロードされていなかったらロードする
		if ( pFontStyleData->spTexture->IsLoaded() == skyFalse )
		{
			pFontStyleData->spTexture->Activate();
		}
	}

	//ＵＶバイナリのロード
	stream::IStreamPtr spFile = CFileUtil_::LoadFile( CHS( uvPath ) );

	//ＵＶバイナリファイルヘッダを取得
	fontbmp::CUvFileHeader *pUvHeader = ( fontbmp::CUvFileHeader * )spFile->vGetData();

//	{ TIMER_OBJCET( L"data create" ); //負荷計測

	//ＵＶデータを一つ一つ構築してコンテナに格納する
	fontbmp::CUvData *pUvData = ( fontbmp::CUvData * )( ( u8 * )pUvHeader + sizeof( fontbmp::CUvFileHeader ) );
	for( u32 i = 0 ; i < pUvHeader->num ; i++ )
	{
		//ハッシュに文字コードをキーに領域を確保する
		COneFontData &data = pFontStyleData->fontHash.Set( pUvData[ i ].code ); //★これが重い

		//文字の幅と高さを渡す
		data.size.W = pUvData[ i ].w;
		data.size.H = pUvData[ i ].h;

		dectype oneX = ( dectype )data.size.W / 2;
		dectype oneY = ( dectype )data.size.H / 2;

		//頂点の座標を設定
		data.pos[ 0 ].Set( -oneX ,  oneY ); //左上
		data.pos[ 1 ].Set(  oneX ,  oneY ); //右上
		data.pos[ 2 ].Set( -oneX , -oneY ); //左下
		data.pos[ 3 ].Set(  oneX , -oneY ); //右下

		//ＵＶ座標を設定
		data.uv[ 0 ].Set( pUvData[ i ].ult , ConvertUVValue( ( dectype )pUvData[ i ].vlt ) ); //左上
		data.uv[ 1 ].Set( pUvData[ i ].urt , ConvertUVValue( ( dectype )pUvData[ i ].vrt ) ); //右上
		data.uv[ 2 ].Set( pUvData[ i ].ulb , ConvertUVValue( ( dectype )pUvData[ i ].vlb ) ); //左下
		data.uv[ 3 ].Set( pUvData[ i ].urb , ConvertUVValue( ( dectype )pUvData[ i ].vrb ) ); //右下
	}

//	} //負荷計測
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
