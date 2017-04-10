#include "StdAfx.h"
#include "SkyBaseLib/RegEx.h"

#ifdef SKYLIB_COMPILER_VC10
#include <regex>
#endif

namespace sky { namespace lib { namespace regex {

//-------------------------------------< 実装 >---------------------------------------------//

#ifdef SKYLIB_COMPILER_VC10

Singleton< CRegExWin32 >	CRegExWin32::Singleton;

void IRegEx::CreateInstance()
{
	CRegExWin32::Singleton.CreateInstance();
}
IRegEx *IRegEx::GetInstance()
{
	return CRegExWin32::Singleton.GetInstance();
}
void IRegEx::DeleteInstance()
{
	CRegExWin32::Singleton.DeleteInstance();
}

void CRegExWin32::Execute( const skyString *pRegEx , const skyString *pSrc , RegResultList &dest )
{
	//C++0xのregexを使用して正規表現を処理する
	std::basic_regex< skyString >			reg( pRegEx );
	std::match_results<const skyString *>	match;

	//ヒットチェック
	if ( std::regex_search( pSrc , match , reg ) == true )
	{//該当文字があった
		for ( u32 i = 0; i < match.size(); ++i )
		{
			dest.AddTail( match[ i ].first );
		}
	}
}
u32 CRegExWin32::Execute( const skyString *pRegEx , const skyString *pSrc )
{
	u32 result = 0;

	//C++0xのregexを使用して正規表現を処理する
	std::basic_regex< skyString >			reg( pRegEx );
	std::match_results<const skyString *>	match;

	//ヒットチェック
	if ( std::regex_search( pSrc , match , reg ) == true )
	{//該当文字があった
		result = match.size();
	}

	return result;
}

#endif

} } }
