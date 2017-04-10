#pragma once

namespace sky { namespace lib { namespace hash {

/// <summary>
/// HashStringの便利関数
/// </summary>
class CHashStringUtil : public base::SkyObject
{
	/// <summary>
	/// 指定の文字列を指定のキーワードで分割してリストで返す
	/// </summary>
	static void GeTokenList( CHashStringPtrList &dest , const skyString *pSrc , const skyString key = _T( ' ' ) );

};

} } }
