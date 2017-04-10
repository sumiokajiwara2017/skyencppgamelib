#include "StdAfx.h"
#include "SkyBaseLib/Args.h"
#include "SkyBaseLib/Directory.h"

SkyImplementRTTI( sky::lib::args::ArgsInfo , sky::lib::base::SkyObject );

namespace sky { namespace lib { namespace args {

Singleton< CArgsManager >	CArgsManager::SingletonMember;

CArgsManager::CArgsManager() : 
	m_OptionValueHash( OPTION_VALUE_HASH_SIZE )
	,m_ArgsInfoHash( ARGS_INFO_HASH_SIZE )
{

}
	
void CArgsManager::AddArgsInfo( const ArgsInfo &argsInfo )
{
	m_ArgsInfoHash.Set( argsInfo.m_Option , argsInfo );
}

void CArgsManager::WriteConsoleArgsInfo()
{
    SKY_PRINTF( _T( "<------------ Option info -------------> \n" ) );
	FOREACH( ArgsInfoHash , it , m_ArgsInfoHash )
	{
		SKY_PRINTF( _T( "%s: %s" ) , ( *it ).m_Option->GetString() , ( *it ).m_Info->GetString() );
	}
}

void CArgsManager::Parse( int argc , skyString * argv[] )
{
	for( s32 i = 0 ; i < argc ; i++ )
	{
		m_List.AddTail( CHS( argv[ i ] ) );
	}

    //オプションと値、形式で格納
    hash::CHashStringPtr optionName = CHS( _T( "" ) );
    hash::CHashStringPtrListPtr valueList;
	for( s32 i = 0 ; i < argc ; i++ )
    {
        //半角スーペースやタブが存在した場合はゴミと判断する
        if ( *argv[ i ] == _T( ' ' ) || *argv[ i ] == _T( '\t' ) )
        {
            continue;
        }

        //先頭が-の場合、オプションとして判断する
        if ( *argv[ i ] == _T( '-' ) )
        {
            //前のオプションが値が存在するはずなのにしない場合はエラーにする
            if ( optionName != CHS(_T( "" ) ) )
            {
                if ( m_OptionValueHash[ optionName ]->GetNum() == 0 && m_ArgsInfoHash[ optionName ].m_eType == eOptionType_OneValue )
                {
					SKY_PANIC_MSG( _T( "One value is necessary in optional[ %s ]." ) , optionName->GetString() ); //%sオプションには値が１つ必要です。
                }

                if ( m_OptionValueHash[ optionName ]->GetNum() == 0 && m_ArgsInfoHash[optionName].m_eType == eOptionType_ListValue)
                {
					SKY_PANIC_MSG( _T( "The value of two or more one ? is necessary in optional[ %s ]." ) , optionName->GetString() ); //%sオプションには値が1～複数必要です。
                }
            }

            optionName = CHS( argv[ i ] );

            SKY_ASSERT_MSG( m_ArgsInfoHash.IsGet( optionName ) , _T( " no support option. option=[ %s ]" ) , optionName->GetString() );

            if ( m_OptionValueHash.IsGet( optionName ) == skyFalse )
            {
                valueList.SetPtr( NEW__( hash::CHashStringPtrList , hash::CHashStringPtrList() ) );
                m_OptionValueHash.Set( optionName , valueList );
            }
            continue;
        }

        if ( optionName != CHS( _T( "" ) ) )
        {
            //オプションに複数の引数があるはずが無い場合、エラーとする
            if ( m_OptionValueHash[ optionName ]->GetNum() > 1 && m_ArgsInfoHash[ optionName ].m_eType == eOptionType_OneValue )
            {
				SKY_PANIC_MSG( _T( "One or more values cannot be set to the value in optional[ %s ]." ) , optionName->GetString() ); //%sオプションには値に値を１つ以上設定することはできません。
            }
			SKY_ASSERT_MSG( m_ArgsInfoHash[ optionName ].m_eType != eOptionType_Switch ,  _T( "The value cannot be set to optional[ %s ]." ) ,  optionName->GetString() ); //%sオプションには値は設定できません。

			if ( m_ArgsInfoHash[ optionName ].m_IsEnvReplaceExec == skyTrue )
			{
				//変換して値を格納
				skyString convValue[ PATH_LEN_MAX ];
				CPathUtil_::ReplaceEnv( argv[ i ] , convValue , PATH_LEN_MAX );
				m_OptionValueHash[ optionName ]->AddTail(  CHS( convValue ) );
			}
			else
			{
				//値を格納
				m_OptionValueHash[ optionName ]->AddTail( CHS( argv[ i ] ) );
			}
        }
    }
}

void CArgsManager::Add( const hash::CHashStringPtr &arg )
{
    m_List.AddTail( arg );
}

skyBool CArgsManager::IsArgsOn( u32 index )
{
    return ( m_List.GetNum() > index );
}

const hash::CHashStringPtr &CArgsManager::GetString( s32 index )
{
    return m_List[ index ];
}

s32 CArgsManager::GetS32( s32 index )
{
	return CStringUtil_::ConvStringTos32(  m_List[index]->GetString() );
}

skyBool CArgsManager::IsOptionOn( const hash::CHashStringPtr &optionName)
{
	return m_OptionValueHash.IsGet( optionName );
}

const hash::CHashStringPtr &CArgsManager::GetOptionValue( const hash::CHashStringPtr &optionName )
{
    SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );		SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );
    SKY_ASSERT_MSG( m_OptionValueHash[ optionName ]->GetNum() > 0 , _T( "The value is necessary for the option. option=%s" ) , optionName->GetString() );
	return m_OptionValueHash[ optionName ]->GetIdx( 0 );
}
s32 CArgsManager::GetOptionValue_s32( const hash::CHashStringPtr &optionName)
{
    SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );		SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );
    SKY_ASSERT_MSG( m_OptionValueHash[ optionName ]->GetNum() > 0 , _T( "The value is necessary for the option. option=%s" ) , optionName->GetString() );

	s32 result = CStringUtil_::ConvStringTos32( m_OptionValueHash[ optionName ]->GetIdx( 0 )->GetString() );

    return result;
}
u32 CArgsManager::GetOptionValue_u32( const hash::CHashStringPtr &optionName)
{
    SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );		SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );
    SKY_ASSERT_MSG( m_OptionValueHash[ optionName ]->GetNum() > 0 , _T( "The value is necessary for the option. option=%s" ) , optionName->GetString() );

	u32 result = CStringUtil_::ConvStringTou32( m_OptionValueHash[ optionName ]->GetIdx( 0 )->GetString() );

    return result;
}
d64 CArgsManager::GetOptionValue_d64( const hash::CHashStringPtr &optionName )
{
    SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );		SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );
    SKY_ASSERT_MSG( m_OptionValueHash[ optionName ]->GetNum() > 0 , _T( "The value is necessary for the option. option=%s" ) , optionName->GetString() );

	d64 result = CStringUtil_::ConvStringTod64( m_OptionValueHash[ optionName ]->GetIdx( 0 )->GetString() );

    return result;
}

hash::CHashStringPtrListPtr &CArgsManager::GetOptionValues( const hash::CHashStringPtr &optionName)
{
    SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );		SKY_ASSERT_MSG( IsOptionOn( optionName ) , _T( "option not found. option=%s" ) , optionName->GetString() );

    return m_OptionValueHash[ optionName ];
}

void CArgsManager::AssertCheckReqOption()
{
	FOREACH( ArgsInfoHash , it , m_ArgsInfoHash )
	{
		if ( ( *it ).m_IsRequired == skyTrue && IsOptionOn( ( *it ).m_Option ) == false)
        {
			SKY_PANIC_MSG( _T( "Optional[ %s ] is indispensable." ), ( *it ).m_Option ); //%sオプションは必須です。
        }
	}
}

void CArgsManager::SetReqFlg( const hash::CHashStringPtr &optionName , skyBool isRequired )
{
	m_ArgsInfoHash[ optionName ].m_IsRequired = isRequired;
}


} } }
