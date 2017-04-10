#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// シーングラフス操作用便利関数群
/// </summary>
class SceneGraphNodeUtil
{

public:

	/// <summary>
	/// 指定のノードの枝の指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachBranch( const CSceneGraphNodePtr &branchRoot , RESULT_TYPE &result )
	{
		if ( branchRoot.IsEmpty() == skyFalse )
		{
			if( SkyIsKindOf( SEARCH_TYPE , branchRoot.GetPtrNoConst() ) )
			{
				result = branchRoot;
				return skyTrue;
			}
			return SeaachBranch< SEARCH_TYPE , RESULT_TYPE >( branchRoot->GetBranchRoot() , result );
		}
		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの枝の指定の型と指定のIDを検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachBranchID( const CSceneGraphNodePtr &branchRoot , s32 id , RESULT_TYPE &result )
	{
		if ( branchRoot.IsEmpty() == skyFalse )
		{
			if( SkyIsKindOf( SEARCH_TYPE , branchRoot.GetPtrNoConst() ) && id == branchRoot->ID )
			{
				result = branchRoot;
				return skyTrue;
			}
			return SeaachBranchID< SEARCH_TYPE , RESULT_TYPE >( branchRoot->GetBranchRoot() , id , result );
		}
		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの枝の指定の型と指定のIDを検索してツリーから削除するテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE >
	static skyBool DetachBranchID( CSceneGraphNodePtr branchRoot , s32 id )
	{
		if ( branchRoot.IsEmpty() == skyFalse )
		{
			if( SkyIsKindOf( SEARCH_TYPE , branchRoot.GetPtrNoConst() ) && id == branchRoot->ID )
			{
				//親がいたら親から自分を切り離す
				if ( branchRoot->Parent().IsEmpty() == skyFalse )
				{
					( ( CSceneGraphNodePtr )branchRoot->Parent() )->DetachBranch();
				}

				//子が居たら親に関連付けする
				if( branchRoot->GetBranchRoot().IsEmpty() == skyFalse )
				{
					( ( CSceneGraphNodePtr )branchRoot->Parent() )->AttachBranch( branchRoot->GetBranchRoot() );
					branchRoot->DetachBranch();
				}

				return skyTrue;
			}
			return DetachBranchID< SEARCH_TYPE >( branchRoot->GetBranchRoot() , id );
		}
		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// 再帰検索はしていません。指定のノードの葉ノードの型のみ検査しています。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachLeaf( const CSceneGraphNodePtr &node , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) )
			{
				 result = ( *it );
				 return skyTrue;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型で検索してツリーから消すテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool DetachLeaf( CSceneGraphNodePtr node , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH_NOINC( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) )
			{  
				result = *it;
	            it = node->LeafList().Erases( it );
				return skyTrue;
			}
			else
			{
				it++;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型で検索してツリーから消すテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE >
	static skyBool DetachLeafAll( CSceneGraphNodePtr node )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH_NOINC( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) )
			{  
	            it = node->LeafList().Erases( it );
			}
			else
			{
				it++;
			}
		}

		return skyFalse;
	}


	/// <summary>
	/// 指定のノードの葉を指定の型とIDで検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// 再帰検索はしていません。指定のノードの葉ノードの型のみ検査しています。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachLeafID( const CSceneGraphNodePtr &node , s32 id , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) && id == ( *it )->ID )
			{
				 result = ( *it );
				 return skyTrue;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型とIDで検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// 再帰検索はしていません。指定のノードの葉ノードの型のみ検査しています。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachLeafIDAndName( const CSceneGraphNodePtr &node , s32 id , RESULT_TYPE &result , const hash::CHashStringPtr &name )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) && id == ( *it )->ID &&
				( name.IsEmpty() == skyTrue || name == ( *it )->Name.GetName() ) )
			{
				 result = ( *it );
				 return skyTrue;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型とIDで検索してツリーから消すテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE >
	static skyBool DetachLeafID( CSceneGraphNodePtr node , s32 id )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH_NOINC( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) && id == ( *it )->ID )
			{  
	            it = node->LeafList().Erases( it );
				return skyTrue;
			}
			else
			{
				it++;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの葉を指定の型とIDで検索してツリーから消すテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE >
	static skyBool DetachLeafIDAndName( CSceneGraphNodePtr node , s32 id , const hash::CHashStringPtr &name )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		FOREACH_NOINC( CSceneGraphNodePtrList , it , node->LeafList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) && id == ( *it )->ID &&
				( name.IsEmpty() == skyTrue || name == ( *it )->Name.GetName() ) )
			{  
	            it = node->LeafList().Erases( it );
				return skyTrue;
			}
			else
			{
				it++;
			}
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの子を指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachChild( const CSceneGraphNodePtr &node , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		if( SkyIsKindOf( SEARCH_TYPE , node.GetPtrNoConst() ) )
        {
             result = node;
	         return skyTrue;
        }

		FOREACH( CSceneGraphNodePtrList , it , node->ChildList() )
		{
            skyBool isFound = SeaachChild< SEARCH_TYPE , RESULT_TYPE >( *it , result );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの子を指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachChildJoint( const CSceneGraphNodePtr &node , const hash::CHashStringPtr &name , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		typename RESULT_TYPE spJoint = node->GetJoint();

		if ( spJoint.IsEmpty() == false && spJoint->Name == name )
		{
			result = spJoint;
			return skyTrue;
		}

		FOREACH( CJointPtrList , it , node->ChildList() )
		{
            skyBool isFound = SeaachChildJoint< SEARCH_TYPE , RESULT_TYPE >( *it , name , result );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachAll( const CSceneGraphNodePtr &node , RESULT_TYPE &result )
	{
		return SeaachAllID< SEARCH_TYPE , RESULT_TYPE >( node , 0 , result , skyFalse );
	}

	/// <summary>
	/// 指定のノードの指定の型を検索して指定の型で返すテンプレートメソッド
	/// isFoundはskyFalseで初期化してください。
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachAllID( const CSceneGraphNodePtr &node , s32 id , RESULT_TYPE &result , skyBool isCheckId = skyTrue )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		if( SkyIsKindOf( SEARCH_TYPE , node.GetPtrNoConst() ) )
        {
			if ( isCheckId == skyFalse )
			{
				result = node;
				return skyTrue;
			}
			else if ( isCheckId == skyTrue && id == node->ID )
			{
				result = node;
				return skyTrue;
			}
        }

		if ( node->GetBranchRoot().IsEmpty() == skyFalse )
		{
			skyBool isFound = SeaachAllID< SEARCH_TYPE , RESULT_TYPE >( node->GetBranchRoot() , id , result , isCheckId );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
        }

		FOREACH( CSceneGraphNodePtrList , it , node->LeafList() )
		{
            skyBool isFound = SeaachAllID< SEARCH_TYPE , RESULT_TYPE >( *it , id , result , isCheckId );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		FOREACH( CSceneGraphNodePtrList , it , node->ChildList() )
		{
            skyBool isFound = SeaachAllID< SEARCH_TYPE , RESULT_TYPE >( *it , id , result , isCheckId );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		return skyFalse;
	}

	/// <summary>
	/// 指定のノードの子を指定の型で検索してツリーから消すテンプレートメソッド
	/// </summary>
	template< class SEARCH_TYPE >
	static void DetachChild( CSceneGraphNodePtr node )
	{
		if ( node.IsEmpty() == skyTrue ) return;

		FOREACH_NOINC( CSceneGraphNodePtrList , it , node->ChildList() )
		{
			if( SkyIsKindOf( SEARCH_TYPE , ( *it ).GetPtrNoConst() ) )
			{  
	            it = node->ChildList().Erases( it );
			}
			else
			{
				it++;
			}
		}
	}

};

} } }