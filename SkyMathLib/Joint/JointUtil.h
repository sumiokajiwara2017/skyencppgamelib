#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ジョイントグラフス操作用便利関数群
/// </summary>
class CJointUtil
{

public:

	/// <summary>
	/// 指定のノードの子を指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachChildName( const CJointPtr &node , const hash::CHashStringPtr &name , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		if( SkyIsKindOf( SEARCH_TYPE , node.GetPtrNoConst() ) && node->Name == name )
        {
             result = node;
	         return skyTrue;
        }

		FOREACH( CJointPtrList , it , node->ChildList() )
		{
            skyBool isFound = SeaachChildName< SEARCH_TYPE , RESULT_TYPE >( *it , name , result );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		return skyFalse;
	}
};

} } }