#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file {

//バイナリプロパティの解析
static void _ParseBinary( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	

	//ファイルヘッダーを読む
	property::FileHeader fileheader;
	src->vReadStream( ( u8 * )&fileheader , sizeof( property::FileHeader ) );

	//プロパティの数でループ
	for ( u32 i = 0 ; i < fileheader.nPropertyCount ; i++ )
	{
		//データヘッダーを読む
		property::DataHeader dataHeader;
		src->vReadStream( ( u8 * )&dataHeader , sizeof( property::DataHeader ) );

		u32 readSize = 0;

		//キーを読む
		string::CString key = ( skyString * )src->vGetSeekData();
		readSize = key.GetSize();
		src->vAddSeek( key.GetSize() );//キー分シークを進める
//※1   hashtype key;
//      CMemoryUtil_::Copy( &key , src->vGetSeekData() , GET_HASH_VALUE_SIZE() );
//      src->vAddSeek( GET_HASH_VALUE_SIZE() );//キー分シークを進める

		hash::CHashStringPtrListPtr valueList( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
		while ( readSize < dataHeader.valueSize )
		{
			//値を読む
			string::CString value = ( skyString * )src->vGetSeekData();
			valueList->AddTail( CHS( value.Get() ) );
			readSize += value.GetSize();
			src->vAddSeek( value.GetSize() );//値分シークを進める
		}

		dest.Set( key , valueList );
	}
}

//テキストプロパティデータの解析
static void _ParseText( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	

#ifdef SKYLIB_PLATFORM_WIN32

	//プログラムがUnicodeの場合ファイルもUnicodeでなければならない
	if ( CStringUtil_::IsProgramUnicode() )
	{
		SKY_ASSERT_MSG( CMemoryUtil_::Cmp( src->vGetData() , &BOM , BOM_SIZE ) , _T( "Please make the file Unicode because the program is Unicode. " ) ); //プログラムがUnicodeなのでファイルもUnicodeにしてください。
	}

#endif

	//BOMがあった場合、それを読み飛ばす
	if ( CMemoryUtil_::Cmp( src->vGetData() , &BOM , BOM_SIZE ) )
	{
		u16 bom;
		src->vReadStream( ( u8 *)&bom , BOM_SIZE );
	}

	//ストリームから一文字ずつ読み込んでいく
    skyString c = 0;
	u32 mode = 0; //0がkey読み込みモード、1がvalue読み込みモード
	string::CStringBuffer key;
	string::CStringBuffer value;
	hash::CHashStringPtrListPtr valueList( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
	u32 lineNum = 0;
	while( src->vReadStream( ( u8* )&c , sizeof( skyString ) ) > 0 )
	{
		//空白だったらすっとばす
		if ( c == EG || c == _T( ' ' ) || c == _T( '	' ) )
		{
			continue;
		}

		//コメントだったら処理をすっとばす
		if ( c == _T( '#' ) )
		{
			//次の改行コードまで一気に読む。読んだら処理を進める。次のifステートメントが必ず処理を捕まえる。
			while( c != EC )
			{
				if ( src->vReadStream( ( u8* )&c , sizeof( skyString ) ) == 0 )
				{
					c = EC;
					break;
				}
			}
		}

		if ( c == EC || c == EOF )
		{//keyと値が確定した

			lineNum++;

			//keyが全くの空白だった場合空行だったと想定して次に行く
            if ( key == _T( "" ) )
			{
				continue;
			}

			//値があれば格納(最後の１文字がカンマの場合、値は空白になってるはず）
            if ( value != _T( "" ) )
			{
				valueList->AddTail( CHS( value.Get() ) );
			}

			//value読み込みモードになってなかったらエラー
			SKY_ASSERT_MSG( mode == 1 , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //プロパティファイルエラー %d行目[ %s ] 記述が不正です。キーと値は=で区切ってください。
			
			//値が空だったらエラー
			SKY_ASSERT_MSG( valueList->GetNum() > 0 , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //プロパティファイルエラー %d行目[ %s ] 記述が不正です。 値が一つも無いです。

			//キーが重複したらエラー
			SKY_ASSERT_MSG( !dest.IsGet( CHS( key.Get() ) ) , _T( "Property file error. line [ %d ] data [ %s ]." ) , lineNum , key.Get() ); //プロパティファイルエラー %d行目[ %s ] キーが重複しています。

			dest.Set( CHS( key.Get() ) , valueList );
			key.Free();
			value.Free();
			mode = 0;
			valueList.Delete(); //管理しているアドレスを破棄。dest内に引き継がれているから参照カウンタが下がるだけ
			valueList.SetPtr( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
			continue;
		}

		if ( c == _T( '=' ) )
		{//キーが確定した

			mode = 1;
			continue;
		}

		if ( mode == 0 )
		{//キーを構築
			key += c;
		}
		else
		{
			if ( c == _T( ',' ) )
			{//値を確定する

				valueList->AddTail( CHS( value.Get() ) );
				value.Free();
			}
			else
			{//値を構築
				value += c;
			}
		}
	}

    if ( key != _T( "" ) )
	{
		//値があれば格納(最後の１文字がカンマの場合、値は空白になってるはず）
        if ( value != _T( "" ) )
		{
			valueList->AddTail( CHS( value.Get() ) );
		}
		dest.Set( CHS( key.Get() ) , valueList );
	}
}

void CPropertyFileUtil::Load( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest )
{
	//バイナリかテキストかを判定する
	if ( CMemoryUtil_::Cmp( src->vGetData() , property::g_cFileCode , sizeof( property::g_cFileCode ) ) )
	{//バイナリの場合バイナリ解析する
		_ParseBinary( src , dest );
	}
	else
	{//テキストの場合テキスト解析する
		_ParseText( src , dest );
	}
}

static skyString *FILE_HEADER_MARK = _T( "fileheadermark" );
static skyString *DATA_HEADER_MARK = _T( "dataheadermark" );
void CPropertyFileUtil::Compile( stream::IStreamPtr &src ,  stream::IStreamPtr &dest )
{
	//データの一時格納領域
	hash::CHashStringPtrListPtrHash propData( property::DEFAULt_HASH_SIZE );

	//テキストプロパティファイルを解析する
	_ParseText( src , propData );

	//バイナリのヘッダーを作る
	property::FileHeader fileheader;
	CMemoryUtil_::Copy( fileheader.cCode , property::g_cFileCode , sizeof( property::g_cFileCode ) );
	fileheader.nVersion = property::VERSION_NO;
	fileheader.nPropertyCount = propData.GetNum();
	fileheader.bIsBigEndian = endian::EndianUtil::GetEndian();
	fileheader.nFileSize = sizeof( property::FileHeader );

	//マーキング
	dest->vMarkWrite( FILE_HEADER_MARK , sizeof( property::FileHeader ) );

	//解析結果データからバイナリデータを作成する
	hash::CHashStringPtrList keyList;
	propData.GetkeyList( keyList );

	FOREACH( hash::CHashStringPtrList , keyit , keyList )
	{
		property::DataHeader dataHeader;

		//マーキング
		dest->vMarkWrite( DATA_HEADER_MARK , sizeof( property::DataHeader ) );

		//キーのサイズを足し込む（文字の長さバイト分と文字終端分）
		dataHeader.valueSize = CStringUtil_::LenByte( ( *keyit )->GetString() ) + sizeof( skyString );
//※1   dataHeader.valueSize = GET_HASH_VALUE_SIZE();

		//キーを書きこむ
		dest->vWriteStream( ( u8 *)( *keyit )->GetString() , dataHeader.valueSize );
//※1   dest->vWriteStream( ( u8 *)( *keyit )->Get() , dataHeader.valueSize );

		FOREACH( hash::CHashStringPtrList , valueListIt , *propData[ *keyit ].GetPtrNoConst() )
		{
			u32 valueSize = CStringUtil_::LenByte( ( *valueListIt )->GetString() ) + sizeof( skyString );
			dest->vWriteStream( ( u8 *)( *valueListIt )->GetString() , valueSize );
			dataHeader.valueSize += valueSize;
		}

		//実書き込み
		dest->vMarkReplace( DATA_HEADER_MARK , ( u8 *)&dataHeader , sizeof( property::DataHeader ) );

		//ファイルサイズに書きこむ
		fileheader.nFileSize += dataHeader.valueSize;
	}

	//実書き込み
	dest->vMarkReplace( FILE_HEADER_MARK , ( u8 *)&fileheader , sizeof( property::FileHeader ) );
}

const hash::CHashStringPtr &CPropertyFileUtil::GetString( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	return dest[ key ]->GetIdx( index );
}

s32 CPropertyFileUtil::Gets32( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	const hash::CHashStringPtr &value = dest[ key ]->GetIdx( index );

	return CStringUtil_::ConvStringTos32( value->GetString() );
}

d64 CPropertyFileUtil::Getd64( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index )
{
	const hash::CHashStringPtr &value = dest[ key ]->GetIdx( index );

	return CStringUtil_::ConvStringTod64( value->GetString() );
}

void CPropertyFileUtil::Dump( hash::CHashStringPtrListPtrHash &src , stream::IStreamPtr &dest )
{
	//解析結果データからバイナリデータを作成する
	hash::CHashStringPtrList keyList;
	src.GetkeyList( keyList );

	FOREACH( hash::CHashStringPtrList , keyit , keyList )
	{
		//キーを書きこむ
		dest->vWriteStream( ( *keyit )->GetString() );
		dest->vWriteStream( _T( "=" ) );
		FOREACH( hash::CHashStringPtrList , valueListIt , *src[ *keyit ].GetPtrNoConst() )
		{
			//値を書き込む
			dest->vWriteStream( ( *valueListIt )->GetString() );
			dest->vWriteStream( _T( "," ) );
		}
		dest->vWriteStream( &EC );
	}
}

} } }
