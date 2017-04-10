#include "StdAfx.h"
#include "SkyBaseLib/Parser.h"

namespace sky { namespace lib { namespace parser {

void CParserUtil::ParseStringPtn0( const skyMString *src , string::CStringList &destlist )
{
	if ( CStringUtil_::Len( src ) == 0 ) return;

	u32 pointer = 0;
	u32 newTokenPointer = 0;
	skyMString token[ TOKEN_LENGTH_MAX ];
	for( ;; )
	{
		switch ( src[ pointer ] )
		{
            case '\0': //文字列終了
			{
				if ( newTokenPointer > 0 )
				{
                    //現在のトークンの末尾に終端子を入れる
                    token[ newTokenPointer ] = '\0';

                    //現在のトークンをリストに追加
                    destlist.AddTail( token ); //勝手にCStringに変換して格納してくれる
				}
				goto END;

				break;
			}
            case ' ':	 //区切り
			{
                //半角スペースが連続で存在した場合はスキップする
                if ( newTokenPointer > 0 )
				{
                    //現在のトークンの末尾に終端子を入れる
                    token[ newTokenPointer ] = '\0';

                    //現在のトークンをリストに追加
                    destlist.AddTail( token );

                    //文字を頭から格納
                    newTokenPointer = 0;
				}
				break;
			}
            default: //区切り以外(V0001等の値と断定する
			{
				SKY_ASSERT_MSG( newTokenPointer < TOKEN_LENGTH_MAX, _T( "The length of the token is invalid. " ) ); //トークンの長さが無効です。

				token[ newTokenPointer ] = src[ pointer ];
				newTokenPointer++;

				break;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn0( const skyWString *src , string::CStringList &destlist )
{
    //文字列の長さが無い場合はそのまま返す
    if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	ParseStringPtn0( cValue , destlist );
	CStringUtil_::DeleteMString( cValue );
}

void CParserUtil::ParseStringPtn1( const skyMString *src , CValues32Map &destmap )
{
    //文字列の長さが無い場合はそのまま返す
    if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString  tokenTbl[ TOKEN_NUM_MAX ][ TOKEN_LENGTH_MAX ];
	u32         tokenCount = 0;

//--パースを2段階に分けているが、処理の見やすさを重視しただけです。処理不可が問題になった場合パースを1段階にしたほうがいいと思われます

//----------------------解析一段階目----------------------------------▽

	//文字列を『_』区切りでトークンに分解する
	//文字列内容をm_Argvに格納する
	u32 pointer = 0;
	u32 newTokenPointer = 0;
	for( ;; )
	{
		switch ( src[ pointer ] )
		{
			case '\0':		//文字列終了
			{
				if ( newTokenPointer > 0 )
				{
					//現在のトークンの末尾に終端子を入れる
					tokenTbl[ tokenCount ][ newTokenPointer ] = '\0';

					//次のトークンへ移行
					tokenCount++;
				}
				goto END;
			}
			case '_':	//区切り
			{
				//アンダーバーが連続で存在した場合はスキップする
				if ( newTokenPointer > 0 )
				{
					//現在のトークンの末尾に終端子を入れる
					tokenTbl[ tokenCount ][ newTokenPointer ] = '\0';

					//次のトークンへ移行
					tokenCount++;

					//文字を頭から格納
					newTokenPointer = 0;
				}
				break;
			}
			default: //区切り以外(V0001等の値と断定する
			{
				SKY_ASSERT_MSG( newTokenPointer < TOKEN_LENGTH_MAX, _T( "The length of the token is invalid. " ) ); //トークンの長さが無効です。

				tokenTbl[ tokenCount ][ newTokenPointer ] = src[ pointer ];
				newTokenPointer++;

				break;
			}
		}
		pointer++;
	}
END:

//----------------------解析二段階目----------------------------------▽

	//一つ一つのトークンを文字と数字に分解する＆文字をキーに、数字を値にしマップに登録する
	skyMString key[ KEY_LENGHT_MAX ];
	u32 keyPointer;
	skyMString value[ VALUE_LENGHT_MAX ];
	u32 valuePointer;
	skyMString value_i[ VALUE_LENGHT_MAX ];
	u32 valuePointer_i;
	skyBool isValueMode;
	for ( u32 i = 0 ; i < tokenCount ; i++ )
	{
		//一つのトークンを解析する
		keyPointer = 0;
		valuePointer = 0;
		valuePointer_i = 0;
		newTokenPointer = 0;
		isValueMode = skyFalse;
		for( ;; )
		{
			//数値を見つけたら値モードのフラグをOnにする
			if ( CStringUtil_::IsNum( tokenTbl[ i ][ newTokenPointer ] ) )
			{
				isValueMode = skyTrue;
			}

			//終端子の場合終了
			if ( tokenTbl[ i ][ newTokenPointer ] == '\0' )
			{
				key[ keyPointer ] = '\0';
				value[ valuePointer ] = '\0';
				value_i[ valuePointer_i ] = '\0';
				break;
			}
			//値モードでない場合はキー値とみなす
			else if ( isValueMode == skyFalse )
			{
				key[ keyPointer ] = tokenTbl[ i ][ newTokenPointer ];
				keyPointer++;
			}
			//値モードの場合は値とみなす
			else if ( isValueMode == skyTrue )
			{
				//値を格納する
				value[ valuePointer ] = tokenTbl[ i ][ newTokenPointer ];
				valuePointer++;

				//純粋な数字の値も格納する
				if ( isdigit( tokenTbl[ i ][ newTokenPointer ] ) )
				{
					value_i[ valuePointer_i ] = tokenTbl[ i ][ newTokenPointer ];
					valuePointer_i++;
				}
			}

			//現在のトークンの次の文字へ
			newTokenPointer++;
		}

		//マップに値を溜める
		s32 m = 1;
		if( CStringUtil_::Strchr( value, 'm' ) )
		{
			// 値が-の場合
			m = -1;
		}
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		skyWString *cKey = CStringUtil_::CreateWString( key );
		destmap.Set( cKey ,  m * CStringUtil_::ConvStringTos32( value_i ) );
		CStringUtil_::DeleteWString( cKey );
#else
		destmap.Set( key ,  m * CStringUtil_::ConvStringTos32( value_i ) );
#endif
	}
}

void CParserUtil::ParseStringPtn1( const skyWString *src , CValues32Map &destmap )
{
	//文字列の長さが無い場合はそのまま返す
	if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	ParseStringPtn1( cValue , destmap );
	CStringUtil_::DeleteMString( cValue );
}

void CParserUtil::ParseStringPtn2( const skyMString *src , const skyMString *keyWord , CValueBoolList &destlist )
{
	//文字列の長さが無い場合はそのまま返す
	if ( CStringUtil_::Len( src ) == 0 ) return;

	u32 srcLen = CStringUtil_::Len( src );
	u32 pointer = 0;
	u32 keywordLen = CStringUtil_::Len( keyWord );
	skyBool valueSwitch = skyFalse;
	u32 distance = 0;
	while( srcLen > pointer )
	{
		if ( valueSwitch == skyFalse )
		{//キーワード探す

			if ( ( distance = CStringUtil_::StrstrDistance( src , keyWord  ) ) < srcLen )
			{//キーワード見つけた
				pointer += distance + keywordLen;
				valueSwitch = skyTrue;
				continue;
			}
		}
		else
		{//値探す
			if ( CStringUtil_::IsNum( src[ pointer ] ) )
			{//数値があったので、SkyBool型にする
				destlist.AddTail( CStringUtil_::ConvertNum( src[ pointer ] ) != 0 );
			}
			else
			{//終わり
				goto END;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn2( const skyWString *src , const skyWString *keyWord , CValueBoolList &destlist )
{
	//文字列の長さが無い場合はそのまま返す
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	skyMString *cKeyWord = CStringUtil_::CreateMString( keyWord );
	ParseStringPtn2( cValue , cKeyWord , destlist );
	CStringUtil_::DeleteMString( cValue );
	CStringUtil_::DeleteMString( cKeyWord );
}

void CParserUtil::ParseStringPtn3( const skyMString *src , const skyMString *keyWord , CValueu8List &destlist )
{
	//文字列の長さが無い場合はそのまま返す
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	u32 srcLen = CStringUtil_::Len( src );
	u32 pointer = 0;
	u32 keywordLen = CStringUtil_::Len( keyWord );
	skyBool valueSwitch = skyFalse;
	u32 distance = 0;
	while( srcLen > pointer )
	{
		if ( valueSwitch == skyFalse )
		{//キーワード探す

			if ( ( distance = CStringUtil_::StrstrDistance( src , keyWord  ) ) < srcLen )
			{//キーワード見つけた
				pointer += distance + keywordLen;
				valueSwitch = skyTrue;
				continue;
			}
		}
		else
		{//値探す
			if ( CStringUtil_::IsNum( src[ pointer ] ) )
			{//数値があったので、SkyBool型にする
				destlist.AddTail( ( const u8 )CStringUtil_::ConvertNum( src[ pointer ] ) );
			}
			else
			{//終わり
				goto END;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn3( const skyWString *src , const skyWString *keyWord , CValueu8List &destlist )
{
	//文字列の長さが無い場合はそのまま返す
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	skyMString *cKeyWord = CStringUtil_::CreateMString( keyWord );
	ParseStringPtn3( cValue , cKeyWord , destlist );
	CStringUtil_::DeleteMString( cValue );
	CStringUtil_::DeleteMString( cKeyWord );
}

} } }
