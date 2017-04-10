#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �V�[���O���t�X����p�֗��֐��Q
/// </summary>
class SceneGraphNodeUtil
{

public:

	/// <summary>
	/// �w��̃m�[�h�̎}�̎w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
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
	/// �w��̃m�[�h�̎}�̎w��̌^�Ǝw���ID���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
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
	/// �w��̃m�[�h�̎}�̎w��̌^�Ǝw���ID���������ăc���[����폜����e���v���[�g���\�b�h
	/// </summary>
	template< class SEARCH_TYPE >
	static skyBool DetachBranchID( CSceneGraphNodePtr branchRoot , s32 id )
	{
		if ( branchRoot.IsEmpty() == skyFalse )
		{
			if( SkyIsKindOf( SEARCH_TYPE , branchRoot.GetPtrNoConst() ) && id == branchRoot->ID )
			{
				//�e��������e���玩����؂藣��
				if ( branchRoot->Parent().IsEmpty() == skyFalse )
				{
					( ( CSceneGraphNodePtr )branchRoot->Parent() )->DetachBranch();
				}

				//�q��������e�Ɋ֘A�t������
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
	/// �w��̃m�[�h�̗t���w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
	/// �ċA�����͂��Ă��܂���B�w��̃m�[�h�̗t�m�[�h�̌^�̂݌������Ă��܂��B
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
	/// �w��̃m�[�h�̗t���w��̌^�Ō������ăc���[��������e���v���[�g���\�b�h
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
	/// �w��̃m�[�h�̗t���w��̌^�Ō������ăc���[��������e���v���[�g���\�b�h
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
	/// �w��̃m�[�h�̗t���w��̌^��ID�Ō������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
	/// �ċA�����͂��Ă��܂���B�w��̃m�[�h�̗t�m�[�h�̌^�̂݌������Ă��܂��B
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
	/// �w��̃m�[�h�̗t���w��̌^��ID�Ō������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
	/// �ċA�����͂��Ă��܂���B�w��̃m�[�h�̗t�m�[�h�̌^�̂݌������Ă��܂��B
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
	/// �w��̃m�[�h�̗t���w��̌^��ID�Ō������ăc���[��������e���v���[�g���\�b�h
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
	/// �w��̃m�[�h�̗t���w��̌^��ID�Ō������ăc���[��������e���v���[�g���\�b�h
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
	/// �w��̃m�[�h�̎q���w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
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
	/// �w��̃m�[�h�̎q���w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
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
	/// �w��̃m�[�h�̎w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachAll( const CSceneGraphNodePtr &node , RESULT_TYPE &result )
	{
		return SeaachAllID< SEARCH_TYPE , RESULT_TYPE >( node , 0 , result , skyFalse );
	}

	/// <summary>
	/// �w��̃m�[�h�̎w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
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
	/// �w��̃m�[�h�̎q���w��̌^�Ō������ăc���[��������e���v���[�g���\�b�h
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